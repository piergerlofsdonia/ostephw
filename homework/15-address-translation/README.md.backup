#### Notes on chapter 5: "Address Translation"

For the most part, programs running on a system run inside a bubble created for that program by the operating system, an environment in which only its chunk of virtual memory is easily accessible. When a system call or hardware interrupt occurs, the operating system again steps in to correctly handle the call/interrupt. This strategy extends to memory virtualisation in the same way it did with CPU virtualisation. 

To allow a program to control its address space in whatever way it requires, virtual addresses are translated to physical addresses. This process is performed within the hardware but the hardware is setup and managed by the operating system - e.g. the hardware components that translate and route memory address inquiries do not keep track of which addresses are free for new allocations, the operating system is what manages this. 

An example is given where a program loads a value into a variable (stack memory) and then increments it by 3. This, when disassembled takes one additional step than when coded visually:

```
movl 0x0(%ebx), %eax
addl $0x03, %eax
movl %eax, 0x0(%ebx)
```

A move from the initial storage register of the value (ebx) into eax, then adding 3 to the value before moving it back to the original ebx register, pretty simple. If these instructions are numbered per their address in the address space of a program (this is given a dumbed-down assumption that the address space is flat and contiguous, and starts from zero) they might have the addresses _154, 158, 161_. If the value of _x_ is stored at virtual address 15KB (remember the stack is at the bottom of the _whole block_ of virtual addresses), the following address accesses take place over the lifetime of the program:

```
Instruction fetch: 154
Execute: Load 15KB (x)
Instruction fetch: 158
Execute: No memory required (addl)
Instruction fetch: 161
Execute: Store 15KB (x)
``` 

This is what the program and the programmer see (in this example), a flat address space running from 0 to 16KB storing the stack, heap, code and other segments. The operating system, however, has to place this address space within the bounds of physical memory, relocating the process to physical memory without disturbing another process or the operating system itself. It _could_ do this by sectioning the physical address space into chunks of even length, if the operating system was 16KB, then this process could be relocated to 32KB, leaving a gap between the operating system and the process - which, thankfully fit within these bounds. 

A process called _dynamic [because addresses are allocated at runtime] relocation_ also called _base and bounds_ uses a __base__ and a __bounds__ register to set the position of the process's address space within physical memory. When a program is compiled and run, the compiler believes that the address space begins at zero, however, the operating system sets the actual zero-point in the _base register_. By using a zero base point in the virtual address space and a base address, the physical address can be simply computed by adding the base address to the virtual address (e.g. using the previous example, if the base address is 32KB, _x_ is located at 47KB - the same is true for the stored instructions (32KB base, 32768) [32922, 32926, 32929]). The operating system uses the _bounds_ register, which is set to the end address of the virtual address space (16KB), to check whether the generated address is valid (inside the bounds). If the generated address is out-of-bounds an exception is raised, this also occurs for negative addresses. This check can either be done prior to the base being added (is the virtual address <=16KB?) or after, in which the _bounds_ register holds the physical address space boundary address (is virtual address + base address <= end physical address?).

