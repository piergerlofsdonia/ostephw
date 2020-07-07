#ifndef __WCAT_H
#define __WCAT_H

#include <stdio.h>
#define MAX_LEN 255; /* See here: https://serverfault.com/questions/9546/filename-length-limits-on-linux */

FILE *OpenFile(char*, const char*);
int CountLines(FILE*, int); 

#endif
