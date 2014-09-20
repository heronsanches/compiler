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
#define QTDE_SEPARATORS 15


typedef enum{
	DECLARE, AS, NUMBER, LETTER, PUT, IN, IF, THEN, ELSE,
	FOREACH, DO, FOR, FROM, TO, RESIZE, READ, PRINT, COMMA=44,
	DOT=46, LEFT_BRACKET=91, RIGHT_BRACKET=93, PLUS=43, MINUS=45, MULTIPLICATION=42,
	DIVISION=47, MOD=37, LEFT_PARENTHESIS=40, RIGHT_PARENTHESIS=41,	LESS=60,
	GREATER=62, LESS_OR_EQUAL, GREATER_OR_EQUAL, EQUAL=61, DIFFERENT, BAR=47, INVERTED_BAR=92,
	ID, SIMPLE_QUOTATION_MARK=39, DOUBLE_QUOTATION_MARK=34, CONSTANT_NUMBER,
	CONSTANT_CHARACTER, CONSTANT_STRING, SPACE=32, TAB=9, FEED_LINE=10, LETTER_T_LOWER=116, LETTER_N_LOWER=110,
	NOTHING=0
}TokenType;

typedef enum{
	S1, S2, S3, S4, S5, S6, S7, S8, S9, S12, S14, S15, S17,
	S18, S20, S21, S23, S24
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


Token *tokens; //point to beginning the Token's list
Token *actualToken; //point to beginning the Token's list


//********function's prototype********
void initTokens(); //initialize "tokens" and "actualToken"
_Bool insToken(TokenType tokenType, char *tokenName);

_Bool isLetter(int letter);
_Bool isNumberDigit(int character);
_Bool isPrintableAll(int character);
_Bool isPrintableL(int character);
TokenType isSeparatorL(int character); //less < ' > " characters
TokenType isSeparatorAll(int character); //include all symbols separators
_Bool isSE(int character); // t n \ ' "

int toLowercase(int c);

void lAnalyzer(const char *fileName); //TODO unique DFA
void freeMemory();




#endif /* LEXICAL_H_ */
