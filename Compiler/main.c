/*
 * main.c
 *
 *  Created on: Sep 8, 2014
 *      Author: heron sanches
 */
#include <stdio.h>
#include "lexical.h"


int main(int argc, char *argv[]){

	if(argc != 2){
		printf("%s", "ERR \"numero de parametros incorreto\"\n"
				"<< Deve-se usar: nomedoPrograma nomeArquivoCodigofonte\n");
	}else{

		lAnalyzer(argv[1]);

		//TODO to make a function to free all memory allocated by program

	}

	//TODO verifying how the operational system empty memory after exit program or before
	return 0;
}

