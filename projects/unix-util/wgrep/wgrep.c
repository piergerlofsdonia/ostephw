/*
	The second utility you will build is called wgrep, a variant of the UNIX tool grep. This tool looks through a file, 
	line by line, trying to find a user-specified search term in the line. If a line has the word within it, 
	the line is printed out, otherwise it is not.
	Here is how a user would look for the term foo in the file bar.txt:

	<prompt> ./wgrep foo bar.txt
			this line has foo in it
			so does this foolish line; do you see where?
			even this line, which has barfood in it, will be printed.

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "wgrep.h"

int main(int argc, char **argv)
{
	/* Input args should be <promp> ./program keyword file 
		1. Check for valid arg count.
		2. Check valid filepath with fopen (OpenFile)
		3. Check valid keyword with strlen check of keyword ( < 255,  > 1).
	 */

	FILE *fileptr = NULL;
	unsigned arg = 2;

	if ( argc < 2 ) { 
		fprintf(stderr, "Invalid number of arguments given: [usage] ./program keyword path\n");
		exit(1);
	} else if ( argc < 3 ) {
		return FAPstdin(argv[1]);
	}

	for ( ; arg < argc; arg++ ) {
		fileptr = OpenFile(argv[arg], "r");
		if ( fileptr == NULL ) {
			fprintf(stdout, "Cannot open file: %s\n", argv[arg]);
			exit(1);
		} else {
			if ( FindAndPrint(fileptr, argv[1]) == -1 ) {
				fprintf(stdout, "No matches found [%s] in file: %s\n", argv[1], argv[arg]);
			}

		}

	}

	return 0;

}

FILE *OpenFile(char *path, const char *mode)
{
	FILE *fileptr = fopen(path, mode);
	if ( fileptr != NULL ) {
		return fileptr;
	} else {
		fprintf(stderr, "Cannot open file %s\n", path);
		exit(EXIT_FAILURE);	
	}

}

int FindAndPrint(FILE *fileptr, char *findstr)
{
	unsigned max = MAX_LEN;
	char *lineptr = malloc(sizeof(char) * max);
	int found_count = 0;

	while(fgets(lineptr, max, fileptr) != NULL) {
		if ( strstr(lineptr, findstr) != NULL ) {
			fprintf(stdout, "%s", lineptr);
			found_count++;
		}

	}
	
	if ( found_count == 0 ) { 
		return -1;		 
	} else {
		return found_count;
	}

}

int FAPstdin(char *findstr)
{
	unsigned max = MAX_STDIN;
	char *strptr = malloc(sizeof(char) * max);
	fprintf(stdout, "Use keyboard input to enter a line to grep\n");

	while(fgets(strptr, max, stdin)) {
		strptr[strlen(strptr)] = '\0';
		if ( strstr(strptr, findstr) != NULL ) {
			/* RED and RESET codes used to somewhat emulate grep */
			fprintf(stdout, "%s%s\n%s", "\x1B[31m", strptr, "\x1B[0m");	
		}
	}

	free(strptr);
	return 1;
}
