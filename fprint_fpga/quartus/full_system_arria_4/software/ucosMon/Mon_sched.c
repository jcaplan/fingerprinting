/****************************************************************************
 * Copyright (c) 2013 Jonah Caplan, McGill University.      				*
 * All rights reserved. All use of this software and documentation is     	*
 * subject to the License Agreement located at the end of this file below.	*
 ****************************************************************************
 * Description:                                                           	*
 * 																			*
 ***************************************************************************/

/**********************************
 * Includes
 **********************************/
#include <stdio.h>
#include "includes.h"
#include "shared_mem.h"
#include <altera_avalon_pio_regs.h>
#include <altera_avalon_mutex.h>
#include <ucos_ii.h>
#include "fingerprint.h"
#include "critical.h"
#include <string.h>
#include <time.h>
#include "priv/alt_busy_sleep.h"
#include "alt_types.h"
#include "stdio.h"
#include "sys/alt_dma.h"
#include "sys/alt_timestamp.h"
#include "qsort_large.h"
#include "basicmath.h"
/***********************************
 * Local Variables
 **********************************/
alt_mutex_dev* mutex; //hardware mutex pointer

void dma(void* tx_data, void* rx_buffer, int size, int core);

alt_dma_txchan txchan[2];
alt_dma_rxchan rxchan[2];

int *isr_0_ptr = (int *) PROCESSOR0_0_CPU_IRQ_0_BASE;
int *isr_1_ptr = (int *) PROCESSOR1_0_CPU_IRQ_0_BASE;
int *isr_2_ptr = (int *) PROCESSOR2_0_CPU_IRQ_0_BASE;
int *isr_3_ptr = (int *) PROCESSOR3_0_CPU_IRQ_0_BASE;

#define   TASK_STACKSIZE       768						//Stack size for all tasks
OS_STK schedule_task_stk[TASK_STACKSIZE] __attribute__ ((section (".critical")));
OS_STK print_status_stk[TASK_STACKSIZE] __attribute__ ((section (".critical")));
OS_STK print_execution_times_stk[TASK_STACKSIZE] __attribute__ ((section (".critical")));

OS_EVENT* done;
OS_EVENT* start_schedule;
/*
 * Definition of Task Priorities
 * Used only to set directory offsets
 */
#define TASK_START_PRIORITY			1
#define PREEMPT_TASK_PRIORITY_2		2
#define PREEMPT_TASK_PRIORITY_3		3
#define PREEMPT_TASK_PRIORITY		4
#define CRITICAL_TASK_PRIORITY		5

static volatile int rx_done = 0;
BasicmathSol answer[2];
/*
 * Monitor task priorities
 */

#define SCHEDULE_TASK_PRIORITY      		5
#define PRINT_STATUS_PRIORITY				4
#define PRINT_EXECUTION_TIMES_PRIORITY		3

int status;
/*
 * IO
 */

/*
 * Critical task. Only code prior to enable fingerprinting is
 * waiting for an activation message. This is necessary
 * to guarantee the stack matches on both cores.
 */
CriticalFunctionPointers cpg;
INT8U err;

void print_times(void){
	CriticalFunctionPointers* cp =
			(CriticalFunctionPointers*) SHARED_MEMORY_BASE;

	printf("Core 0 started  NCT at time: %d\n",cp->timelog[0][0]);
	printf("Core 0 finished NCT at time: %d\n",cp->timelog[0][1]);
	printf("Core 0 started  NCT at time: %d\n",cp->timelog[0][2]);
	printf("Core 0 finished NCT at time: %d\n",cp->timelog[0][3]);
	printf("Core 1 started  NCT at time: %d\n",cp->timelog[1][0]);
	printf("Core 1 finished NCT at time: %d\n",cp->timelog[1][1]);
	printf("Core 1 started  CT3 at time: %d\n",cp->timelog[1][2]);
	printf("Core 1 finished CT3 at time: %d\n",cp->timelog[1][3]);
	printf("Core 1 started  NCT at time: %d\n",cp->timelog[1][4]);
	printf("Core 1 finished NCT at time: %d\n",cp->timelog[1][5]);
	printf("Core 2 started  NCT at time: %d\n",cp->timelog[2][0]);
	printf("Core 2 finished NCT at time: %d\n",cp->timelog[2][1]);
	printf("Core 2 started  CT3 at time: %d\n",cp->timelog[2][2]);
	printf("Core 2 finished CT3 at time: %d\n",cp->timelog[2][3]);
	printf("Core 2 started  CT4 at time: %d\n",cp->timelog[2][4]);
	printf("Core 2 finished CT4 at time: %d\n",cp->timelog[2][5]);
	printf("Core 3 started  NCT at time: %d\n",cp->timelog[3][0]);
	printf("Core 3 finished NCT at time: %d\n",cp->timelog[3][1]);
	printf("Core 3 started  CT4 at time: %d\n",cp->timelog[3][2]);
	printf("Core 3 finished CT4 at time: %d\n",cp->timelog[3][3]);


}

