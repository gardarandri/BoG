#include "BoG_Lexer_Generated.c"
#include "stdio.h"
#include "string.h"

#define BOG_FINISHED 999

int current_token;

// Notkun:	throw_error(s);
// Fyrir:	s er strengur
// Eftir:	Villuskilaboðin:
// 			line l, char c: Expected 's' but found 'r'\n
//			hafa verið gefin þar sem l er línunúmer lesgreinisins,
//			c er stafanúmerið, s er það sem búist var við
//			en r er það sem lesgreinirinn fann.
void throw_error(char* expected_token){
	printf("line %d, char %d: Expected '%s' but found '%s'\n",lexer_at_line,lexer_at_char,expected_token,yytext);
	exit(0);
}

// Notkun:	start_lexer(s);
// Fyrir:	s er strengur með skráarnafni
// Eftir:	Lesgreining er hafin á skránni s
void start_lexer(const char* input_file){
	yyin = fopen(input_file, "r");
	current_token = yylex();
}

// Notkun:	finish_lexing();
// Fyrir:	Lesgreining er í gangi
// Eftir:	Lesgreiningu er hætt
int finish_lexing(){
	fclose(yyin);
	return BOG_FINISHED;
}

/*
int current_lexeme_size(){
	return sizeof(char) * strlen(yytext);
}
*/

/*
void current_lexeme(char* write_to){
	strcpy(write_to,yytext);
}
*/

// Notkun:	b = lexeme_equals(s)
// Fyrir:	s er strengur
// Eftir:	b er satt þ.þ.a.a. tókið sem lesgreinirinn
// 			er á hefur sömu strengjaframsetningu og s
int lexeme_equals(char* query){
	return !strcmp(yytext,query);
}

// Notkun:	advance(b);
// Fyrir:	Lesgreining er í gangi
// Eftir:	Lesgreinirinn hefur labbað yfir 
// 			eitt tók, Ef tókið er óþekkt er gefinn
// 			villa og keyrslu hætt
int advance(
#ifdef LEXER_DEBUG
		int parser_line
#endif
		){
#ifdef LEXER_DEBUG
	printf("parser line %d: walking over: ", parser_line);
	printf("%s",yytext);
	printf("\n");
#endif

	current_token = yylex();

	if(current_token == BOG_ERROR){
		printf("line %d: An lexing error ocurred! Could not lex '%s'.\n",lexer_at_line,yytext);
		exit(0);
	}

	return current_token;
}

// Notkun:	advance_over_BOG(b);
// Fyrir:	b er eitthvað BOG token
// Eftir:	Lesgreinirinn hefur labbað yfir 
// 			eitt tók, ef það var ekki b hætti
// 			forritið keyrslu og gefur villu.
int advance_over_BOG(
#ifdef LEXER_DEBUG
		int parser_line,
#endif
		int bog_enum){
	if(current_token != bog_enum){
		throw_error(enum_to_str(bog_enum));
	}
	advance(
#ifdef LEXER_DEBUG
			parser_line
#endif
		   );
}


// Notkun:	advance_over_srt(s);
// Fyrir:	s er strengur
// Eftir:	Lesgreinirinn hefur labbað yfir 
// 			eitt tók, ef það var ekki strengurinn s
// 			þá hættir forritið keyrslu og gefur villu.
int advance_over_str(
#ifdef LEXER_DEBUG
		int parser_line,
#endif
		char* targ){
	if(strcmp(yytext,targ)){
		// yytext != targ
		throw_error(targ);
	}
	advance(
#ifdef LEXER_DEBUG
			parser_line
#endif
			);
}


