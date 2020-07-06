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
		fprintf(stderr, "No argument provided, enter a path to a program to print\n(e.g. ./wcat \"/media/<user>/src/file.c\")\n");
		exit(EXIT_FAILURE);
	}

	for ( c = 1; c < argc; c++ ) {
		fileptr = OpenFile(argv[c], "r", 0);
		CountLines(fileptr, 1);
		fputs("\n", stdout);
		fclose(fileptr);
	}
	
}

FILE *OpenFile(char *path, const char *mode, unsigned depth)
{
	FILE *fileptr = fopen(path, mode);
	
	if ( fileptr != NULL ) {
			return fileptr;
	}

	if ( depth > 0) {
		fprintf(stderr, "Error [%d]: Opening %s\n", __LINE__, path);
		exit(EXIT_FAILURE);
	} else {
		depth++;
		fileptr = OpenFile(path, "ab+", depth);
		fileptr = freopen(path, mode, fileptr);
		return fileptr;
	} 

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
