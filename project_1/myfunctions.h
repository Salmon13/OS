#ifndef _MY_FUNCTIONS_H_
#define _MY_FUNCTIONS_H_

#include <sys/types.h>

#define PROC_CPU	1
#define SCHED_CPU	0

#define WAKEN 1
#define BLOCK 0

#define TIMEUNIT() { volatile unsigned long i; for (i = 0; i < 1000000UL; i++); }

/* Structure of process */
typedef struct {
	char name[32];
	int t_ready;
	int t_exec;
	pid_t pid;
} PROC ;

/* Assign process to core */
int proc_assign_cpu(int pid, int core);

/* Fork and return pid */
int proc_exec(PROC proc);

/* Block or wake up the process */
int proc_activity(int pid, int act);

#endif