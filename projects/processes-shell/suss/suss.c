#define _GNU_SOURCE
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h> /* Required for posix_openpt */
#include <stdio.h>
#include <string.h>
#include <linux/limits.h> /* PATH_MAX */

#define USERNAME_MAX 32
#define MAX_ARG_LEN 10

typedef struct {
	char *username;
	char *cwd;
} user_details;

user_details GetDetails();
void ShellLoop(user_details);

int main()
{
	user_details ud_struct = GetDetails();
	ShellLoop(ud_struct);	
	return 0;
}

void ShellLoop(user_details ud_struct)
{
	char *clinput = malloc((MAX_ARG_LEN * PATH_MAX) + 1);
	memset(clinput, 0, PATH_MAX); 

	while(1) 
	{
		fprintf(stdout, "%s: %s$ ", ud_struct.username, ud_struct.cwd);
		fgets(clinput, (MAX_ARG_LEN * PATH_MAX), stdin);
		printf("%s\n", clinput);
		/* TODO: Write parser to take clinput string and splice it down into a set of commands & arguments */
	}
}

user_details GetDetails() 
{
	user_details blank;
	blank.username = malloc(USERNAME_MAX+1);
	blank.username = cuserid(blank.username);
	blank.cwd = malloc(PATH_MAX+1);
	blank.cwd = getcwd(blank.cwd, PATH_MAX);
	return blank;
}
