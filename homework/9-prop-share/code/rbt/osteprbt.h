#ifndef OSTEPRBT_H
#define OSTEPRBT_H

#include <stdlib.h>
#include <stdio.h>

#define RED 1
#define BLACK 0
#define PCOUNT 10

typedef struct node {
	struct node *parent;
	struct node *left;
	struct node *right;
	int key;
	int colour;
} process_node;

process_node *ROOT;
process_node *NIL;

void LeftRotate(process_node**, process_node*);
void RightRotate(process_node**, process_node*);
void Insert(process_node**, int);
void Fixup(process_node**, process_node*);

void LeftRotate(process_node **root, process_node *x)
{
	if ( x->right->parent == NULL ) return;
	process_node *y = x->right;
	x->right = y->left;
	if ( y->left != NULL ) y->left->parent = NIL;
	y->parent = x->parent;
	if ( x->parent->key == 0 ) {
		*root = y;
		y->parent = NIL;
	}
	else if ( x == x->parent->left ) x->parent->left = y;
	else x->parent->right = y;
	y->left = x;
	x->parent = y;
}

void RightRotate(process_node **root, process_node *y)
{
	if ( y->left->parent == NULL ) return;
	process_node *x = y->left;
	y->left = x->right;
	if ( x->right != NULL ) x->right->parent = NIL;
	x->parent = y->parent;
	if ( y->parent->key == 0 ) { 
		*root = x; 
		x->parent = NIL; 
	} 
	else if ( y == y->parent->left ) y->parent->left = x;
	else y->parent->right = x;
	x->left = y;
	y->parent = x;
	
}

void Insert(process_node **root, int key)
{
	process_node *z = (process_node *) malloc(sizeof(process_node));
	z->key = key;
	process_node *y, *x;
	y = NIL; x = *root;
	while ( x->key != 0 ) 
	{
		y = x;
		if ( z->key < x->key ) x = x->left;
		else x = x->right;
	}
	z->parent = y;
	if ( y == NIL ) *root = z;
	else if ( z->key < y->key ) y->left = z;
	else y->right = z;
	z->left = NIL;
	z->right = NIL;
	z->colour = RED;
	Fixup(root, z);
}

void Fixup(process_node **root, process_node *z)
{
	process_node *u = (process_node *) malloc(sizeof(process_node));
	int semicase = -1; // 0 for p=g->left, 1 for p=g->right.

	while ( z->parent->colour == RED && z != (*root)) 
	{
		if ( z->parent == z->parent->parent->left ) 
		{ 
			u = z->parent->parent->right;
			semicase = 0;	
		}
		else 
		{
			u = z->parent->parent->left;
			semicase = 1;
		}
		if ( u->colour == RED )
		{
			z->parent->colour = BLACK;
			u->colour = BLACK;
			z->parent->parent->colour = RED;
			z = z->parent->parent;
		}
		else 
		{
			// Determine what configuration z is in (e.g. left-left, left-right, right-left, right-right).
			switch(semicase)
			{
				case 0:
					if ( z->key > z->parent->key ) LeftRotate(root, z->parent); // z is g->left->right. 	
					RightRotate(root, z->parent->parent); // z is either g->left->left or has undergone left rotation.
					break;
				case 1:
					if ( z->key < z->parent->key ) RightRotate(root, z->parent); // z is g->right->left.
					LeftRotate(root, z->parent->parent); // z is either g->right->right or has undergone right rotation.
					break;		
			}

			z->parent->parent->colour = z->parent->colour; // Swap grandparent and parent colours.
			z->parent->colour = (z->parent->colour == RED) ? BLACK : RED;
		}
	}

	if ( z == (*root) ) z->colour = BLACK;
}

void PrintTree(process_node *root, int spacing)
{
	if ( root == NIL ) return;
	spacing += PCOUNT;
	PrintTree(root->right, spacing);
	printf("\n");
	for ( int i = PCOUNT; i < spacing; i++ ) 
	{
		printf(" ");
	}
	printf("[%d:%d]\n", root->key, root->colour);

	PrintTree(root->left, spacing);
}

#endif
