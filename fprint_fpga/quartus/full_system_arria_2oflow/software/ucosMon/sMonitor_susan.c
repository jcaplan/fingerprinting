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
#include "susan.h"
/***********************************
 * Local Variables
 **********************************/
alt_mutex_dev* mutex; //hardware mutex pointer

void dma(void* tx_data, void* rx_buffer, int size, int core);

alt_dma_txchan txchan[2];
alt_dma_rxchan rxchan[2];

int *isr_0_ptr = (int *) PROCESSOR0_0_CPU_IRQ_0_BASE;
int *isr_1_ptr = (int *) PROCESSOR1_0_CPU_IRQ_0_BASE;
int *isr_m_ptr = (int *) PROCESSORM_0_CPU_IRQ_0_BASE;
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
char answer[2][2000];
/*
 * Monitor task priorities
 */

#define SCHEDULE_TASK_PRIORITY      		5
#define PRINT_STATUS_PRIORITY				4
#define PRINT_EXECUTION_TIMES_PRIORITY		3

int x_size, y_size;
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

void schedule_task(void* pdata) {
	//int i;

	printf("Monitor!\n");
	CriticalFunctionPointers* cp =
			(CriticalFunctionPointers*) SHARED_MEMORY_BASE;
	unsigned char *in;
	get_image("/mnt/rozipfs/input_small1.pgm", &in, &x_size, &y_size);
	SusanTestArgs args[2] = {{5,(unsigned char*)0x8203000,x_size,y_size},
			{5,(unsigned char*)0x8203000,x_size,y_size}
	};




	while (1) {
		OSSemPend(start_schedule, 0, &err);
		if (alt_timestamp_start() < 0) {
			printf("No timestamp device available\n");
		}

		int size = x_size * y_size;

		void* tx_data = (void*) in; /* pointer to data to send */
		void* rx_buffer = (void*) 0x8203000; /* pointer to rx buffer */
		dma(tx_data,rx_buffer,size,0);
		dma(tx_data,rx_buffer,size,1);
		while(rx_done < 2)
			;
		rx_done = 0;

		int i;
		altera_avalon_mutex_lock(mutex, 1);
		{
			for (i = 0; i < 2; i++) {
				cp->task[i] = susan_test;
				cp->priority[i] = (5);
				cp->blocksize[i] = 0xfff;
				cp->args[i] = &args[i];
			}
			*isr_1_ptr = 1;
			*isr_0_ptr = 1;

		}
		altera_avalon_mutex_unlock(mutex);
		OSTimeDlyHMSM(0, 1, 0, 0);
	}
}
int endstate[10];
int compare_results(void) {

	int i;
	int size = x_size * y_size;
	for(i = 0; i < size; i++){
		if(answer[0][i] != answer[1][i])
			return -1;
	}
	return 0;

}
void print_status_task(void* pdata) {
	while (1) {
		OSSemPend(done, 0, &err);

		//only need to transfer from one core for fingerprinting
		void* tx_data = (void*) 0x8203000;
		void* rx_buffer = (void*) &answer;
		int size = x_size * y_size;
		dma(tx_data, rx_buffer, size, 0);
		rx_buffer = (void*) &answer[1];
		dma(tx_data, rx_buffer, size, 1);
		while (rx_done < 2)
			;
		rx_done = 0;
		//Wait till DMA is done before ending timer

		int match = compare_results();
		alt_u64 endtime = alt_timestamp();
		OSTimeDlyHMSM(0, 0, 1, 0);

		int success = endstate[5];

		printf("task 5 was ");
		if (success == 1) {
			printf("successful \n");
		} else if (success == -1) {
			printf("unsuccessful \n");
		} else {
			printf("not executed \n");
		}

		int i;
		for (i = 0; i < 10; i++) {
			endstate[i] = 0;
		}
		CriticalFunctionPointers* cp =
				(CriticalFunctionPointers*) SHARED_MEMORY_BASE;
		for (i = 0; i < 2; i++) {

			printf("core %d finished at time %llu\n", i, cp->core_time[i]);
		}

		printf("monitor finished at time %llu\n", endtime);
		if (match == 0)
			printf("executions matched");
		else
			printf("executions did not match");
	}
}

