#ifndef __WZIP_H
#define __WZIP_H
#include <stdio.h>
#define MAXLEN 4095
#define BYTESIZE 5

FILE *OpenFile(char*, const char*);
int EncodeString(char*);
int EncodeFile(FILE*);
int DecodeString(char*);
int DecodeFile(FILE*);

#endif
