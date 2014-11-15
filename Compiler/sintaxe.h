#ifndef SINTAXE_H_
#define SINTAXE_H_

#include "lexical.h"

#define QTDE_COLUMNS 74
#define QTDE_ROWS 159
#define QTDE_PRODUCTIONS 72 //but in real this beginning from 1 to 72

/** column positions of terminals and non-terminals */
typedef enum {
	P_DOLLAR, P_PERCENT, P_LEFT_PARENTHESIS, P_RIGHT_PARENTHESIS,
	P_MULTIPLICATION, P_PLUS, P_COMMA, P_MINUS, P_DIVISION,
	P_LESS, P_EQUAL, P_GREATER, P_AS, P_CTE_CHARACTER, P_CTE_NUM,
	P_CTE_STRING, P_DECLARE, P_DIFFERENT, P_DO, P_ELSE, P_FOR, P_FOREACH,
	P_FROM, P_ID, P_IF, P_IN, P_LETTER, P_GREATER_EQUAL, P_LESS_EQUAL, P_NUMBER,
	P_DOT, P_PRINT, P_PUT, P_READ, P_RESIZE, P_THEN, P_TO, P_LEFT_BRACKET,
	P_RIGHT_BRACKET, P_ATR, P_ATRA, P_ATRAA, P_AV, P_CMD, P_CMDS, P_DC,
	P_DCA, P_DEC, P_EA, P_EA2, P_EA3, P_EA3A, P_ER, P_FOR_NT, P_FORA_NT,
	P_FORE_NT, P_FOREA_NT, P_LI, P_LIA, P_MDP, P_OP_LI, P_OP_LIA, P_OP_R,
	P_P, P_PA, P_PAA, P_R, P_RA, P_RB, P_S, P_SM, P_TIP, P_VAR, P_VARA, P_NOTHING=-1
}SymbolPosition;


typedef struct{
	TokenType type;
	SymbolPosition position;
}TokenTypePosition;


typedef struct{
	char* name;
	SymbolPosition position;
}NonTerminalPosition;


typedef struct{
	SymbolPosition lposition; //the location on table of the left side of production
	int rqtde; //number of elements
}Production;


typedef struct{
	char* contentTable;
}Cell;


typedef struct StackNode{
	struct StackNode *next;
	struct StackNode *before;
	int value;
}StackNode;

StackNode *stack;
Production productions[QTDE_PRODUCTIONS];

void initializeStack();
void push();
void pop();
int topStack();
void initializeTable();
void initializeProductions();
SymbolPosition getColumnToken(TokenType token);
char getBeginning(char* name, int* go);
int getGoto(char* name);
TokenType getToken();
_Bool sAnalyzer();//TODO



#endif /* SINTAXE_H_ */
