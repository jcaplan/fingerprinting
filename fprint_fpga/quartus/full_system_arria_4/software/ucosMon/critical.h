/*
 * critical.h
 *
 *  Created on: 2014-03-02
 *      Author: Jonah
 */

#ifndef CRITICAL_H_
#define CRITICAL_H_
#include "fingerprint.h"

typedef struct BasicmathArgs{
	int priority;
	int loopvalues[4];
}BasicmathArgs;
typedef struct QsortTestArgs{
	int priority;
	int* vectors;
	int size;
}QsortTestArgs;

typedef struct BitcountTestArgs{
	int priority;
	int seed;
	int iterations;
}BitcountTestArgs;

typedef struct SusanTestArgs{
	int priority;
	unsigned char *input;
	int x_size;
	int y_size;
}SusanTestArgs;

void critical_task(int count);
void basicmath_test(void* args);
void qsort_test(void* args);
void bitcount_test(void* args);
void susan_test(void* args);
extern void context_switch(long *reg_stack);
extern void context_restore(long *reg_stack);

#endif /* CRITICAL_H_ */
