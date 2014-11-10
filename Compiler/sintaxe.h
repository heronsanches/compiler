
#ifndef SINTAXE_H_
#define SINTAXE_H_

#define QTDE_COLUMNS 70
#define QTDE_ROWS 151

/** column positions of terminals and non-terminals */
typedef enum {
	P_DOLLAR=0, P_PERCENT, P_LEFT_PARENTHESIS, P_RIGHT_PARENTHESIS,
	P_MULTIPLICATION, P_SUM, P_PLUS, P_COMMA, P_MINUS, P_DIVISION,
	P_LESS, P_EQUAL, P_GREATER, P_AS, P_CTE_CHARACTER, P_CTE_NUM,
	P_CTE_STRING, P_DECLARE, P_DIFFERENT, P_DO, P_ELSE, P_FOR, P_FOREACH,
	P_FROM, P_ID, P_IF, P_IN, P_GREATER_EQUAL, P_LESS_EQUAL, P_DOT,
	P_PRINT, P_PUT, P_READ, P_RESIZE, P_THEN, P_TO, P_LEFT_BRACKET,
	P_RIGHT_BRACKET, P_ATR, P_ATRA, P_ATRAA, P_AV, P_CMD, P_CMDS, P_DC,
	P_DCA, P_DEC, P_EA, P_EA2, P_EA3, P_ER, P_FOR_NT, P_FORA_NT,
	P_FORE_NT, P_FOREA_NT, P_LI, P_LIA, P_MDP, P_OP_LI, P_OP_LIA, P_OP_R,
	P_P, P_PA, P_PAA, P_R, P_RA, P_S, P_SM, P_VAR, P_VARA
}SymbolPosition;


typedef struct{
	TokenType type;
	SymbolPosition position;
}TokenTypePosition;

/**//TODO
 * jump a line, so beginnig populate the table from state 0 and go through from o column to 150
 * feed_line means other line
 * tab means other column
 */
char *table[QTDE_ROWS][QTDE_COLUMNS]; //TODO populate table from file
char *stack; //TODO

void push(); //TODO
void pop(); //TODO
char* topStack(); //TODO


#endif /* SINTAXE_H_ */
