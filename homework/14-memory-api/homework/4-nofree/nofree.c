#include <stdlib.h>

int main()
{
	int *p = (int *) malloc(sizeof(int));
	if ( p == NULL ) exit(1);
	*p = 1;
	return *p;
}
