#include "sintaxe.h"

/* TODO organize this code bellow, it take the productions from file
FILE *sf; //source file
	int c;
	char *tokenReaded =  NULL;
	int qp = 1; //quantity of productions
	int qcp = 0;
	_Bool go = 1;
	_Bool l = 1;

	typedef struct{
		char* lp;
		char* rp;
	}Production;

	//begin this from productions[1] until productions[67]
	Production productions[68];
	productions[0].lp = NULL;
	productions[0].rp = NULL;

	if( (sf = fopen("parsetableReductions.arq", "r")) != NULL ){

		tokenReaded = (char*)malloc(10);

		while(go){

			c = fgetc(sf);

			if(c == EOF){
				go = 0;
			}else if(c != '#' && l){

				tokenReaded[qcp] = (char)c;
				qcp++;

			}else if(c == '#'){

				tokenReaded[qcp] = '\0';
				productions[qp].lp = tokenReaded;
				tokenReaded = (char*)malloc(10);
				qcp = 0;
				l = 0;

			}else if(!l && c != 10){

				tokenReaded[qcp] = (char)c;
				qcp++;

			}else if(c == 10){

				tokenReaded[qcp] = '\0';
				productions[qp].rp = tokenReaded;
				tokenReaded = (char*)malloc(10);
				qcp = 0;
				l=1;
				qp++;

			}

		}

		fclose(sf);

		int i;
		for(i=1; i<=qp; i++){
			printf("r%d: %s -> %s\n",i, productions[i].lp, productions[i].rp);
		}

	}else{
		printf("file not opened");
	}
*/
