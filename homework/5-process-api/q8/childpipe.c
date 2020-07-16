#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#define N_CHILD 2

/* Taken from Rutgers U Pipes Tutorial: https://www.cs.rutgers.edu/~pxk/416/notes/c-tutorials/pipe.html 

Note: Found it extremely difficult to make the below work without converting the proceses to programs - e.g. close(in/out), dup2(in/out, stdin/stdout) and print across processes. 
 */

int main()
{
		
	int pid[N_CHILD];
	int fd[N_CHILD];
	char *acmds[4] = {"ls", "-al", "/", NULL}; // Print root directory 
	char *bcmds[4] = {"tr", "a-z", "A-Z", NULL}; // Convert into capitals.

	if ( pipe(fd) == -1 ) exit(1);

	pid[0] = fork(); // Create child A.
	switch(pid[0])
	{
		case 0: // Child A, SOURCE
				close(fd[0]); 
				dup2(fd[1], STDOUT_FILENO);
				execvp(acmds[0], acmds);
				exit(1);

		default: // Parent 	
				pid[1] = fork(); // Create child B.
				switch(pid[1]) 
				{
					case 0: // Child B, DEST.
						close(fd[1]);	
						dup2(fd[0], STDIN_FILENO);
						execvp(bcmds[0], bcmds);

				}
				waitpid(-1, NULL, 0);
				break;
	}
	
	close(fd[0]); 
	close(fd[1]);
	return 0;
}
