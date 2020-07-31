#ifndef OSTEPARRAY_H
#define OSTEPARRAY_H

#include <stdlib.h>
#include <stdio.h>
/* Function that removes or adds a value from an array and shifts up/down the values within that array accordingly. */
int *InsertElement(int*, size_t, unsigned, int);
int *AppendElement(int*, size_t, int);
int *RemoveElement(int*, size_t, unsigned);
void PrintArray(int *, size_t);

int *InsertElement(int *arrptr, size_t arrlen, unsigned i, int v) 
{
	/* Insert an integer into the input integer array and return its reallocated pointer.
	 1. Reallocate heap memory to sufficient size.
	 2. Shift along the position of all elements right of inserted value by +1.
	 3. Overwrite value at position i with the new value.
	 4. Return new reallocated pointer. */

	unsigned n;
	arrlen++;
	arrptr = (int *) realloc(arrptr, sizeof(int) * arrlen);
	arrptr[arrlen-1] = 0;

	for ( n = arrlen-1; n > i; n-- ) 
	{
		arrptr[n] = arrptr[n-1];		
	}

	arrptr[i] = v;
	
	return arrptr;
}

int *AppendElement(int *arrptr, size_t arrlen, int v)
{
	arrlen++;
	arrptr = (int *) realloc(arrptr, sizeof(int) * arrlen);
	arrptr[arrlen-1] = v;
	return arrptr;
}

int *RemoveElement(int *arrptr, size_t arrlen, unsigned i)
{
	/* Remove an integer from an input array and return its reallocated pointer.
	  1. Shift along the position of all elements right of element-to-delete by -1.
	  2. Null the last value in the array.
	  3. Reallocate array pointer to new size.
	  4. Return reallocated pointer. 
	*/

	unsigned n;
	
	for ( n = i; n < arrlen-1; n++ ) 
	{
		arrptr[n] = arrptr[n+1];
	}

	arrptr[arrlen-1] = 0;
	arrlen--;
	arrptr = (int *) realloc(arrptr, sizeof(int) * arrlen);
	
	return arrptr;	
}

void PrintArray(int *arrptr, size_t arrlen)
{
	unsigned i;
	fprintf(stdout, "[");
	for (i = 0; i < arrlen; i++ )
	{
		if ( i < arrlen-1) fprintf(stdout, "%d, ", arrptr[i]);
	}

	fprintf(stdout, "%d]\n", arrptr[--i]);
}

#endif
