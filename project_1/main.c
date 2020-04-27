#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sched.h>
#include <errno.h>
#include <unistd.h>
#include "myfunctions.h"
#include "scheduler.h"

int main() {

	char S[16];
	int policy, N;
	scanf("%s%d", S, &N);
	
	PROC *P;
	P = (PROC *)malloc(N * sizeof(PROC));

	for (int i = 0; i < N; i++)
		scanf("%s%d%d", P[i].name, &P[i].t_ready, &P[i].t_exec);

	switch(S[0]) {
		case 'F':
			policy = FIFO;
			break;
		case 'R':
			policy = RR;
			break;
		case 'S':
			policy = SJF;
			break;
		case 'P':
			policy = PSJF;
			break;
		default:
			fprintf(stderr, "Invalid Policy");
			break;
	}

	proc_scheduling(P, N, policy);

	exit(0);
}