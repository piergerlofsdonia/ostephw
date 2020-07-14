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


Sources:
[1] Waitpid(2) manual: https://linux.die.net/man/2/waitpid

[2] 'Zombie Process' wiki page, example and explaination given below article: https://en.wikipedia.org/wiki/Zombie_process

[3] :(){ :|:& };: explained: https://askubuntu.com/questions/159491/why-did-the-command-make-my-system-lag-so-badly-i-had-to-reboot

[4] Execvp(3) manual: https://linux.die.net/man/3/execvp

[5] Open system call (wiki): http://codewiki.wikidot.com/c:system-calls:open

[6] Linux Documentation Project guide on pipes, see below.

[7] Linux Programmer's Guide (Chapter 6) by Burkett: https://tldp.org/LDP/lpg/node11.html

[8] Sigaction function example (GNU.org): https://www.gnu.org/software/libc/manual/html_node/Sigaction-Function-Example.html