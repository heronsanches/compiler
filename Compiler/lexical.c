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

TokenType separatorsCharacter[QTDE_SEPARATORS] = {SPACE, TAB, FEED_LINE, COMMA, DOT, LEFT_BRACKET, RIGHT_BRACKET, PLUS, MINUS, MULTIPLICATION,
		BAR, MOD, LEFT_PARENTHESIS, RIGHT_PARENTHESIS, EQUAL};

//const int QTDE_SEPARATORS = 15; //exclusive for dfa

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

	if( character != INVERTED_BAR && character != SIMPLE_QUOTATION_MARK && character != DOUBLE_QUOTATION_MARK
			&& isPrintableAll(character))
		return TRUE;

	return FALSE;
}


TokenType isSeparatorL(int character){

	int i;

	for(i=0; i<QTDE_SEPARATORS; i++){

		if(character == separatorsCharacter[i])
			return separatorsCharacter[i];

	}

	return NOTHING;
}


TokenType isSeparatorAll(int character){

	TokenType tt;

	if(tt = isSeparatorL(character))
		return tt;
	else if(character == LESS)
		return LESS;
	else if(character == GREATER )
		return GREATER;
	else if(character == SIMPLE_QUOTATION_MARK)
		return SIMPLE_QUOTATION_MARK;
	else if(character == DOUBLE_QUOTATION_MARK)
		return DOUBLE_QUOTATION_MARK;
	else
		return NOTHING;

}


int toLowercase(int c){

	if(c >= 65 && c <= 90) //uppercase
		return c + 32;

	return c;
}


