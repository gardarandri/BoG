
%{
enum bog_tokens{
BOG_INT,
BOG_FLOAT,
BOG_BOOL,
BOG_IF,
BOG_ELSE,
BOG_WHILE,
BOG_CONST,
BOG_PLUS,
BOG_MINUS,
BOG_MULT,
BOG_DIV,
BOG_OPEN_BRACKET,
BOG_CLOSE_BRACKET,
BOG_OPEN_PARAN,
BOG_CLOSE_PARAN,
BOG_SEMICOLON,
BOG_ASSIGN,
BOG_QUOTE,
BOG_INT_CONST,
BOG_FLOAT_CONST,
BOG_BOOLEAN_CONST,
BOG_NAME
};

#ifdef DEBUG
#define HANLE_ENUM(E) printf(#E);printf("\n");
#else
#define HANLE_ENUM(E) printf("%d\n",E);
#endif
%}

WS			[ \t\n\r\f]
INTEGER		[0-9]+
FLOAT		[0-9]+\.[0-9]+([Ee][+-]?[0-9]+)?
BOOLEAN		(true)|(false)
NAME		[a-zA-Z][a-zA-Z0-9_]*

%%

{WS}				/* Do nothing */
int					HANLE_ENUM(BOG_INT)
float				HANLE_ENUM(BOG_FLOAT)
bool				HANLE_ENUM(BOG_BOOL)
if					HANLE_ENUM(BOG_IF)
else				HANLE_ENUM(BOG_ELSE)
while				HANLE_ENUM(BOG_WHILE)
const				HANLE_ENUM(BOG_CONST)
"+"					HANLE_ENUM(BOG_PLUS)
"-"					HANLE_ENUM(BOG_MINUS)
"*"					HANLE_ENUM(BOG_MULT)
"/"					HANLE_ENUM(BOG_DIV)
"{"					HANLE_ENUM(BOG_OPEN_BRACKET)
"}"					HANLE_ENUM(BOG_CLOSE_BRACKET)
"("					HANLE_ENUM(BOG_OPEN_PARAN)
")"					HANLE_ENUM(BOG_CLOSE_PARAN )
";"					HANLE_ENUM(BOG_SEMICOLON )
"="					HANLE_ENUM(BOG_ASSIGN )
"\""				HANLE_ENUM(BOG_QUOTE )
{FLOAT}				HANLE_ENUM(BOG_INT_CONST )
{INTEGER}			HANLE_ENUM(BOG_FLOAT_CONST )
{BOOLEAN}			HANLE_ENUM(BOG_BOOLEAN_CONST )
{NAME}				HANLE_ENUM(BOG_NAME)