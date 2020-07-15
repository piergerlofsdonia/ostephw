#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>

int main() 
{
	int main_pid = getpid();
	int rtnfork = -1;
	rtnfork = fork();

	if ( rtnfork == 0 ) 
	{	
		/* Child process */
		printf("Hello! ");
		kill(main_pid, SIGCONT);
	} else if ( rtnfork > 0 ){
		/* Parent process */
		kill(main_pid, SIGSTOP);
		printf("Goodbye!\n");
	} else 
	{
		fprintf(stderr, "Fork error!\n");
		exit(1);
	}

	return 0;

}
