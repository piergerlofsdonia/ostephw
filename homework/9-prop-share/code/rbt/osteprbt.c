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
	process_node *root = malloc(sizeof(process_node));
	Insert(&root, 13);
	Insert(&root, 10);
	Insert(&root, 12);
	Insert(&root, 11);
	PrintTree(root, 0);	
	Remove(&root, 12);
	PrintTree(root, 0);
	free(ROOT);
}

/* FIX Fixup for insert which is currently wiping out a node (10). */
