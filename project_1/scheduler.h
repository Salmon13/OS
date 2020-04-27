#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include "myfunctions.h"

#define FIFO	1
#define RR		2
#define SJF		3
#define PSJF	4

#define t_quantum 500

int proc_scheduling(PROC *P, int N, int policy);

#endif