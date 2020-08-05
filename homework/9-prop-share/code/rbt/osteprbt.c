#include "osteprbt.h"
#include <stdio.h>

int main()
{
	NIL = malloc(sizeof(process_node));
	NIL->parent = NIL;
	NIL->left = NIL;
	NIL->right = NIL;
	NIL->key = 0;
	NIL->colour = BLACK;
	//process_node *root = malloc(sizeof(process_node));
	free(ROOT);
}
