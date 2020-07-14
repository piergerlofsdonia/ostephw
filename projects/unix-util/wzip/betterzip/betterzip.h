#ifndef __WZIP_H
#define __WZIP_H
#include <stdio.h>
#define MAXLEN 4095
#define BYTESIZE 5

int ParseCmd(int, char**);
FILE *OpenFile(char*, const char*);
int EncodeFile(FILE*, FILE*);
void EncodeString(FILE*, char*, char, unsigned);

#endif
