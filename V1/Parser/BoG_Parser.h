#include <stdio.h>
#include <assert.h>
#include "BoG_Lexer.h"
#include "BoG_Parser_util.h"
#include "BoG_vartable.h"
#include "BoG_stack.h"

#ifdef LEXER_DEBUG
	#define advance_over_BOG(E) advance_over_BOG(__LINE__,E)
	#define advance_over_str(E) advance_over_str(__LINE__,E)
	#define advance() advance(__LINE__)
	#define new_BOG_syntax_node(E) new_BOG_syntax_node(E,__LINE__)
	#define new_BOG_syntax_node_c(E,F) new_BOG_syntax_node_c(E,F,__LINE__)
#endif


#ifdef BOG_STACK_DEBUG
	#define bog_stack_top(E) bog_stack_top(E,__LINE__)
	#define bog_stack_new() bog_stack_new(__LINE__)
	#define bog_stack_push(E,F) bog_stack_push(E,F,__LINE__)
	#define bog_stack_pop(E) bog_stack_pop(E,__LINE__)
	#define bog_stack_pop_and_free(E) bog_stack_pop_and_free(E,__LINE__)
	#define bog_stack_is_empty(E) bog_stack_is_empty(E,__LINE__)
	#define bog_stack_delete(E) bog_stack_delete(E,__LINE__)
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


