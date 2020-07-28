#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <unistd.h>
#include <time.h>

typedef struct p {
	unsigned pid;
	int nicerating;
	unsigned tickets;
	struct p *next;
} l_process;

void *Ecmalloc(size_t);
unsigned long long ParseProcesses(l_process*);
int DrawWinner(l_process*, unsigned long long);

int main(int argc, char **argv)
{
	unsigned long long max_tickets;
	l_process *plist;
	plist = (l_process *) Ecmalloc(sizeof(l_process));
	max_tickets = ParseProcesses(plist);
	
	printf("Winning process: %d \n", DrawWinner(plist, max_tickets));
	free(plist);
}

unsigned long long ParseProcesses(l_process *head) 
{
	unsigned count = 0;
	DIR *dirp;
	struct dirent *entry;
	unsigned long pid;
	unsigned long long ticketcount = 0;

	l_process *current = head;
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
				current->pid = pid;
				current->nicerating = getpriority(PRIO_PROCESS, pid);	
				current->tickets = rand() % 100;
				ticketcount += current->tickets;
				if ( current->next == NULL ) { current->next = (l_process *) Ecmalloc(sizeof(l_process)); }
				current = current->next;
			}
			count++;
		}
		printf("Parsed directory /proc/ [%u files]\n", count);
	}

	return ticketcount;
}

void *Ecmalloc(size_t nbytes) 
{
	void *allocated = malloc(nbytes);
	if ( allocated == NULL ) 
	{
		fprintf(stderr, "Error whilst allocating %lu bytes at %p\n", nbytes, allocated);
		exit(1);
	} 
	
	return allocated;
}

int DrawWinner(l_process *head, unsigned long long max_tickets)
{
	/* Randomise winner */
	srand(time(NULL) * clock() / getpid());
	l_process *current = head;
	unsigned long long winner = rand() % max_tickets;
	unsigned long long tickcount = 0;

	printf("Finding ticket holder: %llu\n", winner);

	while(tickcount < winner) 
	{
		tickcount += current->tickets;
		current = current->next;
	}

	return current->pid;
}


/* TODO:
	* Assign tickets to each process based upon max_ticket count and weighting formula (probably based upon niceness?)
	* Add checking to DrawWinner
	* Add sort before DrawWinner to sort list into order of most tickets (descending)./
*/
