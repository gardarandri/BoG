#include <stdio.h>
#include <assert.h>
#include "BoG_Lexer.h"
#include "BoG_Parser_util.h"
#include "BoG_vartable.h"

#ifdef LEXER_DEBUG
	#define advance_over_BOG(E) advance_over_BOG(__LINE__,E)
	#define advance_over_str(E) advance_over_str(__LINE__,E)
	#define advance() advance(__LINE__)
	#define new_BOG_syntax_node(E) new_BOG_syntax_node(E,__LINE__)
	#define new_BOG_syntax_node_c(E,F) new_BOG_syntax_node_c(E,F,__LINE__)
#endif



#define VARTABLE_SIZE 1000
BOG_vartable* function_table;
BOG_vartable* localvar_table;

int function_count = 0;
int localvar_count = 0;
int label_count = 0;

// Notkun:	add_local_variable(n);
// Fyrir:	Þáttun er í gangi, n er BOG_syntax_node bendir
// Eftir:	breyta með nafnið sem name bendir á
// 			hefur verið bætt í safn staðværa breyta í núverandi falli
// 			og gefin sætis númer á hlaða. Ef þetta tókst ekki er keyrslu hætt
// 			og villumelding gefin.
void add_local_variable(BOG_syntax_node* n){
	if(bog_vartable_get(localvar_table,(char*)n->content) != -1){
		printf("line %d, char %d: Redecleration of variable '%s'\n",n->linenum,n->charnum,(char*)n->content);
		exit(1);
	}else{
		bog_vartable_put(localvar_table,(char*)n->content,localvar_count++);
	}
}


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

	BOG_syntax_node* res = new_BOG_syntax_node(BOG_ST_FUNCTION);

	BOG_function_info* fi = malloc(sizeof(BOG_function_info));
	fi->name = malloc(sizeof(char)*(strlen(current_lexeme)+1));
	strcpy(fi->name,current_lexeme);


	advance_over_BOG(BOG_NAME_ENUM);

	advance_over_str("(");

	res->down = args();

	int arg_cnt = 0;
	BOG_syntax_node* argptr = res->down;

	while(argptr != NULL){
		arg_cnt++;
		argptr = argptr->next;
	}

	fi->number_of_arguments = arg_cnt;

	res->content = (void*)fi;

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

