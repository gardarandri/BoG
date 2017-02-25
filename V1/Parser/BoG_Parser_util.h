#define BOG_ST_PROGRAM 1
#define BOG_ST_FUNCTION 2

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
//      NAME -> CALL -> EXPR -> EXPR -> EXPR -> ... -> EXPR
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



typedef BOG_syntax_node struct{
	int type;
	void* content;

	BOG_syntax_node* next;
	BOG_syntax_node* down;
} BOG_syntax_node;



BOG_syntax_node* new_BOG_syntax_node(int type){
	BOG_syntax_node* res = malloc(sizeof(*res));
	res.type = BOG_ST_PROGRAM;
	res.next = NULL;
	res.down = NULL;

	return res;
}
