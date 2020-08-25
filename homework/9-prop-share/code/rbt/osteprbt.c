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

	int npids = 1000;
	int *pids = calloc(npids, sizeof(int));
	//int pids[16] = {87, 10, 71, 45, 11, 54, 59, 15, 77, 5, 51, 82, 68, 2, 53, 60};
	int i = 0;

	for (; i < npids; i++ ) pids[i] = i+1;
	RandomiseArray(pids, npids);
	PrintArray(pids, npids);
	for (i=0; i < npids; i++) 
	{
		Insert(&root, pids[i]);
		if ( countTree(root) != (i+1) ) 
		{
			fprintf(stdout, "ERROR: Incorrect number of nodes\n");
			PrintTree(root, 0);
			exit(1);
		}
	}
	PrintTree(root, 0);
	printf("NODES [%lu]\n", countTree(root));
	free(root);
	free(NIL);
}

/* TODO:
1. Implement lottery draw using rbt.`
 */
