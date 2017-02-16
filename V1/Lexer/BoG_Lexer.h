
#include "BoG_Lexer_Generated.c"
#include "stdio.h"
#include "string.h"

#define BOG_FINISHED 999

int current_token;

void throw_error(char* expected_token){
	printf("line %d: Expected '%s' but found '%s'\n",lexer_at_line,expected_token,yytext);
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
#ifdef LEXER_DEBUG
	printf("walking over: ");
	printf("%s",yytext);
	printf("\n");
#endif

	current_token = yylex();

	if(current_token == BOG_ERROR){
		printf("line %d: An lexing error ocurred!\n",lexer_at_line);
		exit(0);
	}

	return current_token;
}

int advance_over_BOG(int bog_enum){
	if(current_token != bog_enum){
		// TODO: Geta gert custom errors
		throw_error(enum_to_str(bog_enum));
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


