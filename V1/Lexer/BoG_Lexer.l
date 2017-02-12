
%{
#define BOG_VAR 1000
#define BOG_IF 1001
#define BOG_ELSE 1002
#define BOG_WHILE 1003
#define BOG_RETURN 1004
#define BOG_FUN 1005
#define BOG_DELIMITER 1006
#define BOG_OP 1007
#define BOG_INT 1008
#define BOG_FLOAT 1009
#define BOG_BOOLEAN 1010
#define BOG_STRING 1011
#define BOG_NAME_ENUM 1012
#define BOG_ERROR (-1)

#ifdef BOG_DEBUG
	#define BOG_KEYWORD(E) printf(#E); printf(" '%s'\n",yytext);
	#define BOG_SPECIAL_CHAR(E) printf(#E); printf(" '%s'\n",yytext);
	#define BOG_OPERATOR(E) printf(#E); printf(" '%s'\n",yytext);
	#define BOG_LITERALS(E) printf(#E); printf(" '%s'\n",yytext);
	#define BOG_NAME(E) printf(#E); printf(" '%s'\n",yytext);
	#define BOG_REPORT printf("Found unrecognized token: '%s'\n",yytext); exit(BOG_ERROR);
#else
	#define BOG_KEYWORD(E) return E;
	#define BOG_SPECIAL_CHAR(E) return E;
	#define BOG_OPERATOR(E) return E;
	#define BOG_LITERALS(E) return E;
	#define BOG_NAME(E) return E;
	#define BOG_REPORT return BOG_ERROR;
#endif
%}

WS				[ \t\n\r\f]
INTEGER			[0-9]+
FLOAT			[0-9]+\.[0-9]+([Ee][+-]?[0-9]+)?
BOOLEAN			(true)|(false)
NAME			[a-zA-Z_][a-zA-Z0-9_]*
OPNAME			[+-/*%<>=|]+
DELIMITERS		[()\{\};.,]
STRING			\"([^\"\n\r\f])*\"

COMMENT			;;;.*

%%

{COMMENT}		/* Do nothing */
{WS}			/* Do nothing */

var				BOG_KEYWORD(BOG_VAR)
if				BOG_KEYWORD(BOG_IF)
else			BOG_KEYWORD(BOG_ELSE)
while			BOG_KEYWORD(BOG_WHILE)
return			BOG_KEYWORD(BOG_RETURN)
fun				BOG_KEYWORD(BOG_FUN)

{DELIMITERS}	BOG_SPECIAL_CHAR(BOG_DELIMITER)

{OPNAME}		BOG_OPERATOR(BOG_OP)

{INTEGER}		BOG_LITERALS(BOG_INT)
{FLOAT}			BOG_LITERALS(BOG_FLOAT)
{BOOLEAN}		BOG_LITERALS(BOG_BOOLEAN)
{STRING}		BOG_LITERALS(BOG_STRING)

{NAME}			BOG_NAME(BOG_NAME_ENUM)

.				BOG_REPORT


