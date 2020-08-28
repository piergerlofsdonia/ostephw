/*
	A partial recreation of the c++ vector STL using malloc, calloc, and realloc.
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
	VectorErase(&v, 0);
	VectorPrint(v);
	VectorPopBack(&v);
	VectorPushBack(&v, 11);
	VectorPrint(v);
	VectorPushBack(&v, 10);
	VectorPushBack(&v, 10);
	VectorPushBack(&v, 2);
	VectorPushBack(&v, 1);
	VectorPrint(v);
	VectorClear(&v);
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
			// Data has been removed leaving the v->data pointer as a null pointer, reinitalise it and fill with data.
			v->data = ecrealloc(v->data, 1);
			v->data[0] = value;
			v->size++;
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
			fprintf(stdout, "No data in vector (v->data is null pointer), so there is nothing to pop!\n");
			return -1;
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
	else if ( v != NULL && v->data == NULL ) fprintf(stdout, "[] (no data in vector, v->data is null pointer)\n");
}

int VectorErase(vector **vptr, int index)
{
	// Removes element at an index, pushing back the proceeding elements to fill its place. Decrement size.
	// e.g. [1, 2, 3, 4] -> VectorErase(<ptr>, 1); -> [1, 3, 4].
	
	vector *v = *vptr;
	
	if ( v != NULL && v->size > 0 && v->data != NULL) 
	{
		if ( index >= v->size ) 
		{
			fprintf(stdout, "Cannot erase an index outside the bounds of the vector [%d vs. %lu]...\n", index, v->size);
			return -1;
		}
		if ( v->size == 1 ) 
		{
			// Remove the sole element, reallocate to zero (will make v->data a NULL pointer).
			v->data[index] = 0;
			v->size = 0;
			v->data = ecrealloc(v->data, 0);
		}
		else
		{
			v->data[index] = 0;
			for ( size_t i = (index+1); i < (v->size-1); i++ ) 
			{
				v->data[i] = v->data[i+1];
			}
			return VectorPopBack(&v);
		}
	}
	else 
	{
		fprintf(stdout, "Cannot erase index from element because either the vector or data does not exist (NULL pointer).\n");
		return -1;
	}

	*vptr = v;
	return 1;
}

int VectorClear(vector **vptr)
{
	vector *v = *vptr;
	
	if ( v != NULL ) 
	{
		if ( v->data != NULL )
		{
			for ( size_t i = 0; i < v->size; i++ ) v->data[i] = 0;
			v->data = ecrealloc(v->data, 0);
			v->size = 0;	
		}
	}

	*vptr = v;
	return 1;
}
