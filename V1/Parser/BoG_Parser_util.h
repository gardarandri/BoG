

#define BOG_ST_BODY 1
#define BOG_ST_IF 2
#define BOG_ST_WHILE 3
#define BOG_ST_LITERAL 4
#define BOG_ST_OPNAME 5
#define BOG_ST_CALL 6
#define BOG_ST_NAME 7
#define BOG_ST_SMALLEXPR 8
#define BOG_ST_BINOPEXPR 9
#define BOG_ST_RETURN 10
#define BOG_ST_EXPR 11
#define BOG_ST_DECL 12
#define BOG_ST_FUNCTION 13
#define BOG_ST_PROGRAM 14

#ifndef BOG_MAKE_CASE
	#define BOG_MAKE_CASE(E) case E: return #E;
#endif



//
//   Eftirfarandi mynd sýnir bendin down sem | og bendinn next sem ->.
//   Einnig má sjá samspil mismunandi BOG_syntax_node'a sem hafa eitthavð tiltekið
//   type:
//
//
//  ->   PROGRAM  ->
//          |
//       FUNCTION -> FUNCTION -> .... -> FUNCTION
//
//
//  ->   FUNCTION  ->
//          |
//        NAME -> NAME -> NAME -> ... -> NAME -> DECL -> DECL -> ... -> EXPR -> EXPR -> ... -> EXPR
//
//        Þarf ekki decl, hefur bara hliðarverkenir
//
//
//  ->   DECL   ->
//        |
//      NAME -> NAME -> NAME -> ... -> NAME
//
//
//  ->   EXPR   ->
//        |
//      RETURN -> EXPR
//
//
//  ->   EXPR   ->
//        |
//      NAME -> EXPR
//
//
//  ->  EXPR ->
//        |
//      NAME
//
//
//  ->  EXPR ->
//        |
//      CALL -> EXPR -> EXPR -> EXPR -> ... -> EXPR
//
//
//  -> EXPR  ->
//       |
//    LITERAL
//
//  
//  -> EXPR  ->
//       |
//      IF
//
//  
//  -> EXPR  ->
//      |
//    WHILE
//
//
//  -> IF  ->
//     |
//    EXPR -> BODY -> EXPR -> BODY -> ... -> EXPR -> BODY -> BODY
//
//
//  -> WHILE ->
//       |
//     EXPR -> BODY
//
//
//  ->  BODY  ->
//       |
//     EXPR -> EXPR -> ... -> EXPR
//
//
//




typedef struct BOG_syntax_node{
	int type;
	void* content;

	int linenum;
	int charnum;

	struct BOG_syntax_node* next;
	struct BOG_syntax_node* down;
} BOG_syntax_node;


typedef struct BOG_function_info{
	char* name;
	int number_of_arguments;
} BOG_function_info;

// Notkun:	x = new_BOG_syntax_node(t);
// Fyrir:	t er int tala sem vísar til einhverrar gerðar af
// 			milliþulu hlekks.
// Eftir:	x er nýr milliþulu hlekkur af gerðinni t.
BOG_syntax_node* new_BOG_syntax_node(int type);

// Notkun:	x = new_BOG_syntax_node_c(t,s);
// Fyrir:	t er int tala sem vísar til einhverrar gerðar af
// 			milliþulu hlekks, s er strengur (char*)
// Eftir:	x er nýr milliþulu hlekkur af gerðinni t og
// 			x.content er strengurinn s.
BOG_syntax_node* new_BOG_syntax_node_c(int type, char* str);


char* str_name_BOG_syntax_node(BOG_syntax_node* n){
	int e = n->type;
	switch(e){
		BOG_MAKE_CASE(BOG_ST_BODY)
		BOG_MAKE_CASE(BOG_ST_IF)
		BOG_MAKE_CASE(BOG_ST_WHILE)
		BOG_MAKE_CASE(BOG_ST_LITERAL)
		BOG_MAKE_CASE(BOG_ST_OPNAME)
		BOG_MAKE_CASE(BOG_ST_CALL)
		BOG_MAKE_CASE(BOG_ST_NAME)
		BOG_MAKE_CASE(BOG_ST_SMALLEXPR)
		BOG_MAKE_CASE(BOG_ST_BINOPEXPR)
		BOG_MAKE_CASE(BOG_ST_RETURN)
		BOG_MAKE_CASE(BOG_ST_EXPR)
		BOG_MAKE_CASE(BOG_ST_DECL)
		BOG_MAKE_CASE(BOG_ST_FUNCTION)
		BOG_MAKE_CASE(BOG_ST_PROGRAM)
		default:
			return "NOT RECOGNIZED!";
	}
}


BOG_syntax_node* new_BOG_syntax_node(int type
#ifdef LEXER_DEBUG
		,int line
#endif
		){

	BOG_syntax_node* res = malloc(sizeof(BOG_syntax_node));

#ifdef LEXER_DEBUG
	if(res == NULL){
		printf("res malloc failed");
	}
#endif

	res->type = type;
	res->next = NULL;
	res->down = NULL;
	res->content = NULL;

	res->linenum = lexer_at_line;
	res->charnum = lexer_at_char;

#ifdef LEXER_DEBUG
	printf("line %d: new BOG_syntax_node: %s\n",line,str_name_BOG_syntax_node(res));
#endif

	return res;
}

BOG_syntax_node* new_BOG_syntax_node_c(int type, char* str
#ifdef LEXER_DEBUG
		,int line
#endif
		){


	BOG_syntax_node* res = malloc(sizeof(BOG_syntax_node));

#ifdef LEXER_DEBUG
	if(res == NULL){
		printf("res malloc failed");
	}
#endif

	res->type = type;
	res->next = NULL;
	res->down = NULL;
	res->content = malloc((strlen(str)+1)*sizeof(char));

	res->linenum = lexer_at_line;
	res->charnum = lexer_at_char;

#ifdef LEXER_DEBUG
	printf("line %d: new BOG_syntax_node: %s\n",line,str_name_BOG_syntax_node(res));
#endif

#ifdef LEXER_DEBUG
	if(res == NULL){
		printf("content malloc failed");
	}
#endif

	strcpy((char*)res->content, str);

	return res;
}

void print_BOG_syntax_node(BOG_syntax_node* n, int depth){
	if(n == NULL) return;

	int i=depth;
	while(i > 0){
		printf("\t");
		i--;
	}

	if(n->content != NULL) printf("%s (%s)\n",str_name_BOG_syntax_node(n),(char*)n->content);
	else printf("%s\n",str_name_BOG_syntax_node(n));

	print_BOG_syntax_node(n->down,depth+1);

	print_BOG_syntax_node(n->next,depth);
}





