/*
 * lexical.h
 *
 *  Created on: Sep 6, 2014
 *      Author: heron sanches
 *
 *  ID: [a-zA-Z]([a-zA-Z]|[0-9])*
 *  CONSTANT_NUMBER: [0-9]+     MÁXIMO DE 10 REPETIÇÕESS
 *  CONSTANT_CHARACTER:
 *  CONSTANT_STRING:            MÁXIMO 256 CHARACTERES
 *
 */

#ifndef LEXICAL_H_
#define LEXICAL_H_


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
typedef struct{
	TokenType type;
	union{
		char* name;
		unsigned long long num;
	}attribute;

}Token;

//it saves tokens already verified (if it belongs to language)
Token *tokens;

#define INITIAL_TOKEN_BUFFER 20


//********function's prototype********
void initializeTokens(); //TODO verify
void lAnalyser(const char *fileName); //TODO verify



#endif /* LEXICAL_H_ */
