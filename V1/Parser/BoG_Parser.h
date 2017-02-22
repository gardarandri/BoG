#include <stdio.h>
#include "BoG_Lexer.h"

#ifdef LEXER_DEBUG
	#define advance_over_BOG(E) advance_over_BOG(__LINE__,E)
	#define advance_over_str(E) advance_over_str(__LINE__,E)
	#define advance() advance(__LINE__)
#endif


void body();
void ifexpr();
void expr();
void smallexpr();
void binopexpr();
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
	advance_over_BOG(BOG_VAR);

	advance_over_BOG(BOG_NAME_ENUM);
	while(lexeme_equals(",")){
		advance();
		advance_over_BOG(BOG_NAME_ENUM);
	}
}


//expr		=	'return', expr
//			|	NAME, '=', expr
//			|	binopexpr
void expr(){
	if(current_token == BOG_RETURN){
		advance();
		expr();
	}else if(next_token == BOG_ASSIGN){
		advance_over_BOG(BOG_NAME_ENUM);
		advance();

		expr();
	}else{
		binopexpr();
	}
}

//binopexpr	=	smallexpr, { OPNAME, smallexpr }
void binopexpr(){
	smallexpr();
	while(current_token == BOG_OPNAME){
		advance();

		smallexpr();
	}
}

//smallexpr	=	NAME
//			|	NAME, '(', [ expr, { ',', expr } ], ')'
//			|	OPNAME, smallexpr
//			| 	LITERAL 
//			|	'(', expr, ')'
//			|	ifexpr
//			|	'while', expr, body
void smallexpr(){
	if(current_token == BOG_NAME_ENUM){
		advance();

		if(lexeme_equals("(")){
			advance();

			if(lexeme_equals(")")) advance();
			else{
				expr();

				while(lexeme_equals(",")){
					advance();

					expr();
				}
				advance_over_str(")");
			}
		}
	}else if(current_token == BOG_OPNAME){
		advance_over_BOG(BOG_OPNAME);

		smallexpr();
	}else if(current_token == BOG_LITERAL){
		advance_over_BOG(BOG_LITERAL);
	}else if(lexeme_equals("(")){
		advance_over_str("(");

		expr();

		advance_over_str(")");
	}else if(current_token == BOG_IF){
		ifexpr();
	}else{
		advance_over_BOG(BOG_WHILE);

		expr();

		body();
	}
}


//ifexpr		= "if" , "(" , expr , ")" , body , [ { "elif" , "(" , expr , ")" , body } ] , [ "else" , body ]
void ifexpr(){
	advance_over_BOG(BOG_IF);
	advance_over_str("(");

	expr();

	advance_over_str(")");

	body();

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





