#include <sched.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

static const int iterations = 1000000;

static __inline__ unsigned long long rdtsc(void)
{
	unsigned hi, lo;
	__asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
	return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}

void* thread() {
	for (int i = 0; i < iterations; i++) {
		sched_yield();
	}
	return NULL;
}

int main(void) {
	const pid_t childPid = fork();
	unsigned long long start = 0, stop = 0;

	if(childPid == 0) {
		start = rdtsc();
		thread();
		stop = rdtsc();
		printf("Process ctx %f\n", (stop - start)/ (float)iterations);
	}
	else {
		int returnStatus;
		waitpid(childPid, &returnStatus, 0);
	}
	return 0;
}
