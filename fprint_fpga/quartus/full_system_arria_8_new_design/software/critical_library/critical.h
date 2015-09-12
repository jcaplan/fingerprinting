/*
 * critical.h
 *
 *  Created on: 2014-03-02
 *      Author: Jonah
 */

#ifndef CRITICAL_H_
#define CRITICAL_H_
#include "fingerprint.h"

typedef struct PreemptTaskArgs{
	int priority;
}PreemptTaskArgs;
typedef struct QsortTestArgs{
	int priority;
	int* vectors;
	int size;
}QsortTestArgs;

void critical_task(int count);
void preempt_task(void* args);
void qsort_test(void* args);
void testing_task(void* args);
extern void context_switch(long *reg_stack);
extern void context_restore(long *reg_stack);

#endif /* CRITICAL_H_ */
