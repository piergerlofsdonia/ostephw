#ifndef RDTSC_H
#define RDTSC_H
	
#if defined(__i386__)

static __inline__ unsigned long long rdtsc(void)
{
	unsigned long long int x;
	/* Generates a 'read time stamp counter' (RDTSC) instruction for the x86 instruction set [0x0F 0x31],
	   this reads the time-stamp counter into edx:eax, then copied over to var 'x' using the o/p contraint "=A"(x) 
		see: https://stackoverflow.com/questions/1273367/what-is-this-x86-inline-assembly-doing
		and: https://c9x.me/x86/html/file_module_x86_id_278.html */
	__asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
	return x;
}

#elif defined(__x86_64__)

static __inline__ unsigned long long rdtscp(void)
{
	unsigned hi, lo;
	/* Run rdtscp instruction with (lo) and (hi) as outputs, no inputs, and clobber rcx register to allow cpu id to be put into ecx. */
	__asm__ __volatile__ ("rdtscp" : "=a"(lo), "=d"(hi) : : "%rcx");
	return ( (unsigned long long) lo) | ( ((unsigned long long)hi)<<32 );
}

/* There ARE better ways to achieve the above using __builtin_ia32_rdtsc and __rdtsc() but this works for the purpose of this benchmark.
	See: https://gcc.gnu.org/wiki/DontUseInlineAsm and: https://stackoverflow.com/questions/13772567/how-to-get-the-cpu-cycle-count-in-x86-64-from-c/51907627#51907627
	for reasons why NOT to use the above methods. */

/* Final method used for rdtscp functions (rdtsc with serialization to stop cpu from reordering calls) taken from https://unix4lyfe.org/benchmarking/,
   Alternatively could use "__asm__ __volatile__ ("mfence":::"memory");" to seralise the original rdtsc function if using i386.
   also see: https://stackoverflow.com/questions/12631856/difference-between-rdtscp-rdtsc-memory-and-cpuid-rdtsc */

#endif

#endif