void basicmathtime(int count) {
	if (alt_timestamp_start() < 0) {
		printf("No timestamp device available\n");
	}
	BasicmathArgs b = { 5, { count * 7, 2, 7, 0 } };
	basicmath_test(&b);
}
BasicmathArgs args[2] = { { 3, { 21, 2, 7, 0 } }, { 4, { 21, 2, 7, 0 } } };
void schedule_task(void* pdata) {
	//int i;

	printf("Monitor!\n");
	CriticalFunctionPointers* cp =
			(CriticalFunctionPointers*) SHARED_MEMORY_BASE;

	while (1) {
		int x = OSTimeGet();
		//First critical task
		basicmathtime(4);
		x = OSTimeGet();

		//set cores 1 and 2 to execute task 3
		int i;
		for(i = 1; i < 3; i++){
			cp->task[i] = basicmath_test;
			cp->priority[i] = 3;
			cp->blocksize[i] = 0xfff;
			cp->args[i] = &args[0];
		}

		//wait until next point in time
		OSTimeDly(40 - x);



		//Next critical task
		x = OSTimeGet();
		basicmathtime(5);

		//set cores 2 and 3 to execute task 4


		for(i = 2; i < 4; i++){
			cp->task[i] = basicmath_test;
			cp->priority[i] = 4;
			cp->blocksize[i] = 0xfff;
			cp->args[i] = &args[1];
		}
		//Wait
		x = OSTimeGet();
		OSTimeDly(100 - x);

		//Final critical task
		x = OSTimeGet();
		basicmathtime(4);
		x = OSTimeGet();

		OSTimeDly(150 - x);
		print_times();
		OSTimeDlyHMSM(0, 4, 0, 0);
	}
}
int endstate[10];


static void handle_collision_interrupt(void* context) {
	Fprint_Status status;
	fprint_status(&status);
	int i;
	for (i = 0; i < 2; i++) {
		int index = 3 + i;
		if ((status.successful_reg & (1 << index)) == (1 << index))
			endstate[index] = 1;

		if ((status.failed_reg & (1 << index)) == (1 << index))
			endstate[index] = -1;

		fprint_reset_irq();
		OSSemPost(done);
	}
}

static void init_collision_isr(void) {
	alt_ic_isr_register(CFPU_0_CSR_IRQ_INTERRUPT_CONTROLLER_ID, CFPU_0_CSR_IRQ,
			handle_collision_interrupt, (void*) NULL, (void*) NULL);
}

static void done_dma(void* handle, void* data) {
	rx_done++;
}

void dma(void* tx_data, void* rx_buffer, int size, int core) {

	int rc;

	if ((rc = alt_dma_txchan_send(txchan[core], tx_data, size, NULL, NULL))
			< 0) {
		printf("Failed to post transmit request, reason = %i\n", rc);

	}
	/* Post the receive request */
	if ((rc = alt_dma_rxchan_prepare(rxchan[core], rx_buffer, size, done_dma,
			NULL)) < 0) {
		printf("Failed to post read request, reason = %i\n", rc);
	}

}

void dma_init(void) {

	int i;
	char *dev = "/dev/processorX_0_dma_0";

	for (i = 0; i < 4; i++) {
		sprintf(dev, "/dev/processor%d_0_dma_0", i);
		if ((txchan[i] = alt_dma_txchan_open(dev)) == NULL) {
			printf("Failed to open transmit channel\n");

		}
		/* Create the receive channel */
		if ((rxchan[i] = alt_dma_rxchan_open(dev)) == NULL) {
			printf("Failed to open receive channel\n");

		}
	}

}

