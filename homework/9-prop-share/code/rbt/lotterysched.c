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


void *ecmalloc(size_t);
DIR *OpenDirectoryStream();
size_t InitaliseProcesses(process_node**, int **pids);
size_t CheckProcesses(process_node**, int**, int**, size_t);
float GenerateTimeSlice(process_node *, process_node *);
process_node *CheckWinner(process_node*, unsigned long, int);
float DrawWinner(process_node *, process_node**, unsigned long, unsigned long);
process_node *GetWinner(process_node*, unsigned long long, int);
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

	int *pids = NULL;
	size_t npids = 0;
	float timev = 0.0;
	unsigned long i = 0;	
	process_node *winning_node = NULL;	
	npids = InitaliseProcesses(&ROOT, &pids);
	AssignNices(ROOT);
	size_t max_tickets = AssignTickets(ROOT);
	timev = DrawWinner(ROOT, &winning_node, i, max_tickets);
	float time_slice = GenerateTimeSlice(ROOT, winning_node);
	printf("Winning node: [%d] in time [%f], slice is [%f]ms {%f}\n", winning_node->pid, timev, time_slice, SCHED_GRAN);
	free(ROOT);
	free(pids);
}

size_t InitaliseProcesses(process_node **root, int **pidptr) 
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

	if ( pids == NULL ) 
	{
		pids = (int *) ecmalloc(sizeof(int));
		pids[0] = 0;
	}

	while((entry = readdir(dirstream)) != NULL)
	{
		pid = atoi(entry->d_name);
			
		if ( pid > 0 ) 
		{
			if ( npids == 0 ) 
			{
				pids = InsertElement(pids, npids, 0, pid);
			} else 
			{
				pids = AppendElement(pids, npids, pid);
			}
				
			npids++;	
		}
	}
	
	closedir(dirstream);
	*pidptr = pids;
	unsigned long nnodes = CreateTree(root, pids, npids);
	if ( nnodes == npids ) return npids;
	else fprintf(stderr, "ERROR: Incorrect number of nodes [%lu : %u].\n", nnodes, npids); exit(1);
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

float DrawWinner(process_node *root, process_node **winning_node, unsigned long i, unsigned long max_tix)
{
	struct timeval start, end;
	if ( *winning_node == NULL ) *winning_node = (process_node *) ecmalloc(sizeof(process_node));
	gettimeofday(&start, NULL);
	srand(time(NULL) * clock() / getpid());
	unsigned long long winner = rand() % max_tix;
	int bias = rand() % 2;
	*winning_node = GetWinner(root, winner, bias);
	gettimeofday(&end, NULL);
	return (float)(end.tv_usec - start.tv_usec) / 1000 + (float)(end.tv_sec - start.tv_sec) * 1000;
}

process_node *GetWinner(process_node *root, unsigned long long winner, int bias)
{
	process_node *winning_node = (process_node *) ecmalloc(sizeof(process_node));
	unsigned long accum = 0;
	SumToFind(root, &winning_node, &accum, winner, bias);
	return winning_node;
}

float GenerateTimeSlice(process_node *root, process_node *winning_node)
{
	// Traverse to get sum of weighted pids. Get node, is the node the same as the input pid? If so, ignore it, if not, accumulate 
	// the weighted conversion (20+nicerating) looked up in prio_to_weight table.
	// Use returned weighted sum to calculate the time slice (weight of pid / summed weights) * SCHED_LATENCY; or SCHED_GRAN if < SCHED_GRAN.

	long sum_weights = SumWeights(root, prio_to_weight[(20+root->nicerating)]);
	int weight = prio_to_weight[(20+winning_node->nicerating)];
	float ts = (float)(weight/sum_weights) * SCHED_LATENCY; 
	ts = ( ts < SCHED_GRAN ) ? SCHED_GRAN : ts;
	return ts;
}
