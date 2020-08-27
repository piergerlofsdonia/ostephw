#### Homework for chapter 14:

1. Causes segmentation fault. NULL pointers point to memory that doesn't exist meaning that a derefence causes an access to an invalid address. 
2. See above, sigsev at the printf line (dereferencing the pointer to print it).
3. This is shown in greater detail using valgrind: "Access not within mapped region at address 0x0". Additionally it reports that the block wasn't freed, which makes sense.
4. Valgrind shows allocation and zero frees, it also shows the malloc line using `--leak-check=yes`.
5. The program works even with the invalid write, this is flagged in valgrind but does not cause any fatal errors (such as SIGSEV).
6. As above, you can use the values within the array still given nothing gets allocated in its place. This shows as an error in valgrind and shows another alloc and free (2 total rather than 1).
7.  
