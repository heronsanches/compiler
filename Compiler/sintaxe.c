#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexical.h"
#include "sintaxe.h"


TokenTypePosition tokenPositions[] = {
		{DOLLAR, P_DOLLAR}, {PERCENT, P_PERCENT}, {LEFT_PARENTHESIS, P_LEFT_PARENTHESIS},
		{RIGHT_PARENTHESIS, P_RIGHT_PARENTHESIS}, {MULTIPLICATION, P_MULTIPLICATION}, {PLUS, P_PLUS},
		{COMMA, P_COMMA}, {MINUS, P_MINUS}, {DIVISION, P_DIVISION},
		{LESS, P_LESS}, {EQUAL, P_EQUAL}, {GREATER, P_GREATER}, {AS, P_AS}, {CONSTANT_CHARACTER, P_CTE_CHARACTER},
		{CONSTANT_NUMBER, P_CTE_NUM}, {CONSTANT_STRING, P_CTE_STRING}, {DECLARE, P_DECLARE}, {DIFFERENT, P_DIFFERENT},
		{DO, P_DO}, {ELSE, P_ELSE}, {FOR, P_FOR}, {FOREACH, P_FOREACH},	{FROM, P_FROM}, {ID, P_ID}, {IF, P_IF},
		{IN, P_IN}, {GREATER_OR_EQUAL, P_GREATER_EQUAL}, {LESS_OR_EQUAL, P_LESS_EQUAL}, {DOT, P_DOT},
		{PRINT, P_PRINT}, {PUT, P_PUT}, {READ, P_READ}, {RESIZE, P_RESIZE}, {THEN, P_THEN}, {TO, P_TO},
		{LEFT_BRACKET, P_LEFT_BRACKET},	{RIGHT_BRACKET, P_RIGHT_BRACKET}, {NUMBER, P_NUMBER}, {LETTER, P_LETTER}
};


NonTerminalPosition nonTerminalPositions[] ={
		{"cmds", P_CMDS}, {"cmd", P_CMD}, {"dec", P_DEC},
		{"av", P_AV}, {"atr", P_ATR}, {"dc", P_DC},
		{"for", P_FOR_NT}, {"fore", P_FORE_NT}, {"r", P_R},
		{"p", P_P}, {"s", P_S}, {"tip", P_TIP}, {"li", P_LI},
		{"lia", P_LIA},	{"op_li", P_OP_LI}, {"op_lia", P_OP_LIA},
		{"ea", P_EA}, {"sm", P_SM},	{"ea2", P_EA2}, {"mdp", P_MDP},
		{"ea3", P_EA3}, {"var", P_VAR},	{"vara", P_VARA}, {"atr", P_ATR},
		{"atra", P_ATRA}, {"atraa", P_ATRAA}, {"er", P_ER}, {"op_r", P_OP_R},
		{"dc", P_DC}, {"dca", P_DCA}, {"for", P_FOR_NT}, {"fora", P_FORA_NT},
		{"fore", P_FORE_NT}, {"forea", P_FOREA_NT}, {"ra", P_RA},
		{"pa", P_PA},	{"paa", P_PAA}
};


Cell table[153][72];


SymbolPosition getColumnToken(TokenType token){

	int i;
	for(i=0; i<QTDE_COLUMNS; i++){

		if(tokenPositions[i].type == token){
			return tokenPositions[i].position;
		}

	}

	return P_NOTHING;

}


SymbolPosition getColumnNonTerminal(char* name){

	int i;
	for(i=0; i<QTDE_COLUMNS; i++){

		if(!strcmp(name, nonTerminalPositions[i].name)){
			return nonTerminalPositions[i].position;
		}

	}

	return P_NOTHING;

}


