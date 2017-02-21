#include <stdlib.h>

#ifndef VAR_ARRAY_HEADER
#define VAR_ARRAY_HEADER

typedef struct i_vararr
{
	int  pos;
	int  length;
	int* array;
} int_var_array;

typedef struct s_vararr
{
	int  max_string_size;
	int  pos;
	int  length;
	char** array;
} string_var_array;

/*STRING*/
void string_var_array_init(string_var_array* va, int _length)
{
	va->max_string_size = 255;
	va->pos             = 0;
	va->length          = _length;
	va->array           = malloc(_length*sizeof(*va->array));
}

void string_var_array_append(string_var_array* va, char* str)
{
	if (va->pos == va->length)
	{
		va->length *= 2;
		va->array = realloc(va->array, va->length*sizeof(*va->array));
	}
	va->array[va->pos++] = str;
}

char* string_var_array_get(string_var_array* va, int i)
{
	return va->array[i];
}

int string_var_array_size(string_var_array* va)
{
	return va->pos;
}

char** string_var_array_to_array(string_var_array* va)
{
	int i;
	char** ret = malloc(va->pos*sizeof(*ret));
	for (i = 0; i < va->pos; i++)
		ret[i] = va->array[i];
	return ret;
}

void string_var_array_print(string_var_array* va)
{
	int i;
	printf("[\n");
	for (i = 0; i < va->pos; i++)
		printf("%s\n", va->array[i]);
	printf("]\n");
}

void string_var_array_destroy(string_var_array* va)
{
	int i;
	for (i = 0; i < va->pos; i++)
		free(va->array[i]);
	free(va->array);
}



/*INT*/
void int_var_array_init(int_var_array* va, int _length)
{
	va->pos    = 0;
	va->length = _length;
	va->array  = malloc(_length*sizeof(*va->array));
}

void int_var_array_append(int_var_array* va, int n)
{
	if (va->pos == va->length)
	{
		va->length *= 2;
		va->array = realloc(va->array, va->length*sizeof(*va->array));
	}
	va->array[va->pos++] = n;
}

int int_var_array_get(int_var_array* va, int i)
{
	return va->array[i];
}

int int_var_array_size(int_var_array* va)
{
	return va->pos;
}

int* int_var_array_to_array(int_var_array* va)
{
	int i;
	int* ret = malloc(va->pos*sizeof(*ret));
	for (i = 0; i < va->pos; i++)
		ret[i] = va->array[i];
	return ret;
}

void int_var_array_print(int_var_array* va)
{
	int i;
	printf("[");
	if (va->pos == 0)
	{
		printf("]");
		return;
	}
	for (i = 0; i < va->pos; i++)
		printf("%d, ", va->array[i]);
	printf("\b\b]\n");
}

void int_var_array_destroy(int_var_array* va)
{
	free(va->array);
}

#endif