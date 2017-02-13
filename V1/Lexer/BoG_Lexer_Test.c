
#include "BoG_Lexer.h"


int main(){
	start_lexer();
	
	while(1){
		if(current_token == 0) return 0;

		printf("%d ",current_token);

		char* t = malloc(current_lexeme_size());
		current_lexeme(t);

		printf("'%s'\n",t);

		advance();
	}
}
