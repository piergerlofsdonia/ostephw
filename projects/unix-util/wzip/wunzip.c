/* 
	The wunzip tool simply does the reverse of the wzip tool, taking in a compressed file and writing (to standard output again) the uncompressed results. 
	For example, to see the contents of file.txt, you would type:

	<prompt> ./wunzip file.z

	wunzip should read in the compressed file (likely using fread()) and print out the uncompressed output to standard output using printf(). 
 */

#include<stdio.h>
#include<stdlib.h>
#include "wzip.h"
