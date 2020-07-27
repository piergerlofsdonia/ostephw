#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <unistd.h>

typedef struct p {
	unsigned pid;
	int nice;
	struct p *next;
} l_process;

void *Ecmalloc(size_t);
void ParseProcesses(l_process*);

int main(int argc, char **argv)
{
	l_process *plist;
	plist = (l_process *) Ecmalloc(sizeof(l_process));
	ParseProcesses(plist);
	free(plist);	
}

void ParseProcesses(l_process *head) 
{
	unsigned count = 0;
	DIR *dirp;
	struct dirent *entry;
	unsigned long pid;

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
				current->nice = getpriority(PRIO_PROCESS, pid);	
				if ( current->next == NULL ) { current->next = (l_process *) Ecmalloc(sizeof(l_process)); }
				printf("%i [%u]\n", current->nice, current->pid);
				current = current->next;
			}
			count++;
		}
		printf("Parsed directory /proc/ [%u files]\n", count);
	}
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

/* TODO:
	* Assign tickets to each process based upon max_ticket count and weighting formula (probably based upon niceness?)
*/
