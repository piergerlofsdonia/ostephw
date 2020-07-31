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
int CheckWinner(process_node*, unsigned long, int);
int DrawWinner(process_node*, unsigned long long);
unsigned long AssignTickets(process_node *);

int main(int argc, char **argv)
{
	unsigned long max_tickets;
	process_node *plist = NULL;
	int winner = -1;

	plist = ParseProcesses();
	max_tickets = AssignTickets(plist);
	printTree(plist, 0);
	winner = DrawWinner(plist, max_tickets);
	printf("%d\n", winner);
	free(plist);
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

int CheckWinner(process_node *head, unsigned long winner, int bias)
{
	process_node *winning_node = (process_node *) ecmalloc(sizeof(process_node));
	unsigned long accum = 0;
	_sum(head, &winning_node, &accum, winner, bias);
	return winning_node->pid;
}

int DrawWinner(process_node *head, unsigned long long max_tickets)
{
	/* Randomise winner */
	srand(time(NULL) * clock() / getpid());
	unsigned long long winner = rand() % max_tickets;
	int bias = rand() % 2;
	printf("[%llu] %llu -> ", max_tickets, winner);
	return CheckWinner(head, winner, bias);	
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
