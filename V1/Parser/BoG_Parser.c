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
	}else if(current_token == BOG_OPNAME){
	}else if(current_token == BOG_LITERAL){
	}else if(lexeme_equals("(")){
	}else if(current_toke == BOG_IF){
	}else if(current_token == BOG_WHILE){
	}else{
		throw_error("Expression");
	}

	if(current_token == BOG_OPNAME){
		advance();
		expr();
	}
}

//ifexpr		= "if" , "(" , expr , ")" , body , [ { "elsif" , "(" , expr , ")" , body } ] , [ "else" , body ]
void ifexpr(){
	advance_over_str("if");
	advance_over_str("(");

	expr();

	advance_over_str(")");

	body();

	// TODO: Add elsif as a token in the lexer
	while(current_token == BOG_ELSIF){
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





//TODO: Beytta í nanomorpho

//program 	= fundcel , ";" , program
//			| vardecl , ";" , program
//			| expr , ";" , program
int program(){
	while(current_token != 0){
		if(fundecl() == BOG_MATCH || vardecl() == BOG_MATCH || expr() == BOG_MATCH){
		}else{
			printf("Error!");
			return BOG_NO_MATCH;
		}
	}

	return BOG_MATCH;
}


//fundecl		= "fun" , NAME , "(" , args , ")" , body
void fundecl(){
	advance_over(BOG_FUN);
	advance_over(BOG_NAME_ENUM);
	advance_over("(");

	args();

	advance_over(")");

	body();
}

//args		= "" 
//			| NAME 
//			| NAME , "," , args
int args(){
	if(current_token != BOG_NAME_ENUM) return BOG_MATCH;

	advance_over(BOG_NAME_ENUM);

	while(current_token == (int)','){
		advance();

		advance_over(BOG_NAME_ENUM);
	}

	return BOG_MATCH;
}

//vardecl		= "var" , vdt1
int vardecl(){
	advance_over("var");
	if(current_token != BOG_VAR) return BOG_NO_MATCH;
	
	return vdt1();
}

//vdt1 		= NAME , ( vdt2 | vdt3 )
int vdt1(){
	if(current_token != BOG_NAME_ENUM) return BOG_NO_MATCH;

	if(vdt2() == BOG_MATCH || vdt3() == BOG_MATCH) return BOG_MATCH;
	else return BOG_NO_MATCH;
}

//vdt2		= "=" , expr , vdt3
int vdt2(){
	if(advance_over("=") == BOG_ERROR) return BOG_NO_MATCH;

	if(expr() == BOG_NO_MATCH) return BOG_NO_MATCH;

	if(vdt3() == BOG_NO_MATCH) return BOG_NO_MATCH;
}
