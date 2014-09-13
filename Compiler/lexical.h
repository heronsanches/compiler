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
	struct PreToken *next;
}PreToken;

Token *tokens;
Token *actualToken;

PreToken *preTokens;
PreToken *actualPreToken;


//********function's prototype********
void initTokens(); //initialize "tokens" and "actualToken"
void initPreTokens(); //initialize "preTokens" and "actualPretoken"
_Bool insPreToken(char *name);
_Bool insToken(TokenType tokenType, PreToken *preToken);
_Bool isLetter(int letter);
_Bool isDigit(int character);
void preLAnalyzer(const char *fileName); //TODO verify
_Bool lAnalyzer(const char *fileName);

//finite automatons
_Bool isID(PreToken *preToken);
_Bool isConstantNumber(PreToken *preToken);



#endif /* LEXICAL_H_ */
