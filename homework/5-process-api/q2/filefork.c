#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main() 
{
	int rtncode = fork();
	FILE *fileptr = fopen("file.txt", "a");

	if ( fileptr == NULL ) 
	{
		fprintf(stderr, "Error opening file!\n");
		exit(1);
	}

	if ( rtncode == 0 ) 
	{
		/* Child process */
		fprintf(fileptr, "Child was here!");
	} else if ( rtncode > 0 )
	{
		/* Parent process */
		fprintf(fileptr, "Parent was here!");
	} else 
	{
		fprintf(stderr, "Fork error!\n");
		exit(1);
	}

	fclose(fileptr);
	return 0;

}
