#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>

typedef struct struct_vector
{
	int *data;
	size_t capacity;
	size_t size;
} vector;

void *ecmalloc(size_t);
int *ecrealloc(int*, size_t);
vector *InitaliseVector(size_t, int);
int VectorFront(vector*);
int VectorBack(vector*);
int VectorPushBack(vector**, int);
int VectorPopBack(vector**);
void VectorPrint(vector*);
int VectorErase(vector**, int);
int VectorClear(vector**);

#endif
