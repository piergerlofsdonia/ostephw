#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>

int main() 
{
	int pid = getpid();
	unsigned r = 0;
	int rtnfork = -1;
	char *argpath = "/bin/ls";
	char *argarr[3] = {"ls", "-l", NULL};
	char *env[2] = {"LANG=en_US.UTF8", NULL};
	
	for ( ; r < 6; r++ ) 
	{
		rtnfork = fork();

		if ( rtnfork < 0 ) 
		{
			fprintf(stderr, "Error whilst forking [%d]\n", r+1);
			exit(1);
		} else if ( rtnfork == 0 )
		{
			pid = getpid();
			fprintf(stdout, "Child process [%d] running...\n", pid);
			switch(r) {
				case 0:
					// Execl, execlp, and execle take a List of arguments, comma seperated as below. Execl requires a path, not a program title.
					fprintf(stdout, "execl\n");
					execl(argpath, "-l", NULL);
					break;
				case 1:
					// Execv, execvp, and execvpe take a vector (array) of arguments rather than a list, execv requires a path, not a program title.
					fprintf(stdout, "execv\n");
					execv(argpath, argarr);
					break;
				case 2:
					// As with execl, this uses a list of args but takes a first argument of a program title, not a path.
					fprintf(stdout, "execlp\n");
					execlp(argarr[0], "-l", NULL);
					break;
				case 3:
					// As with execv, this uses an array of args but takes a first argument of a path, not a program title.
					fprintf(stdout, "execvp\n");
					execvp(argarr[0], argarr);
					break;
				case 4:
					fprintf(stdout, "execle\n");
					execle(argpath, "-l", NULL, env);
					break;	
				case 5:
					fprintf(stdout, "execve\n");
					execve(argpath, argarr, env);
					break;
			}
		} else {
			wait(NULL);
			kill(rtnfork, SIGKILL);		
		}

	}

	return 0;

}

/* See; https://man7.org/linux/man-pages/man3/exec.3.html and https://www.cems.uwe.ac.uk/~irjohnso/coursenotes/lrc/system/pc/pc4.htm */