int op_priority(char* opname){
	if(!strcmp(opname,"||") || !strcmp(opname,"&&")){
		return 0;
	}

	if(!strcmp(opname,"==") || !strcmp(opname,"!=") || !strcmp(opname,"<=") || !strcmp(opname,">=") || !strcmp(opname,"<") || !strcmp(opname,">")){
		return 1;
	}

	if(!strcmp(opname,"+") || !strcmp(opname,"-")){
		return 2;
	}

	if(!strcmp(opname,"*") || !strcmp(opname,"/")){
		return 3;
	}

	return 4;
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

	bog_vartable_put(function_table,fi->name,arg_cnt);

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
		BOG_stack* exp_stack = bog_stack_new();
		BOG_stack* op_stack = bog_stack_new();

		BOG_syntax_node* t = new_BOG_syntax_node(BOG_ST_EXPR);
		t->down = first_expr;

		bog_stack_push(exp_stack,t);
		bog_stack_push(op_stack,copy_of_lexeme());

		advance();

		t = new_BOG_syntax_node(BOG_ST_EXPR);
		t->down = smallexpr();
		bog_stack_push(exp_stack,t);

		while(current_token == BOG_OPNAME){
			if(bog_stack_is_empty(op_stack) || op_priority(current_lexeme) > op_priority(bog_stack_top(op_stack))){
				bog_stack_push(op_stack,copy_of_lexeme());

				advance();

				t = new_BOG_syntax_node(BOG_ST_EXPR);
				t->down = smallexpr();
				bog_stack_push(exp_stack,t);
			}else{
				t = new_BOG_syntax_node(BOG_ST_EXPR);
				t->down = new_BOG_syntax_node_c(BOG_ST_CALL,bog_stack_pop(op_stack));

				BOG_syntax_node* e2 = bog_stack_pop(exp_stack);
				BOG_syntax_node* e1 = bog_stack_pop(exp_stack);

				t->down->next = e1;
				t->down->next->next = e2;

				bog_stack_push(exp_stack,t);
			}
		}

		while(!bog_stack_is_empty(op_stack)){
				t = new_BOG_syntax_node(BOG_ST_EXPR);
				t->down = new_BOG_syntax_node_c(BOG_ST_CALL,bog_stack_pop(op_stack));

				BOG_syntax_node* e2 = bog_stack_pop(exp_stack);
				BOG_syntax_node* e1 = bog_stack_pop(exp_stack);

				t->down->next = e1;
				t->down->next->next = e2;

				bog_stack_push(exp_stack,t);
		}

		res = bog_stack_pop(exp_stack);


		bog_stack_delete(exp_stack);
		bog_stack_delete(op_stack);
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

		res->down = expr();

		res->down->next = body();
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

//  ->   FUNCTION  ->
//          |
//        NAME -> NAME -> NAME -> ... -> NAME -> DECL -> DECL -> ... -> EXPR -> EXPR -> ... -> EXPR
void function_asm(FILE* outfileptr, BOG_syntax_node* f){
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

//  ->   DECL   ->
//        |
//      NAME -> NAME -> NAME -> ... -> NAME
void decl_asm(FILE* outfileptr, BOG_syntax_node* d){
	assert(d->type == BOG_ST_DECL);

	BOG_syntax_node* t = d->down;

	while(t != NULL){
		add_local_variable(t);

		fprintf(outfileptr,"(MakeVal null)\n(Push)\n");

		t = t->next;
	}
}

void expr_asm(FILE* outfileptr, BOG_syntax_node* e){
	assert(e->down != NULL);
	assert(e->type == BOG_ST_EXPR);

	if(e->down->type == BOG_ST_RETURN){
//  ->   EXPR   ->
//        |
//      RETURN -> EXPR
		expr_asm(outfileptr,e->down->next);
		fprintf(outfileptr,"(Return)\n");
	}else if(e->down->type == BOG_ST_NAME){
		if(e->down->next == NULL){
//  ->  EXPR ->
//        |
//      NAME
			fprintf(outfileptr,"(Fetch %d)\n",bog_vartable_get(localvar_table,(char*)e->down->content));
		}else{
//  ->   EXPR   ->
//        |
//      NAME -> EXPR
			expr_asm(outfileptr,e->down->next);
			fprintf(outfileptr,"(Store %d)\n",bog_vartable_get(localvar_table,(char*)e->down->content));
		}
	}else if(e->down->type == BOG_ST_CALL){
//  ->  EXPR ->
//        |
//      CALL -> EXPR -> EXPR -> EXPR -> ... -> EXPR
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

		int expected_number_of_arguments = bog_vartable_get(function_table,(char*)e->down->content);
		if(expected_number_of_arguments != -1 && parameter_count != expected_number_of_arguments){
			printf("line %d, char %d: To few arguments to function '%s'\n", e->down->linenum, e->down->charnum,(char*)e->down->content);
			exit(1);
		}

		fprintf(outfileptr,"(Call #\"%s[f%d]\" %d)\n",(char*)e->down->content,parameter_count,parameter_count);
	}else if(e->down->type == BOG_ST_EXPR){
		BOG_syntax_node* t = e->down;

		expr_asm(outfileptr,t);
	}else if(e->down->type == BOG_ST_LITERAL){
//  -> EXPR  ->
//       |
//    LITERAL
		fprintf(outfileptr,"(MakeVal %s)\n",(char*)e->down->content);
	}else if(e->down->type == BOG_ST_IF){
//  -> EXPR  ->
//       |
//      IF
		if_asm(outfileptr,e->down);
	}else if(e->down->type == BOG_ST_WHILE){
//  -> EXPR  ->
//      |
//    WHILE
		while_asm(outfileptr,e->down);
	}else{
		printf("Error: Expression type not recognized!");
		exit(1);
	}
}

void if_asm(FILE* outfileptr, BOG_syntax_node* i){
//  -> IF  ->
//     |
//    EXPR -> BODY -> EXPR -> BODY -> ... -> EXPR -> BODY -> BODY
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
//  -> WHILE ->
//       |
//     EXPR -> BODY
	assert(w->type == BOG_ST_WHILE);

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
//  ->  BODY  ->
//       |
//     EXPR -> EXPR -> ... -> EXPR
	BOG_syntax_node* t = b->down;

	while(t != NULL){
		expr_asm(outfileptr,t);
		t = t->next;
	}
}


void output_asm(char* outfilenameprefix, BOG_syntax_node* program){
	assert(program->type == BOG_ST_PROGRAM);

	char outfilename[1000];
	outfilename[0] = '\0';
	strcat(outfilename,outfilenameprefix);
	strcat(outfilename,".masm");

	FILE* outfileptr = fopen(outfilename,"w");

	fprintf(outfileptr,"\"%s.mexe\" = main in\n!{{\n",outfilenameprefix);

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

#ifdef SYNTAX_NODE_DEBUG
	print_BOG_syntax_node(t,0);
#endif

	char output_file_name_prefix[1000];
	strcpy(output_file_name_prefix,input_file);
	int i=strlen(input_file);
	while(i>0 && output_file_name_prefix[i] !='.') i--;

	if(i == 0){
		output_asm(output_file_name_prefix,t);
	}else{
		output_file_name_prefix[i] = '\0';
		output_asm(output_file_name_prefix,t);
	}
}







