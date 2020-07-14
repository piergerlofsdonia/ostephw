### Chapter 5 Notes:
* Processes are created on UNIX systems using a pair of system calls: 'fork()' and 'exec()'.
* Processes can be awaited in the creator (master) process using 'wait()'.
* The CPU scheduler is non-determinstic, making it difficult to predict what it will do at any one time (particularly in m.t. programs). However, using wait() [or waitpid()] will force a parent to wait for a child process to finish executing.
* If a child process terminates without being waited for by the parent it becomes a 'zombie' process [1] - the parent can still read its return code due to the process still existing in the process table [2].
* Fork() is used when a process wants to be run within the calling program, exec() [and variants] are used when a process is required within a seperate, different program.
* The premise that fork() spawns processes within the calling program allows malicious programs such as a fork bomb to function - a fork bomb is a method of using up all the processing resource on a system by recursively calling fork() (often using a bash script) [3].
* Execvp() is a routine from unistd that changes the current process into a different program. The newly altered process then runs a given program/command (first argument) using a list of null-terminated string pointers [4] (arg1, arg2, arg3, ... argc-1, represented as char** argv in a normal program) as an argument list.
e.g. `execvp("ls", {"ls", "-a", NULL})`;
* Unix is the best thing in the world, even better than ice cream.

Sources:
[1] Waitpid(2) manual: https://linux.die.net/man/2/waitpid
[2] 'Zombie Process' wiki page, example and explaination given below article: https://en.wikipedia.org/wiki/Zombie_process
[3] :(){ :|:& };: explained: https://askubuntu.com/questions/159491/why-did-the-command-make-my-system-lag-so-badly-i-had-to-reboot
[4] Execvp(3) manual: https://linux.die.net/man/3/execvp
