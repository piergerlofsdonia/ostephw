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
vector *InitaliseVector(size_t);

#endif
