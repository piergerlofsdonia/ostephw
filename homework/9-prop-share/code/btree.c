#include <stdio.h>
#include <stdlib.h>

typedef struct node {
	int pid;
	int nicerating;
	struct node  *left;
	struct node *right;
} process_node;

void *ecmalloc(size_t);
process_node *makeNode(int, int);
process_node *insert(process_node*, int, int);
void printTree(process_node*);
process_node *createTree(int[], int[], size_t);

void *ecmalloc(size_t nbytes)
{
	void *voidptr = malloc(nbytes);
	if ( voidptr == NULL )
	{
		fprintf(stdout, "ERROR: Malloc failure [%d].\n", __LINE__);
		exit(1);
	}

	return voidptr;
}

process_node *makeNode(int pid, int nice)
{
	process_node *n = ecmalloc(sizeof(process_node));
	n->pid = pid; n->left = NULL; n->right = NULL;
	return n;
}

process_node *insert(process_node* node, int pid, int nice)
{
	if (node == NULL) 
	{
		node = (process_node *) ecmalloc(sizeof(process_node));
		node->pid = pid;
		node->nicerating = nice;
		node->left = NULL;
		node->right = NULL;
	} else if (pid < node->pid) {
		printf("Inserting %d left\n", pid);
		node->left = insert(node->left, pid, nice);
	} else {
		printf("Inserting %d right\n", pid);
		node->right = insert(node->right, pid, nice);
	}

	return node;
}

void printTree(process_node *n)
{
	if ( n == NULL ) return;	
	printf("\n    [%d]    \n   /   \\  \n", n->pid);
	if ( n->left != NULL ) { printf(" [%d]   ", n->left->pid); 
	} else { printf(" [ ]    "); }
	if ( n->right != NULL ) { printf("[%d]  ", n->right->pid); 
	} else { printf("[ ]"); }
	printf("\n____________________________\n");
	printTree(n->left);
	printTree(n->right);
}

process_node *createTree(int pids[], int nices[], size_t arrsize)
{
	process_node *root = NULL;

	for (unsigned p = 0; p < arrsize; p++ ) 
	{
		root = insert(root, pids[p], nices[p]);		
	}	

	return root;
}

int main()
{
	int pids[10] = {14, 100, 56, 20, 23, 42, 50, 12, 15, 90};
	int nices[10] = {-5, 0, 0, 0, 0, 10, 19, -10, -3, 5};
	size_t arrsize = 10;
	process_node *root = createTree(pids, nices, arrsize);
	printTree(root);
}

/* TODO: Import processes and niceness ratings for all process, insert into tree and search it (timed). Compare against red-black tree */
