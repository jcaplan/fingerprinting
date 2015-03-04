/*
 * critical.c
 *
 *  Created on: 2014-03-02
 *      Author: Jonah
 */
#include "critical.h"
#include "basicmath.h"
#include "qsort_small.h"
#include <altera_avalon_pio_regs.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "qsort_large.h"
#include "bitcnts.h"
#include "susan.h"
void write_led(int count) {

	IOWR_ALTERA_AVALON_PIO_DATA(0x10, count);
}



void critical_task(int priority) {

	  //
	  /* should get 3 solutions: 2, 6 & 2.5   */

		enable_fprint_task(priority);
		basicmath_small(NULL,NULL);
	//	bitcnts();
	//	qsort_small();
		write_led(0xF);

		disable_fprint_task(priority);

}

void basicmath_test(void* args){
	int priority = *(int*)args;
	int* loopvalues = ((BasicmathArgs*)args)->loopvalues;
	enable_fprint_task(priority);
	BasicmathSol *solution = (BasicmathSol*)0x8203000;
	basicmath_small(loopvalues,solution);
	disable_fprint_task(priority);

}
void qsort_test(void* args){
	QsortTestArgs* a = (QsortTestArgs*)args;
	enable_fprint_task(a->priority);
	qsort_large(a->vectors,a->size);
	disable_fprint_task(a->priority);

}

void bitcount_test(void* args){
	BitcountTestArgs* a = (BitcountTestArgs*)args;
	enable_fprint_task(a->priority);
	int *ans = (int*)0x8203000;
	*ans = do_bitcount(a->seed,a->iterations);
	disable_fprint_task(a->priority);

}

void susan_test(void* args){
	SusanTestArgs* a = (SusanTestArgs*)args;
	enable_fprint_task(a->priority);
	susan(a->input,a->x_size,a->y_size);
	disable_fprint_task(a->priority);

}
