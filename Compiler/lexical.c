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

//TODO struct RelationalOperators
//TODO struct RelationalOperators


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


_Bool isPrintableCharacter(int character){

	if(character >= 32 && character <= 126)
		return TRUE;

	return FALSE;
}


_Bool isSeparatorCharacter(int character){

	_Bool is = FALSE;

	for(int i=0; i<QTDE_SEPARATORS; i++){

		if(character == separatorsCharacter[i]){

			is = TRUE;
			break;

		}

	}

	return is;

}


int toLowercase(int c){

	if(c >= 65 && c <= 90) //uppercase
		return c + 32;

	return c;
}


//TODO test phase
void lAnalyzer(const char *fileName){

	FILE *sf; //source file
	initPreTokens();
	initTokens();

	if( (sf = fopen(fileName, "r")) != NULL ){

		int c; //value of character in it
		int qc = 0; //quantity of characters read
		int al = 1; //actual line
		char *tokenReaded = (char*)malloc(INITIAL_TOKEN_BUFFER);

		States s = S1;
		_Bool outWhileDFA = FALSE;
		_Bool outWhileRead = FALSE;

		//Read
		while( (c = fgetc(sf)) != EOF ){

			//increase memory allocated for "tokenReaded"
			if(qc > INITIAL_TOKEN_BUFFER-1){
				tokenReaded = (char*)realloc(tokenReaded, qc + 1);
			}

			//TODO DFA
			while( s == S1 || s == S2 || s == S3 || s == S4 || s == S5 || s == S6 ||
					s == S7 || s == S8 || s == S9 || s == S10 || s == S11 || s == S12 ||
					s == S13 || s == S14 || s == S15 || s == S16 || s == S17 || s == S18 ||
					s == S19 || s == S20 || s == S21 || s == S22 || s == S23 || s == S24 ||
					s == S25 || s == S26 ){

				tokenReaded[qc] = (char)c;

				switch(s){

					case s == S1:

						if(isLetter(c)){

							s = S2;
							qc++;

						}else if(isNumberDigit(c)){

							s = S3;
							qc++;

						}else if(c == 39){ //simple quote

							s = S4;
							qc++;

						}else if(c == 34){ //quotation marks

							s = S5;
							qc++;

						}else if(c == 60){ //less than

							s = S23;
							qc++;

						}else if(c == 62){ //greater than

							s = S20;

						}else if(isSeparatorCharacter(c)){

							s = S18;

						}else{

							tokenReaded[qc+1] = '\0';
							printf("%s", "ERR \"nao pertence a linguagem\"\n"
									"<< Deve-se usar: nomedoPrograma nomeArquivoCodigofonte\n");
							s = SN;
						}

					break;
				}

			}




		}

		fclose(sf);

	}else{
		printf("ERR \"Possivelmente o arquivo --%s-- nao existe.\"\n"
				"<< Verifique o nome do arquivo\n", fileName);
	}

}


/*_Bool lAnalyzer(const char *fileName){

	preLAnalyzer(fileName);
	actualPreToken = preTokens;

	//traverses the list "preTokens" for match elements the language
	for(; actualPreToken != NULL; actualPreToken = actualPreToken->next){

		//TODO igonre the others, tab and \n
		//*actualPreToken->name == '\0'; it ignores blank spaces
		if( *actualPreToken->name == '\0' || isID(actualPreToken->name) || isConstantNumber(actualPreToken->name)
				|| isConstantCharacter(actualPreToken->name) ) // TODO or, is the others finite machines
			;
		else{
			printf("ERR \"Token nao reconhecido em -- %s --\"\n<<linha: %d   token: %s\n", fileName,
					actualPreToken->line, actualPreToken->name);

			return FALSE;
		}

	}

	return TRUE;

}*/
