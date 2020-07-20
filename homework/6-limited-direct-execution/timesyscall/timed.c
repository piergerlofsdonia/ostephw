#define _POSIX_C_SOURCE 199309L
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include "rdtsc.h"
#define	CYCLES 250000

float MeasureCycToMs()
{
	double cps = 0;
	double cpstotal = 0;
	unsigned long long prevtime = 0;
	unsigned long long currtime = 0;
	long c = 0;

	while(c < CYCLES) 
	{
		prevtime = rdtscp();
		read(1, NULL, 0);
		currtime = rdtscp();
		cps = (double) (currtime - prevtime) / CLOCKS_PER_SEC;
		cpstotal += cps;
		fprintf(stdout, "Cycle: %li\nCurrent: %fms\nTotal: %fms\n\n", c, cps*1000, cpstotal*1000);
		c++;
	}	

	return cpstotal / CYCLES;
}

long MeasureNs()
{
	struct timespec currtime, prevtime;
	int rtncode = 0;
	long c = 0;
	long timediff = 0;
	unsigned long long timetotal = 0;
	clock_t clockid = CLOCK_REALTIME;

	while ( c < CYCLES )
	{
		switch(rtncode)
		{
			case -1:
				perror("Error whilst getting systime: ");
				exit(1);
			default:
				rtncode = clock_gettime(clockid, &prevtime);
				read(1, NULL, 0);
				rtncode = clock_gettime(clockid, &currtime);
				timediff = currtime.tv_nsec - prevtime.tv_nsec;
				timetotal += timediff;
				fprintf(stdout, "Cycle: %li\nCurrent: %lins\nTotal: %llins\n\n", c, timediff, timetotal);
				c++;
				break;
		}
	}
	
	return timetotal / CYCLES;
}

int main()
{
	float result = MeasureCycToMs();
	long resultns = MeasureNs();

	fprintf(stdout, "Measured using rdtscp converted...\nCycles:%d: Time taken (avg.): %fms\n", CYCLES, result*1000);
	fprintf(stdout, "Measured using clock_gettime...\nCycles: %d: Time taken (avg.): %fus\n", CYCLES, (float)resultns/1000);
	return 0;
}
