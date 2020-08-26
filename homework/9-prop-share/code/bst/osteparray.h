#ifndef OSTEPARRAY_H
#define OSTEPARRAY_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef struct freq {
	int pid;
	unsigned occurances;
} frequency;

/* Function that removes or adds a value from an array and shifts up/down the values within that array accordingly. */
int *InsertElement(int*, size_t, unsigned, int);
int *AppendElement(int*, size_t, int);
int *ReplaceElement(int *, size_t, unsigned, int);
int *RemoveElement(int*, size_t, unsigned);
void PrintArray(int *, size_t);
void SwapElements(int*, int*);
void RandomiseArray(int*, size_t);
long int RandomValue(long int);
frequency *HighestFreq(int*, size_t);

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

int *ReplaceElement(int *arrptr, size_t arrlen, unsigned i, int v)
{
	if ( i > arrlen ) 
	{
		printf("%lu !<> %d\n", arrlen, i);
		perror("Cannot replace element outside of array!");
		exit(1);
	}

	arrptr[i] = v;
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

void SwapElements(int *elemptr, int *swapptr)
{
	int x = *elemptr;
	*elemptr = *swapptr;
	*swapptr = x;
}

void RandomiseArray(int *arrptr, size_t arrsize)
{
	srand(time(NULL));
	for ( int i = arrsize-1; i > 0; i--)
	{
		int j = rand() % (i+1);
		SwapElements(&arrptr[i], &arrptr[j]);
	}	
}

long int RandomValue(long int max_r)
{
	srand(time(NULL));
	return rand() % max_r;
}

frequency *HighestFreq(int *inparr, size_t arrsize)
{
	frequency *f = (frequency *) malloc(sizeof(int) * 2);
	f->occurances = 0;
	f->pid = 0;	
	int *copyarr = (int *) calloc(1, sizeof(unsigned));
	size_t copysize = 1;
	unsigned o;

	for ( unsigned i = 0; i < arrsize; i++ )
	{	
		if ( (i+1) > copysize ) 
		{
			copyarr = (int *) realloc(copyarr, (i+1) * sizeof(unsigned));
			copysize++;
		}
		copyarr[i] = inparr[i];
		o = 0;
		for ( unsigned n = 0; n < copysize; n++ ) 
		{
			if ( copyarr[n] == inparr[i] ) o++;
		}
		if ( o > f->occurances ) 
		{
			f->occurances = o;
			f->pid = inparr[i];
		}
	}

	if ( f->pid == 0 )
	{
		f->pid = inparr[0];
		f->occurances = 1;
	}
	return f;
}

#endif
