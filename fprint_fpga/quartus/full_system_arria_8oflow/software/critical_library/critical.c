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

void write_led(int count) {

	IOWR_ALTERA_AVALON_PIO_DATA(0x10, count);
}



void critical_task(int priority) {



	  //
	  /* should get 3 solutions: 2, 6 & 2.5   */

		enable_fprint_task(priority);
		basicmath_small();
	//	bitcnts();
	//	qsort_small();
//		write_led(0xF);

		disable_fprint_task(priority);

}

void preempt_task(void* args){
	int priority = *(int*)args;
	enable_fprint_task(priority);

	basicmath_small();
	disable_fprint_task(priority);

}
void qsort_test(void* args){
	QsortTestArgs* a = (QsortTestArgs*)args;
	enable_fprint_task(a->priority);
	qsort_large(a->vectors,a->size);
	disable_fprint_task(a->priority);

}

void testing_task(void* args) {

	int* a = (int*)args;

	int id = a[0];
	args++;
	int length = a[1];
	args++;
	int enable = a[2];
	int i;

	if(enable)
		enable_fprint_task(id);

	for(i=0 ; i<length ; i++)
		basicmath_small();

	if(enable)
		disable_fprint_task(id);

}



