### Example: Create processes using fork() and execvp().
__To run:__

0. (optional) Recompile 'count' and 'exectest' using compiler of choice (e.g. prompt> gcc -o program source.c -Wall -Werror).
1. Run exectest using the count program: prompt> ./exectest count t.txt t2.txt
2. Attempt to run exectest using another program or with different arguments to verify it works correctly (e.g. prompt> ./exectest ls -a).

### Example: Wait for a process to finish using wait().
__To run:__

1. Run forktest: prompt> ./forktest
2. Validate the output of the program (child process print-out comes prior to the parent process).

