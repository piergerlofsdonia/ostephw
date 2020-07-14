#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <arpa/inet.h>
#include "betterzip.h"

int main(int argc, char **argv)
{
	unsigned arg = 1;
	unsigned rtncode = 0;
	FILE *wfp, *rfp;

	if ( argc < 2 ) {
		fprintf(stdout, "wzip: incorrect number of arguments.\n");
		exit(EXIT_FAILURE);
	} 

	wfp = OpenFile(argv[argc-1], "w+");

	for ( ; arg < (argc-1) ; arg++ ) {
		rfp = OpenFile(argv[arg], "r");
		rtncode = EncodeFile(rfp, wfp);
		fclose(rfp);
	}

	fclose(wfp);
	return rtncode;
}

FILE *OpenFile(char *path, const char *mode) 
{
	FILE *fileptr = fopen(path, mode);
	
	if ( fileptr == NULL ) {
		fprintf(stdout, "wzip: cannot open file\n");
		exit(EXIT_FAILURE);
	}

	return fileptr;

}

int EncodeFile(FILE *readfile, FILE *writefile) 
{
	char *c = malloc(sizeof(char));
	char *chunk = calloc(1, BYTESIZE);
	char prev = 0;
	unsigned read = 0;
	unsigned occur = 1;

	while((read = fread(c, sizeof(char), 1, readfile)) == 1) {
		if ( *c == prev ) {
			occur++;
		} else if ( prev != 0 ) {
			EncodeString(writefile, chunk, prev, occur);
			occur = 1;
		} else { occur = 1; }

		prev = *c;	
	}

	free(chunk);
	free(c);
	return read;
}

void EncodeString(FILE *fp, char *buffer, char c, unsigned o)
{
	snprintf(buffer, BYTESIZE, "%d%c", o, c);
	fwrite(buffer, BYTESIZE, 1, fp);	
}
