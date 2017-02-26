#include <stdio.h>
#include "BoG_Lexer.h"
#include "BoG_Parser_util.h"

#ifdef LEXER_DEBUG
	#define advance_over_BOG(E) advance_over_BOG(__LINE__,E)
	#define advance_over_str(E) advance_over_str(__LINE__,E)
	#define advance() advance(__LINE__)
	#define new_BOG_syntax_node(E) new_BOG_syntax_node(E,__LINE__)
	#define new_BOG_syntax_node_c(E,F) new_BOG_syntax_node_c(E,F,__LINE__)
#endif





BOG_syntax_node* body();
BOG_syntax_node* ifexpr();
BOG_syntax_node* expr();
BOG_syntax_node* smallexpr();
BOG_syntax_node* binopexpr();
BOG_syntax_node* expr();
BOG_syntax_node* decl();
BOG_syntax_node* args();
BOG_syntax_node* function();
BOG_syntax_node* program();

//program 	= function , program
//			| function
BOG_syntax_node* program(){
	BOG_syntax_node* res = new_BOG_syntax_node(BOG_ST_PROGRAM);

	BOG_syntax_node* func = NULL;

	while(current_token != BOG_EOF){
		if(res->down == NULL){
			res->down = function();
			func = res->down;
		}else{
			func->next = function();
			func = func->next;
		}
	}
	finish_lexing();
	return res;
}

//function	= "fun" , NAME , "(" , args , ")" , "{" , { decl , ";" } , { expr , ";" } , "}"
BOG_syntax_node* function(){
	advance_over_BOG(BOG_FUN);

	BOG_syntax_node* res = new_BOG_syntax_node_c(BOG_ST_FUNCTION,current_lexeme);

	advance_over_BOG(BOG_NAME_ENUM);

	advance_over_str("(");

	res->down = args();

	advance_over_str(")");
	advance_over_str("{");

	BOG_syntax_node* t = NULL;
	BOG_syntax_node* f = NULL;

	while(current_token == BOG_VAR){
		if(t == NULL){
			t = decl();
			f = t;
		}else{
			t->next = decl();
			t = t->next;
		}
		advance_over_str(";");
	}

	while(!lexeme_equals("}")){
		if(t == NULL){
			t = expr();
			f = t;
		}else{
			t->next = expr();
			t = t->next;
		}
		advance_over_str(";");
	}

	if(res->down == NULL){
		res->down = f;
	}else{
		t = res->down;
		while(t->next != NULL) t = t->next;
		t->next = f;
	}

	advance_over_str("}");

	return res;
}

//args		= NAME , { "," , NAME }
//			| ""
BOG_syntax_node* args(){
	BOG_syntax_node* res = NULL;
	if(current_token != BOG_NAME_ENUM) return res;

	BOG_syntax_node* t = new_BOG_syntax_node_c(BOG_ST_NAME,current_lexeme);
	res = t;

	advance();

	while(lexeme_equals(",")){
		advance();

		t->next = new_BOG_syntax_node_c(BOG_ST_NAME,current_lexeme);
		t = t->next;

		advance_over_BOG(BOG_NAME_ENUM);
	}

	return res;
}


//decl		= "var" , NAME , { "," , NAME }
BOG_syntax_node* decl(){
	BOG_syntax_node* res = new_BOG_syntax_node(BOG_ST_DECL);

	advance_over_BOG(BOG_VAR);

	BOG_syntax_node* name = new_BOG_syntax_node_c(BOG_ST_NAME,current_lexeme);
	res->down = name;

	advance_over_BOG(BOG_NAME_ENUM);
	while(lexeme_equals(",")){
		advance();

		name->next = new_BOG_syntax_node_c(BOG_ST_NAME,current_lexeme);
		name = name->next;

		advance_over_BOG(BOG_NAME_ENUM);
	}

	return res;
}


//expr		=	'return', expr
//			|	NAME, '=', expr
//			|	binopexpr
BOG_syntax_node* expr(){
	BOG_syntax_node* res = new_BOG_syntax_node(BOG_ST_EXPR);

	if(current_token == BOG_RETURN){
		res->down = new_BOG_syntax_node(BOG_ST_RETURN);
		advance();
		res->down->next = expr();
	}else if(next_token == BOG_ASSIGN){
		res->down = new_BOG_syntax_node_c(BOG_ST_NAME,current_lexeme);

		advance_over_BOG(BOG_NAME_ENUM);
		advance();

		res->down->next = expr();
	}else{
		res->down = binopexpr();
	}

	return res;
}

