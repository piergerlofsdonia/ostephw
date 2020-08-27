#include<stdlib.h>
#include<stdio.h>

int main()
{
	int *p = malloc(sizeof(int));
	p = NULL;
	printf("%d\n", *p);
	free(p);
}
