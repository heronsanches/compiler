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
#define QTDE_SEPARATORS_STRANGERS 15


typedef enum{
	DECLARE, AS, NUMBER, LETTER, PUT, IN, IF, THEN, ELSE,
	FOREACH, DO, FOR, FROM, TO, RESIZE, READ, PRINT, COMMA=44,
	DOT=46, LEFT_BRACKET=91, RIGHT_BRACKET=93, PLUS=43, MINUS=45, MULTIPLICATION=42,
	DIVISION=47, MOD=37, LEFT_PARENTHESIS=40, RIGHT_PARENTHESIS=41,	LESS=60,
	GREATER=62, LESS_OR_EQUAL, GREATER_OR_EQUAL, EQUAL=61, DIFFERENT, BAR=47, INVERTED_BAR=92,
	ID, SIMPLE_QUOTATION_MARK=39, DOUBLE_QUOTATION_MARK=34, CONSTANT_NUMBER,
	CONSTANT_CHARACTER, CONSTANT_STRING, SPACE=32, TAB=9, FEED_LINE=10, LETTER_T_LOWER=116, LETTER_N_LOWER=110,
	NOTHING=0, DOLLAR=36, PERCENT=37
}TokenType;

typedef struct{
	char *name;
	TokenType type;
}ReservedWord;

/*typedef enum{
	EXCLAMATION=33, OLD_GAME=35, DOLLAR=36, E_COMMERCIAL=38, COLON=58, SEMICOLON=59, INTERROGATION=63,
	AT=64, CIRCUMFLEX=94, UNDERLINE=95, CRASE=96, RIGHT_KEY=125, LEFT_KEY=123, TIO_ACCENT=126, UP_BAR=124
}StrangerType;*/

typedef enum{
	S1, S2, S3, S4, S5, S6, S7, S8, S9, S12, S14, S15, S17,
	S18, S20, S21, S23, S24, S10, S11, S13
}States;

//it is used to save a token after its validation
typedef struct Token{
	TokenType type;
	struct Token *next;
	union{
		char *name;
		long long int num; //TODO see more about ..
	}attribute;

}Token;


Token *tokens; //point to beginning the Token's list
Token *actualToken; //point to actual token of Token's list


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
TokenType isReseverdWord(char *name);
//_Bool isStrangerSeparator(int character);

int toLowercase(int c);
_Bool verifyingFile(const char *fileName); //it verifies characters invalids in the file, return true if do not exists invalid character

_Bool lAnalyzer(const char *fileName); //unique DFA
void freeMemory();

TokenType getToken(); //TODO

#endif /* LEXICAL_H_ */
