/* */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h> /* Used for strdup */
#include <sys/wait.h> /* Used for wait system call */

int SetupArgs(char **argv, size_t nargs, char **strarr);

int main(int argc, char **argv)
{
	int waitcode;
	int rtncode;
	unsigned pid;
	char **argarr = calloc(sizeof(char*), argc);
	char *pcmd = NULL;

	/* Put input arguments into a different array, append the name of the file to run on the end of a str with the prefix "./" */
	rtncode = SetupArgs(argv, argc-1, argarr);
	pcmd = malloc(strlen(argarr[0])+3);
	strncpy(pcmd, "./", 3);	
	strcat(pcmd, argarr[0]);

	if ( rtncode != 0 )
	{
		return rtncode;
	}

	fprintf(stdout, "Running process: %d\n", getpid());
	rtncode = fork();
	fprintf(stdout, "Return code: %d\n", rtncode);

	if ( rtncode < 0 ) 
	{
		fprintf(stderr, "Error whilst creating process\n");
		exit(1);
	} else if ( rtncode > 0 ) 
	{
		pid = getpid(); /* Parent pid */ 
		waitcode = wait(NULL); /* Adding this wait makes the execution deterministic */ 
		fprintf(stdout, "Parent process: %d [wait code: %d]\n", pid, waitcode); /* Order: 2. */
		free(argarr);
	} else 
	{
		pid = getpid(); /* Child process id */
		
		fprintf(stdout, "Changing process to run: %s\n", argarr[0]);
		
		if ( execvp(pcmd, argarr) < 0 ) 
		{
			printf("%s, %s, %s, %s\n", argarr[0], argarr[1], argarr[2], argarr[3]);
			perror("Error whilst executing execvp: ");
			exit(1);
		}

		fprintf(stdout, "Child process wait code: [pid: %d]\n", pid); /* Order: 1. */
	}

	return 0;
}

int SetupArgs(char **argv, size_t nargs, char **strarr)
{
	/* Copy argv array strings to strarr strings, terminate with a NULL then return a success/fail code */
	unsigned n;
		
	printf("nargs: %lu\n", nargs);
	for ( n = 0; n < nargs; n++ )
	{
		strarr[n] = strdup(argv[n+1]);
		if ( strarr[n] == NULL ) 
		{
			perror("Error whilst setting args: ");
			return 1;
		} else { printf("%s\n", strarr[n]); }
	}

	strarr[nargs] = NULL;
	return 0;

}
