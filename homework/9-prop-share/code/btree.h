#ifndef BTREE_H
#define BTREE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct node {
	int pid;
	int nicerating;
	unsigned tickets;
	struct node  *left;
	struct node *right;
} process_node;

void *ecmalloc(size_t);
process_node *makeNode(int, int);
process_node *insert(process_node*, int, int);
void padTree(int);
void printTree(process_node*, int);
process_node *createTree(int[], int[], int, int);

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
	process_node *n = (process_node *) ecmalloc(sizeof(process_node));
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
		node->left = insert(node->left, pid, nice);
	} else {
		node->right = insert(node->right, pid, nice);
	}

	return node;
}

void padTree(int padding)
{
	for ( int i = 0; i < padding; i++ ) 
	{
		putchar(' ');
	}
}

void printTree(process_node *n, int depth)
{
	if ( n == NULL ) 
	{
		padTree(depth);
		puts("~");
	} else
   	{
		printTree(n->right, depth+1);
		padTree(depth);
		printf("%d\n", n->pid);
		printTree(n->left, depth+1);
	}
}

process_node *createTree(int pids[], int nices[], int start, int end)
{
	int mid;
	process_node *root = NULL;
	if ( start > end ) return NULL;
	mid = start + (end-start) / 2;
	
	root = insert(root, pids[mid], nices[mid]);	
	root->left = createTree(pids, nices, start, mid-1);
	root->right = createTree(pids, nices, mid+1, end);
	return root;
}

#endif
