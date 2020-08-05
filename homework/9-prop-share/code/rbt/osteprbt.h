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
void Transplant(process_node **, process_node*, process_node*);
void Remove(process_node**, int);
void Insert(process_node**, int);
void Fixup(process_node**, process_node*);
void FixupDel(process_node**, process_node*);
process_node *Search(process_node *, int);
process_node *Minimum(process_node*);
void PrintTree(process_node*, int);

void LeftRotate(process_node **root, process_node *x)
{
	if ( x->right->parent == NIL ) { printf("Trying to rotate from a NIL node\n"); return; }
	process_node *y = x->right;
	x->right = y->left;
	if ( y->left != NIL ) y->left->parent = NIL;
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
	if ( y->left->parent == NIL ) { printf("Trying to rotate from a NIL node\n"); return; }
	process_node *x = y->left;
	y->left = x->right;
	if ( x->right != NIL ) x->right->parent = NIL;
	x->parent = y->parent;
	if ( y->parent == NIL ) { 
		(*root) = x; 
	} 
	else if ( y == y->parent->left ) y->parent->left = x;
	else y->parent->right = x;
	x->right = y;
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

void Transplant(process_node **root, process_node *u, process_node *v)
{
	if ( u->parent == NIL ) (*root) = v;
	else if ( u == u->parent->left ) u->parent->left = v;
	else u->parent->right = v;
	v->parent = u->parent;
}

process_node *Search(process_node *root, int key)
{
	if ( root == NIL || root->key == key ) return root;
	else if ( root->key < key ) return Search(root->right, key);
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
