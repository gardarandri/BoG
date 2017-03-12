
#include <assert.h>



typedef struct bog_stack{
	void** base;
	int n;
	int allocated;
} BOG_stack;



BOG_stack* bog_stack_new(
#ifdef BOG_STACK_DEBUG
		int linenum
#endif
		){
#ifdef BOG_STACK_DEBUG
	printf("parser line %d: called bog_stack_new\n",linenum);
#endif

	BOG_stack* res = malloc(sizeof(BOG_stack));
	
	res->n = 0;
	res->base = NULL;
	res->allocated = 0;

	return res;
}


void bog_stack_push(BOG_stack* s, void* val
#ifdef BOG_STACK_DEBUG
		,int linenum
#endif
		){
#ifdef BOG_STACK_DEBUG
	printf("parser line %d: called bog_stack_push\n",linenum);
#endif

	if(s->base == NULL){
		s->base = malloc(sizeof(void*)*2);
		s->allocated = 2;
	}else{
		if(s->allocated < s->n+1){
			s->allocated = 2*s->allocated;
			void** tmp = malloc(sizeof(void*)*s->allocated);
			for(int i=0; i<s->n; i++){
				tmp[i] = s->base[i];
			}
			free(s->base);
			s->base = tmp;
		}
	}
	s->base[s->n++] = val;
}


void* bog_stack_pop(BOG_stack* s
#ifdef BOG_STACK_DEBUG
		,int linenum
#endif
		){
#ifdef BOG_STACK_DEBUG
	printf("parser line %d: called bog_stack_pop\n",linenum);
#endif

	assert(s->n > 0);
	return s->base[--s->n];
}

void bog_stack_pop_and_free(BOG_stack* s
#ifdef BOG_STACK_DEBUG
		,int linenum
#endif
		){
#ifdef BOG_STACK_DEBUG
	printf("parser line %d: called bog_stack_pop_and_free\n",linenum);
#endif

	assert(s->n > 0);
	free(s->base[--s->n]);
}

void* bog_stack_top(BOG_stack* s
#ifdef BOG_STACK_DEBUG
		,int linenum
#endif
		){
#ifdef BOG_STACK_DEBUG
	printf("parser line %d: called top\n",linenum);
#endif

	assert(s->n > 0);
	return s->base[s->n-1];
}

int bog_stack_is_empty(BOG_stack* s
#ifdef BOG_STACK_DEBUG
		,int linenum
#endif
		){
#ifdef BOG_STACK_DEBUG
	printf("parser line %d: called bog_stack_is_empty\n",linenum);
#endif

	return s->n == 0;
}

void bog_stack_delete(BOG_stack* s
#ifdef BOG_STACK_DEBUG
		,int linenum
#endif
		){
#ifdef BOG_STACK_DEBUG
	printf("parser line %d: called bog_stack_delete\n",linenum);
#endif

	free(s->base);
	free(s);
}