void print_execution_times(void* pdata) {
	CriticalFunctionPointers* cp =
			(CriticalFunctionPointers*) SHARED_MEMORY_BASE;
	while (1) {
		OSTimeDlyHMSM(0, 0, 5, 0);
		int i;
		for (i = 0; i < 8; i++) {

			printf("core %d finished at time %llu\n", i, cp->core_time[i]);
		}
	}
}

static void handle_cpum_interrupt(void* context) {
	*isr_m_ptr = 0;
	CriticalFunctionPointers* cp =
					(CriticalFunctionPointers*) SHARED_MEMORY_BASE;
	if((cp->core_ready[0] == 1) && (cp->core_ready[1] == 1)){
		cp->core_ready[0] = 0;
		cp->core_ready[1] = 0;
		OSSemPost(done);
	}
}



static void init_cpum_isr(void) {
	alt_ic_isr_register(PROCESSORM_0_CPU_IRQ_0_IRQ_INTERRUPT_CONTROLLER_ID, PROCESSORM_0_CPU_IRQ_0_IRQ,
			handle_cpum_interrupt, (void*) NULL, (void*) NULL);
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

	for (i = 0; i < 2; i++) {
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
/*
 * Main
 */

int main(void) {

	printf("Hello from Nios II!\n");
	dma_init();

	int i, j;
	init_cpum_isr();
	CriticalFunctionPointers* cp =
			(CriticalFunctionPointers*) SHARED_MEMORY_BASE;

	mutex = altera_avalon_mutex_open(MUTEX_0_NAME); //Initialize the hardware mutex
	altera_avalon_mutex_lock(mutex, 1);
	{
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
		d.key = 5;
		d.start_ptr = 1;
		d.end_ptr = 500;
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

	set_core_assignment_table(&ca);

	//Wait for both cores to be ready
	int p0 = 0, p1 = 0;
	while ((p0 != 1) || (p1 != 1)) {
		altera_avalon_mutex_lock(mutex, 1);
		{
			memcpy(&cpg, (void*) SHARED_MEMORY_BASE,
					sizeof(CriticalFunctionPointers));
			p0 = cpg.core_ready[0];
			p1 = cpg.core_ready[1];
		}
		altera_avalon_mutex_unlock(mutex);

	}
	cp->core_ready[0] = 0;
	cp->core_ready[1] = 0;

	int arg_5 = CRITICAL_TASK_PRIORITY;
	OSTaskCreateExt(schedule_task, &arg_5,
			&schedule_task_stk[TASK_STACKSIZE - 1], SCHEDULE_TASK_PRIORITY,
			SCHEDULE_TASK_PRIORITY, schedule_task_stk, TASK_STACKSIZE, NULL,
			OS_TASK_OPT_STK_CHK + OS_TASK_OPT_STK_CLR);
	OSTaskCreateExt(print_status_task, &arg_5,
			&print_status_stk[TASK_STACKSIZE - 1], PRINT_STATUS_PRIORITY,
			PRINT_STATUS_PRIORITY, print_status_stk, TASK_STACKSIZE, NULL,
			OS_TASK_OPT_STK_CHK + OS_TASK_OPT_STK_CLR);
//	OSTaskCreateExt(print_execution_times, &arg_5, &print_execution_times_stk[TASK_STACKSIZE - 1],
//				PRINT_EXECUTION_TIMES_PRIORITY, PRINT_EXECUTION_TIMES_PRIORITY,
//				print_execution_times_stk, TASK_STACKSIZE, NULL,OS_TASK_OPT_STK_CHK + OS_TASK_OPT_STK_CLR);

	//Start operating system
	OSStart();

	return 0;
}
