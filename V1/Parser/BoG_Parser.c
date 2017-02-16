#include "BoG_Lexer.h"

#define BOG_MATCH 5
#define BOG_NO_MATCH 6

//program 	= function , program
//			| function
void program(){
	while(current_token != BOG_EOF){
		function();
	}

	return;
}

//function	= NAME , "(" , args , ")" , "{" , { decl , ";" } , { expr , ";" } , "}"
void function(){
	advance_over_BOG(BOG_NAME_ENUM, "name");

	advance_over_str("(");

	args();

	advance_over_str(")");
	advance_over_str("{");

	while(current_token == BOG_VAR){
		decl();
	}

	while(!lexeme_equals("}")){
		expr();
	}

	advance_over_str("}");
}

//decl		= "var" , NAME , { NAME }
void decl(){
	advance_over_str("var");

	advance_over_BOG(BOG_NAME_ENUM);
	while(current_token == BOG_NAME_ENUM){
		advance_over_BOG(BOG_NAME_ENUM);
	}
}

//expr		= NAME
//			| NAME , "=" , expr
//			| NAME , "(" , [ expr , { "," , expr } ] , ")"
//			| "return" , expr
//			| OPNAME , expr
//			| expr OPNAME expr
//			| LITERAL
//			| "(" , expr , ")"
//			| ifexpr
//			| "while" , "(" , expr , ")" , body
void expr(){
	if(current_token == BOG_NAME_ENUM){
	}else if(current_token == BOG_RETURN){
		advance();

		expr();
	}else if(current_token == BOG_OPNAME){
		advance();

		expr();
	}else if(current_token == BOG_LITERAL){
		advance();
	}else if(lexeme_equals("(")){
		expr();

		advance_over_str("(");
	}else if(current_token == BOG_IF){
		ifexpr();
	}else if(current_token == BOG_WHILE){
		advance();
		advance_over_str("(");

		expr();

		advance_over_str(")");

		body();
	}else{
		throw_error("Expression");
	}

	if(current_token == BOG_OPNAME){
		advance();
		expr();
	}
}

//ifexpr		= "if" , "(" , expr , ")" , body , [ { "elif" , "(" , expr , ")" , body } ] , [ "else" , body ]
void ifexpr(){
	advance_over_str("if");
	advance_over_str("(");

	expr();

	advance_over_str(")");

	body();

	// TODO: Add elif as a token in the lexer
	while(current_token == BOG_ELIF){
		advance();
		advance_over_str("(");

		expr();

		advance_over_str(")");

		body();
	}

	if(current_token == BOG_ELSE){

		advance();
		advance_over_str("(");

		expr();

		advance_over_str(")");

		body();
	}
}

//body		= "{" , expr , ";" , { expr , ";" } , "}"
void body(){
	advance_over_str("{");

	expr();
	advance_over_str(";");

	while(!lexeme_equals("}")){
		expr();
	}

	advance();
}


void parse(){
	program();
}





