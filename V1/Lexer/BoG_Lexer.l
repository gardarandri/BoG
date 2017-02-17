
%{
#define BOG_VAR 1000
#define BOG_IF 1001
#define BOG_ELSE 1002
#define BOG_ELIF 1003
#define BOG_WHILE 1004
#define BOG_RETURN 1005
#define BOG_FUN 1006
#define BOG_DELIMITER 1007
#define BOG_OPNAME 1008
#define BOG_INT 1009
#define BOG_FLOAT 1010
#define BOG_BOOLEAN 1011
#define BOG_STRING 1012
#define BOG_NAME_ENUM 1013
#define BOG_LITERAL 1014
#define BOG_ERROR (-1)
#define BOG_EOF 0

#ifdef BOG_DEBUG
	#define BOG_KEYWORD(E) printf(#E); printf(" '%s'\n",yytext);
	#define BOG_SPECIAL_CHAR(E) printf(#E); printf(" '%s'\n",yytext);
	#define BOG_OPERATOR(E) printf(#E); printf(" '%s'\n",yytext);
	#define BOG_LITERALS(E) printf(#E); printf(" '%s'\n",yytext);
	#define BOG_NAME(E) printf(#E); printf(" '%s'\n",yytext);
	#define BOG_REPORT printf("Found unrecognized token: '%s'\n",yytext); exit(BOG_ERROR);
#else
	#define BOG_KEYWORD(E) lexer_incr(); return E;
	#define BOG_SPECIAL_CHAR(E) lexer_incr(); return E;
	#define BOG_OPERATOR(E) lexer_incr(); return E;
	#define BOG_LITERALS(E) lexer_incr(); return E;
	#define BOG_NAME(E) lexer_incr(); return E;
	#define BOG_REPORT return BOG_ERROR;
#endif

#define BOG_MAKE_CASE(E) case E: return #E;

char* enum_to_str(int bog_enum){
	switch(bog_enum){
		BOG_MAKE_CASE(BOG_VAR)
		BOG_MAKE_CASE(BOG_IF)
		BOG_MAKE_CASE(BOG_ELSE)
		BOG_MAKE_CASE(BOG_ELIF)
		BOG_MAKE_CASE(BOG_WHILE)
		BOG_MAKE_CASE(BOG_RETURN)
		BOG_MAKE_CASE(BOG_FUN)
		BOG_MAKE_CASE(BOG_DELIMITER)
		BOG_MAKE_CASE(BOG_OPNAME)
		BOG_MAKE_CASE(BOG_INT)
		BOG_MAKE_CASE(BOG_FLOAT)
		BOG_MAKE_CASE(BOG_BOOLEAN)
		BOG_MAKE_CASE(BOG_STRING)
		BOG_MAKE_CASE(BOG_NAME_ENUM)
		BOG_MAKE_CASE(BOG_LITERAL)
		BOG_MAKE_CASE(BOG_ERROR)
		BOG_MAKE_CASE(BOG_EOF)
		default: return "Unrecognized token";
	}
}

int lexer_at_line = 1;
int lexer_at_char = 0;
int lexer_last_token_len = 0;

void lexer_incr(){
	lexer_at_char += lexer_last_token_len;
	lexer_last_token_len = strlen(yytext);
}

%}

NEWLINE			\n
WS				[ \t\r\f]
INTEGER			[0-9]+
FLOAT			[0-9]+\.[0-9]+([Ee][+-]?[0-9]+)?
BOOLEAN			(true)|(false)
NAME			[a-zA-Z_][a-zA-Z0-9_]*
OPNAME			[+-/*%<>=|!&]+
DELIMITERS		[()\{\};.,]
STRING			\"([^\"\n\r\f])*\"

COMMENT			;;;.*

%%

{NEWLINE}		lexer_at_char = 0; lexer_at_line++;
{COMMENT}		lexer_incr(); /* Do nothing */
{WS}			lexer_incr(); /* Do nothing */

var				BOG_KEYWORD(BOG_VAR)
if				BOG_KEYWORD(BOG_IF)
else			BOG_KEYWORD(BOG_ELSE)
while			BOG_KEYWORD(BOG_WHILE)
return			BOG_KEYWORD(BOG_RETURN)
fun				BOG_KEYWORD(BOG_FUN)

{DELIMITERS}	BOG_SPECIAL_CHAR(BOG_DELIMITER)

{OPNAME}		BOG_OPERATOR(BOG_OPNAME)

{INTEGER}		BOG_LITERALS(BOG_LITERAL)
{FLOAT}			BOG_LITERALS(BOG_LITERAL)
{BOOLEAN}		BOG_LITERALS(BOG_LITERAL)
{STRING}		BOG_LITERALS(BOG_LITERAL)

{NAME}			BOG_NAME(BOG_NAME_ENUM)

.				BOG_REPORT


