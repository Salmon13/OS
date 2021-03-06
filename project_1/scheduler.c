#define _GNU_SOURCE
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sched.h>
#include <signal.h>
#include "myfunctions.h"
#include "scheduler.h"

/* Last context switch time */
static int t_last;

/* Current time */
static int t_currunt;

/* Index of running process. (-1 : no process running) */
static int running;

/* Number of process done */
static int done_cnt;

int cmp(const void *a, const void *b) {
	return ((PROC *)a)->t_ready - ((PROC *)b)->t_ready;
}

/* Things for RR */
int Q[20], head = 0, tail = 0, Q_cnt = 0;

void Q_push(int index) {
	Q[tail] = index;
	tail = (tail + 1) % 20;
	Q_cnt++;
}

int Q_pop() {
	int ret = Q[head];
	head = (head + 1) % 20;
	Q_cnt--;
	return ret;
}

/* Return index of next process  */
int proc_next(PROC *P, int N, int policy) {

	/* Nonpreemptive */
	if (running != -1 && (policy == FIFO || policy == SJF))
		return running;

	int ret = -1;

	switch(policy) {
		case FIFO:
			for (int i = 0; i < N; i++) {
				if (P[i].pid == -1 || P[i].t_exec == 0)
					continue;
				if(ret == -1 || P[i].t_ready < P[ret].t_ready)
					ret = i;
			}
			break;

		case RR:
			if (running == -1) {
				if (Q_cnt != 0)
					ret = Q_pop();

			} else if ((t_currunt - t_last) % t_quantum == 0) {
				if (Q_cnt == 0)
					ret = running;
				else {
					ret = Q_pop();
					Q_push(running);
				}
			} else
				ret = running;

			break;

		case SJF:
		case PSJF:
			for (int i = 0; i < N; i++) {
				if (P[i].pid == -1 || P[i].t_exec == 0)
					continue;
				if (ret == -1 || P[i].t_exec < P[ret].t_exec)
					ret = i;
			}
			break;

	}

	return ret;
}

int proc_scheduling(PROC *P, int N, int policy) {

	/* Sort processes by ready time */
	qsort(P, N, sizeof(PROC), cmp);

	/* Initial pid to -1 : Not ready yet */
	for (int i = 0; i < N; i++)
		P[i].pid = -1;

	/* Set scheduler to another core */
	proc_assign_cpu(getpid(), SCHED_CPU);

	/* Set high priority to scheduler */
	proc_activity(getpid(), WAKEN);

	/* Initial scheduler */
	running = -1;
	t_currunt = 0;
	done_cnt = 0;

	while(1) {

		/* Check if running process finish */
		if (running != -1 && P[running].t_exec == 0) {

			waitpid(P[running].pid, NULL, 0);
			printf("%s %d\n", P[running].name, P[running].pid);
			fflush(stdout);
			running = -1;
			done_cnt++;

			/* All processes are done */
			if (done_cnt == N)
				break;
		}

		/* Check if process ready and fork */
		for (int i = 0; i < N; i++) {
			if (P[i].t_ready == t_currunt) {

				P[i].pid = proc_exec(P[i]);
				proc_activity(P[i].pid, BLOCK);
				if (policy == RR)
					Q_push(i);

			}

		}

		/* Select next running  process */
		int next = proc_next(P, N, policy);
		if (next != -1) {
			/* Context switch */
			if (running != next) {
				proc_activity(P[running].pid, BLOCK);
				kill(P[next].pid, SIGUSR1);
				proc_activity(P[next].pid, WAKEN);
				running = next;
				t_last = t_currunt;
			}
		}

		/* Run an unit time */
		TIMEUNIT();

		if (running != -1){
#ifdef DEBUG
			if(P[running].t_exec % 100 == 0)
				fprintf(stderr, "%s left %d unit time\n", P[running].name, P[running].t_exec);
#endif
			P[running].t_exec--;
		}

		t_currunt++;
	}

	return 0;
}