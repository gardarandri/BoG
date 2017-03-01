#ifndef BOG_VARTABLE_H
#define BOG_VARTABLE_H

#include <string.h>

typedef struct {
	// names inniheldur nöfn breytanna sem
	// eru í töflunni og vals inniheldur sæti þeirra.
	// Ef names[i] == NULL er ekki breyta í því sæti,
	// annars hefur breytan með nafnið names[i]
	// sætið vals[i].
	// 
	// Size er hve stórt fylkið names er.

	char** names;
	int* vals;
	int size;

} BOG_vartable;


// Notkun:	t = bog_vartable_new(s);
// Fyrir:	s er heiltala
// Eftir:	t er bendir á nýja bogvar töflu af stærð s
BOG_vartable* bog_vartable_new(int s){
	BOG_vartable* res = malloc(sizeof(BOG_vartable));

	res->names = malloc(sizeof(char*) * s);
	res->vals = malloc(sizeof(int) * s);

	res->size = s;

	return res;
}


// Notkun:	x = bog_vartable_get(t,n);
// Fyrir:	t er bog vartafla, n er strengur
// Eftir:	x er sætisnúmer breytunnar sem hefur nafið n.
int bog_vartable_get(BOG_vartable* t, char* name){
	for(int i=0; i<t->size; i++){
		if(!strcmp(name,t->names[i])){
			return t->vals[i];
		}
	}
	return -1;
}


// Notkun:	x = bog_vartable_put(t,n,p);
// Fyrir:	t er bogvar tafla, n er strengur og p er heiltala, p>=0
// Eftir:	búið er að bæta breytu í töfluna með nafnið n
// 			og staðsetninguna p. Ef innsetning tóks ekki er 
// 			x == -1.
int bog_vartable_put(BOG_vartable* t, char* name, int pos){
	int i=0;
	while(i<t->size && t->names[i] != NULL) i++;

	if(i == t->size){
		printf("Error: BOG_vartable is full!");
		return -1;
	}

	strcpy(t->names[i],name);
	vals[i] = pos;
}


// Notkun:	bog_vartable_clear(t);
// Fyrir:	t er bogvar tafla
// Eftir:	búið er að hreinsa töfluna t
void bog_vartable_clear(BOG_vartable* t){
	for(int i=0; i<t->size; i++){
		t->names[i] = NULL;
	}
}


void bog_vartable_delete(BOG_vartable* t){
	free(t->vals);
	free(t->names);
	free(t);
}



#endif
