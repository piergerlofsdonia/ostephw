#define _GNU_SOURCE
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sched.h>

#define	CYCLES 250000

int InitCpuset(cpu_set_t*, unsigned);
int InitPipes(int**, unsigned);
void SetAffinity(int, cpu_set_t*);

int main()
{
	int pid;
	cpu_set_t cpuset;
	int pipea[2] = {0, 0};
	int pipeb[2] = {0, 0};
	int *ptrarr[2] = {pipea, pipeb};
	struct timeval currtime, prevtime;
	unsigned long c;
	float diffs = 0; float diffus = 0;
	
	if ( InitCpuset(&cpuset, 0) < 0 ) { 
		fprintf(stderr, "Invalid number of CPUs checked during initalisation\n");
		exit(1);
	}

	if ( InitPipes(ptrarr, 2 ) < 0 ) {
		fprintf(stderr, "Error whilst initalising pipes\n");
		exit(1);
	}

	pid = fork();
	
	switch(pid)
	{
		case -1:
			perror("Invalid fork: ");
			exit(1);

		case 0:
			/* Child */
			SetAffinity(getpid(), &cpuset);	
			for ( c = 0; c < CYCLES; c++ ) 
			{
				read(pipea[0], NULL, 0); 
				write(pipeb[1], NULL, 0);
			}
			
			return 0;	

		default:
			/* Parent */
			SetAffinity(getpid(), &cpuset);
			
			gettimeofday(&prevtime, NULL);
			for ( c = 0; c < CYCLES; c++ ) 
			{
				write(pipea[1], NULL, 0);
				read(pipeb[0], NULL, 0);
			}

			gettimeofday(&currtime, NULL);
			waitpid(-1, NULL, 0);
			diffs = (currtime.tv_sec - prevtime.tv_sec);
			diffus = (currtime.tv_usec - prevtime.tv_usec);
			break;
	}
	
	fprintf(stdout, "%d: %fus\n", CYCLES, ((diffs * 1000000) + diffus)/CYCLES);	
	return 0;
}

int InitCpuset(cpu_set_t *cpusetptr, unsigned cputoadd)
{
	/* Zero the cpu_set_t struct - this represents a set of CPUs, it is used for the sched_<> procedures.
		Add a new CPU to the set (given by cputoadd).
		Count the set to determine only one CPU exists within it, return the result of this check.
	*/

	CPU_ZERO(cpusetptr);
	CPU_SET(cputoadd, cpusetptr);
	
	if ( CPU_COUNT(cpusetptr) != 1 ) return -1;
	
	return 0;
}

int InitPipes(int **pipeptrarr, unsigned n_pipes)
{
	unsigned n = 0;
	int *pipeptr;

	for ( ; n < n_pipes; n++ )
	{
		pipeptr = pipeptrarr[n];
		if ( pipe(pipeptr) == -1 ) return -1;
	}

	return 0;
}

void SetAffinity(int pid, cpu_set_t *cpusetptr)
{
	int schedrtn = sched_setaffinity(pid, sizeof(cpu_set_t), cpusetptr);
	if ( schedrtn == -1 ) 
	{
		fprintf(stderr, "Error: Cannot set affinity for process [%d].\n", pid);
		exit(1);
	}
}
