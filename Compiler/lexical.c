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


//TODO test phase
void preLAnalyser(const char *fileName){

	FILE *sf; //source file
	initPreTokens();

	if( (sf = fopen(fileName, "r")) != NULL ){

		int c; //value of character in it
		int qc = 0; //quantity of characters read
		int qw = 0; //quantity of words read
		char *tokenReaded = (char*)malloc(INITIAL_TOKEN_BUFFER);

		//reading source file
		while( (c = fgetc(sf)) != EOF ){

			//increase memory allocated for "tokenReaded"
			if(qc > INITIAL_TOKEN_BUFFER-1){
				tokenReaded = (char*)realloc(tokenReaded, qc + 2); //+2: one char for save the "last separator(it has conditions)" and other to save '/0'
			}

			//is a separator? //TODO I must save the others separators besides them
			if(c == 32 || c == 9 || c == 10){ //space, tab, line feed

				tokenReaded[qc] = '\0';
				insPreToken(tokenReaded);

				tokenReaded = (char*)malloc(INITIAL_TOKEN_BUFFER);
				qc = 0;
				qw++;

			}else{

				tokenReaded[qc] = (char)c;
				qc++;

			}

		}

		//TODO remove this after tests' phases
		PreToken *pta = preTokens;
		for(; pta != NULL; pta = pta->next)
			printf("%s\n", pta->name);

		//TODO make free in "preTokens" after analyzer these tokens
		//free(preTokens);
		fclose(sf);

	}else{
		printf("%s", "ERR \"Possivelmente este arquivo nao existe.\"\n"
				"<< Verifique o nome do arquivo\n");
	}

}


/*//TODO
 //ID FINITE MACHINE
			char state = '1'; //beginning

			while(state == '1' || state == '2'){

				switch(state){

					case '1':

						if(isLetter(c)){

							tokenReaded[i] = (char)c;
							i++;
							state = '2';

						}else
							printf("%s\n", "Caractere invalido.");
					break;

					case '2':

						if(isLetter(c) || isdigit(c)){

						}

					break;

				}

			}
 */
