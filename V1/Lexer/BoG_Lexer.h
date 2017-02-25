#include "string.h"
#include "BoG_Lexer_Generated.c"
#include "stdio.h"

#define BOG_FINISHED 999
#define BOG_LEXEME_MAX_LEN 4096

// Breyta sem inniheldur t�ki� sem lesgreinirinn labbar n�st yfir
int current_token;
// Breyta sem inniheldur t�ki� sem lesgreinirinn labbar �ar n�st yfir
int next_token;

char current_lexeme[BOG_LEXEME_MAX_LEN];

// Notkun:	throw_error(s);
// Fyrir:	s er strengur
// Eftir:	Villuskilabo�in:
// 			line l, char c: Expected 's' but found 'r'\n
//			hafa veri� gefin �ar sem l er l�nun�mer lesgreinisins,
//			c er stafan�meri�, s er �a� sem b�ist var vi�
//			en r er �a� sem lesgreinirinn fann.
void throw_error(char* expected_token){
	printf("line %d, char %d: Expected '%s' but found '%s'\n",lexer_at_line,lexer_at_char,expected_token,current_lexeme);
	exit(0);
}

// Notkun:	copy_lexeme_over()
// Fyrir:	Lesgreining er � gangi
// Eftir:	B�i� er a� afrita yytext � breytuna
// 			current_lexeme e�a gefa villu ef yytext er of
// 			langur strengur.
void copy_lexeme_over(){
	int i=0;
	while(1){
		if(i >= BOG_LEXEME_MAX_LEN){
			printf("line %d, char %d: Lexeme to large! (This might be caused by a literal being to long)",lexer_at_line,lexer_at_char);
			exit(1);
		}
		current_lexeme[i] = yytext[i];
		if(yytext[i] == '\0') break;
		i++;
	}
}

// Notkun:	start_lexer(s);
// Fyrir:	s er strengur me� skr�arnafni
// Eftir:	Lesgreining er hafin � skr�nni s
void start_lexer(const char* input_file){
	yyin = fopen(input_file, "r");
	if(!yyin){
		printf("%s: Fatal: No such file!\n", input_file);
		exit(1);
	}
	current_token = yylex();
	copy_lexeme_over();
	next_token = yylex();
}

// Notkun:	finish_lexing();
// Fyrir:	Lesgreining er � gangi
// Eftir:	Lesgreiningu er h�tt
int finish_lexing(){
	fclose(yyin);
	return BOG_FINISHED;
}

int current_lexeme_size(){
	return sizeof(char) * (strlen(yytext)+1);
}

void write_current_lexeme_to(char* write_to){
	strcpy(write_to,yytext);
}


// Notkun:	b = lexeme_equals(s)
// Fyrir:	s er strengur
// Eftir:	b er satt �.�.a.a. t�ki� sem lesgreinirinn
// 			er � hefur s�mu strengjaframsetningu og s
int lexeme_equals(char* query){
	return !strcmp(current_lexeme,query);
}

// Notkun:	b = next_lexeme_equals(s)
// Fyrir:	s er strengur
// Eftir:	b er satt �.�.a.a. t�ki� sem lesgreinirinn
// 			er � eftir current_token hefur s�mu strengjaframsetningu og s
int next_lexeme_equals(char* query){
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

	current_token = next_token;
	copy_lexeme_over();
	next_token = yylex();

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
	return advance(
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
	if(strcmp(current_lexeme,targ)){
		// yytext != targ
		throw_error(targ);
	}
	return advance(
#ifdef LEXER_DEBUG
			parser_line
#endif
			);
}


