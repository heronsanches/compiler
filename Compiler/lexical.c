/*
 * lexical.c
 *
 *  Created on: Sep 11, 2014
 *      Author: heron
 */
#include <stdio.h>
#include <stdlib.h>
#include "lexical.h"


void initializeTokens(){
	tokens = (Token*)malloc(sizeof(Token));
}

//TODO test phase
void lAnalyser(const char *fileName){

	FILE *sf; //source file

	if( (sf = fopen(fileName, "r")) != NULL ){

		int c; //value of character in it
		int i = 0;
		char *tokenReaded = (char*)malloc(INITIAL_TOKEN_BUFFER);

		while( (c = fgetc(sf)) != EOF ){

			//increase memory allocated for "tokenReaded"
			if(i > INITIAL_TOKEN_BUFFER-1){
				tokenReaded = (char*)realloc(tokenReaded, i + INITIAL_TOKEN_BUFFER);
			}

			//TODO make finite machines of each thing of the language
			//TODO go next if this character does not a separator
			//TODO if it is, save in "tokens"

			tokenReaded[i] = (char)c;
			i++;
		}

		printf("%s", tokenReaded);
		free(tokenReaded);
		fclose(sf);

	}else{
		printf("%s", "ERR \"Possivelmente este arquivo nao existe.\"\n"
				"<< Verifique o nome do arquivo\n");
	}

}
