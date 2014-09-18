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
#define TRUE 1
#define FALSE 0
#define QTDE_RESERVED_WORDS 17


typedef enum{
	DECLARE, AS, NUMBER, LETTER, PUT, IN, IF, THEN, ELSE,
	FOREACH, DO, FOR, FROM, TO, RESIZE, READ, PRINT, COMMA,
	DOT, LEFT_BRACKET, RIGHT_BRACKET, PLUS, MINUS, MULTIPLICATION,
	DIVISION, MOD, LEFT_PARENTHESIS, RIGHT_PARENTHESIS,	LESS,
	GREATER, LESS_OR_EQUAL, GREATER_OR_EQUAL, EQUAL, DIFFERENT,
	ID, SIMPLE_QUOTATION_MARK, DOUBLE_QUOTATION_MARK, CONSTANT_NUMBER,
	CONSTANT_CHARACTER, CONSTANT_STRING
}TokenType;

//SN = State Null
typedef enum{
	S1, S2, S3, S4, S5, S6, S7, S8, S9, S10, S11, S12, S13, S14, S15, S16, S17,
	S18, S19, S20, S21, S22, S23, S24, S25, S26
}States;

//it is used to save a token after its validation
typedef struct Token{
	TokenType type;
	struct Token *next;
	union{
		char *name;
		unsigned long long num; //TODO see more about .. at least 64bit
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


//********function's prototype********
void initTokens(); //initialize "tokens" and "actualToken"
void initPreTokens(); //initialize "preTokens" and "actualPretoken"
_Bool insPreToken(char *name, int line);
_Bool insToken(TokenType tokenType, char *tokenName);

_Bool isLetter(int letter);
_Bool isNumberDigit(int character);
_Bool isPrintableCharacter(int character);
_Bool isSeparatorCharacter(int character);

int toLowercase(int c);

void lAnalyzer(const char *fileName); //TODO unique DFA
//_Bool lAnalyzer(const char *fileName);





#endif /* LEXICAL_H_ */
