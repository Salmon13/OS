#define _GNU_SOURCE
#include "myfunctions.h"
#include <sched.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <signal.h>
#define MY_TIME  333
#define MY_PRINT 334

int proc_assign_cpu(int pid, int core) {

	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(core, &mask);

	sched_setaffinity(pid, sizeof(mask), &mask);

	return 0;
}

static bool isBLOCK = true;
static void wakeup() {
	isBLOCK = false;
}

int proc_exec(PROC p) {

	signal(SIGUSR1, wakeup);

	int pid = fork();
	if (pid == 0) {

		/* Assign child to CHILD core */
		proc_assign_cpu(getpid(), PROC_CPU);

		/* Block on ready */
		while(isBLOCK);

		/* Process start */
		unsigned long st_sec, st_nsec, ed_sec, ed_nsec;
		
		syscall(MY_TIME, &st_sec, &st_nsec);

		for (int i = 0; i < p.t_exec; i++) {
#ifdef DEBUG
		if(i % 100 == 0)
			fprintf(stderr, "%s runs %d unit time on cpu %d\n", p.name, i, sched_getcpu());
#endif
			TIMEUNIT();
		}

		syscall(MY_TIME, &ed_sec, &ed_nsec);
		/* Process end */

		syscall(MY_PRINT, st_sec, st_nsec, ed_sec, ed_nsec, getpid());

		exit(0);

	}

	return pid;
}

int proc_activity(int pid, int act) {

	struct sched_param param;
	param.sched_priority = 0;

	/* WAKEN: set high priority */
	if (act == WAKEN)
		sched_setscheduler(pid, SCHED_OTHER, &param);

	/* BLOCK: set low priority */
	else
		sched_setscheduler(pid, SCHED_IDLE, &param);

	return 0;
}