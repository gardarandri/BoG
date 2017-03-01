#ifndef BOG_VARTABLE_H
#define BOG_VARTABLE_H

#include <string.h>

typedef struct {
	// names inniheldur n�fn breytanna sem
	// eru � t�flunni og vals inniheldur s�ti �eirra.
	// Ef names[i] == NULL er ekki breyta � �v� s�ti,
	// annars hefur breytan me� nafni� names[i]
	// s�ti� vals[i].
	// 
	// Size er hve st�rt fylki� names er.

	char** names;
	int* vals;
	int size;

} BOG_vartable;


// Notkun:	t = bog_vartable_new(s);
// Fyrir:	s er heiltala
// Eftir:	t er bendir � n�ja bogvar t�flu af st�r� s
BOG_vartable* bog_vartable_new(int s){
	BOG_vartable* res = malloc(sizeof(BOG_vartable));

	res->names = malloc(sizeof(char*) * s);
	res->vals = malloc(sizeof(int) * s);

	res->size = s;

	return res;
}


// Notkun:	x = bog_vartable_get(t,n);
// Fyrir:	t er bog vartafla, n er strengur
// Eftir:	x er s�tisn�mer breytunnar sem hefur nafi� n.
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
// Eftir:	b�i� er a� b�ta breytu � t�fluna me� nafni� n
// 			og sta�setninguna p. Ef innsetning t�ks ekki er 
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
// Eftir:	b�i� er a� hreinsa t�fluna t
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
