/*
	The next tools you will build come in a pair, because one (wzip) is a file compression tool, and the other (wunzip) is a file decompression tool.
	The type of compression used here is a simple form of compression called run-length encoding (RLE). RLE is quite simple: 
	 when you encounter n characters of the same type in a row, the compression tool (wzip) will turn that into the number n and a single instance of the character.
	
	Thus, if we had a file with the following contents:

	aaaaaaaaaabbbb

	the tool would turn it (logically) into:

	10a4b
*/

/* Use fwrite and fread exclusively to maintain correspondance between wzip and wunzip, redo wzip accordingly */

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "wzip.h"

int main(int argc, char **argv)
{
	unsigned arg = 1;
	unsigned rtncode = 0;

	if ( argc < 2 ) {
		fprintf(stdout, "wzip: incorrect number of arguments.\n");
		exit(EXIT_FAILURE);
	} 

	for ( ; arg < argc ; arg++ ) {
		FILE *fp = OpenFile(argv[arg], "r");
		unsigned rtncode = EncodeFile(fp);
		if ( rtncode != 0 ) {
			fprintf(stdout, "wzip: error whilst parsing file %s\n", argv[arg]);
			return 1;
		}

		fclose(fp);
	}

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

int EncodeFile(FILE *fileptr) 
{
	char *c = malloc(sizeof(char));
	char *chunk = malloc(BYTESIZE);
	char prev = 0;
	unsigned read = 0;
	unsigned occur = 0;

	/* fread is required to both zip and unzip the file as it keeps characters like newlines intact
	   - using a combination of fgets and fwrite caused issues. */
	while((read = fread(c, sizeof(char), 1, fileptr)) == 1) {
		if ( *c == prev ) {
			occur++;
		} else if ( occur > 0 ) {
			snprintf(chunk, BYTESIZE, "%d%c", occur, prev);
			fwrite(chunk, BYTESIZE, 1, stdout);
			occur = 1;
		} else { occur = 1; }

		prev = *c;	
	}

	free(chunk);
	free(c);
	return read;

	/*
	char *lp = malloc(sizeof(char) * MAXLEN);
	   while(fgets(lp, MAXLEN, fileptr) != NULL) {
		if ( EncodeString(lp) != 0 ) {
			return 1;
		}
	}
	free(lp);
	*/

}
