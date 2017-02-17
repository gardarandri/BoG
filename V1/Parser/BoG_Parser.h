#include <stdio.h>
#include "BoG_Lexer.h"

#define BOG_MATCH 5
#define BOG_NO_MATCH 6

void body();
void ifexpr();
void expr();
void decl();
void args();
void function();
void program();


//program 	= function , program
//			| function
void program(){
	while(current_token != BOG_EOF){
		function();
	}
	finish_lexing();
}

//function	= "fun" , NAME , "(" , args , ")" , "{" , { decl , ";" } , { expr , ";" } , "}"
void function(){
	advance_over_BOG(BOG_FUN);
	advance_over_BOG(BOG_NAME_ENUM);

	advance_over_str("(");

	args();

	advance_over_str(")");
	advance_over_str("{");

	while(current_token == BOG_VAR){
		decl();
		advance_over_str(";");
	}

	while(!lexeme_equals("}")){
		expr();
		advance_over_str(";");
	}

	advance_over_str("}");
}

//args		= NAME , { "," , NAME }
//			| ""
void args(){
	if(current_token != BOG_NAME_ENUM) return;

	advance();

	while(lexeme_equals(",")){
		advance();
		advance_over_BOG(BOG_NAME_ENUM);
	}
}


//decl		= "var" , NAME , { "," , NAME }
void decl(){
	advance_over_str("var");

	advance_over_BOG(BOG_NAME_ENUM);
	while(lexeme_equals(",")){
		advance();
		advance_over_BOG(BOG_NAME_ENUM);
	}
}

//expr		= NAME
//			| NAME , "=" , expr
//			| NAME , "(" , [ expr , { "," , expr } ] , ")"
//			| "return" , expr
//			| OPNAME , expr
//			| expr , OPNAME , expr
//			| LITERAL
//			| "(" , expr , ")"
//			| ifexpr
//			| "while" , "(" , expr , ")" , body
void expr(){
	if(current_token == BOG_NAME_ENUM){
		advance();
		if(lexeme_equals("=")){
			advance();
			
			expr();
		}else if(lexeme_equals("(")){
			if(!lexeme_equals(")")){
				expr();
				while(lexeme_equals(",")){
					advance();

					expr();
				}
			}else{
				advance_over_str(")");
			}
		}
	}else if(current_token == BOG_RETURN){
		advance();

		expr();
	}else if(current_token == BOG_OPNAME){
		advance();

		expr();
	}else if(current_token == BOG_LITERAL){
		advance();
	}else if(lexeme_equals("(")){
		advance_over_str("(");

		expr();

		advance_over_str(")");
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
	advance_over_BOG(BOG_IF);
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
		advance_over_str(";");
	}

	advance();
}


void parse(const char* input_file){
	start_lexer(input_file);

	program();

}





