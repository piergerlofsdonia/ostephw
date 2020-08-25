#ifndef OSTEPRBT_H
#define OSTEPRBT_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "../bst/osteparray.h"

#define RED 1
#define BLACK 0
#define PCOUNT 10

typedef struct node {
	struct node *parent;
	struct node *left;
	struct node *right;
	int pid;
	unsigned tickets;
	int nicerating;
	int colour;
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

process_node *ROOT;
process_node *NIL;

void LeftRotate(process_node**, process_node*);
void RightRotate(process_node**, process_node*);
void Transplant(process_node **, process_node*, process_node*);
void Remove(process_node**, int);
void Insert(process_node**, int);
void Fixup(process_node**, process_node*);
void FixupDel(process_node**, process_node*);
process_node *Search(process_node *, int);
process_node *Minimum(process_node*);
void PrintTree(process_node*, int);
unsigned long countTreeR(process_node*);
unsigned long countTree(process_node*);
unsigned long CreateTree(process_node**, int*, size_t);
void AssignNices(process_node*);
unsigned long AssignTickets(process_node*);
int SumToFind(process_node*, process_node**, unsigned long*, unsigned long, int);
long SumWeights(process_node*, long);

void LeftRotate(process_node **root, process_node *x)
{
	if ( x->right->parent == NIL ) { printf("Trying to rotate from a NIL node\n"); return; }
	process_node *y = x->right;
	x->right = y->left;
	if ( y->left->pid != 0 ) y->left->parent = NIL;
	y->parent = x->parent;
	if ( x->parent->pid == 0 ) {
		*root = y;
		y->parent = NIL;
	}
	else if ( x == x->parent->left ) x->parent->left = y;
	else x->parent->right = y;
	y->left = x;
	x->parent = y;
	x->right->parent = x;
}

void RightRotate(process_node **root, process_node *y)
{
	if ( y->left->parent == NIL ) { printf("Trying to rotate from a NIL node\n"); return; }
	process_node *x = y->left;
	y->left = x->right;
	if ( x->right->pid != 0 ) x->right->parent = NIL;
	x->parent = y->parent;
	if ( y->parent->pid == 0 ) { 
		(*root) = x; 
	} 
	else if ( y == y->parent->left ) y->parent->left = x;
	else y->parent->right = x;
	x->right = y;
	y->parent = x;
	y->left->parent = y;
}

void Insert(process_node **root, int key)
{
	/* Insert a new node into the tree.
	  Three nodes are required for the insert operation: z represents the node-to-add; y represents, dependent on the portion of the 
		operation, either the NIL node or the parent of the node-to-add; the x node represents the root node initially and will later 
		be set to the leaf node-to-add.
	*/
	process_node *z = (process_node *) malloc(sizeof(process_node));
	z->pid = key;
	process_node *y, *x;
	y = NIL; x = *root; // Set y to NIL, x to ROOT.
	
	if ( x->pid == 0 ) // If the root node does not exist yet, replace it with z.
	{
		z->colour = BLACK;
		z->pid = key;
		z->parent = NIL;
		z->left = NIL;
		z->right = NIL;
		*root = z;
		return;
	}
	
	while ( x != NIL ) // Otherwise, traverse the tree setting y to the parent, x to the node until we find the node-to-add (LEAF node is equal to NIL).
	{
		y = x;
		if ( z->pid < x->pid ) x = x->left;
		else x = x->right;
	}

	z->parent = y;
	if ( y == NIL ) *root = z;
	else if ( z->pid < y->pid ) y->left = z;
	else y->right = z;
	z->left = NIL;
	z->right = NIL;
	z->colour = RED;
	Fixup(root, z);
}

void Transplant(process_node **root, process_node *u, process_node *v)
{
	if ( u->parent == NIL ) (*root) = v;
	else if ( u == u->parent->left ) u->parent->left = v;
	else u->parent->right = v;
	v->parent = u->parent;
}

process_node *Search(process_node *root, int key)
{
	if ( root == NIL || root->pid == key ) return root;
	else if ( root->pid < key ) return Search(root->right, key);
	else return Search(root->left, key);
}

process_node *Minimum(process_node *root)
{
	if ( root->left != NIL ) return Minimum(root);
	else return root;
}

void Remove(process_node **root, int key)
{
	int saved_colour = -1;
	process_node *z = (process_node *) malloc(sizeof(process_node));
	process_node *y = (process_node *) malloc(sizeof(process_node));
	process_node *x = (process_node *) malloc(sizeof(process_node));
	z = Search((*root), key);
	if ( z == NIL ) return;
	y = z;
	saved_colour = y->colour;
	if ( z->left == NIL ) 
	{
		x = z->right;
		Transplant(root, z, z->right);
	}
	else if (z->right == NIL )
	{
		x = z->left;
		Transplant(root, z, z->left);
	}
	else 
	{
		y = Minimum(z->right);
		saved_colour = y->colour;
		x = y->right;
		if ( y->parent == z && x != NIL ) x->parent = y;
		else 
		{
			Transplant(root, y, y->right);
			y->right = z->right;
			y->right->parent = y;
		}
		Transplant(root, z, y);
		y->left = z->left;
		y->left->parent = y;
		y->colour = z->colour;
	}
	
	if ( saved_colour == BLACK ) FixupDel(root, x); 
}

void FixupDel(process_node **root, process_node *x)
{
	process_node *w = (process_node *) malloc(sizeof(process_node));
	while ( x != (*root) &&  x->colour == BLACK ) 
	{
		if ( x == x->parent->left ) 
		{
			if ( x == x->parent->left )
			{
				w = x->parent->right;
				if ( w->colour == RED ) 
				{
					w->colour = BLACK;
					x->parent->colour = RED;
					LeftRotate(root, x->parent);
					w = x->parent->right;
				}
				if ( w->left->colour == BLACK && w->right->colour == BLACK ) 
				{
					w->colour = RED;
					x = x->parent;
				}
				else if ( w->right->colour == BLACK ) 
				{
					w->left->colour = BLACK;
					w->colour = RED;
					RightRotate(root, w);
					w = x->parent->right;
				}

				w->colour = x->parent->colour;
				x->parent->colour = BLACK;
				w->right->colour = BLACK;
				LeftRotate(root, x->parent);
				x = (*root);
			}
		}
		else
		{
			if ( x == x->parent->right ) 
			{
				w = x->parent->left;
				if ( w->colour == RED ) 
				{
					w->colour = BLACK;
					x->parent->colour = RED;
					RightRotate(root, x->parent);
					w = x->parent->left;
				}
				if ( w->right->colour == BLACK && w->left->colour == BLACK ) 
				{
					w->colour = RED;
					x = x->parent;
				}
				else if ( w->left->colour == BLACK ) 
				{
					w->right->colour = BLACK;
					w->colour = RED;
					LeftRotate(root, w);
					w = x->parent->left;
				}

				w->colour = x->parent->colour;
				x->parent->colour = BLACK;
				w->left->colour = BLACK;
				RightRotate(root, x->parent);
				x = (*root);
			}
		}
	}
	x->colour = BLACK;
}

void Fixup(process_node **root, process_node *z)
{
	/* Amend any rules violations caused by the most recent insert:
	   * If black uncle/aunt, rotate - if red aunt/uncle, colour-flip.
	*/
		
	process_node *u;
	
	while ( z->parent->colour == RED && z != (*root))
	{
		if ( z->parent == z->parent->parent->left ) 
		{ // Uncle is right branch.
			u = z->parent->parent->right;
			if ( u->colour == RED ) 
			{
				// COLOUR FLIP if uncle is red.
				z->parent->colour = BLACK;
				u->colour = BLACK;
				z->parent->parent->colour = RED;
				z = z->parent->parent;
				PrintTree(*root, 0);
			}
			else {
				if ( z == z->parent->right ) 
				{
					// ROTATE if uncle is black.
					z = z->parent;
					LeftRotate(root, z); // LR rotate.
					PrintTree(*root,0);
				}
				z->parent->colour = BLACK;
				z->parent->parent->colour = RED;
				RightRotate(root, z->parent->parent); // Potential sole right-rotate
				PrintTree(*root, 0);
			}
		} else
		{ // Uncle is left branch.
			u = z->parent->parent->left;
			if ( u->colour == RED )
			{
				z->parent->colour = BLACK;
				u->colour = BLACK;
				z->parent->parent->colour = RED;
				z = z->parent->parent;
				PrintTree(*root, 0);
			}
			else 
			{	
				if ( z == z->parent->left )
				{
					z = z->parent;
					RightRotate(root, z); // RL rotate.
					PrintTree(*root, 0);
				}
				z->parent->colour = BLACK;
				z->parent->parent->colour = RED;
				LeftRotate(root, z->parent->parent);
				PrintTree(*root, 0);
			}
		}
	}

	(*root)->colour = BLACK;
}

void PrintTree(process_node *root, int spacing)
{
	if ( root->pid == 0 || root == NULL) return;
	spacing += PCOUNT;
	PrintTree(root->right, spacing);
	printf("\n");
	for ( int i = PCOUNT; i < spacing; i++ ) 
	{
		printf(" ");
	}
	printf("[%d:%d]\n", root->pid, root->colour);

	PrintTree(root->left, spacing);
}

unsigned long countTree(process_node *root)
{
	unsigned long c = 0;
	if ( root->pid != 0 ) 
	{
		c+=countTreeR(root);
	}
	return c;
}

unsigned long countTreeR(process_node *n)
{
	unsigned long c = 1;
	if ( n->left->pid != 0 ) c+=countTreeR(n->left);
	if ( n->right->pid != 0) c+=countTreeR(n->right);
	return c;
}

unsigned long CreateTree(process_node **root, int *pids, size_t npids)
{
	unsigned long i;
	int *scrampids = (int *) calloc(npids, sizeof(int));
	for ( i = 0; i < npids; i++ ) scrampids[i] = pids[i];
	RandomiseArray(scrampids, npids);
	for ( i = 0; i < npids; i++ ) Insert(root, scrampids[i]);
	PrintTree(*root, 0);
	return countTree(*root);
}

void AssignNices(process_node *n)
{
	if ( n->pid != 0 && n->nicerating != 0 ) n->nicerating = getpriority(PRIO_PROCESS, n->pid);
	if ( n->left->pid != 0 ) AssignNices(n->left);
	if ( n->right->pid != 0) AssignNices(n->right);
}

unsigned long AssignTickets(process_node *n)
{
	if ( n->pid == 0 ) return 0;

	unsigned tickets = 0;
	unsigned utick = 0;
	float ftick = 50.0;

	if ( n->nicerating != 0 ) ftick = ftick - ( n->nicerating * 2.5);
	utick = ftick;
	n->tickets = utick;
	tickets+=utick;
	tickets+=AssignTickets(n->left);
	tickets+=AssignTickets(n->right);
	return tickets;
}	

int SumToFind(process_node *n, process_node **winner, unsigned long *a, unsigned long w, int bias)
{
	if ( n == NULL || n->pid == 0 ) return 0;
	*a += n->tickets;
	if ( *a > w ) 
	{
		*winner = n;
		return 1;
	}
	else 
	{
		switch(bias) // Bias determines the priority side of the tree to search (left first, or right first). 
		{
			case 0:
				SumToFind(n->right, winner, a, w, bias);
				if ( *a > w ) return 0;
				SumToFind(n->left, winner, a, w, bias);
				break;
			case 1:
				SumToFind(n->left, winner, a, w, bias);
				if ( *a > w ) return 0;
				SumToFind(n->right, winner, a, w, bias);
				break;
		}
	}
	
	return 0;
}

long SumWeights(process_node *n, long a)
{
	if ( n->pid == 0 ) return 0;
	if ( n->left->pid != 0 ) a+=SumWeights(n->left, a);
	if ( n->right->pid != 0 ) a+=SumWeights(n->right, a);
	
	return prio_to_weight[(20+n->nicerating)];
}
#endif