void print_status(void* pdata) {
	while(1){
		OSSemPend(done,0,&err);
		int time = OSTimeGet();
		if(endstate[3] == -1){
			printf("ct 3 failed at time %d\n",time);
			endstate[3] = 0;
		}
		else if(endstate[3] == 1){
			printf("ct 3 succeeded at time %d\n",time);
			endstate[3] = 0;
		}
		if(endstate[4] == -1){
			printf("ct 4 failed at time %d\n",time);
			endstate[4] = 0;
		}
		else if(endstate[4] == 1){
			printf("ct 4 succeeded at time %d\n",time);
			endstate[4] = 0;
		}
	}
}

/*
 * Main
 */

int main(void) {

	printf("Hello from Nios II!\n");
	dma_init();

	//basicmathtime();

	int i, j;
	init_collision_isr();
	CriticalFunctionPointers* cp =
			(CriticalFunctionPointers*) SHARED_MEMORY_BASE;

	mutex = altera_avalon_mutex_open(MUTEX_0_NAME); //Initialize the hardware mutex
	altera_avalon_mutex_lock(mutex, 1);
	{
		OSTimeSet(0);
		cp->init_complete = 1;
	}
	altera_avalon_mutex_unlock(mutex);

	done = OSSemCreate(0);
	start_schedule = OSSemCreate(1);
	//Initialize the message box
	//Initialize the directory for the fingerprinting unit:
	Directory_Init_Struct d;

	for (i = 0; i < 2; i++) {
		d.core_id = i;
		d.key = 3;
		d.start_ptr = 1;
		d.end_ptr = 255;
		set_task_directory(&d);

		d.core_id = i;
		d.key = 4;
		d.start_ptr = 256;
		d.end_ptr = 511;
		set_task_directory(&d);

	}
	Core_Assignment_Table ca;

	/* TABLE VALUES:
	 * x x
	 * x x
	 * 0 1
	 * 2 3
	 * 4 5
	 * 6 7
	 * ...
	 */
	//Default table
	for (i = 0; i < CA_TABLE_MAX_REDUNDANCY; i++) {
		for (j = 0; j < CA_TABLE_NUM_TASKS; j++) {
			ca.table[i][j] = i;
		}
	}

	ca.table[0][3] = 1;
	ca.table[1][3] = 2;
	ca.table[0][4] = 3;
	ca.table[1][4] = 2;
	set_core_assignment_table(&ca);

	//Wait for both cores to be ready
	int p0 = 0, p1 = 0, p2 = 0, p3 = 0;
	while ((p0 != 1) || (p1 != 1) || (p2 != 1) || (p3 != 1)) {
		altera_avalon_mutex_lock(mutex, 1);
		{
			memcpy(&cpg, (void*) SHARED_MEMORY_BASE,
					sizeof(CriticalFunctionPointers));
			p0 = cpg.core_ready[0];
			p1 = cpg.core_ready[1];
			p2 = cpg.core_ready[2];
			p3 = cpg.core_ready[3];
		}
		altera_avalon_mutex_unlock(mutex);

	}
	cpg.core_ready[0] = 0;
	cpg.core_ready[1] = 0;
	cpg.core_ready[2] = 0;
	cpg.core_ready[3] = 0;

	int arg_5 = CRITICAL_TASK_PRIORITY;
	OSTaskCreateExt(schedule_task, &arg_5,
			&schedule_task_stk[TASK_STACKSIZE - 1], SCHEDULE_TASK_PRIORITY,
			SCHEDULE_TASK_PRIORITY, schedule_task_stk, TASK_STACKSIZE, NULL,
			OS_TASK_OPT_STK_CHK + OS_TASK_OPT_STK_CLR);

	OSTaskCreateExt(print_status, &arg_5,
			&print_status_stk[TASK_STACKSIZE - 1], PRINT_STATUS_PRIORITY,
			PRINT_STATUS_PRIORITY, print_status_stk, TASK_STACKSIZE, NULL,
			OS_TASK_OPT_STK_CHK + OS_TASK_OPT_STK_CLR);

	//Start operating system

	OSStart();

	return 0;
}
