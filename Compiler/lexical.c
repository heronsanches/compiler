/*
 * lexical.c
 *
 *  Created on: Sep 11, 2014
 *      Author: heron
 */
#include <stdio.h>
#include <stdlib.h>
#include "lexical.h"


struct{
	char *name;
	TokenType type;
}ReservedWords[QTDE_RESERVED_WORDS] =
	{ {"declare", DECLARE}, {"as", AS}, {"number", NUMBER}, {"letter", LETTER},
	   {"put", PUT}, {"in", IN}, {"then", THEN}, {"else", ELSE}, {"foreach", FOREACH},
	   {"do", DO}, {"for", FOR}, {"from", FROM}, {"to", TO}, {"resize", RESIZE}, {"read", READ},
	   {"print", PRINT} };

const int separatorsCharacter[] = {32, 10, 9, 44, 46, 91, 93, 43, 45, 42, 47, 37, 40, 41, 61}; //size = 15
const int QTDE_SEPARATORS = 15; //exclusive for dfa


//TODO struct RelationalOperators
//TODO struct RelationalOperators


void initTokens(){

	tokens = NULL;
	actualToken = NULL;

}


_Bool insToken(TokenType tokenType, char *tokenName){

	Token *t;

	if(t = (Token*)malloc(sizeof(Token))){

		if(!tokens && !actualToken){ //are null, first element to insert

			tokens = t;
			tokens->type = tokenType;

			if(tokenType != CONSTANT_NUMBER && tokenType != NUMBER)
				tokens->attribute.name = tokenName;
			else
				tokens->attribute.num = tokenName; //TODO string to long long

			tokens->next = NULL;
			actualToken = tokens;

			return TRUE;

		}else{

			actualToken->next = t; //previous "actualToken"

			actualToken = t;
			actualToken->type = tokenType;

			if(tokenType != CONSTANT_NUMBER && tokenType != NUMBER)
				actualToken->attribute.name = tokenName;
			else
				actualToken->attribute.num = tokenName; //TODO string to long long

			actualToken->next = NULL;

			return TRUE;
		}

	}else{
		printf("%s\n", "Memoria insuficiente.");
	}

	return FALSE;

}


_Bool isLetter(int character){

	if( (character >= 65 && character <= 90) || (character >= 97 && character <= 122) )
		return TRUE;

	return FALSE;

}


_Bool isNumberDigit(int character){

	if( character >= 48 && character <= 57)
		return TRUE;

	return FALSE;

}


_Bool isPrintableAll(int character){

	if(character >= 32 && character <= 126)
		return TRUE;

	return FALSE;
}

_Bool isPrintableL(int character){

	if( character != 92 && character != 39 && character != 34 && isPrintableAll(character))
		return TRUE;

	return FALSE;
}


_Bool isSeparatorL(int character){

	int i;

	for(i=0; i<QTDE_SEPARATORS; i++){

		if(character == separatorsCharacter[i])
			return TRUE;

	}

	return FALSE;
}


_Bool isSeparatorAll(int character){

	if(isSeparatorL(character) || character == 60 || character == 62 || character == 39
			|| character == 34)
		return TRUE;

	return FALSE;

}


int toLowercase(int c){

	if(c >= 65 && c <= 90) //uppercase
		return c + 32;

	return c;
}


