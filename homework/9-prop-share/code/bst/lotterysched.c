/* 
	Lottery-style scheduling using binary search tree structure.  
*/

#include "ostepbtree.h" /* Binary search tree functions */
#include "osteparray.h" /* Array helper functions */
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <unistd.h>
#include <time.h>

#define SCHED_LATENCY 18
#define SCHED_GRAN 2.25

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

size_t InitaliseProcesses(process_node**, int **pids, int **nices);
size_t CheckProcesses(process_node**, int**, int**, size_t);
float GenerateTimeSlice(process_node*, size_t, int*, int*);
process_node *CheckWinner(process_node*, unsigned long, int);
process_node *DrawWinner(process_node*, unsigned long long);
unsigned long AssignTickets(process_node *);

int main(int argc, char **argv)
{
	unsigned long max_tickets;
	int *pids = NULL;
	int *nices = NULL;
	process_node *plist = NULL;
	process_node *winnode = NULL;
	size_t npids = 0;
	struct timeval start, stop;
	float ms = 0.0;
	float ts = 0.0;

	// Initalise.
	npids = InitaliseProcesses(&plist, &pids, &nices);
	max_tickets = AssignTickets(plist);
	winnode = DrawWinner(plist, max_tickets);
	printf("%d\n%lu, %lu\n", winnode->pid, npids, max_tickets);
	ts = GenerateTimeSlice(winnode, npids, pids, nices);
	printf("Time slice: %f\n", ts); 
	
	while(1) 
	{
		gettimeofday(&start, NULL);
		while ((ms = (float)(stop.tv_usec - start.tv_usec) / 1000 + (float)(stop.tv_sec - start.tv_sec) * 1000) < ts)
		{
			gettimeofday(&stop, NULL);
		}
	
		printf("Timed: %fms\n", ms);
	
		npids = CheckProcesses(&plist, &pids, &nices, npids);
		max_tickets = AssignTickets(plist);
		winnode = DrawWinner(plist, max_tickets);
		printf("%d\n%lu, %lu\n", winnode->pid, npids, max_tickets);
		ts = GenerateTimeSlice(winnode, npids, pids, nices);
		printf("Time slice: %f\n", ts); 
	}
	free(plist);
}

size_t InitaliseProcesses(process_node **root, int **pidptr, int **niceptr) 
{
	/* 
		Open the /proc/ directory and scan through the directory, if there is a numerical format item, get its priority (niceness) 
		and store this information in the pids and nices arrays. Then build a tree from this data if it doesn't already exist. 
		If the head node already exists, simply insert the new nodes.
	*/

	unsigned npids = 0;
	DIR *dirpath;
	struct dirent *entry;
	int pid = 0;
	int *pids = NULL;
	int *nices = NULL;

	if ( pids == NULL || nices == NULL ) 
	{
		pids = (int *) ecmalloc(sizeof(int));
		nices = (int *) ecmalloc(sizeof(int));
	}

	dirpath = opendir("/proc/");
	
	if ( dirpath == NULL ) 
	{
		perror("Cannot open directory: ");
		exit(1);
	} else {
		while((entry = readdir(dirpath)) != NULL)
		{
			pid = atoi(entry->d_name);
			
			if ( pid > 0 ) 
			{
				if ( npids == 0 ) 
				{
					pids = InsertElement(pids, npids, 0, pid);
					nices = InsertElement(nices, npids, 0, getpriority(PRIO_PROCESS, pid));
				} else 
				{
					pids = AppendElement(pids, npids, pid);
					nices = AppendElement(nices, npids, getpriority(PRIO_PROCESS, pid));
				}
				
				npids++;	
			}
		}
		printf("Parsed directory /proc/ [%u files]\n", npids);
	}
	
	closedir(dirpath);
	*pidptr = pids;
	*niceptr = nices;
	*root = createTree(pids, nices, 0, npids-1);
	return npids;
}

