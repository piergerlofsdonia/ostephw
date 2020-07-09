/*
	The next tools you will build come in a pair, because one (wzip) is a file compression tool, and the other (wunzip) is a file decompression tool.
	The type of compression used here is a simple form of compression called run-length encoding (RLE). RLE is quite simple: 
	 when you encounter n characters of the same type in a row, the compression tool (wzip) will turn that into the number n and a single instance of the character.
	
	Thus, if we had a file with the following contents:

	aaaaaaaaaabbbb

	the tool would turn it (logically) into:

	10a4b
*/
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

int EncodeString(char *inpstr)
{
	const size_t max = MAXLEN;
	const size_t bs = BYTESIZE;
	unsigned o = 1; /* Occurances */
	unsigned n = 0; /* Counter */
	unsigned len = strlen(inpstr);
	char *buffer;
	
	if ( len <= max ) {
		buffer = malloc(bs+1); /* Maximum length required */
	} else {
		fprintf(stdout, "Error [%d]: Line entered is too long (> %lu)\n", __LINE__, max);
		exit(EXIT_FAILURE);
	}

	for ( ; n < len ; n++ ) {
		if ( n > 0 ) {
			if ( inpstr[n-1] != inpstr[n] ) {
				snprintf(buffer, bs+1, "%d%c", o, inpstr[n-1]);
				fwrite(buffer, bs, 1, stdout);
				o = 1;

			} else {
				o++;
			}
		}
	}

	fprintf(stdout, "\n");
	free(buffer);
	return 0;
}

int EncodeFile(FILE *fileptr) 
{
	char *lp = malloc(sizeof(char) * MAXLEN);

	while(fgets(lp, MAXLEN, fileptr) != NULL) {
		if ( EncodeString(lp) != 0 ) {
			return 1;
		}
	}

	free(lp);
	return 0;
}
