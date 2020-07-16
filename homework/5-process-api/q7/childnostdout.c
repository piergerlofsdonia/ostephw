#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
	int pid;

	pid = fork();

	if ( pid > 0 ) 
	{
		pid = getpid();
		printf("[%d, Parent] Printing to the console\n", pid);
	} else if ( pid == 0 )
	{
		pid = getpid();
		close(STDOUT_FILENO);
		printf("[%d, Child] Printing to the console\n", pid);
	}

	return 0;
}
