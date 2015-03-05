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
#include "bitcnts.h"
#include "gp.h"
#include "ShiftLogic.h"                /* Model's header file */
#include "rtwtypes.h"                  /* MathWorks ty*/
void write_led(int count) {

	IOWR_ALTERA_AVALON_PIO_DATA(0x10, count);
}


#define UNLIMIT
#define MAXARRAY 10000
/* this number, if too large, will cause a seg. fault!! */

struct myStringStruct {
  char qstring[15];
};

int compare(const void *elem1, const void *elem2)
{
  int result;

  result = strcmp((*((struct myStringStruct *)elem1)).qstring, (*((struct myStringStruct *)elem2)).qstring);

  return (result < 0) ? 1 : ((result == 0) ? 0 : -1);
}


void critical_task(int priority) {



	  //
	  /* should get 3 solutions: 2, 6 & 2.5   */

		enable_fprint_task(priority);
		basicmath_small();
	//	bitcnts();
	//	qsort_small();
		write_led(0xF);

		disable_fprint_task(priority);

}

//int i __attribute__ ((section (".global_data")));
void preempt_task(int priority){

	enable_fprint_task(priority);


	int i;
	for(i = 0; i < 1001; i++)
	 ShiftLogic_step();
	disable_fprint_task(priority);


}
