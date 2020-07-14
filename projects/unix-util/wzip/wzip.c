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
#include <arpa/inet.h>
#include "wzip.h"

int main(int argc, char **argv)
{
	unsigned rtncode = 0;

	if ( argc < 2 ) {
		fprintf(stdout, "wzip: file1 [file2 ...]\n");
		exit(EXIT_FAILURE);
	} 

	rtncode = EncodeFile(argc, argv);
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

int EncodeFile(int argc, char **argv) 
{
	FILE *fp = NULL;
	int nl = 0;
	unsigned arg = 1;
	char *c = malloc(sizeof(char));
	char *chunk = calloc(1, BYTESIZE);
	char prev = 0;
	unsigned read = 0;
	unsigned occur = 1;
	
	for ( ; arg < argc; arg++ ) {
		fp = OpenFile(argv[arg], "rb"); 
		/* fread is required to both zip and unzip the file as it keeps characters like newlines intact
		- using a combination of fgets and fwrite caused issues. */
		while((read = fread(c, sizeof(char), 1, fp)) == 1) {
			if ( *c == prev ) {
				occur++;
			} else if ( prev != '\0' && *c != '\n') {
				//printf("\nOccur: %d, Char: %c - %c\n", occur, *c, prev);
				EncodeString(chunk, prev, occur);
				occur = 1;
				if ( nl != 0 ) {
					EncodeString(chunk, '\n', nl);
					nl = 0;
				}

			}
			
			if ( *c == '\n' ) {
				nl++;
			} else {
				prev = *c;
			}

		}
			
	}

	fclose(fp);
		
	if ( occur > 0 ) { 
		EncodeString(chunk, prev, occur);
	}
	
	if ( *c == '\n') { 
		EncodeString(chunk, '\n', 1);
	}
	free(c);
	free(chunk);	
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

void EncodeString(char *buffer, char c, unsigned o)
{
	snprintf(buffer, BYTESIZE, "%d%c", o, c);
	fwrite(&o, BYTESIZE-1, 1, stdout);
	fwrite(&c, sizeof(char), 1, stdout);
	//fprintf(stdout, "%d%c", o, c);
}
