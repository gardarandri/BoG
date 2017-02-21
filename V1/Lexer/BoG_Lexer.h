#include "BoG_Lexer_Generated.c"
#include "stdio.h"
#include "string.h"

#define BOG_FINISHED 999

int current_token;

// Notkun:	throw_error(s);
// Fyrir:	s er strengur
// Eftir:	Villuskilabo�in:
// 			line l, char c: Expected 's' but found 'r'\n
//			hafa veri� gefin �ar sem l er l�nun�mer lesgreinisins,
//			c er stafan�meri�, s er �a� sem b�ist var vi�
//			en r er �a� sem lesgreinirinn fann.
void throw_error(char* expected_token){
	printf("line %d, char %d: Expected '%s' but found '%s'\n",lexer_at_line,lexer_at_char,expected_token,yytext);
	exit(0);
}

// Notkun:	start_lexer(s);
// Fyrir:	s er strengur me� skr�arnafni
// Eftir:	Lesgreining er hafin � skr�nni s
void start_lexer(const char* input_file){
	yyin = fopen(input_file, "r");
	current_token = yylex();
}

// Notkun:	finish_lexing();
// Fyrir:	Lesgreining er � gangi
// Eftir:	Lesgreiningu er h�tt
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
// Eftir:	b er satt �.�.a.a. t�ki� sem lesgreinirinn
// 			er � hefur s�mu strengjaframsetningu og s
int lexeme_equals(char* query){
	return !strcmp(yytext,query);
}

// Notkun:	advance(b);
// Fyrir:	Lesgreining er � gangi
// Eftir:	Lesgreinirinn hefur labba� yfir 
// 			eitt t�k, Ef t�ki� er ��ekkt er gefinn
// 			villa og keyrslu h�tt
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
// Fyrir:	b er eitthva� BOG token
// Eftir:	Lesgreinirinn hefur labba� yfir 
// 			eitt t�k, ef �a� var ekki b h�tti
// 			forriti� keyrslu og gefur villu.
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
// Eftir:	Lesgreinirinn hefur labba� yfir 
// 			eitt t�k, ef �a� var ekki strengurinn s
// 			�� h�ttir forriti� keyrslu og gefur villu.
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


