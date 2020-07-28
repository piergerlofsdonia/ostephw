#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/*

	Notes:
		* Red-black trees are emphasised by the following rules (Introduction to Algorithms, 3rd Ed.):
			1. Every node is red or black.
			2. The root (start node) is black.
			3. Every leaf (NULL pointers) are black.
			4. Red nodes have black children.
			5. For each node, all simple paths from the node to descendant leaves contain the same number of black nodes.

	* Point 5 above is the main reason the tree can be self-balanced, as there should never be a path where the number of black nodes between
	 it and the end-points (leaves) is x+1 for it and x or x-1 for another node.
*/

typedef struct node {
	int key;
	int colour;
	struct node *parent;
	struct node *left;
	struct node *right;
} node;

static node *NULLPTR;
static node *ROOTPTR;

void *ecmalloc(size_t);
void leftrotate(node *y);

int main() 
{
	// Setup top node.
	NULLPTR = (node *) ecmalloc(sizeof(node));
	NULLPTR->colour = 1;
	ROOTPTR = NULLPTR;
	

}

void *ecmalloc(size_t nbytes)
{
	void *voidptr = malloc(nbytes);
	if ( voidptr != NULL ) 
	{
		return voidptr;
	} else 
	{
		fprintf(stderr, "Cannot allocate [%lu] bytes!\n", nbytes);
		exit(1);
	}
}

void leftrotate(node *y) 
{
	/* 
		Imagine two nodes in a tree, x and y:

			p              p                 p         p
			|              |                 |         |
			x              x              x  y         y
		   / \            / \            / \  \       / \
		  a   y          a   b  y       a   b  c     x   c
			 / \                 \                  / \ 
			b   c                 c                a   b
	 */

	node *x = y->parent;

	if ( y->left != NULL )
	{
		// Assign x as the parent of the left subtree of y.
		y->left->parent = x;
		if ( x->parent == NULL) 
		{
			ROOTPTR = y;
		} // TODO: Continue (4. https://www.programiz.com/dsa/red-black-tree)
	}	
}

/* TODO:
   * Add insert function 
   * Add print function
*/
