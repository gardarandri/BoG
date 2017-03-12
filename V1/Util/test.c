#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "var_array.h"
#include "BoG_stack.h"

int main()
{
	int i;
	srand(time(0));
	int_var_array* va   = malloc(sizeof(*va));
	string_var_array* vb = malloc(sizeof(*vb));
	int_var_array_init(va, 2);
	string_var_array_init(vb, 2);

	string_var_array_append(vb, "bergur");
	string_var_array_append(vb, "er");
	string_var_array_append(vb, "bestur");
	string_var_array_append(vb, "fact");
	string_var_array_append(vb, "fuck");
	string_var_array_append(vb, "off");
	string_var_array_append(vb, "scrub");

	int n = string_var_array_size(vb);
	char** arr1 = string_var_array_to_array(vb);

	for (i = 0; i < n; i++)
	int_var_array_append(va, rand()%2);

	string_var_array_print(vb);
	int_var_array_print(va);

	int size = int_var_array_size(va);
	int* arr = int_var_array_to_array(va);

	int_var_array_destroy(va);
	free(va);

	for (i = 0; i < size; i++)
		printf("%d: %s\n", arr[i], arr1[i]);

	free(va);
	free(vb);



	BOG_stack* s = bog_stack_new();

	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;
	int e = 5;
	int f = 6;
	int g = 7;
	int h = 8;

	bog_stack_push(s,&a);
	bog_stack_push(s,&b);
	bog_stack_push(s,&c);
	bog_stack_push(s,&d);
	bog_stack_push(s,&e);
	bog_stack_push(s,&f);

	printf("%d\n",*(int*)bog_stack_top(s));
	printf("%d\n",*(int*)bog_stack_pop(s));

	bog_stack_push(s,&h);

	while(!bog_stack_is_empty(s))
		printf("%d\n",*(int*)bog_stack_pop(s));

	bog_stack_delete(s);

	return 0;
}
