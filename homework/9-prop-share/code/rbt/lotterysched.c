/* 
	Lottery-style scheduling using binary search tree structure.  
*/

#include "osteprbt.h" /* Binary search tree functions */
#include "../bst/osteparray.h" /* Array helper functions */
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <unistd.h>
#include <time.h>

#define RUNMAX 10000
#define SCHED_LATENCY 18
#define SCHED_GRAN 2.25
#define DIRPATH "/proc/"

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
DIR *OpenDirectoryStream();
size_t InitaliseProcesses(process_node**, int **pids, int **nices);
size_t CheckProcesses(process_node**, int**, int**, size_t);
float GenerateTimeSlice(process_node*, size_t, int*, int*);
process_node *CheckWinner(process_node*, unsigned long, int);
float DrawWinner(process_node *, process_node**, unsigned long, unsigned long);
unsigned long AssignTickets(process_node *);
void AverageResults(process_node[], float*, float*);

void *ecmalloc(size_t nbytes)
{
	void *voidptr = malloc(nbytes);
	if ( voidptr == NULL ) 
	{
		fprintf(stdout, "ERROR: Malloc failure on line [%d]!\n", __LINE__);
		exit(EXIT_FAILURE);
	}
	return voidptr;
}

int main(int argc, char **argv)
{
	ROOT = (process_node *) ecmalloc(sizeof(process_node));
	NIL = (process_node *) ecmalloc(sizeof(process_node));
	NIL->parent = NIL; NIL->left = NIL; NIL->right = NIL;
	NIL->pid = 0;
	NIL->colour = BLACK;

	// 

	unsigned long max_tickets;
	int *pids = NULL;
	int *nices = NULL;
	process_node *winning_node = NULL;
	process_node winners[RUNMAX];
	size_t npids = 0;
	float *times = (float *) ecmalloc(sizeof(float) * RUNMAX);
	float *slices = (float *) ecmalloc(sizeof(float) * RUNMAX);
	
	npids = InitaliseProcesses(&ROOT, &pids, &nices);
	
	/*max_tickets = AssignTickets(plist);
	times[i] = DrawWinner(plist, &winnode, i, max_tickets);
	winners[i] = *winnode;
	slices[i] = GenerateTimeSlice(&winners[i], npids, pids, nices);
	i++;

	for ( ; i < RUNMAX ; i++ ) 
	{
		npids = CheckProcesses(&plist, &pids, &nices, npids);
		max_tickets = AssignTickets(plist);
		times[i] = DrawWinner(plist, &winnode, i, max_tickets);
		winners[i] = *winnode;
		slices[i] = GenerateTimeSlice(&winners[i], npids, pids, nices);
	}

	AverageResults(winners, times, slices);

	*/

	free(ROOT);
	free(pids);
	free(nices);
	free(times);
	free(slices);
}

void AverageResults(process_node winners[], float *times, float *slices)
{
	/*
		Average the results of the times and slices arrays. Print each winner, time, and slice to the standard 
		output alongside the averages for the entire run.
	*/
	unsigned i = 0;
	float timesum = 0.0;
	float slicesum = 0.0;
	float timeavg = 0.0;
	float sliceavg = 0.0;

	for ( ; i < RUNMAX ; i++ )
	{
		fprintf(stdout, "Winner for lottery draw %d: [pid: %d, nice: %d, tickets: %d, slice: %f] in a time of %fms.\n",
					   	i, winners[i].pid, winners[i].nicerating, winners[i].tickets, slices[i], times[i]);
		timesum += times[i];
		slicesum += slices[i];
	}

	timeavg = timesum / RUNMAX;
	sliceavg = slicesum / RUNMAX;
	fprintf(stdout, "\nAverages: time-taken to draw [%fms]; time-slice [%fms].\n", timeavg, sliceavg); 
}