//TODO test phase
void lAnalyzer(const char *fileName){

	FILE *sf; //source file
	initTokens();

	if( (sf = fopen(fileName, "r")) != NULL ){

		int c; //value of character in it
		int qc = 0; //quantity of characters read
		int al = 1; //actual line
		char *tokenReaded = (char*)malloc(INITIAL_TOKEN_BUFFER);

		States s = S1;
		_Bool outWhileDFA = FALSE; //read another character
		_Bool outWhileRead = FALSE; //exit read of the characters

		//Read
		while( !outWhileRead && (c = fgetc(sf)) != EOF ){

			if(c == 10)
				al++;

			outWhileDFA = FALSE;

			//increase memory allocated for "tokenReaded"
			if(qc > INITIAL_TOKEN_BUFFER-1){
				tokenReaded = (char*)realloc(tokenReaded, qc + 1);
			}

			//TODO DFA
			while( !outWhileDFA && (s == S1 || s == S2 || s == S3 || s == S4 || s == S5 || s == S6 ||
					s == S7 || s == S8 || s == S9 || s == S10 || s == S11 || s == S12 || s == S13 ||
					s == S14 || s == S15 || s == S17 || s == S18 || s == S19 || s == S20 ||
					s == S21 || s == S22 || s == S23 || s == S24 ||	s == S25 || s == S26) ){

				tokenReaded[qc] = (char)c;

				switch(s){

					case S1:

						if(isLetter(c)){

							s = S2; //ID
							outWhileDFA = TRUE;
							qc++;

						}else if(isNumberDigit(c)){

							s = S3; //CONSTANT_NUMBER continuation
							outWhileDFA = TRUE;
							qc++;

						}else if(c == 39){

							s = S4; //CONSTANT_CHARACTER continuation
							outWhileDFA = TRUE;
							qc++;

						}else if(c == 34){

							s = S5;
							outWhileDFA = TRUE;
							qc++; //CONSTANT_STRING continuation

						}else if(c == 60){

							s = S23; //< <> <= continuation
							outWhileDFA = TRUE;
							qc++;

						}else if(c == 62){

							s = S20; //> >= continuation
							outWhileDFA = TRUE;
							qc++;

						}else if(isSeparatorL(c)){

							s = S18; //SEPARATOR continuation
							outWhileDFA = TRUE;
							qc++;

						}else{

							tokenReaded[qc+1] = '\0';

							printf("ERR \"nao pertence a linguagem\"\n"
									"<< linha: %d   %s\n", al, tokenReaded);

							outWhileRead = TRUE;
							outWhileDFA = TRUE;

						}

					break;

					case S2:

						if(isLetter(c) || isNumberDigit(c)){

							qc++;
							outWhileDFA = TRUE;

						}else
							s = S14;

					break;

					case S14: //accepted ID

						if(isSeparatorAll(c)){

							tokenReaded[qc] = '\0';
							insToken(ID, tokenReaded);
							s = S1;

							tokenReaded = (char*)malloc(INITIAL_TOKEN_BUFFER);
							qc = 0;
							outWhileDFA = TRUE;

						}else{

							tokenReaded[qc+1] = '\0';

							printf("ERR \"nao pertence a linguagem\"\n"
									"<< linha: %d   %s\n", al, tokenReaded);

							outWhileRead = TRUE;
							outWhileDFA = TRUE;

						}


					break;

					case S3:

						if(qc < 10){

							if(isNumberDigit(c)){

								qc++;
								outWhileDFA = TRUE;

							}else
								s = S15;

						}else
							s = S15;

					break;

					case S15: //accepted CONSTANT_NUMBER

						if(isSeparatorAll(c)){

							tokenReaded[qc] = '\0';
							insToken(CONSTANT_NUMBER, tokenReaded);
							s = S1;

							tokenReaded = (char*)malloc(INITIAL_TOKEN_BUFFER);
							qc = 0;
							outWhileDFA = TRUE;

						}else{

							tokenReaded[qc+1] = '\0';

							printf("ERR \"nao pertence a linguagem\"\n"
									"<< linha: %d   %s\n", al, tokenReaded);

							outWhileRead = TRUE;
							outWhileDFA = TRUE;

						}

					break;

					case S4:

						if(isPrintableL(c)){

							s = S6;
							qc++;
							outWhileDFA = TRUE;

						}else if(c == 92){

							s = S8;
							qc++;
							outWhileDFA = TRUE;

						}else{

							tokenReaded[qc+1] = '\0';

							printf("ERR \"nao pertence a linguagem\"\n"
									"<< linha: %d   %s\n", al, tokenReaded);

							outWhileRead = TRUE;
							outWhileDFA = TRUE;

						}

					break;

					case S6:

						if(c == 39)
							s = S7;
						else{

							tokenReaded[qc+1] = '\0';

							printf("ERR \"nao pertence a linguagem\"\n"
									"<< linha: %d   %s\n", al, tokenReaded);

							outWhileRead = TRUE;
							outWhileDFA = TRUE;

						}

					break;

					case S7: //accepted CONSTANT_CHARACTER

						tokenReaded[qc+1] = '\0';
						insToken(CONSTANT_CHARACTER, tokenReaded);
						s = S1;

						tokenReaded = (char*)malloc(INITIAL_TOKEN_BUFFER);
						qc = 0;
						outWhileDFA = TRUE;

					break;

					case S8:

						if(isSE(c)){

							s = S9;
							qc++;
							outWhileDFA = TRUE;

						}else{

							tokenReaded[qc+1] = '\0';

							printf("ERR \"nao pertence a linguagem\"\n"
									"<< linha: %d   %s\n", al, tokenReaded);

							outWhileRead = TRUE;
							outWhileDFA = TRUE;

						}

					break;

					case S9:

						if(c == 39)
							s = S7;
						else{

							tokenReaded[qc+1] = '\0';

							printf("ERR \"nao pertence a linguagem\"\n"
									"<< linha: %d   %s\n", al, tokenReaded);

							outWhileRead = TRUE;
							outWhileDFA = TRUE;

						}

					break;

					//TODO the left



				}

			}

		}

		fclose(sf);

	}else{
		printf("ERR \"Possivelmente o arquivo --%s-- nao existe.\"\n"
				"<< Verifique o nome do arquivo\n", fileName);
	}

}


_Bool isSE(int character){

	if(character == 116 || character == 110 || character == 92 || character == 39
			|| character == 34)
		return TRUE;

	return FALSE;
}
