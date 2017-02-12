
#include "BoG_Lexer_Generated.c"
#include "stdio.h"
#include "string.h"

int current_token;

void start_lexer(){
	current_token = yylex();
}

int current_token(){
	return current_token;
}

int advance(){
	current_token = yylex();

	if(current_token == BOG_ERROR){
		printf("An lexing error ocurred!\n");
		finish_lexing();
		return BOG_ERROR;
	}

	if(current_token == 0) return finish_lexing();

	return current_token;
}

int advance_over(char* targ){
	current_token = yylex();

	if(strcmp(yytext,targ)){
		// yytext != targ
		printf("Expected '%s' but found '%s'\n",targ,yytext);
		finish_lexing();
		return BOG_ERROR;
	}

	if(current_token == BOG_ERROR){
		printf("An lexing error ocurred!\n");
		finish_lexing();
		return BOG_ERROR;
	}

	if(current_token == 0) return finish_lexing();

	return current_token;
}

int main(){
}
