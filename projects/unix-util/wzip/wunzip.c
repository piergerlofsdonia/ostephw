/* 
	The wunzip tool simply does the reverse of the wzip tool, taking in a compressed file and writing (to standard output again) the uncompressed results. 
	For example, to see the contents of file.txt, you would type:

	<prompt> ./wunzip file.z

	wunzip should read in the compressed file (likely using fread()) and print out the uncompressed output to standard output using printf(). 
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "wzip.h"

int main(int argc, char **argv) 
{
	unsigned arg = 1;
	FILE *fp = NULL;

	for ( ; arg < argc; arg++ ) {
		fp = OpenFile(argv[arg], "rb");
		return DecodeFile(fp);	
	}

}

FILE *OpenFile(char *path, const char *mode)
{
	FILE *fileptr = fopen(path, mode);
	
	if ( fileptr == NULL ) {
		fprintf(stdout, "wunzip: error opening file %s\n", path);
		exit(EXIT_FAILURE);
	}
	
	return fileptr;
}

int DecodeFile(FILE *fileptr)
{
	char *chunk = malloc(BYTESIZE);
	char *conv = malloc(BYTESIZE-1);
	char c = 0;
	size_t len = 0;
	unsigned o = 0;
	unsigned n;
	unsigned read = 0;

	while((read=fread(chunk, BYTESIZE, 1, fileptr)) == 1) {
		len = strlen(chunk);
		c = chunk[len-1];
		o = atoi(chunk);
		for(n=0; n<o; n++) 
		{
			fprintf(stdout, "%c", c);
		}
	}

	free(chunk);
	free(conv);
	return 0;
}