void lAnalyzer(const char *fileName){

	FILE *sf; //source file
	initTokens();

	if( (sf = fopen(fileName, "r")) != NULL ){

		int c; //value of character in it
		int qc = 0; //quantity of characters read
		int al = 1; //actual line
		char *tokenReaded = (char*)malloc(INITIAL_TOKEN_BUFFER);

		States s = S1;
		_Bool outWhileDFA = FALSE; //exit DFA
		_Bool goNextCharacter = TRUE;

		TokenType tt;

		//DFA
		while( !outWhileDFA && (s == S1 || s == S2 || s == S3 || s == S4 || s == S5 || s == S6 ||
				s == S7 || s == S8 || s == S9 || s == S12 || s == S14 || s == S15 || s == S17 ||
				s == S18 || s == S20 ||	s == S21 || s == S23 || s == S24) ){

			//increase memory allocated for "tokenReaded"
			if(qc > INITIAL_TOKEN_BUFFER-1){
				tokenReaded = (char*)realloc(tokenReaded, qc + 1);
			}

			switch(s){

				case S1:

					if(goNextCharacter){

						if((c = fgetc(sf)) != EOF ){

							if(c == FEED_LINE)
								al++;

							if(isLetter(c)){

								s = S2; //ID
								tokenReaded[qc] = (char)c;
								qc++;

							}else if(isNumberDigit(c)){

								s = S3; //CONSTANT_NUMBER continuation
								tokenReaded[qc] = (char)c;
								qc++;

							}else if(c == SIMPLE_QUOTATION_MARK){

								s = S4; //CONSTANT_CHARACTER continuation
								tokenReaded[qc] = (char)c;
								qc++;

							}else if(c == DOUBLE_QUOTATION_MARK){

								s = S5;
								tokenReaded[qc] = (char)c;
								qc++; //CONSTANT_STRING continuation

							}else if(c == LESS){

								s = S23; //< <> <= continuation
								tokenReaded[qc] = (char)c;
								qc++;

							}else if(c == GREATER){

								s = S20; //> >= continuation
								tokenReaded[qc] = (char)c;
								qc++;

							}else if(tt = isSeparatorL(c)){

								s = S18;
								tokenReaded[qc] = (char)c;

							}else{

								tokenReaded[qc] = (char)c;
								tokenReaded[qc+1] = '\0';

								printf("ERR \"nao pertence a linguagem\"\n"
										"<< linha: %d   %s\n", al, tokenReaded);

								//outWhileRead = TRUE;
								outWhileDFA = TRUE;

							}
						}else
							outWhileDFA = TRUE; //nothing to analyzer


					}else{ //TODO duplicate code!

						goNextCharacter = TRUE;

						if(isLetter(c)){

							s = S2; //ID
							tokenReaded[qc] = (char)c;
							qc++;

						}else if(isNumberDigit(c)){

							s = S3; //CONSTANT_NUMBER continuation
							tokenReaded[qc] = (char)c;
							qc++;

						}else if(c == SIMPLE_QUOTATION_MARK){

							s = S4; //CONSTANT_CHARACTER continuation
							tokenReaded[qc] = (char)c;
							qc++;

						}else if(c == DOUBLE_QUOTATION_MARK){

							s = S5;
							tokenReaded[qc] = (char)c;
							qc++; //CONSTANT_STRING continuation

						}else if(c == LESS){

							s = S23; //< <> <= continuation
							tokenReaded[qc] = (char)c;
							qc++;

						}else if(c == GREATER){

							s = S20; //> >= continuation
							tokenReaded[qc] = (char)c;
							qc++;

						}else if(tt = isSeparatorL(c)){

							s = S18;
							tokenReaded[qc] = (char)c;

						}else{

							tokenReaded[qc] = (char)c;
							tokenReaded[qc+1] = '\0';

							printf("ERR \"nao pertence a linguagem\"\n"
									"<< linha: %d   %s\n", al, tokenReaded);

							//outWhileRead = TRUE;
							outWhileDFA = TRUE;

						}

					}

				break;

				case S2:

					if( (c = fgetc(sf)) != EOF ){

						if(c == FEED_LINE)
							al++;

						if(isLetter(c) || isNumberDigit(c)){

							tokenReaded[qc] = (char)c;
							qc++;

						}else{
							s = S14;
						}
					}else{ //accepted ID

						tokenReaded[qc+1] = '\0';
						insToken(ID, tokenReaded);
						outWhileDFA = TRUE;
					}

				break;

				case S14: //accepted ID

					if(isSeparatorAll(c)){

						tokenReaded[qc] = '\0';
						insToken(ID, tokenReaded);
						s = S1;

						tokenReaded = (char*)malloc(INITIAL_TOKEN_BUFFER);
						qc = 0;
						goNextCharacter = FALSE;

					}else{

						/*tokenReaded[qc] = (char)c;
						tokenReaded[qc+1] = '\0';*/

						printf("ERR \"nao pertence a linguagem\"\n"
								"<< linha: %d   %c\n", al, c);

						outWhileDFA = TRUE;

					}


				break;

				case S3:

					if( (c = fgetc(sf)) != EOF ){

						if(c == FEED_LINE)
							al++;

						if(isNumberDigit(c) && qc < 10){

							tokenReaded[qc] = (char)c;
							qc++;

						}else
							s = S15;
					}else{ //accepted CONSTANT_NUMBER

						tokenReaded[qc+1] = (char)c;
						insToken(CONSTANT_NUMBER, tokenReaded);
						outWhileDFA = TRUE;

					}

				break;

				case S15: //accepted CONSTANT_NUMBER

					if(isSeparatorAll(c)){

						tokenReaded[qc] = '\0';
						insToken(CONSTANT_NUMBER, tokenReaded);
						s = S1;

						tokenReaded = (char*)malloc(INITIAL_TOKEN_BUFFER);
						qc = 0;

					}else{

						tokenReaded[qc] = (char)c;
						tokenReaded[qc+1] = '\0';

						printf("ERR \"nao pertence a linguagem\"\n"
								"<< linha: %d   %s\n", al, tokenReaded);

						outWhileDFA = TRUE;

					}

				break;

				case S4:

					if( (c = fgetc(sf)) != EOF ){

						if(c == FEED_LINE)
							al++;

						if(isPrintableL(c)){

							s = S6;
							tokenReaded[qc] = (char)c;
							qc++;

						}else if(c == INVERTED_BAR){

							s = S8;
							tokenReaded[qc] = (char)c;
							qc++;

						}else{

							tokenReaded[qc] = (char)c;
							tokenReaded[qc+1] = '\0';

							printf("ERR \"nao pertence a linguagem\"\n"
									"<< linha: %d   %s\n", al, tokenReaded);

							outWhileDFA = TRUE;

						}
					}else{

						tokenReaded[qc] = '\0';

						printf("ERR \"nao pertence a linguagem\"\n"
								"<< linha: %d   %s\n", al, tokenReaded);

						outWhileDFA = TRUE;

					}

				break;

				case S6:

					if( (c = fgetc(sf)) != EOF ){

						if(c == FEED_LINE)
							al++;

						if(c == SIMPLE_QUOTATION_MARK){

							s = S7;
							tokenReaded[qc] = (char)c;

						}else{

							tokenReaded[qc] = (char)c;
							tokenReaded[qc+1] = '\0';

							printf("ERR \"nao pertence a linguagem\"\n"
									"<< linha: %d   %s\n", al, tokenReaded);

							outWhileDFA = TRUE;
						}

					}else{

						tokenReaded[qc] = '\0';

						printf("ERR \"nao pertence a linguagem\"\n"
								"<< linha: %d   %s\n", al, tokenReaded);

						outWhileDFA = TRUE;

					}

				break;
				//TODO tratar os barra n etc
				case S7: //accepted CONSTANT_CHARACTER //TODO tratar gfg'

					tokenReaded[qc+1] = '\0';
					insToken(CONSTANT_CHARACTER, tokenReaded);
					s = S1;

					tokenReaded = (char*)malloc(INITIAL_TOKEN_BUFFER);
					qc = 0;

				break;

				case S8:

					if( (c = fgetc(sf)) != EOF ){

						if(c == FEED_LINE)
							al++;

						if(isSE(c)){

							s = S9;
							tokenReaded[qc] = (char)c;
							qc++;

						}else{

							tokenReaded[qc] = (char)c;
							tokenReaded[qc+1] = '\0';

							printf("ERR \"nao pertence a linguagem\"\n"
									"<< linha: %d   %s\n", al, tokenReaded);

							outWhileDFA = TRUE;

						}

					}else{

						tokenReaded[qc] = '\0';

						printf("ERR \"nao pertence a linguagem\"\n"
								"<< linha: %d   %s\n", al, tokenReaded);

						outWhileDFA = TRUE;

					}

				break;

				case S9:

					if( (c = fgetc(sf)) != EOF ){

						if(c == FEED_LINE)
							al++;

						if(c == SIMPLE_QUOTATION_MARK){

							s = S7;
							tokenReaded[qc] = (char)c;

						}else{

							tokenReaded[qc] = (char)c;
							tokenReaded[qc+1] = '\0';

							printf("ERR \"nao pertence a linguagem\"\n"
									"<< linha: %d   %s\n", al, tokenReaded);

							outWhileDFA = TRUE;

						}

					}else{

						tokenReaded[qc] = '\0';

						printf("ERR \"nao pertence a linguagem\"\n"
								"<< linha: %d   %s\n", al, tokenReaded);

						outWhileDFA = TRUE;

					}

				break;

				case S5:

					if( (c = fgetc(sf)) != EOF ){

						if(c == FEED_LINE)
							al++;

						if(qc < 258){

							if(isPrintableL(c)){

								tokenReaded[qc] = (char)c;
								qc++;

							}else if(c == DOUBLE_QUOTATION_MARK){

								tokenReaded[qc] = (char)c;
								s = S17;

							}else if(c == INVERTED_BAR){

								s = S12;
								tokenReaded[qc] = (char)c;
								qc++;

							}

						}else{

							tokenReaded[qc] = (char)c;
							tokenReaded[qc+1] = '\0';

							printf("ERR \"nao pertence a linguagem\"\n"
									"<< linha: %d   %s\n", al, tokenReaded);

							outWhileDFA = TRUE;

						}

					}else{

						tokenReaded[qc] = '\0';

						printf("ERR \"nao pertence a linguagem\"\n"
								"<< linha: %d   %s\n", al, tokenReaded);

						outWhileDFA = TRUE;

					}

				break;

				case S17: //accepted CONSTANT_STRING

					tokenReaded[qc+1] = '\0';
					insToken(CONSTANT_STRING, tokenReaded);
					s = S1;

					tokenReaded = (char*)malloc(INITIAL_TOKEN_BUFFER);
					qc = 0;

				break;

				case S12:

					if( (c = fgetc(sf)) != EOF ){

						if(c == FEED_LINE)
							al++;

						if(isSE(c)){

							s = S5;
							tokenReaded[qc] = (char)c;
							qc++;

						}else{

							tokenReaded[qc] = (char)c;
							tokenReaded[qc+1] = '\0';

							printf("ERR \"nao pertence a linguagem\"\n"
									"<< linha: %d   %s\n", al, tokenReaded);

							outWhileDFA = TRUE;
						}

					}else{

						tokenReaded[qc] = '\0';

						printf("ERR \"nao pertence a linguagem\"\n"
								"<< linha: %d   %s\n", al, tokenReaded);

						outWhileDFA = TRUE;

					}

				break;

				case S23: //accepted relational operator <

					if( (c = fgetc(sf)) != EOF ){

						if(c == FEED_LINE)
							al++;

						if(c == GREATER || c == EQUAL){

							s = S24;
							tokenReaded[qc] = (char)c;

						}else{

							tokenReaded[qc] = '\0';
							insToken(LESS, tokenReaded);
							s = S1;

							qc = 0;
							tokenReaded = (char*)malloc(INITIAL_TOKEN_BUFFER);
							goNextCharacter = FALSE;

						}

					}else{

						tokenReaded[qc] = '\0';
						insToken(LESS, tokenReaded);
						outWhileDFA = TRUE;

					}

				break;

				case S24: //accepted relational operator <> <=

					tokenReaded[qc+1] = '\0';

					if(c == GREATER)
						insToken(DIFFERENT, tokenReaded);
					else
						insToken(LESS_OR_EQUAL, tokenReaded);

					s = S1;

					tokenReaded = (char*)malloc(INITIAL_TOKEN_BUFFER);
					qc = 0;

				break;

				case S20: //accepted  >

					if( (c = fgetc(sf)) != EOF ){

						if(c == FEED_LINE)
							al++;

						if(c == EQUAL){

							s = S21;
							tokenReaded[qc] = (char)c;

						}else{

							tokenReaded[qc] = '\0';
							insToken(GREATER, tokenReaded);
							s = S1;

							tokenReaded = (char*)malloc(INITIAL_TOKEN_BUFFER);
							qc = 0;
							goNextCharacter = FALSE;
						}

					}else{

						tokenReaded[qc] = '\0';
						insToken(GREATER, tokenReaded);

						outWhileDFA = TRUE;

					}

				break;

				case S21: //accepted >=

					tokenReaded[qc+1] = '\0';
					insToken(GREATER_OR_EQUAL, tokenReaded);
					s = S1;

					tokenReaded = (char*)malloc(INITIAL_TOKEN_BUFFER);
					qc = 0;

				break;

				case S18: //accepted , . [ ] + - * / % ( ) = espaco tab quebra_de_linha

					tokenReaded[qc+1] = '\0';
					insToken(tt, tokenReaded);
					s = S1;

					tokenReaded = (char*)malloc(INITIAL_TOKEN_BUFFER);
					qc = 0;

				break;

			} //final switc

		} //final DFA

		fclose(sf);

	}else{
		printf("ERR \"Possivelmente o arquivo --%s-- nao existe.\"\n"
				"<< Verifique o nome do arquivo\n", fileName);
	}

}


_Bool isSE(int character){

	if(character == LETTER_T_LOWER || character == LETTER_N_LOWER || character == INVERTED_BAR ||
			character == SIMPLE_QUOTATION_MARK	|| character == DOUBLE_QUOTATION_MARK)
		return TRUE;

	return FALSE;
}

void freeMemory(){

	actualToken = tokens;
	Token *next  = NULL;

	for(; actualToken != NULL; actualToken = next){

		next = actualToken->next;
		free(actualToken);

	}

}
