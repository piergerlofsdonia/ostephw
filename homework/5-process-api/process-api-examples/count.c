#include <stdio.h>
#include <stdlib.h>
#define MAX_LINE_LEN 4095

FILE *OpenFile(char*, const char*);
unsigned long CountLines(FILE*);

int main(int argc, char **argv) 
{
	unsigned arg = 1;
	unsigned count = 0;
	FILE *fileptr;
	
	printf("Starting count program...\n");

	if ( argc < 2 ) 
	{
		fprintf(stderr, "Error whilst parsing input arguments [%d provided, 1+ required]\n", argc-1);
		exit(1);
	}

	for ( ; arg < argc; arg++ ) 
	{
		fileptr = OpenFile(argv[arg], "r");
		count = CountLines(fileptr);
		fprintf(stdout, "%d lines in file: %s\n", count, argv[arg]);
		fclose(fileptr);
	}	

	return 0;
}

FILE *OpenFile(char *path, const char *filemode) 
{
	FILE *fp = fopen(path, filemode);

	if ( fp == NULL ) 
	{
		fprintf(stderr, "Error whilst opening file [%s] using mode [%s]\n", path, filemode);
		exit(1);
	}

	return fp;
}

unsigned long CountLines(FILE *fp) 
{
	unsigned long l = 0;
	char *lp = malloc(MAX_LINE_LEN+1);

	while ( fgets(lp, MAX_LINE_LEN, fp) != NULL )
	{
		l++;
	}


	free(lp);
	return l;
	
}