void initializeTable(){

	FILE *sf;
	int c;
	char *tokenReaded =  NULL;
	int column = 0;
	int line = 0;
	int qc = 0;
	_Bool go = 1;

	/**
	 * feed_line means other line
	 * tab means other column
	 */
	if( (sf = fopen("parseTable.arq", "r")) != NULL ){
		tokenReaded = (char*)malloc(4);
		while(go){

			c = fgetc(sf);

			if(c == EOF){
				go = 0;
			}else if(c != 9 && c != 10){

				tokenReaded[qc] = (char)c;
				qc++;

			}else if(c == 9){

				if(qc != 0){
					tokenReaded[qc] = '\0';
					table[line][column].contentTable = tokenReaded;
					column++;
					tokenReaded = (char*)malloc(4);
					qc = 0;
				}

			}else if(c == 10){
				tokenReaded[qc] = '\0';
				//printf("\n%s", tokenReaded);

				table[line][column].contentTable = tokenReaded;
				column = 0;
				line++;
				tokenReaded = (char*)malloc(4);
				qc = 0;
				while(fgetc(sf) != 9);

			}

		}

		fclose(sf);

		//printf("imprimiuuu%s", table[139][34].contentTable);


	}else{
		printf("file not opened");
	}
}


void initializeProductions(){

	FILE *sf;
	int c;
	char *tokenReaded =  NULL;
	int qp = 1; //quantity of productions
	int qcp = 0; //quantity of characters of a production's side
	int qe = 0; //quantity of characters of a production's side
	_Bool go = 1;
	_Bool l = 1;

	productions[0].lposition = P_NOTHING;
	productions[0].rqtde = NOTHING;

	if( (sf = fopen("parseTableReductions.arq", "r")) != NULL ){

		tokenReaded = (char*)malloc(10);

		while(go){

			c = fgetc(sf);

			if(c == EOF){
				go = 0;
			}else if(c != '#' && l && c != 32){

				tokenReaded[qcp] = (char)c;
				qcp++;
				//printf("else if(c != '#' && l)\n");

			}else if(c == '#'){

				tokenReaded[qcp] = '\0';

				if(getColumnNonTerminal(tokenReaded) != P_NOTHING) //TODO remove this condition after test
					productions[qp].lposition = getColumnNonTerminal(tokenReaded);
				else
					printf("este non terminal nao existe.");

				//tokenReaded = (char*)malloc(10);
				//qcp = 0;
				l = 0;
				//printf("else if(c == '#')\n");

			}else if(!l && c != 10 && c != 32){

				if(c == 64)
					qe++;

				/*else{
					tokenReaded[qcp] = (char)c;
					qcp++;
				}*/
				//printf("else if(!l && c != 10 && c != 32)\n");

			}else if(c == 10){

				//tokenReaded[qcp] = '\0';
				productions[qp].rqtde = qe; //TODO armazenar numero elementos
				tokenReaded = (char*)malloc(10);
				qcp = 0;
				qe = 0;
				l=1;
				qp++;
				//printf("else if(c == 10)\n");
			}

		}

		fclose(sf);
		int i;
		for(i=1; i<70; i++)
			printf("r%d posicaoTable: %d -> qtdElements: %d\n", i, productions[i].lposition, productions[i].rqtde);
	}else{
		printf("Falha ao carregar produções, verificar arquivo \"parsetableReductions\"");
	}
}


void initializeStack(){
	stack = NULL;
}


int topStack(){

	return stack->value;

}


void push(int value){

	StackNode *partial;

	if(stack == NULL){

		stack = (StackNode*)malloc(sizeof(StackNode));
		stack->before = NULL;
		stack->next = NULL;
		stack->value = value;

	}else{

		partial = (StackNode*)malloc(sizeof(StackNode));
		partial->before = stack;
		partial->next = NULL;
		partial->value = value;
		stack->next = partial;
		stack = partial;

	}

}


void pop(int qtde){

	StackNode *partial;

	int i;

	for(i=0; i<qtde; i++){

		partial = stack;
		partial->before->next = NULL;
		stack = partial->before;
		free(partial);

	}

}


_Bool sAnalyzer(){

	initializeTable();
	initializeProductions();
	initializeStack();

	//algorithm slr(1)
	push(0);

	while(1){

		switch( table[topStack()][getColumnToken(getToken())] ){
			//TODO this switch stored a constant characater,
			//treat this information for "reduction" or "shift"
		case :

		break;
		}

	}


	return 1;
}
