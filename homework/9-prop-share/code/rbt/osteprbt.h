#ifndef OSTEPRBT_H
#define OSTEPRBT_H

#include <stdlib.h>

typedef struct node {
	struct node *parent;
	struct node *left;
	struct node *right;
	int key;
	int colour;
} process_node;

extern process_node *ROOT;
extern process_node *NIL;

void LeftRotate(process_node*);
void RightRotate(process_node*);

void LeftRotate(process_node *x)
{
	if ( x->right->parent == NULL ) return;
	process_node *y = x->right;
	x->right = y->left;
	if ( y->left != NULL ) y->left->parent = NIL;
	y->parent = x->parent;
	if ( x->parent->key == 0 ) {
		ROOT = y;
		y->parent = NIL;
	}
	else if ( x == x->parent->left ) x->parent->left = y;
	else x->parent->right = y;
	y->left = x;
	x->parent = y;
}

void RightRotate(process_node *y)
{
	if ( y->left->parent == NULL ) return;
	process_node *x = y->left;
	y->left = x->right;
	if ( x->right != NULL ) x->right->parent = NIL;
	x->parent = y->parent;
	if ( y->parent->key == 0 ) { 
		ROOT = x; 
		x->parent = NIL; 
	} 
	else if ( y == y->parent->left ) y->parent->left = x;
	else y->parent->right = x;
	x->left = y;
	y->parent = x;
	
}

#endif
