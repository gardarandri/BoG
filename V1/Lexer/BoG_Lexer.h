
#include "BoG_Lexer_Generated.c"
#include "stdio.h"
#include "string.h"

#define BOG_FINISHED 999

int current_token;

void start_lexer(){
	current_token = yylex();
}

int finish_lexing(){
	return BOG_FINISHED;
}

int current_lexeme_size(){
	return sizeof(char) * strlen(yytext);
}

void current_lexeme(char* write_to){
	strcpy(write_to,yytext);
}

int lexeme_equals(char* query){
	return !strcmp(yytext,query);
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
	if(strcmp(yytext,targ)){
		// yytext != targ
		printf("Expected '%s' but found '%s'\n",targ,yytext);
		finish_lexing();
		return BOG_ERROR;
	}

	return advance();
}


