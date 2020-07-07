#ifndef __WGREP_H
#define __WGREP_H

#include <stdio.h>
#define MAX_LEN 4095;
#define MAX_STDIN 255;

FILE *OpenFile(char*, const char*);
int FindAndPrint(FILE*, char*);
int FAPstdin(char*);

#endif
