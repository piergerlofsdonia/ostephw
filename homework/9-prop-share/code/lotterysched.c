#include "btree.h" /* Binary search tree functions */

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <unistd.h>
#include <time.h>

process_node *ParseProcesses();
int _sum(process_node *, process_node *, int*, int, int);
int CheckWinner(process_node*, unsigned long long, unsigned long long, int);
int DrawWinner(process_node*, unsigned long long);
unsigned long AssignTickets(process_node *);

int _sum(process_node *p, process_node *r, int *accum, int winner, int bias)
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
		r = p;
		return 1;
	} else 
	{
		switch(bias) 
		{
			case 0:
				_sum(p->right, r, accum, winner, bias);
				if ( *accum > winner ) return 0;
				_sum(p->left, r, accum, winner, bias);
				break;
			case 1:
				_sum(p->left, r, accum, winner, bias);
				if ( *accum > winner ) return 0;
				_sum(p->right, r, accum, winner, bias);
				break;	
		}
	}

	return 0;
}

int main(int argc, char **argv)
{
	int pids[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	int nices[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	process_node *head = createTree(pids, nices, 0, 9);
	int accum = 0;
	AssignTickets(head);
	process_node *winner = (process_node *) ecmalloc(sizeof(process_node));
	printTree(head, 0);
	_sum(head, winner, &accum, 449, 0);

	/*unsigned long max_tickets;
	process_node *plist = NULL;
	plist = ParseProcesses();
	max_tickets = AssignTickets(plist);
	printTree(plist, 0);
	printf("\n%lu -> %d\n", max_tickets, DrawWinner(plist, max_tickets));	
	free(plist);*/
}

process_node *ParseProcesses() 
{
	unsigned count = 0;
	DIR *dirp;
	struct dirent *entry;
	int pid = 0;
	int *pids = (int *) ecmalloc(sizeof(int));
	int *nices = (int *) ecmalloc(sizeof(int));
	process_node *root = NULL;

	dirp = opendir("/proc/");
	
	if ( dirp == NULL ) 
	{
		perror("Cannot open directory: ");
		exit(1);
	} else {
		while((entry = readdir(dirp)) != NULL)
		{
			pid = atoi(entry->d_name);
			
			if ( pid > 0 ) 
			{
				pids[count] = pid;
				nices[count] = getpriority(PRIO_PROCESS, pid);
				count++;
				pids = realloc(pids, sizeof(int) * (count+1));
				nices = realloc(nices, sizeof(int) * (count+1));
			}
		}
		printf("Parsed directory /proc/ [%u files]\n", count);
	}

	root = createTree(pids, nices, 0, count);
	return root;
}

int DrawWinner(process_node *head, unsigned long long max_tickets)
{
	/* Randomise winner */
	srand(time(NULL) * clock() / getpid());
	unsigned long long winner = rand() % max_tickets;
	//int preference = rand() % 2;
	printf("Finding ticket holder: %llu\n", winner);
	return 0;	
	//return CheckWinner(head, 0, winner, preference);	
}

unsigned long AssignTickets(process_node *head)
{
	if ( head == NULL ) return 0;

	unsigned long tickets = 0;
	unsigned utick = 0;
	float ftick = 50.0;

	if ( head->nicerating == 0 ) 
	{
		utick = ftick;
	} else
   	{
		ftick = ftick - ( head->nicerating * 2.5);	
		utick = ftick;
	}
	
	head->tickets = utick;
	tickets += utick;
	tickets += AssignTickets(head->left);
	tickets += AssignTickets(head->right);
	return tickets;
}

/* TODO:
	Get draw winner working, will probably require a L,R,L,R alternating check and accumulation but the starting value should be swapped each time the function is run to keep the draw fair.	
*/
