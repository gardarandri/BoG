
#define BOG_MATCH 5
#define BOG_NO_MATCH 5

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
int fundecl(){
	if(current_token != BOG_FUN){
		printf("Error!");
	}
	advance();

	if(current_token != BOG_NAME){
		printf("Error!");
	}
	advance();

	if(advance_over("(") == BOG_ERROR) return BOG_NO_MATCH;

	if(args() == BOG_NO_MATCH) return BOG_NO_MATCH;

	if(advance_over(")") == BOG_ERROR) return BOG_NO_MATCH;

	if(body() == BOG_NO_MATCH) return BOG_NO_MATCH;

	return BOG_MATCH;
}

//args		= "" 
//			| NAME 
//			| NAME , "," , args
int args(){
	while(current_token == BOG_NAME_ENUM){
		advance();

		if(lexeme_equals(",")) advance_over(",");
		else break;
	}

	return BOG_MATCH;
}

//vardecl		= "var" , vdt1
int vardecl(){
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

//vdt3		= ""
//			| "," , vdt1
int vdt3(){
}

//expr		= "return" , expr
//			| NAME , "=" , expr
//			| orexpr
int expr(){
}

//orexpr		= andexpr , orexprt
int orexpr(){
}

//orexpr1		= "||" , orexpr
int orexpr1(){
}
//			| ""

//andexpr		= notexpr , andexprt
int andexpr(){
}

//andexprt	= "&&" , andexpr
//			| ""
int andexprt(){
}

//notexpr		= "!" , notexpr
//			| opexpr
int notexpr(){
}

//opexpr		= smallexpr , opexprt
int opexpr(){
}

//opexprt		= OPNAME , opexpr
//			| ""
int opexprt(){
}

//smallexpr	= NAME , smallexprn
//			| OPNAME , expr
//			| LITERAL
//			| "(" , expr , ")"
//			| ifexpr
//			| "while" , "(" , expr , ")" , body
//			| fundecl
int smallexpr(){
}

//smallexprn	= ""
//			| "(" , manyexpr , ")"
int smallexprn(){
}

//manyexpr	= expr , manyexprt
int manyexpr(){
}

//manyexprt	= "," , manyexpr
//			| ""
int manyexprt(){
}

//ifexpr		= "if" , "(" , expr , ")" , body , ifexprt1
int ifexpr(){
}

//ifexprt1	= ""
//			| "elseif" , "(" , expr , ")" , body , ifexprt2
int ifexprt1(){
}

//ifexprt2	= ""
//			| "else" , body
int ifexprt2(){
}

//body		= "{" , innerbody , "}"
int body(){
}

//innerbody	= funcdecl , ";" , innerbodyt
//			| vardecl , ";" , innerbodyt
//			| expr , ";" , innerbodyt
int innerbody(){
}

//innerbodyt	= ""
//			| funcdecl , ";" , innerbodyt
//			| vardecl , ";" , innerbodyt
//			| expr , ";" , innerbodyt
int innerbodyt(){
}
