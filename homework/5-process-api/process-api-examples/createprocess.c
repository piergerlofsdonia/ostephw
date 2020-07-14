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

/*

	Notes:
		* Processes are created on UNIX systems using a pair of system calls: 'fork()' and 'exec()'.
		* Processes can be awaited in the creator (master) process using 'wait()'.
		* The CPU scheduler is non-determinstic, making it difficult to predict what it will do at any one time (particularly in m.t. programs).
			However, using wait() [or waitpid()] will force a parent to wait for a child process to finish executing.
		* If a child process terminates without being waited for by the parent it becomes a 'zombie' process [1] - the parent can still read its return code due 
			to the process still existing in the process table [2]. 	
		* Fork() is used when a process wants to be run within the calling program, exec() [and variants] are used when a process is required within a seperate,
			different program.
		* The premise that fork() spawns processes within the calling program allows malicious programs such as a fork bomb to function - a fork bomb is
			a method of using up all the processing resource on a system by recursively calling fork() (often using a bash script) [3].
		* Execvp() is a routine from unistd that replaces the current process with a new one, running the program given in its first argument. The second 
			argument of execvp() is a list of null-terminated string pointers [4] (arg1, arg2, arg3, ... argc-1, represented as char** argv in a normal program)
			e.g. execvp("ls", {"ls", "-a", NULL});
		* Unix is the best thing in the world, even better than ice cream.

	Sources:
	[1] Waitpid(2) manual: https://linux.die.net/man/2/waitpid
	[2] 'Zombie Process' wiki page, example and explaination given below article: https://en.wikipedia.org/wiki/Zombie_process
	[3] :(){ :|:& };: explained: https://askubuntu.com/questions/159491/why-did-the-command-make-my-system-lag-so-badly-i-had-to-reboot
	[4] Execvp(3) manual: https://linux.die.net/man/3/execvp
 */
