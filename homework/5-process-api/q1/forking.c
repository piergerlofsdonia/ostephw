#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main() 
{
	int rtncode = fork();
	int x = 0; int y = 25;

	if ( rtncode == 0 ) 
	{
		/* Child process */
		//x = 50;
		printf("Child process [%d]: %d, %d\n", getpid(), x, y);
	} else if ( rtncode > 0 )
	{
		/* Parent process */
		x = 100;
		printf("Parent process [%d]: %d, %d\n", getpid(), x, y);
	} else 
	{
		fprintf(stderr, "Fork error!\n");
		exit(1);
	}

	return 0;

}
