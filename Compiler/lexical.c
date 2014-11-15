/*
 * lexical.c
 *
 *  Created on: Sep 11, 2014
 *      Author: heron
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexical.h"


ReservedWord reservedWords[QTDE_RESERVED_WORDS] =
	{ {"declare", DECLARE}, {"as", AS}, {"number", NUMBER}, {"letter", LETTER},
	   {"put", PUT}, {"in", IN}, {"if", IF}, {"then", THEN}, {"else", ELSE}, {"foreach", FOREACH},
	   {"do", DO}, {"for", FOR}, {"from", FROM}, {"to", TO}, {"resize", RESIZE}, {"read", READ},
	   {"print", PRINT} };

TokenType separatorsCharacter[QTDE_SEPARATORS] = {SPACE, TAB, FEED_LINE, COMMA, DOT, LEFT_BRACKET, RIGHT_BRACKET, PLUS, MINUS,
		MULTIPLICATION, BAR, MOD, LEFT_PARENTHESIS, RIGHT_PARENTHESIS, EQUAL};

/*StrangerType separatorsStrangers[] = {EXCLAMATION, OLD_GAME, DOLLAR, E_COMMERCIAL, COLON, SEMICOLON, INTERROGATION,
		AT, CIRCUMFLEX, UNDERLINE, CRASE, RIGHT_KEY, LEFT_KEY, TIO_ACCENT, UP_BAR};*/



void initTokens(){

	tokens = NULL;
	actualToken = NULL;

}


TokenType isReseverdWord(char *name){

	int i;
	for(i=0; i<QTDE_RESERVED_WORDS; i++){

		if(!strcmp(name, reservedWords[i].name)) //TODO test this
			return  reservedWords[i].type;
	}

	return NOTHING;
}


char* convertAllToLower(char *name){

	int i;
	int size = strlen(name);
	char* aux = (char*)malloc(size+1);

	for(i=0; i<size; i++)
		aux[i] = toLowercase((int)name[i]);

	aux[size] = '\0';
	return aux;

}


