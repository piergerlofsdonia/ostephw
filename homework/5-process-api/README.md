### Chapter 5 Notes:
* Processes are created on UNIX systems using a pair of system calls: 'fork()' and 'exec()'.
* Processes can be awaited in the creator (master) process using 'wait()'.
* The CPU scheduler is non-determinstic, making it difficult to predict what it will do at any one time (particularly in m.t. programs). However, using wait() [or waitpid()] will force a parent to wait for a child process to finish executing.
* If a child process terminates without being waited for by the parent it becomes a 'zombie' process [1] - the parent can still read its return code due to the process still existing in the process table [2].
* Fork() is used when a process wants to be run within the calling program, exec() [and variants] are used when a process is required within a seperate, different program.
* The premise that fork() spawns processes within the calling program allows malicious programs such as a fork bomb to function - a fork bomb is a method of using up all the processing resource on a system by recursively calling fork() (often using a bash script) [3].
* Execvp() is a routine from unistd that changes the current process into a different program. The newly altered process then runs a given program/command (first argument) using a list of null-terminated string pointers [4] (arg1, arg2, arg3, ... argc-1, represented as char** argv in a normal program) as an argument list.
e.g. `execvp("ls", {"ls", "-a", NULL})`;
* The Unix shell runs a similar set of routines as the createprocess example: It locates a file in the filesystem, it then forks a new child process and runs some variant of `exec()` to run the command, it will wait for the process to complete with `wait()` and then returns to wait for another user prompt. The separation of `fork()` and `exec()` is a crucial design feature of the shell as it allows for redirection (e.g. `ls > dir.txt`) among other features.
* As a side-note, redirection works by closing the STDOUT_FILENO and using the `open()` system call to re-route output from stdout to a file instead. The example used in _OSTEP_ is: `open("./p4.output", O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);` which opens (or creates, if it doesn't exist) a file and clears it down - the file is write-only and has the permissions of read, write, and execute to the owner of the file [5].
* Unix pipes use the `pipe()` system call to 'connect' two programs together (output -> input) (e.g. `grep -o line t.txt | wc -l` to print out the number of occurances of the word 'line' in /process-api-examples/t.txt). Pipe() works by opening two files (one in read, the other in write) using their file descriptors [6].
* While a pipe() connection (data is transmitted through the kernel) is open, one 'end' of the pipe (the file descriptor we are _not_ concerned about) can be closed in order to facilitate writing from parent to child or vice versa (see [7] for examples. ).
* The `kill()` system call sends _signals_ to processes, users know the CTRL+C signal (SIGINT) which interrupts and terminates the process, CTRL+Z is another useful interrupt (SIGSTP) that pauses the process - `fg` will start the process back up, usually.
* The `signal()` system call is a useful tool for catching and enacting upon certain signals within a process, for example a clear down procedure can be enforced when a user terminates a process with SIGINT, see: `sigaction()` instead of `signal()` for this [8].
* It is obvious that being able to send signals to other processes (especially those that _you_ as a user do not own) is undesirable, this is why the user may only send signals (e.g. SIGINT) to processes they own - obviously if you have access to root you can ignore this. 
* _RTFM: Read. The. Fucking. MANUAL._
* Unix is the best thing in the world, even better than ice cream.

### Chapter 5 Homework Notes:
#### Q1:
* _x_ will be the same value in each process if it is not changed within the process. 
* _x_ will be equal to the value set within the process (e.g. if _x_ is set to 50 in the child process and 100 in the parent process it is only equal to this value within that process).
* As above, if _x_ is set within a process and is referenced in another (without having its value changed) it will equal the value initially set in the main block (e.g. `x = 0 in main(), x is set to 100 in parent process, x = 0 in child process`). 
* Unlike multi-threaded code, referencing the same variable within multiple process does not cause concurrency errors (e.g. race conditions). 

#### Q2:
* Both the parent and child may access an open file descriptor, it is _NOT_ required that the file is reopened with freopen or closed and opened before using it in a different process.
* If the file isopen using write mode (e.g. `fopen("file", "w")` ) rather than append (e.g. `fopen("file", "a")` ), the contents written by one process will be overwritten by the other.
* The file cannot be written to _concurrently_ as with multi-threaded code as one process runs before the other as discussed in the notes for the chapter.

#### Q3:
* Ensuring the child prints first IS possible without wait but is probably not advised. The method for doing this is outlined in the q3 program - a SIGSTOP signal is sent via `kill()` and a SIGCONT signal is sent once the child process has finished printing. This could also be done using the sigaction struct and/or signal function call.

#### Q4:
* Exec() is split into a wide family of variants, which are libc wrapped built upon the same `exec()` base. However, instead of having a single function that serves each purpose: e.g. ability to use a path or static filename; An input list or array, the variants each used specifically. The real reason the multiple variants is their development over a stretch of time - allowing programs to remain backwards compatible by retaining the functionality of (for example: execv) whilst adding new variants to fill a niche [9].

#### Q5:
* The `wait()` system call does nothing when executed in the child process. It will return the child process id when used within the parent process. 

#### Q6: 
* `waitpid()` and `wait()` do similar things, they are, infact, identical when used in the manner shown in the q6 example code (using WUNTRACED and the child pid). However, `waitpid()` provides additional functionality such as running without blocking [10].

#### Q7:
* If you close the standard output, `close(STDOUT_FILENO)`, no output can be written by the child process using `printf`. No error results from this, however.

#### Q8:
* Pipes between processes are pretty simple when executing programs within the process (e.g, see q8/childpipe.c), it's also pretty simple when piping between a parent and the child (e.g. see q8/parentpipe.c)[see: 11, 12]. Primarily this is done through casting the standard output to a pipe-end using `dup2(old_fd, new_fd);`, but buffering and writing the data across the pipe works just as well, depending on the required use of the pipe (e.g. `write(fd[1], buff, buffsize); -> read(fd[0], readbuff, buffsize);`).

Sources:
[1] Waitpid(2) manual: https://linux.die.net/man/2/waitpid

[2] 'Zombie Process' wiki page, example and explaination given below article: https://en.wikipedia.org/wiki/Zombie_process

[3] :(){ :|:& };: explained: https://askubuntu.com/questions/159491/why-did-the-command-make-my-system-lag-so-badly-i-had-to-reboot

[4] Execvp(3) manual: https://linux.die.net/man/3/execvp

[5] Open system call (wiki): http://codewiki.wikidot.com/c:system-calls:open

[6] Linux Documentation Project guide on pipes, see below.

[7] Linux Programmer's Guide (Chapter 6) by Burkett: https://tldp.org/LDP/lpg/node11.html

[8] Sigaction function example (GNU.org): https://www.gnu.org/software/libc/manual/html_node/Sigaction-Function-Example.html

[9] Knome on exec(): https://www.reddit.com/r/osdev/comments/ai71o0/ask_why_so_many_variants_of_exec_call/

[10] Wait vs. Waitpid: https://stackoverflow.com/questions/40014963/about-wait-and-waitpid

[11] C Tutorial: Pipes: https://www.cs.rutgers.edu/~pxk/416/notes/c-tutorials/pipe.html.

[12] Creating Pipes in C: https://tldp.org/LDP/lpg/node11.html