//binopexpr	=	  smallexpr,
//				| smallexpr, OPNAME, binopexpr
BOG_syntax_node* binopexpr(){
	BOG_syntax_node* first_expr = smallexpr();
	BOG_syntax_node* res;

	if(current_token == BOG_OPNAME){
		BOG_syntax_node* t = new_BOG_syntax_node(BOG_ST_EXPR);
		t->down = first_expr;
		res = t;

		while(current_token == BOG_OPNAME){
			t->next = new_BOG_syntax_node_c(BOG_ST_OPNAME,current_lexeme);
			t = t->next;

			advance();

			t->next = new_BOG_syntax_node(BOG_ST_EXPR);
			t->next->down = smallexpr();
			t = t->next;
		}
	}else{
		res = first_expr;
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
	BOG_syntax_node* res;

	if(current_token == BOG_NAME_ENUM){
		if(next_lexeme_equals("(")){
			BOG_syntax_node* t = new_BOG_syntax_node_c(BOG_ST_CALL,current_lexeme);
			res = t;

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
			res = t;
			advance();
		}
	}else if(current_token == BOG_OPNAME){
		res = new_BOG_syntax_node_c(BOG_ST_CALL,current_lexeme);

		advance_over_BOG(BOG_OPNAME);

		res->next = new_BOG_syntax_node(BOG_ST_EXPR);
		res->next->down = smallexpr();
	}else if(current_token == BOG_LITERAL){
		res = new_BOG_syntax_node_c(BOG_ST_LITERAL,current_lexeme);

		advance_over_BOG(BOG_LITERAL);
	}else if(lexeme_equals("(")){
		advance_over_str("(");

		res = expr();

		advance_over_str(")");
	}else if(current_token == BOG_IF){
		res = ifexpr();
	}else{
		res = new_BOG_syntax_node(BOG_ST_WHILE);

		advance_over_BOG(BOG_WHILE);

		res->next = expr();

		res->next->next = body();
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

void function_asm(FILE* outfileptr, BOG_syntax_node* f);
void decl_asm(FILE* outfileptr, BOG_syntax_node* d);
void expr_asm(FILE* outfileptr, BOG_syntax_node* e);
void if_asm(FILE* outfileptr, BOG_syntax_node* i);
void while_asm(FILE* outfileptr, BOG_syntax_node* w);
void body_asm(FILE* outfileptr, BOG_syntax_node* b);
void output_asm(char* outfilename, BOG_syntax_node* program);

void function_asm(FILE* outfileptr, BOG_syntax_node* f){
	printf("in function function_asm\n");
	assert(f->type == BOG_ST_FUNCTION);

	fprintf(outfileptr,"#\"%s[f%d]\" =\n[\n",((BOG_function_info*)f->content)->name,
			((BOG_function_info*)f->content)->number_of_arguments,((BOG_function_info*)f->content)->number_of_arguments);

	bog_vartable_clear(localvar_table);
	localvar_count = 0;

	BOG_syntax_node* t = f->down;

	while(t != NULL && t->type == BOG_ST_NAME){
		add_local_variable(t);
		t = t->next;
	}

	while(t != NULL && t->type == BOG_ST_DECL){
		decl_asm(outfileptr,t);
		t = t->next;
	}

	while(t != NULL && t->type == BOG_ST_EXPR){
		expr_asm(outfileptr,t);
		t = t->next;
	}

	fprintf(outfileptr,"];\n");
}

void decl_asm(FILE* outfileptr, BOG_syntax_node* d){
	printf("in function decl_asm\n");
	assert(d->type == BOG_ST_DECL);

	BOG_syntax_node* t = d->down;

	while(t != NULL){
		add_local_variable(t);

		fprintf(outfileptr,"(MakeVal null)\n(Push)\n");

		t = t->next;
	}
}

void expr_asm(FILE* outfileptr, BOG_syntax_node* e){
	printf("in function expr_asm\n");
	assert(e->down != NULL);
	assert(e->type == BOG_ST_EXPR);

	if(e->down->type == BOG_ST_RETURN){
		printf("BOG_ST_RETURN\n");
		expr_asm(outfileptr,e->down->next);
		fprintf(outfileptr,"(Return)\n");
	}else if(e->down->type == BOG_ST_NAME){
		printf("BOG_ST_NAME\n");
		if(e->down->next == NULL){
			printf("is NULL");
			printf("localvar_table %p\n(char*)e->down->content %p\n",localvar_table,(char*)e->down->content);
			printf((char*)e->down->content);
			fprintf(outfileptr,"(Fetch %d)\n",bog_vartable_get(localvar_table,(char*)e->down->content));
			printf("????");
		}else{
			printf("not NULL");
			expr_asm(outfileptr,e->down->next);
			fprintf(outfileptr,"(Store %d)\n",bog_vartable_get(localvar_table,(char*)e->down->content));
		}
	}else if(e->down->type == BOG_ST_CALL){
		printf("BOG_ST_CALL\n");
		BOG_syntax_node* t = e->down->next;

		int parameter_count = 0;
		if(t != NULL){
			expr_asm(outfileptr,t);
			t = t->next;
			parameter_count++;
		}
		while(t != NULL){
			fprintf(outfileptr,"(Push)\n");
			expr_asm(outfileptr,t);
			t = t->next;
			parameter_count++;
		} 

		if(parameter_count != ((BOG_function_info*)e->down->content)->number_of_arguments){
			printf("line %d, char %d: To few arguments to function '%s', expected %d arguments.\n",
					e->down->linenum, e->down->charnum, ((BOG_function_info*)e->down->content)->name,
					((BOG_function_info*)e->down->content)->number_of_arguments);
			exit(1);
		}

		fprintf(outfileptr,"(Call #\"%s[f%d]\" %d)\n",(char*)e->down->content,parameter_count,parameter_count);
	}else if(e->down->type == BOG_ST_EXPR){
		printf("BOG_ST_EXPR\n");
		BOG_syntax_node* t = e->down;

		expr_asm(outfileptr,t);

		t = t->next;
		while(t != NULL){
			fprintf(outfileptr,"(Push)\n");

			char* opname = (char*)t->content;
			t = t->next;

			expr_asm(outfileptr,t);

			fprintf(outfileptr,"(Call #\"%s[f2]\" 2)\n",opname);

			t = t->next;
		}
	}else if(e->down->type == BOG_ST_OPNAME){
		printf("BOG_ST_OPNAME\n");
		expr_asm(outfileptr,e->down->next);
		fprintf(outfileptr,"(Call #\"%s[f1]\" 1)\n",(char*)e->down->content);
	}else if(e->down->type == BOG_ST_LITERAL){
		printf("BOG_ST_LITERAL\n");
		fprintf(outfileptr,"(MakeVal %s)\n",(char*)e->down->content);
	}else if(e->down->type == BOG_ST_IF){
		printf("BOG_ST_IF\n");
		if_asm(outfileptr,e->down);
	}else if(e->down->type == BOG_ST_WHILE){
		printf("BOG_ST_WHILE\n");
		while_asm(outfileptr,e->down);
	}else{
		printf("Error: Expression type not recognized!");
		exit(1);
	}
}

void if_asm(FILE* outfileptr, BOG_syntax_node* i){
	printf("in function if_asm\n");
	BOG_syntax_node* t = i->down;

	int behind_label = label_count++;

	while(t != NULL && t->type != BOG_ST_BODY){
		assert(t->type == BOG_ST_EXPR);

		expr_asm(outfileptr,t);

		fprintf(outfileptr,"(GoFalse _%d)\n",label_count);

		body_asm(outfileptr,t->next);

		fprintf(outfileptr,"(Go _%d)\n",behind_label);

		fprintf(outfileptr,"_%d:\n",label_count++);
		
		t = t->next->next;
	}

	if(t != NULL){
		assert(t->type == BOG_ST_BODY);

		body_asm(outfileptr,t);
	}

	fprintf(outfileptr,"_%d:\n",behind_label);
}

void while_asm(FILE* outfileptr, BOG_syntax_node* w){
	printf("in function while_asm\n");
	int start_label = label_count++;
	int done_label = label_count++;

	fprintf(outfileptr,"_%d:\n",start_label);

	expr_asm(outfileptr,w->down);
	fprintf(outfileptr,"(GoFalse _%d)\n",done_label);

	body_asm(outfileptr,w->down->next);

	fprintf(outfileptr,"(Go _%d)\n",start_label);
	fprintf(outfileptr,"_%d:\n",done_label);
}

void body_asm(FILE* outfileptr, BOG_syntax_node* b){
	printf("in function body_asm\n");
	BOG_syntax_node* t = b->down;

	while(t != NULL){
		expr_asm(outfileptr,t);
		t = t->next;
	}
}


void output_asm(char* outfilename, BOG_syntax_node* program){
	printf("in function output_asm\n");
	assert(program->type == BOG_ST_PROGRAM);

	FILE* outfileptr = fopen(outfilename,"w");

	fprintf(outfileptr,"\"%s.mexe\" = main in\n!{{\n",outfilename);

	BOG_syntax_node* t = program->down;

	while(t != NULL){
		function_asm(outfileptr,t);
		t = t->next;
	}

	fprintf(outfileptr,"}}*BASIS;");
}


void parse(const char* input_file){
	start_lexer(input_file);

	function_table = bog_vartable_new(VARTABLE_SIZE);
	localvar_table = bog_vartable_new(VARTABLE_SIZE);

	BOG_syntax_node* t = program();

	print_BOG_syntax_node(t,0);

	output_asm("asmtest",t);
}







