#include <stdlib.h>
#include <math.h>
#include <sys/mman.h>
#include <stdio.h>

typedef struct s_node {
	int size;
	int isfree;
	struct s_node *sptr;
	struct s_node *next;
} node;

node *splitBlock(node*, size_t);
node *allocateBlock(node*);
node *firstFit(node*, size_t);
node *coalesceBlocks(node*);
void printBlocks(node*);

int main()
{
	/* mmap() creates a mapping in free virtual space within the process.
	   It requires an 'addr' argument - the starting address for the mapping - and a length. 
	   Flags and protections can be provided and a file descriptor & offset is also required.
	node *head = mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE, -1, 0);
	head->size = 4096 - sizeof(node);
	head->next = NULL;
	*/
	
	// Build a simulation of a memory management system.
	// Employ each technique (best fit, worst fit, etc).
	size_t nbytes = 4096;
	node *head = (node *) malloc(sizeof(node) + nbytes);
	head->size = nbytes;
	head->sptr = head + 1; // Set start of memory block in new pointer.
	head->isfree = 1;
	head->next = NULL;
	node *n = head;
	n = splitBlock(n, 154);
	printBlocks(head);
	n = splitBlock(n->next, 250);
	printBlocks(head);
	if ( head == NULL ) {
		fprintf(stdout, "Error whilst assigning, size was invalid for a split\n");
		exit(1);
	}
	head->next->next->isfree = 0;	
	coalesceBlocks(head);
	printBlocks(head);
}

node *splitBlock(node *blockptr, size_t fixsize)
{
	size_t nbytes = blockptr->size + sizeof(node);	
	node *nptr = NULL;

	switch(fixsize) {
		case 0: // We want two blocks of equal size.
			if ( floor((float)(nbytes / 2)) <= sizeof(node) ) return NULL; // Cannot split the block if its size is less than (sizeof(node)+1) * 2.
			fixsize = nbytes / 2;
			blockptr = realloc(blockptr, fixsize);
			blockptr->size = fixsize - sizeof(node);
			nptr = (node *) malloc(fixsize);
			nptr->size = blockptr->size;
			break;
		default:
			if ( fixsize < (blockptr->size-(sizeof(node)+1)) ) blockptr = realloc(blockptr, fixsize+sizeof(node));
			else return NULL; // Block resize cannot be greater than block-size and must be smaller enough to split block into two. TODO: Add a clause to simply return the block in an edge case.
			blockptr->size = fixsize;
			fixsize = nbytes - (fixsize+sizeof(node));
			nptr = (node *) malloc(fixsize);
			nptr->size = fixsize-sizeof(node);
			break;
		}
	
	nptr->sptr = nptr + 1;	
	if ( blockptr->next == NULL ) nptr->next = NULL;
	else nptr->next = blockptr->next;
	nptr->isfree = 1;
	blockptr->next = nptr;
		
	return blockptr;
}

node *coalesceBlocks(node *blockptr)
{
	if ( blockptr == NULL ) return NULL;
	else if ( blockptr->next == NULL ) return blockptr;

	if ( blockptr->isfree > 0 && blockptr->next->isfree > 0 ) 
	{
		// Merge blocks together, restart with blockptr.
		// Reallocate blockptr to make it as big as blockptr + size (with header) of blockptr->next.
		// Update size of blockptr.
		// Copy heirarchy (set tempptr as blockptr->next->next, set blockptr->next->next = NULL, set blockptr->next = tempptr).
		int newsize = blockptr->size + blockptr->next->size;
		blockptr->size = newsize + sizeof(node);
		node *tempptr = blockptr->next->next;
		blockptr->next->next = NULL;
		free(blockptr->next);
		blockptr->next = tempptr;
		return coalesceBlocks(blockptr);
	}
	else if ( blockptr->isfree <= 0 ) return coalesceBlocks(blockptr->next);
	else return NULL; 
}

node *firstFit(node *head, size_t nbytes)
{
	// Is the block free? If so, move to the next block.
	// Is the block bigger than what we need + the header size? If so, split it and use the returned block.
	// Is the block smaller than what we need + the header size? If so, move to the next block.
	// Are we at the end with no match? Coalesce and repeat.
	// Are we at the end after coalescing the blocks? We need to ask for more memory by reallocating (this would be sys call in the OS [sbrk] in a real system).
	// When done, mark the block as in use and return it. 
	return NULL;
}

void printBlocks(node *head)
{
	if ( head == NULL ) return;
	else {
		while ( head != NULL ) 
		{
			fprintf(stdout, "[%lu | %d | %d]->", sizeof(node), head->size, head->isfree);
			head = head->next;	
		}
		fprintf(stdout, "%p\n", head);
	}
}
