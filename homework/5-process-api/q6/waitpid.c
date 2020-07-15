#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>

int main() 
{
	int pid = getpid();
	int rtnfork = -1;
	int rtnwait = -1;
	int waitstatus;
	rtnfork = fork();

	if ( rtnfork == 0 ) 
	{	
		/* Child process */
		pid = getpid();
		printf("%d[%d] Hello! ", rtnwait, pid);
	} else if ( rtnfork > 0 ){
		/* Parent process */
		rtnwait = waitpid(rtnfork, &waitstatus, WUNTRACED);	
		printf("Goodbye! %d[%d]\n", rtnwait, pid);
	} else 
	{
		fprintf(stderr, "Fork error!\n");
		exit(1);
	}

	return 0;

}
