#include<stdlib.h>
#include<stdio.h>

#define ARRAYSIZE 5

int main() 
{
	int *data = (int *) malloc(sizeof(int) * ARRAYSIZE);
	if ( data == NULL ) exit(1);
	for ( unsigned i = 0; i < ARRAYSIZE; i++ ) data[i] = i+1;
	free(data);
	printf("%d\n", data[ARRAYSIZE-1]);
}
