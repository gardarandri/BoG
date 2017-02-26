

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
//  ->   EXPR   ->
//        |
//     BINOPEXPR
//
//  
//  -> BINOPEXPR ->
//        |
//      SMALLEXPR -> OPNAME -> ... -> OPNAME -> SMALLEXPR
//
//
//  ->  SMALLEXPR ->
//        |
//      NAME
//
//
//  ->  SMALLEXPR ->
//        |
//      CALL -> EXPR -> EXPR -> EXPR -> ... -> EXPR
//
//  
//  -> SMALLEXPR  ->
//        |
//      OPNAME -> SMALLEXPR
//
//  
//  -> SMALLEXPR  ->
//        |
//      LITERAL
//
//  
//  -> SMALLEXPR  ->
//        |
//       EXPR
//
//  
//  -> SMALLEXPR  ->
//        |
//        IF
//
//  
//  -> SMALLEXPR  ->
//        |
//      WHILE
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
//  ->  BODY  ->
//       |
//     EXPR -> EXPR -> ... -> EXPR



typedef struct BOG_syntax_node{
	int type;
	char* content;

	struct BOG_syntax_node* next;
	struct BOG_syntax_node* down;
} BOG_syntax_node;

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


BOG_syntax_node* new_BOG_syntax_node(int type,
#ifdef LEXER_DEBUG
		int line
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

#ifdef LEXER_DEBUG
	printf("line %d: new BOG_syntax_node: %s\n",line,str_name_BOG_syntax_node(res));
#endif

	return res;
}

BOG_syntax_node* new_BOG_syntax_node_c(int type, char* str,
#ifdef LEXER_DEBUG
		int line
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

#ifdef LEXER_DEBUG
	printf("line %d: new BOG_syntax_node: %s\n",line,str_name_BOG_syntax_node(res));
#endif

#ifdef LEXER_DEBUG
	if(res == NULL){
		printf("content malloc failed");
	}
#endif

	strcpy(res->content, str);

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
