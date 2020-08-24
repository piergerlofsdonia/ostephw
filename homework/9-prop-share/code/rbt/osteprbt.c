#include "osteprbt.h"
#include "../bst/osteparray.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
	NIL = malloc(sizeof(process_node));
	NIL->parent = NIL;
	NIL->left = NIL;
	NIL->right = NIL;
	NIL->pid = 0;
	NIL->colour = BLACK;
	process_node *root = malloc(sizeof(process_node));
	root->pid = 0;

	int npids = 12;
	//int *pids = calloc(npids, sizeof(int));
	int pids[12] = {8, 6, 3, 2, 7, 10, 12, 4, 11, 9, 1, 5};
	int i = 0;

	//for (; i < npids; i++ ) pids[i] = i+1;
	//RandomiseArray(pids, npids);
	PrintArray(pids, npids);
	for (i=0; i < npids; i++) 
	{
		Insert(&root, pids[i]);
		PrintTree(root, 0);
	}
	free(root);
	free(NIL);
}

/* TODO:
1. Fix insert fix up algorithm, see header file for issue regarding black nodes. A further, probably-related issue occurs when using a large array, so there's probably a glaring issue with the implementation.
2. Implement lottery draw using rbt.`
 */
