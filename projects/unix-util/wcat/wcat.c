/* The program wcat is a simple program. Generally, it reads a file as specified by the user and prints 
   its contents. A typical usage is as follows, in which the user wants to see the contents of main.c,
   and thus types:

   prompt> ./wcat main.c
		#include <stdio.h>
		...
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "wcat.h"

int main(int argc, char **argv)
{
	unsigned c;
	FILE *fileptr = NULL;

	if ( argc <= 1 ) {
		/* This SHOULD be a while loop printing stdin -> stdout (see: wgrep) */
		exit(0);
	}

	for ( c = 1; c < argc; c++ ) {
		fileptr = OpenFile(argv[c], "r");
		CountLines(fileptr, 1);
		fclose(fileptr);
	}
	
	return 0;
}

FILE *OpenFile(char *path, const char *mode)
{
	FILE *fileptr = fopen(path, mode);

	if ( fileptr != NULL ) {
		return fileptr;
	}
	
	fprintf(stdout, "wcat: cannot open file\n");
	exit(EXIT_FAILURE);
}

int CountLines(FILE *fileptr, int print_flag)
{
	int max = MAX_LEN;
	char *lineptr = malloc(sizeof(char) * max);
	int line = 0;
	
	while(fgets(lineptr, max, fileptr) != NULL) {
		if ( print_flag != 0 ) {
			fputs(lineptr, stdout);
		}

		line++;

	}
	
	free(lineptr);
	return line;
}