//binopexpr	=	smallexpr, { OPNAME, smallexpr }
BOG_syntax_node* binopexpr(){
	BOG_syntax_node* res = new_BOG_syntax_node(BOG_ST_BINOPEXPR);

	res->down = smallexpr();

	BOG_syntax_node* t = res->down;

	while(current_token == BOG_OPNAME){
		t->next = new_BOG_syntax_node_c(BOG_ST_OPNAME,current_lexeme);
		t = t->next;

		advance();

		t->next = smallexpr();
		t = t->next;
	}

	return res;
}

//smallexpr	=	NAME
//			|	NAME, '(', [ expr, { ',', expr } ], ')'
//			|	OPNAME, smallexpr
//			| 	LITERAL 
//			|	'(', expr, ')'
//			|	ifexpr
//			|	'while', expr, body
BOG_syntax_node* smallexpr(){
	BOG_syntax_node* res = new_BOG_syntax_node(BOG_ST_SMALLEXPR);

	if(current_token == BOG_NAME_ENUM){
		if(next_lexeme_equals("(")){
			BOG_syntax_node* t = new_BOG_syntax_node_c(BOG_ST_CALL,current_lexeme);
			res->down = t;

			advance();  // NAME
			advance();  // '('

			if(lexeme_equals(")")) advance();
			else{
				t->next = expr();
				t = t->next;

				while(lexeme_equals(",")){
					advance();

					t->next = expr();
					t = t->next;
				}
				advance_over_str(")");
			}
		}else{
			BOG_syntax_node* t = new_BOG_syntax_node_c(BOG_ST_NAME,current_lexeme);
			res->down = t;
			advance();
		}
	}else if(current_token == BOG_OPNAME){
		res->down = new_BOG_syntax_node_c(BOG_ST_OPNAME,current_lexeme);

		advance_over_BOG(BOG_OPNAME);

		res->down->next = smallexpr();
	}else if(current_token == BOG_LITERAL){
		res->down = new_BOG_syntax_node_c(BOG_ST_LITERAL,current_lexeme);

		advance_over_BOG(BOG_LITERAL);
	}else if(lexeme_equals("(")){
		advance_over_str("(");

		res->down = expr();

		advance_over_str(")");
	}else if(current_token == BOG_IF){
		res->down = ifexpr();
	}else{
		res->down = new_BOG_syntax_node(BOG_ST_WHILE);

		advance_over_BOG(BOG_WHILE);

		res->down->next = expr();

		res->down->next->next = body();
	}

	return res;
}


//ifexpr		= "if" , "(" , expr , ")" , body , [ { "elif" , "(" , expr , ")" , body } ] , [ "else" , body ]
BOG_syntax_node* ifexpr(){
	BOG_syntax_node* res = new_BOG_syntax_node(BOG_ST_IF);

	advance_over_BOG(BOG_IF);
	advance_over_str("(");

	res->down = expr();

	BOG_syntax_node* t = res->down;

	advance_over_str(")");

	t->next = body();
	t = t->next;

	while(current_token == BOG_ELIF){
		advance();
		advance_over_str("(");

		t->next = expr();
		t = t->next;

		advance_over_str(")");

		t->next = body();
		t = t->next;
	}

	if(current_token == BOG_ELSE){
		advance();

		t->next = body();
		t = t->next;
	}

	return res;
}

//body		= "{" , expr , ";" , { expr , ";" } , "}"
BOG_syntax_node* body(){
	BOG_syntax_node* res = new_BOG_syntax_node(BOG_ST_BODY);

	advance_over_str("{");

	res->down = expr();
	BOG_syntax_node* t = res->down;

	advance_over_str(";");

	while(!lexeme_equals("}")){
		t->next = expr();
		t = t->next;
		advance_over_str(";");
	}

	advance();

	return res;
}

void parse(const char* input_file){
	start_lexer(input_file);

	BOG_syntax_node* t = program();

	print_BOG_syntax_node(t,0);
}





