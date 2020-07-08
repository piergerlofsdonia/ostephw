#ifndef __WZIP_H
#define __WZIP_H
#include <stdio.h>

FILE *OpenFile(char*, const char*);
char *EncodeString(char*);
int EncodeFile(FILE*);

#endif