size_t CheckProcesses(process_node **headptr, int **pidsptr, int **nicesptr, size_t npids)
{
	/* Read through running processes list (/proc/ directory) and check the values retrieved against the current pid list.
	   If the value read is less than the pid at the same index, it needs to be inserted into the tree.
	   If the value read is more than the pid at the same index, the pid used as a comparative (currently in the tree) needs to be removed. 
	*/
	
	DIR *dirpath;
	struct dirent *entry;
	int readpid = 0;
	int nicerating = 0;
	unsigned i = 0;
	int *pids = *pidsptr;
	int *nices = *nicesptr;
	process_node *head = *headptr;

	dirpath = opendir("/proc/");
	if ( dirpath == NULL ) 
	{
		perror("Cannot open directory: ");
		exit(1);
	}

	while ((entry = readdir(dirpath)) != NULL )
	{
		readpid = atoi(entry->d_name);
		if ( readpid > 0 ) {	
			if ( i > npids ) 
			{
				printf("i too large, appending new pids... ");
				/* If the value of i becomes greater than the number of elements in the present array
				we need to start appending new pids */
				nicerating = getpriority(PRIO_PROCESS, readpid);
				head = insert(head, readpid, nicerating); 
				pids = AppendElement(pids, npids, readpid);
				nices = AppendElement(nices, npids, nicerating);
				npids++;
			}
			else if ( readpid < pids[i] ) 
			{
				printf("readpid %d is less than pid at current index %d [%d], adding to tree... ", readpid, i, pids[i]);
				/* If read pid is less than the pid at the current index, a new node needs to be added to the tree
				and the array */
				nicerating = getpriority(PRIO_PROCESS, readpid);
				head = insert(head, readpid, nicerating);
				pids = InsertElement(pids, npids, i, readpid);
				nices = InsertElement(nices, npids, i, nicerating);
				npids++;
			} else if ( readpid > pids[i] ) 
			{
				printf("readpid %d is greater than pid at current index %d [%d] replacing it... ", readpid, i, pids[i]);
				/* If the readpid is greater than the pid at the current index then the old pid no longer exists
				and needs to be replaced by the new pid. */
				nicerating = getpriority(PRIO_PROCESS, readpid);
				head = _delete(head, pids[i]);
				head = insert(head, readpid, nicerating);
				pids = ReplaceElement(pids, npids, i, readpid);
				nices = ReplaceElement(nices, npids, i, nicerating);
			}
			
			i++;
		}
	}

	if ( i < npids ) 
	{
		for ( unsigned n = npids-1; n >= i; n-- ) 
		{
			printf("Removing un-needed pid %d... ", pids[n]);
			head = _delete(head, pids[n]);
			pids = RemoveElement(pids, npids, n);
			npids--;
		}
	}
	
	closedir(dirpath);
	printf("\n");
	*pidsptr = pids;
	*nicesptr = nices;
	*headptr = head;
	return npids;
}

process_node *CheckWinner(process_node *head, unsigned long winner, int bias)
{
	process_node *winning_node = (process_node *) ecmalloc(sizeof(process_node));
	unsigned long accum = 0;
	_sum(head, &winning_node, &accum, winner, bias);
	return winning_node;
}

process_node *DrawWinner(process_node *head, unsigned long long max_tickets)
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

float GenerateTimeSlice(process_node *node, size_t npids, int *pids, int *nices)
{
	unsigned w;
	long s_weights = 0;
	unsigned weight = prio_to_weight[20+(node->nicerating)];
	float ts = 0.0;

	for ( w = 0; w < npids; w++ )
	{
		if ( pids[w] != node->pid ) s_weights += prio_to_weight[(20+nices[w])];
	}
	
	ts = (float)(weight / s_weights) * SCHED_LATENCY;	
	if ( ts < SCHED_GRAN ) return SCHED_GRAN;
	else return ts;
}

/* TODO:
	1. Add timed loop where, after time slice is complete, we do a recheck of the process list. 
		[ This might require some tweaking, such as keeping the dir open the whole time to avoid
			reopening it every loop ].
*/
