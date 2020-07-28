#include <stdio.h>
#include <stdlib.h>

typedef struct node {
	int key;
	struct node  *left;
	struct node *right;
} node;

node *ROOT;

void *ecmalloc(size_t);
node *makeNode(int);
void insert(node*, int, int);
void printTree(node*);

int main()
{
	ROOT = makeNode(1);
	insert(ROOT, 2, 0);
	insert(ROOT, 3, 1);
	insert(ROOT->left, 4, 0);
	insert(ROOT->left, 5, 1);
	insert(ROOT->right, 6, 0);
	printTree(ROOT);
}

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

node *makeNode(int key)
{
	node *n = ecmalloc(sizeof(node));
	n->key = key; n->left = NULL; n->right = NULL;
	return n;
}

void insert(node* parent, int key, int side)
{
	switch(side)
	{
		case 0:
			parent->left = makeNode(key);
			break;
		case 1:
			parent->right = makeNode(key);
	}
}

void printTree(node *n)
{
	if ( n == NULL ) return;
	
	printf("\n    [%d]    \n   /   \\  \n", n->key);
	if ( n->left != NULL ) { printf(" [%d]   ", n->left->key); }
	if ( n->right != NULL ) { printf("[%d]  ", n->right->key); }
	printf("\n____________________________\n");
	printTree(n->left);
	printTree(n->right);
}
