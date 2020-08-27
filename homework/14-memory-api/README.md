#### Notes on chapter 14: "Interlude: Memory API"

Memory within the bounds of a program is stored either in the _heap_ or in the _stack_ (this is excluding saved variables initialised and stored elsewhere, e.g. those variables given to a function during a call, or static variables stored in the bss segment). Stack operations involve initialising a variable and assigning it a value - the compiler does the rest (including allocating space for the variable and deallocating the space when the function in question returns), e.g. 

```
int main()
{
	int x = 1; // Store a value of 1 in variable 'x' on the stack.
	return x; // Will be deallocated by the compiler after function return.
}
```

Memory allocated onto the heap is done so using a set of standard operations: _malloc_ is the standard function used but several others exist that fill a niche role (e.g. _calloc_ for allocating memory for an array whilst also zeroing each element). _Realloc_ is used to edit existing heap memory allocations. All of these functions return a void pointer that is typically automatically adapted for the target assignment  (e.g. `char *strptr = malloc(sizeof(char) * 10);` will be stored correctly as an array of characters and a character pointer will be assigned to 'strptr'), however, sometimes casting is used to ensure this is the case (e.g. `int *p = (int *) malloc(sizeof(int));`) this is just personal preference.

```
int main()
{
	int *p = (int *) malloc(sizeof(int)); // Allocates heap memory of 4 bytes (size of integer type)... 
										  // assigns pointer to this memory to *p cast as an integer pointer.
	*p = 1;
	return *p;
}
```

The above snippet shows how heap allocation is handled, however, interestingly, it is technically a stack and a heap allocation in one line: the heap allocation occurs as a result of the malloc() call, which allocates a 4 byte block of memory. The resulting pointer to this allocated block is then returned, first as a void pointer, then cast to an integer pointer and assigned to the variable ('p', initialised as '*p' and used in pointer form as 'p'). 

The `ecmalloc()` variation created and used in previous week's homework simply checks the output of `malloc()`, which returns a NULL pointer if unsuccessful. It's quite rare for this to occur given a valid input, but needs to be asserted given the consequences of using a malformed pointer or misallocated heap memory! Allocated heap memory also needs to be freed using `free(p)` providing the pointer to the allocated memory. For short programs, freeing is usually handled by the operating system but is still considered good practice. Frees are crucial for persistent, long-running systems such as server-side programs as it allows allocated blocks of memory to be reused later in a programs life-cycle. Note: Freed memory can still be used (assuming the pointer still exists) until it has been written over, it is not recommended, however, as there is no useful method for knowing when this memory has been overwritten.

SIGSEV (segmentation faults) occur when a misallocation or lack of allocation occurs among other potential issues. If a program returns this error, usually the block allocated is not big enough (e.g. 8 bytes for a 2 item int array), is not allocated at all, or hasn't been initialised.

The _malloc_ variants are _library calls_, not _system calls_, this allows them to abstract the lower-level _system call_ routines into an easier-to-use form. The _system calls_ used to allocate heap memory include _brk_ which alters the _break_, the heap end address, which moves the address boundaries of the heap, increasing or decreasing it. Additional library functions can be used to allocate heap memory in a roundabout way, _mmap()_ allocates memory outside of the program in _swap space_ and will be treated like regular heap memory.