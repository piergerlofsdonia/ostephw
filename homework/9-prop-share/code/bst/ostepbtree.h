#ifndef OSTEPBTREE_H
#define OSTEPBTREE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct node {
	int pid;
	int nicerating;
	unsigned tickets;
	struct node  *left;
	struct node *right;
} process_node;


static const int prio_to_weight[40] = {
	/* -20 */     88761,     71755,     56483,     46273,     36291,
	/* -15 */     29154,     23254,     18705,     14949,     11916,
	/* -10 */      9548,      7620,      6100,      4904,      3906,
	/*  -5 */      3121,      2501,      1991,      1586,      1277,
	/*   0 */      1024,       820,       655,       526,       423,
	/*   5 */       335,       272,       215,       172,       137,
	/*  10 */       110,        87,        70,        56,        45,
	/*  15 */        36,        29,        23,        18,        15,
};

void *ecmalloc(size_t);
process_node *makeNode(int, int);
process_node *minimum(process_node*);
process_node *insert(process_node*, int, int);
process_node *_delete(process_node*, int);
int _sum(process_node*, process_node**, unsigned long*, unsigned long, int);
void padTree(int);
void printTree(process_node*, int);
process_node *createTree(int*, int*, int, int);
long SumWeights(process_node*, long);

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

process_node *_delete(process_node *n, int pid)
{
	/* Delete a node using basic bst removal process dictated by the number of nodes below the deleted node:
	   0. Remove the node, no other changes are required.
	   1. Take the pid value of the sole child and replace the deleted node with it.
	   2. Search for the lowest right-hand value below the removed node, that node replaces the removed node.
	   3. Something has gone dreadfully, terribly wrong.
	*/

	if ( n == NULL ) return NULL;
	
	process_node *current = NULL;

	if ( pid < n->pid ) n->left = _delete(n->left, pid);
	else if ( pid > n->pid ) n->right = _delete(n->right, pid);
	else 
	{
		/* Check number of children on the node to delete. */
		if ( n->left != NULL && n->right != NULL )
		{
			current = minimum(n->right);
			n->pid = current->pid;
			n->right = _delete(n->right, current->pid);	
		} else if (n->left != NULL || n->right != NULL ) 
		{
			current = (n->left == NULL) ? n->right : n->left;
			free(n);
			return current;
		} else 
		{
			free(n);
			return NULL;
		}
	}

	return n;
}

process_node *minimum(process_node *n)
{
	if ( n == NULL ) return NULL;
	else if (n->left != NULL ) return minimum(n->left);
	return n;
}

int _sum(process_node *p, process_node **w, unsigned long *accum, unsigned long winner, int bias)
{ 
	/* 
	   Find a winning ticket within the bst structure by searching one side of the tree first, then the inverse of that side of the tree,
	   then the opposite branch of the tree and the inverse of that branch - if you catch my drift? The 'bias' parameter determines whether the 
	   accumulation of tickets traverses the right side or left side first. Use 'printTree' with a small tree to see how this works. 
	   
	   e.g. if bias is 1 and winner is 250 and each node in the tree has 50 tickets:
	   
	             50
		        /  \
	 (winner) 250  100
				  /  \
				 200  150
			  
	   It's not pretty but it works. */

	if ( p == NULL ) return 0; 

	*accum += p->tickets;
	if ( *accum > winner ) 
	{
		*w = p;
		return 1;
	} else 
	{
		switch(bias) 
		{
			case 0:
				_sum(p->right, w, accum, winner, bias);
				if ( *accum > winner ) return 0;
				_sum(p->left, w, accum, winner, bias);
				break;
			case 1:
				_sum(p->left, w, accum, winner, bias);
				if ( *accum > winner ) return 0;
				_sum(p->right, w, accum, winner, bias);
				break;	
		}
	}

	return 0;
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

process_node *createTree(int *pids, int *nices, int start, int end)
{
	int mid;
	if ( start > end ) return NULL;
	mid = start + (end-start) / 2;
	process_node *root = insert(NULL, pids[mid], nices[mid]);
	root->left = createTree(pids, nices, start, mid-1);
	root->right = createTree(pids, nices, mid+1, end);
	return root;
}

long SumWeights(process_node *n, long a)
{
	if ( n->pid == 0 ) return 0;
	if ( n->left->pid != 0 ) a+=SumWeights(n->left, a);
	if ( n->right->pid != 0) a+=SumWeights(n->right, a);
	return prio_to_weight[(20+n->nicerating)];
}

#endif
