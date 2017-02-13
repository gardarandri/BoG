
#include "BoG_Lexer_Generated.c"
#include "stdio.h"
#include "string.h"

#define BOG_FINISHED 999

int current_token;

void throw_error(char* expected_token){
	printf("Expected '%s' but found '%s'\n",expected_token,yytext);
	exit(0);
}

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
		exit(0);
	}

	if(current_token == BOG_EOF) exit(0);

	return current_token;
}

int advance_over_BOG(int BOG, char* expected_token_name){
	if(current_token != bog_enum){
		// TODO: Geta gert custom errors
		throw_error(expected_token_name);
	}
	advance();
}

int advance_over_str(char* targ){
	if(strcmp(yytext,targ)){
		// yytext != targ
		throw_error(targ);
	}
	advance();
}


