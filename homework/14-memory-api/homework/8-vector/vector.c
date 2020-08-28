/*
	A recreation of the c++ vector STL using malloc, calloc, and realloc.

TODO: 
	1. Recreate 'erase' function.

*/

#include "vector.h"
#include <stdlib.h>
#include <stdio.h>

#define DEFAULT_CAPACITY 100

int main()
{
	vector *v = InitaliseVector(DEFAULT_CAPACITY, 11);
	VectorPushBack(&v, 10);
	VectorPrint(v);
	VectorPopBack(&v);
	VectorPrint(v);
	VectorPopBack(&v);
	VectorPrint(v);
	free(v);
}

void *ecmalloc(size_t nbytes)
{
	void *vptr = malloc(nbytes);
	if ( vptr != NULL ) return vptr;
	fprintf(stderr, "Malloc error: %d\n", __LINE__);
	exit(1);	
}

int *ecrealloc(int *origptr, size_t newsize)
{
	origptr = (int *) realloc(origptr, newsize);
	if ( origptr != NULL ) return origptr;
	else if ( newsize == 0 ) return NULL;
	fprintf(stderr, "Realloc error: %d\n", __LINE__);
	exit(1);
}

vector *InitaliseVector(size_t capacity, int value)
{
	// [Int Pointer][Size_t][size_t][size_t][size_t][size_t][size_t][size_t][size_t] (FIXED AT 9 BYTES)
	// v->data pointer is variable size. 
	vector *v = (vector *) ecmalloc(sizeof(int*) + (sizeof(size_t) * 2)); 
	v->data = (int *) ecmalloc(sizeof(int));
	v->data[0] = value;
	v->capacity = capacity;
	v->size = 1;
	return v;	
}

int VectorFront(vector *v)
{
	if ( v->data != NULL && v->size > 0 ) return v->data[0];
	fprintf(stderr, "ERROR: No vector data initialised, cannot return front integer.\n");
	exit(1);
}

int VectorBack(vector *v)
{
	if ( v->data != NULL &&  v->size > 0 ) return v->data[v->size-1];
	fprintf(stderr, "ERROR: No vector data initalised, cannot return back integer.\n");
	exit(1);
}

int VectorPushBack(vector **vptr, int value)
{
	vector *v = *vptr;
	if ( v != NULL ) {
		if ( v->data != NULL ) {
			if ( (v->size+1) < v->capacity) {
				v->size++;
				v->data = ecrealloc(v->data, (v->size*sizeof(int)));
				v->data[(v->size-1)] = value;
			} 
			else 
			{
				fprintf(stdout, "Cannot allocate anymore integers to vector [at capacity (%lu)].\n", v->capacity);
				return -1;
			}
		} else {
			fprintf(stderr, "Data pointer is not allocated, check VectorPushBack()...\n");
			exit(1);
		}
	} 
	else 
	{
		v = InitaliseVector(DEFAULT_CAPACITY, 0);
		v->data[0] = value;
	}

	*vptr = v;
	return 1;
}

int VectorPopBack(vector **vptr)
{
	vector *v = *vptr;
	if ( v != NULL ) {
		if ( v->data != NULL ) 
		{
			if ( v->size > 1 ) 
			{
				v->data[--(v->size)] = 0;
				v->data = ecrealloc(v->data, (v->size*sizeof(int)));
			}
			else 
			{
				fprintf(stdout, "Cannot pop the last item in the vector, use VectorErase() instead!\n");
				return -1;
			}
		}
		else 
		{
			fprintf(stderr, "Data pointer is not allocated (null ptr), check VectorPopBack()...\n");
			exit(1);
		}
	}
	else 
	{
		v = InitaliseVector(DEFAULT_CAPACITY, 0);
		*vptr = v;
		return VectorPopBack(vptr);
	}

	*vptr = v;
	return 1;
}

void VectorPrint(vector *v)
{
	if ( v != NULL && v->data != NULL )
	{
		fprintf(stdout, "[");
		for ( size_t i = 0; i < (v->size-1); i++ ) fprintf(stdout, "%d, ", v->data[i]);
		fprintf(stdout, "%d]\n", v->data[v->size-1]);
	}
}
