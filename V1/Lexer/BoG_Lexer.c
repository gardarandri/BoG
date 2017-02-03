
#include "BoG_Lexer_Generated.c"
#include "stdio.h"

int main(){
	int current_token = yylex();
	while(1){
		printf("%d '%s'\n",current_token,yytext);

		current_token = yylex();
		if(current_token == 0) break;
	}
}
