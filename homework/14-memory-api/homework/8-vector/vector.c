/*
	A recreation of the c++ vector STL using malloc, calloc, and realloc.

TODO: 
	1. Recreate 'assign' function.

*/

#include "vector.h"
#include <stdlib.h>
#include <stdio.h>

int main()
{
	vector *v = InitaliseVector(10);
	
}

void *ecmalloc(size_t nbytes)
{
	void *vptr = malloc(nbytes);
	if ( vptr != NULL ) return vptr;
	fprintf(stderr, "Malloc error: %d\n", __LINE__);
	exit(1);	
}

vector *InitaliseVector(size_t capacity)
{
	vector *v = NULL; 
	v->data = (int *) ecmalloc(capacity * sizeof(int));
	v->capacity = capacity;
	v->size = 1;
	return v;	
}
