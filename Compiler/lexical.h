/*
 * lexical.h
 *
 *  Created on: Sep 6, 2014
 *      Author: heron sanches
 *
 *  ID: [a-zA-Z]([a-zA-Z]|[0-9])*
 *  CONSTANT_NUMBER: [0-9]+     MÁXIMO DE 10 REPETIÇÕESS
 *  CONSTANT_CHARACTER: ''
 *  CONSTANT_STRING:   ""         MÁXIMO 256 CHARACTERES
 *
 */

#ifndef LEXICAL_H_
#define LEXICAL_H_

#define INITIAL_TOKEN_BUFFER 20
#define TRUE 1;
#define FALSE 0;


typedef enum{
	DECLARE, AS, NUMBER, LETTER, PUT, IN, IF, THEN, ELSE,
	FOREACH, DO, FOR, FROM, TO, RESIZE, READ, PRINT, COMMA,
	DOT, LEFT_BRACKET, RIGHT_BRACKET, PLUS, MINUS, MULTIPLICATION,
	DIVISION, MOD, LEFT_PARENTHESIS, RIGHT_PARENTHESIS,	LESS,
	GREATER, LESS_OR_EQUAL, GREATER_OR_EQUAL, EQUAL, DIFFERENT,
	ID, SIMPLE_QUOTATION_MARK, DOUBLE_QUOTATION_MARK, CONSTANT_NUMBER,
	CONSTANT_CHARACTER, CONSTANT_STRING
}TokenType;

//it is used to save a token after its validation
typedef struct Token{
	TokenType type;
	struct Token *next;
	union{
		char *name;
		unsigned long long num;
	}attribute;

}Token;

//it is used to save tokens yet not validated
typedef struct PreToken{
	char *name;
	int line;
	struct PreToken *next;
}PreToken;

Token *tokens;
Token *actualToken;

PreToken *preTokens;
PreToken *actualPreToken;

const int separatorsCharacter[] = {32, 10, 9, 44, 46, 91, 93, 43, 45, 42, 47, 37, 40, 41, 61, 60, 62}; //size = 17


//********function's prototype********
void initTokens(); //initialize "tokens" and "actualToken"
void initPreTokens(); //initialize "preTokens" and "actualPretoken"
_Bool insPreToken(char *name, int line);
_Bool insToken(TokenType tokenType, char *tokenName);

_Bool isLetter(int letter);
_Bool isDigit(int character);
_Bool isPrintableCharacter(int character);
_Bool isSeparatorCharacter(int character);

void preLAnalyzer(const char *fileName); //TODO verify
_Bool lAnalyzer(const char *fileName);

//finite automatons
_Bool isID(char *tokenName);
_Bool isConstantNumber(char *tokenName);
_Bool isConstantCharacter(char *tokenName);
_Bool isConstantString(char *tokenReaded); //TODO



#endif /* LEXICAL_H_ */