size_t InitaliseProcesses(process_node **root, int **pidptr, int **niceptr) 
{
	/* 
		Open the /proc/ directory and scan through the directory, if there is a numerical format item, get its priority (niceness) 
		and store this information in the pids and nices arrays. Then build a tree from this data if it doesn't already exist. 
		If the head node already exists, simply insert the new nodes.
	*/

	DIR *dirstream = OpenDirectoryStream();
	unsigned npids = 0;
	struct dirent *entry;
	int pid = 0;
	int *pids = NULL;
	int *nices = NULL;

	if ( pids == NULL || nices == NULL ) 
	{
		pids = (int *) ecmalloc(sizeof(int));
		pids[0] = 0;
		nices = (int *) ecmalloc(sizeof(int));
		nices[0] = 0;
	}

	while((entry = readdir(dirstream)) != NULL)
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
	
	closedir(dirstream);
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
		
	DIR *dirstream = OpenDirectoryStream();
	struct dirent *entry;
	int readpid = 0;
	int nicerating = 0;
	unsigned i = 0;
	int *pids = *pidsptr;
	int *nices = *nicesptr;
	process_node *head = *headptr;

	while ((entry = readdir(dirstream)) != NULL )
	{
		readpid = atoi(entry->d_name);
		if ( readpid > 0 ) {	
			if ( i > npids ) 
			{
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
				/* If read pid is less than the pid at the current index, a new node needs to be added to the tree
				and the array */
				nicerating = getpriority(PRIO_PROCESS, readpid);
				head = insert(head, readpid, nicerating);
				pids = InsertElement(pids, npids, i, readpid);
				nices = InsertElement(nices, npids, i, nicerating);
				npids++;
			} 
			else if ( readpid > pids[i] ) 
			{
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
		/* Remove residual pids from the original list that are no longer in the /proc/ directory (e.g. [1, 2, 3, 4] vs. [1, 2, 3] then remove 4). */
		unsigned n = npids-1;
		for (; n >= i; n--) 
		{
			head = _delete(head, pids[n]);
			pids = RemoveElement(pids, npids, n);
			npids--;
			if ( n == i ) break;
		}
	}
	
	closedir(dirstream);
	*pidsptr = pids;
	*nicesptr = nices;
	*headptr = head;
	return npids;
}

process_node *CheckWinner(process_node *head, unsigned long winner, int bias)
{
	/* Run the _sum function to accumulate ticket values until we find the winner, then return that node.  */
	process_node *winning_node = (process_node *) ecmalloc(sizeof(process_node));
	unsigned long accum = 0;
	_sum(head, &winning_node, &accum, winner, bias);
	return winning_node;
}

float DrawWinner(process_node *head, process_node **winnode, unsigned long i, unsigned long max_tickets)
{
	/* 
	   Randomise a winning ticket number between 0 and max_tickets, then search through the tree to find the winning pid.
	   Time the majority of this function to find the time it takes to randomise the winner and find it in the bst.
	   Return the time different in ms (floating point).
	 */
	struct timeval start, stop;
	if ( *winnode == NULL ) *winnode = (process_node *) ecmalloc(sizeof(process_node));	
	gettimeofday(&start, NULL);
	/* Randomise winner */
	srand(time(NULL) * clock() / getpid());
	unsigned long long winner = rand() % max_tickets;
	int bias = rand() % 2;
	*winnode = CheckWinner(head, winner, bias);
	gettimeofday(&stop, NULL);
	
	return (float)(stop.tv_usec - start.tv_usec) / 1000 + (float)(stop.tv_sec - start.tv_sec) * 1000;
}

unsigned long AssignTickets(process_node *head)
{
	/*
	   For each node, assign a ticket value to it between 0 and 100 dependent on the node's niceness (e.g. +19 = 1 ticket, -20 = 100 tickets).
	   Return the number of tickets allocated (accumulated value).
	*/
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
	/*
		Generate a time slice value by dividing the weighted value of the node by the sum of all weights in the run queue.
		If this value is less than the scheduler granularity minimum (min_granularity) then return this value, otherwise
		return the value calculated.
	*/
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

DIR *OpenDirectoryStream()
{
	DIR *dirstream = opendir(DIRPATH);
	if ( dirstream == NULL ) 
	{
		perror("Error opening directory: ");
		exit(1);
	}

	return dirstream;
}
