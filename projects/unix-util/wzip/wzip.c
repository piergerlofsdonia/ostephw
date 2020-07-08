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
#define MAX_LEN 255

int main(int argc, char **argv)
{
	FILE *fp = OpenFile("unencoded.txt", "r");
	unsigned rtncode = EncodeFile(fp);
	fclose(fp);
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

char *EncodeString(char *inpstr)
{
	unsigned o = 1; /* Occurances */
	unsigned n = 0; /* Counter */
	unsigned nadd = 0;
	unsigned len = strlen(inpstr);
	unsigned buffsize = 0;
	char *buffer;
	
	if ( len <= MAX_LEN ) {
		buffer = malloc(sizeof(char) * (len+1)); /* Maximum length required */
	} else { 
		fprintf(stdout, "Error [%d]: Line entered is too long (> %d)\n", __LINE__, MAX_LEN);
		exit(EXIT_FAILURE);
	}

	for ( ; n < len ; n++ ) {
		if ( n > 0 ) {
			if ( inpstr[n-1] != inpstr[n] ) {
				if ( o > 1 ) {
					/* Log10 of n rounded down will give the number of digits -1 */
					nadd = sizeof(char) * (floor(log10(o))+2);
					snprintf(buffer+buffsize, nadd+1, "%d%c", o, inpstr[n-1]);
				} else {
					nadd = sizeof(char);
					snprintf(buffer+buffsize, nadd+1, "%c", inpstr[n-1]);
				}

				buffsize += nadd;	
				o = 1;

			} else {
				o++;
			}
		}
	}

	return buffer;
}

int EncodeFile(FILE *fileptr) 
{
	char *lp = malloc(sizeof(char) * MAX_LEN);
	FILE *nfileptr = OpenFile("encoded.txt", "wb");
	char *buff;
	size_t bwrite = sizeof(unsigned) + sizeof(char);

	while(fgets(lp, MAX_LEN, fileptr) != NULL) {
		buff = EncodeString(lp);
		fprintf(stdout, "%s\n%s\n", lp, buff);
		fwrite(buff, bwrite, ceil(strlen(buff)/bwrite), nfileptr);
	}

	fclose(nfileptr);
	free(buff);
	free(lp);
	return 0;
}