_Bool insToken(TokenType tokenType, char *tokenName){

	Token *t;
	TokenType tt;

	tokenName = convertAllToLower(tokenName);

	if( (t = (Token*)malloc(sizeof(Token))) ){

		if(!tokens && !actualToken){ //are null, first element to insert

			tokens = t;
			tokens->type = tokenType;

			if(tokenType != CONSTANT_NUMBER){

				if(tokenType == ID){

					tt = isReseverdWord(tokenName);

					if(tt != NOTHING)
						tokens->type = tt;

				}

				tokens->attribute.name = tokenName;

				//printf("%d %s", tokens->type, tokens->attribute.name);

			}else
				tokens->attribute.num = atoll(tokenName); //TODO string to long long

			tokens->next = NULL;
			actualToken = tokens;

			return TRUE;

		}else{

			actualToken->next = t; //previous "actualToken"

			actualToken = t;
			actualToken->type = tokenType;

			if(tokenType != CONSTANT_NUMBER){

				if(tokenType == ID){

					tt = isReseverdWord(tokenName);

					if(tt != NOTHING)
						actualToken->type = tt;

				}

				actualToken->attribute.name = tokenName;

			}else
				actualToken->attribute.num = atoll(tokenName); //TODO string to long long

			actualToken->next = NULL;

			//printf("%s", actualToken->attribute.name);

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

	if((tt = isSeparatorL(character)) != NOTHING)
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

/*
_Bool isStrangerSeparator(int character){

	int i;

	for(i=0; i<QTDE_SEPARATORS_STRANGERS; i++){

		if(character == separatorsStrangers[i])
			return TRUE;

	}

	return FALSE;
}*/


int toLowercase(int c){

	if(c >= 65 && c <= 90) //uppercase
		return c + 32;

	return c;
}


_Bool verifyingFile(const char *fileName){

	int c;
	FILE *sf;
	_Bool ok = TRUE;

	if( (sf = fopen(fileName, "r")) != NULL ){

		while( (c = fgetc(sf)) != EOF){

			if(isPrintableAll(c) || c == FEED_LINE || c == TAB)
				continue;
			else{
				ok = FALSE;
				break;
			}
		}

	}else{

		printf("ERR \"Possivelmente o arquivo --%s-- nao existe.\"\n"
						"<< Verifique o nome do arquivo\n", fileName);
	}

	fclose(sf);
	return ok;

}


_Bool lAnalyzer(const char *fileName){

	FILE *sf; //source file
	initTokens();
	_Bool okAnalyzer = TRUE;

	if( (sf = fopen(fileName, "r")) != NULL ){

		if(verifyingFile(fileName)){

			int c; //value of character in it
			int qc = 0; //quantity of characters read nominal
			int qcr = 0; //for CONSTANT_STRING, real quantity
			int al = 1; //actual line
			char *tokenReaded = (char*)malloc(INITIAL_TOKEN_BUFFER);

			States s = S1;
			_Bool outWhileDFA = FALSE; //exit DFA
			_Bool goNextCharacter = TRUE;

			TokenType tt;

			//DFA
			while( !outWhileDFA && (s == S1 || s == S2 || s == S3 || s == S4 || s == S5 || s == S6 ||
					s == S7 || s == S8 || s == S9 || s == S12 || s == S10 || s == S14 || s == S15 ||
					s == S11 ||s == S17 || s == S13 || s == S18 || s == S20 ||	s == S21 || s == S23 || s == S24) ){

				//increase memory allocated for "tokenReaded"
				if(qc > INITIAL_TOKEN_BUFFER-1){
					tokenReaded = (char*)realloc(tokenReaded, qc + 2);
				}

				switch(s){

					case S1:

						if(goNextCharacter){

							if((c = fgetc(sf)) != EOF ){

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

								}else if((tt = isSeparatorL(c)) != NOTHING){

									s = S18;
									tokenReaded[qc] = (char)c;

								}else{

									tokenReaded[qc] = (char)c;
									tokenReaded[qc+1] = '\0';
									printf("LINHA %d: %s\n", al, tokenReaded);
									okAnalyzer = FALSE;

									s = S1;
									tokenReaded = (char*)malloc(INITIAL_TOKEN_BUFFER);
									qc = 0;

								}
							}else
								outWhileDFA = TRUE; //nothing to analyzer


						}else{ //TODO duplicate code! VERIFY THE MODIFICATIONS IN DFA!

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

							}else if((tt = isSeparatorL(c)) != NOTHING){

								s = S18;
								tokenReaded[qc] = (char)c;

							}else{

								tokenReaded[qc] = (char)c;
								tokenReaded[qc+1] = '\0';

								printf("LINHA %d: %s\n", al, tokenReaded);
								okAnalyzer = FALSE;

								tokenReaded = (char*)malloc(INITIAL_TOKEN_BUFFER);
								qc = 0;
								s = S1;

							}

						}

					break;

					case S2:

						if( (c = fgetc(sf)) != EOF ){

							if(isLetter(c) || isNumberDigit(c)){

								tokenReaded[qc] = (char)toLowercase(c);
								qc++;

							}else{
								s = S14;
							}

						}else{ //accepted ID

							tokenReaded[qc] = '\0';
							insToken(ID, tokenReaded);
							outWhileDFA = TRUE;

						}

					break;

					case S14: //accepted ID

							tokenReaded[qc] = '\0';
							insToken(ID, tokenReaded);

							tokenReaded = (char*)malloc(INITIAL_TOKEN_BUFFER);
							qc = 0;

							goNextCharacter = FALSE;
							s = S1;

					break;

					case S3:

						if( (c = fgetc(sf)) != EOF ){

							if(isNumberDigit(c) && qc < 10){

								tokenReaded[qc] = (char)c;
								qc++;

							}else{

								s = S15;

							}

						}else{ //accepted CONSTANT_NUMBER

							tokenReaded[qc] = '\0';
							insToken(CONSTANT_NUMBER, tokenReaded);
							outWhileDFA = TRUE;

						}

					break;

					case S15: //accepted CONSTANT_NUMBER

						if(!isNumberDigit(c) && !isLetter(c)){ //is it a separator?

							tokenReaded[qc] = '\0';
							insToken(CONSTANT_NUMBER, tokenReaded);
							tokenReaded = (char*)malloc(INITIAL_TOKEN_BUFFER);
							qc = 0;
							goNextCharacter = FALSE;
							s = S1;

						}else{

							s = S11;

						}

					break;

					case S11: //err


						if( isNumberDigit(c) || isLetter(c) ){

							tokenReaded[qc] = (char)c;
							qc++;

							if(qc > INITIAL_TOKEN_BUFFER-1){
								tokenReaded = (char*)realloc(tokenReaded, qc + 2);
							}

							if((c = fgetc(sf)) != EOF){
								;
							}else{

								tokenReaded[qc] = '\0';
								printf("LINHA %d: %s\n", al, tokenReaded);
								okAnalyzer = FALSE;
								outWhileDFA = TRUE;

							}

						}else{

							tokenReaded[qc] = '\0';
							printf("LINHA %d: %s\n", al, tokenReaded);
							okAnalyzer = FALSE;
							s = S1;

							tokenReaded = (char*)malloc(INITIAL_TOKEN_BUFFER);
							qc = 0;
							goNextCharacter = FALSE;

						}

					break;

					case S4:

						if( (c = fgetc(sf)) != EOF ){

							if(isPrintableL(c)){

								s = S6;
								tokenReaded[qc] = (char)c;
								qc++;

							}else if(c == INVERTED_BAR){

								s = S8;
								tokenReaded[qc] = (char)c;
								qc++;

							}else
								s = S10; //treatment of err

						}else{

							tokenReaded[qc] = '\0';
							printf("LINHA %d: %s\n", al, tokenReaded);
							okAnalyzer = FALSE;
							outWhileDFA = TRUE;

						}

					break;

					case S10: //err's treatment


							if(qc > INITIAL_TOKEN_BUFFER-1){
								tokenReaded = (char*)realloc(tokenReaded, qc + 2);
							}

							if( c == SIMPLE_QUOTATION_MARK){

								tokenReaded[qc] = (char)c;
								tokenReaded[qc+1] = '\0';
								printf("LINHA %d: %s\n", al, tokenReaded);
								okAnalyzer = FALSE;
								tokenReaded = (char*)malloc(INITIAL_TOKEN_BUFFER);
								qc = 0;
								s = S1;

							}else if(c == TAB){

								tokenReaded[qc] = '\0';
								printf("LINHA %d: %s\n", al, tokenReaded);
								okAnalyzer = FALSE;
								tokenReaded = (char*)malloc(INITIAL_TOKEN_BUFFER);
								qc = 0;
								s = S1;

							}else if(c == FEED_LINE){

								tokenReaded[qc] = '\0';
								printf("LINHA %d: %s\n", al, tokenReaded);
								okAnalyzer = FALSE;
								tokenReaded = (char*)malloc(INITIAL_TOKEN_BUFFER);
								qc = 0;
								al++;
								s = S1;

							}else{ //anything else

								tokenReaded[qc] = (char)c;
								qc++;

								if((c = fgetc(sf)) != EOF){
									;
								}else{

									tokenReaded[qc] = '\0';
									printf("LINHA %d: %s\n", al, tokenReaded);
									okAnalyzer = FALSE;
									outWhileDFA = TRUE;

								}
							}

					break;

					case S6:

						if( (c = fgetc(sf)) != EOF ){

							if(c == SIMPLE_QUOTATION_MARK){

								s = S7;
								tokenReaded[qc] = (char)c;

							}else{

								s = S10;

							}

						}else{

							tokenReaded[qc] = '\0';
							printf("LINHA %d: %s\n", al, tokenReaded);
							okAnalyzer = FALSE;
							outWhileDFA = TRUE;

						}

					break;

					case S7: //accepted CONSTANT_CHARACTER

						tokenReaded[qc+1] = '\0';
						insToken(CONSTANT_CHARACTER, tokenReaded);
						s = S1;

						tokenReaded = (char*)malloc(INITIAL_TOKEN_BUFFER);
						qc = 0;

					break;

					case S8:

						if( (c = fgetc(sf)) != EOF ){

							if(isSE(c)){

								s = S9;
								tokenReaded[qc] = (char)c;
								qc++;

							}else{

								s = S10;

							}

						}else{

							tokenReaded[qc] = '\0';
							printf("LINHA %d: %s\n", al, tokenReaded);
							okAnalyzer = FALSE;
							outWhileDFA = TRUE;

						}

					break;

					case S9:

						if( (c = fgetc(sf)) != EOF ){

							if(c == SIMPLE_QUOTATION_MARK){

								s = S7;
								tokenReaded[qc] = (char)c;

							}else{

								s = S10;

							}

						}else{

							tokenReaded[qc] = '\0';
							printf("LINHA %d: %s\n", al, tokenReaded);
							okAnalyzer = FALSE;
							outWhileDFA = TRUE;

						}

					break;

					case S5:

						if( (c = fgetc(sf)) != EOF ){

							if(qcr < 257){

								if(isPrintableL(c) && qcr < 256){

									tokenReaded[qc] = (char)c;
									qc++;
									qcr++;

								}else if(c == DOUBLE_QUOTATION_MARK){

									tokenReaded[qc] = (char)c;
									s = S17;

								}else if(c == INVERTED_BAR && qcr < 256){

									s = S12;
									tokenReaded[qc] = (char)c;
									qc++;

								}else{

									s = S13;

								}

							}else{

								s = S13;

							}

						}else{

							tokenReaded[qc] = '\0';
							printf("LINHA %d: %s\n", al, tokenReaded);
							okAnalyzer = FALSE;
							outWhileDFA = TRUE;

						}

					break;

					case S13:

							if(qc > INITIAL_TOKEN_BUFFER-1){
								tokenReaded = (char*)realloc(tokenReaded, qc + 2);
							}

							if( c == DOUBLE_QUOTATION_MARK){

								tokenReaded[qc] = (char)c;
								tokenReaded[qc+1] = '\0';
								printf("LINHA %d: %s\n", al, tokenReaded);
								okAnalyzer = FALSE;
								tokenReaded = (char*)malloc(INITIAL_TOKEN_BUFFER);
								qc = 0;
								qcr = 0;
								s = S1;

							}else if(c == TAB){

								tokenReaded[qc] = '\0';
								printf("LINHA %d: %s\n", al, tokenReaded);
								okAnalyzer = FALSE;
								tokenReaded = (char*)malloc(INITIAL_TOKEN_BUFFER);
								qc = 0;
								qcr = 0;
								s = S1;

							}else if(c == FEED_LINE){

								tokenReaded[qc] = '\0';
								printf("LINHA %d: %s\n", al, tokenReaded);
								okAnalyzer = FALSE;
								tokenReaded = (char*)malloc(INITIAL_TOKEN_BUFFER);
								qc = 0;
								qcr = 0;
								al++;
								s = S1;

							}else{ //anything else

								tokenReaded[qc] = (char)c;
								qc++;

								if((c = fgetc(sf)) != EOF){
									;
								}else{

									tokenReaded[qc] = '\0';
									printf("LINHA %d: %s\n", al, tokenReaded);
									okAnalyzer = FALSE;
									outWhileDFA = TRUE;

								}
							}

					break;

					case S17: //accepted CONSTANT_STRING

						tokenReaded[qc+1] = '\0';
						insToken(CONSTANT_STRING, tokenReaded);
						s = S1;

						tokenReaded = (char*)malloc(INITIAL_TOKEN_BUFFER);
						qc = 0;
						qcr = 0;

					break;

					case S12:

						if( (c = fgetc(sf)) != EOF ){

								if(isSE(c)){

									s = S5;
									tokenReaded[qc] = (char)c;
									qc++;
									qcr++;

								}else{

									s = S13;
								}

						}else{

							tokenReaded[qc] = '\0';
							printf("LINHA %d: %s\n", al, tokenReaded);
							okAnalyzer = FALSE;
							outWhileDFA = TRUE;

						}

					break;

					case S23: //accepted relational operator <

						if( (c = fgetc(sf)) != EOF ){

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

						if(c != 32 && c != 9 && c != 10){

							tokenReaded[qc+1] = '\0';
							insToken(tt, tokenReaded);

						}

						s = S1;

						if(c == FEED_LINE)
							al++;

						tokenReaded = (char*)malloc(INITIAL_TOKEN_BUFFER);
						qc = 0;

					break;

				} //final switch

			} //final DFA

		}else{

			printf("ARQUIVO INVALIDO!\n");
			okAnalyzer = FALSE;

		}

		fclose(sf);

	}else{

		printf("ERR \"Possivelmente o arquivo --%s-- nao existe.\"\n"
				"<< Verifique o nome do arquivo\n", fileName);

		okAnalyzer = FALSE;

	}

	return okAnalyzer;

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
