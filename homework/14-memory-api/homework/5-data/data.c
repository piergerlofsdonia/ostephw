#include <stdlib.h>

int main()
{
	int *data = (int *) malloc(100 * sizeof(int));
	if ( data == NULL ) exit(1);
	data[100] = 0;
	free(data);
	return 1;
}
