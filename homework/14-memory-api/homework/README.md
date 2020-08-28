#### Homework for chapter 14:

1. Causes segmentation fault. NULL pointers point to memory that doesn't exist meaning that a derefence causes an access to an invalid address. 
2. See above, sigsev at the printf line (dereferencing the pointer to print it).
3. This is shown in greater detail using valgrind: "Access not within mapped region at address 0x0". Additionally it reports that the block wasn't freed, which makes sense.
4. Valgrind shows allocation and zero frees, it also shows the malloc line using `--leak-check=yes`.
5. The program works even with the invalid write, this is flagged in valgrind but does not cause any fatal errors (such as SIGSEV).
6. As above, you can use the values within the array still given nothing gets allocated in its place. This shows as an error in valgrind and shows another alloc and free (2 total rather than 1).
7. As long as the argument is a valid pointer no error will be raised, e.g. an array of integers is allocated on the heap and assigned to 'x', `int *p = &i[2]; free(p);` is valid). Passing a non-pointer to free will result in an _invalid pointer_ or _invalid free_ error. 
8. See `8-vector` directory. The performance of a vector structure versus a linked list are comparable but differ in operations such as start insert/deletions (O(1) for linked list, O(_n_) for dyn. array) and indexing (O(_n_) for linked list, O(1) for dyn. array). Other issues such as inserting in randomised positions (due to having to shift the array) exist for dyn. arrays - issues that are counteracted by the problems associated with linked lists (e.g. locality of memory is not as congruent as with a dynamic array (contiguous block of memory), see [here](https://stackoverflow.com/questions/35401508/dynamic-array-vs-linked-list-in-c). 
