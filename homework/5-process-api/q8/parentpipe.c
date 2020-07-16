#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#define N_CHILD 2
#define MAX_LEN 255

int main()
{	
	int pid[2];
	int afd[2]; // Parent -> Child A.
	//int bfd[2]; // Parent -> Child B.

	char *buffer = malloc(MAX_LEN + 1);
	unsigned read_n = 0;

	if ( pipe(afd) == -1 ) exit(1);
	
	pid[0] = fork();
	switch(pid[0])
	{
		case 0: // Child A
				close(afd[0]); // Close read end.
				close(STDOUT_FILENO);
				dup2(afd[1], STDOUT_FILENO);
				fprintf(stdout, "[Child %d]: %s", getpid(), "Echo... ");				
				close(afd[1]);
				break;


		default: // Parent 	
				close(afd[1]); // Close read end of A pipe.
				wait(NULL);
				read_n = read(afd[0], buffer, MAX_LEN);
				buffer[read_n-1] = '\0';
				buffer = realloc(buffer, read_n);
				close(afd[0]);
				fprintf(stdout, "[Parent %d]: %s\n", getpid(), buffer);
				break;
	}

	free(buffer);
	return 0;
}
