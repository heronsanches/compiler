/*
 * lexical.c
 *
 *  Created on: Sep 11, 2014
 *      Author: heron
 */
#include <stdio.h>
#include <stdlib.h>
#include "lexical.h"



void initTokens(){

	preTokens = NULL;
	actualToken = NULL;

}


void initPreTokens(){

	tokens = NULL;
	actualPreToken = NULL;

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


_Bool insPreToken(char *name){

	PreToken *pt;

	if( pt = (PreToken*)malloc(sizeof(PreToken)) ){

		if( !preTokens && !actualPreToken){ //are nulls, first element to insert

			preTokens = pt;
			preTokens->name = name;
			preTokens->next = NULL;

			actualPreToken = preTokens;

			return TRUE;

		}else{

			actualPreToken->next = pt; //previous "actualPreToken"

			actualPreToken = pt;
			actualPreToken->name = name;
			actualPreToken->next = NULL;

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


_Bool isDigit(int character){

	if( character >= 48 && character <= 57)
		return TRUE;

	return FALSE;

}


_Bool isPrintableCharacter(int character){

	if(character >= 32 && character <= 126)
		return TRUE;

	return FALSE;
}


//TODO test phase
void preLAnalyzer(const char *fileName){

	FILE *sf; //source file
	initPreTokens();
	initTokens();

	if( (sf = fopen(fileName, "r")) != NULL ){

		int c; //value of character in it
		int qc = 0; //quantity of characters read
		char *tokenReaded = (char*)malloc(INITIAL_TOKEN_BUFFER);

		//reading source file
		while( (c = fgetc(sf)) != EOF ){

			//increase memory allocated for "tokenReaded"
			if(qc > INITIAL_TOKEN_BUFFER-1){
				tokenReaded = (char*)realloc(tokenReaded, qc + 2); //+2: one char for save the "last separator(it has conditions)" and other to save '\0'
			}

			//is a separator? //TODO I must save the others separators besides them
			if(c == 32 || c == 9 || c == 10){ //space, tab, line feed

				tokenReaded[qc] = '\0';
				insPreToken(tokenReaded);

				tokenReaded = (char*)malloc(INITIAL_TOKEN_BUFFER);
				qc = 0;

			}else{

				tokenReaded[qc] = (char)c;
				qc++;

			}

		}

		//TODO remove this after tests' phases
		/*PreToken *pta = preTokens;
		for(; pta != NULL; pta = pta->next)
			printf("%s\n", pta->name);*/

		//TODO make free in "preTokens" after analyzer these tokens, I think no??? references...
		//free(preTokens);
		fclose(sf);

	}else{
		printf("%s", "ERR \"Possivelmente este arquivo nao existe.\"\n"
				"<< Verifique o nome do arquivo\n");
	}

}


_Bool lAnalyzer(const char *fileName){

	preLAnalyzer(fileName);
	actualPreToken = preTokens;

	//traverses the list "preTokens" for match elements the language
	for(; actualPreToken != NULL; actualPreToken = actualPreToken->next){

		if( isID(actualPreToken->name) || isConstantNumber(actualPreToken->name)
				|| isConstantCharacter(actualPreToken->name)) // TODO or, is the others finite machines
			;
		else{
			printf("ERR \"Token nao reconhecido.\"\n<< token: %s\n", actualPreToken->name);
			return FALSE;
		}

	}

	return TRUE;

}


_Bool isConstantCharacter(char *tokenName){

	char state = '1';
	int i = 0;

	while( (state == '1' || state == '2' || state == '3'
			|| state == '4'|| state == '5') && tokenName[i] != '\0' ){

		switch(state){

			case '1':

				if(tokenName[i] == (char)39){ //is it a single quotation?

					i++;
					state = '2';

				}else
					state = '0';

			break;

			case '2':

				if( tokenName[i] != (char)92 &&  tokenName[i] != (char)39 &&
						tokenName[i] != (char)34 && isPrintableCharacter((int)tokenName[i]) ){

					state = '3';
					i++;

				}else if(tokenName[i] == (char)92){ //is it a invert bar?

					state = '4';
					i++;
				}else
					state ='0';

			break;

			case '3':

				if(tokenName[i] == (char)39){

					state = '6';
					i++;

				}else
					state = '0';

			break;

			case '4':

				if(tokenName[i] == (char)116 || tokenName[i] == (char)110 || tokenName[i] == (char)92
						|| tokenName[i] == (char)39 || tokenName[i] == (char)34){ // t n \ ' "

					state = '5';
					i++;

				}else
					state = '0';

			break;

			case '5':

				if(tokenName[i] == (char)39){

					state = '6';
					i++;

				}else
					state = '0';

			break;

		}

	}

	if(state == '6'){

		insToken(CONSTANT_CHARACTER, tokenName);
		return TRUE;

	}

	return FALSE;
}


_Bool isID(char *tokenName){

	char state = '1'; //beginning
	int i = 0;

	while( (state == '1' || state == '2') && tokenName[i] != '\0'){

		switch(state){

			case '1':

				if(isLetter((int)tokenName[i])){

					i++;
					state = '2';

				}else
					state = '0';


			break;

			case '2':

				if(isLetter((int)tokenName[i]) || isdigit((int)tokenName[i])){
					i++;
				}else{
					state = '0';
				}

			break;

		}

	}

	if(state == '2'){ //final state

		insToken(ID, tokenName);
		return TRUE;
	}

	return FALSE;

}


_Bool isConstantNumber(char *tokenName){

	char state = '1';
	int i = 0;

	while( (state == '1' || state == '2') && tokenName[i] != '\0'){

		switch(state){

			case '1':

				if(isDigit((int)tokenName[i])){

					i++;
					state = '2';

				}else
					state = '0';

			break;

			case '2':

				if(isDigit((int)tokenName[i]) && i < 10) //the CONSTANT_NUMBER has a limit of 10
					i++;
				else
					state = '0';

			break;
		}

	}

	if(state == '2'){ //final state

		insToken(CONSTANT_NUMBER, tokenName);
		return TRUE;

	}

	return FALSE;

}
