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
/***********************************
 * Local Variables
 **********************************/
alt_mutex_dev* mutex;									//hardware mutex pointer
OS_FLAG_GRP* dma_fgrp;
OS_FLAGS dma_flag;
OS_FLAG_GRP* schedule_fgrp;
OS_FLAGS schedule_flag;

void dma(void* tx_data, void* rx_buffer, int size);
void dma_end(void);
int *isr_0_ptr = (int *) PROCESSOR0_0_CPU_IRQ_0_BASE;
int *isr_1_ptr = (int *) PROCESSOR1_0_CPU_IRQ_0_BASE;
int *isr_2_ptr = (int *) PROCESSOR2_0_CPU_IRQ_0_BASE;
int *isr_3_ptr = (int *) PROCESSOR3_0_CPU_IRQ_0_BASE;
int *isr_4_ptr = (int *) PROCESSOR4_0_CPU_IRQ_0_BASE;
int *isr_5_ptr = (int *) PROCESSOR5_0_CPU_IRQ_0_BASE;
int *isr_6_ptr = (int *) PROCESSOR6_0_CPU_IRQ_0_BASE;
int *isr_7_ptr = (int *) PROCESSOR7_0_CPU_IRQ_0_BASE;

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

/*
 * Monitor task priorities
 */

#define SCHEDULE_TASK_PRIORITY      		5
#define PRINT_STATUS_PRIORITY				4
#define PRINT_EXECUTION_TIMES_PRIORITY		3

int count[4][2];
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

alt_dma_txchan txchan[8];
alt_dma_rxchan rxchan[8];


void schedule_task(void* pdata){
	//int i;

	printf("Monitor!\n");
	CriticalFunctionPointers* cp =
						(CriticalFunctionPointers*) SHARED_MEMORY_BASE;

	while(1){
		OSFlagPend(schedule_fgrp, 0b111100, OS_FLAG_WAIT_SET_ALL + OS_FLAG_CONSUME, 0, &err);

		int size = 1024;

		void* tx_data = (void*) vectors; /* pointer to data to send */
		void* rx_buffer = (void*) 0x8203000; /* pointer to rx buffer */

		dma(tx_data,rx_buffer,size);
		OSFlagPend(dma_fgrp, 0xff, OS_FLAG_WAIT_SET_ALL + OS_FLAG_CONSUME, 0, &err);
		int i;
		for(i = 0; i < 1; i ++){
			altera_avalon_mutex_lock(mutex, 1);
			{
				cp->critical = critical_task;
				cp->preempt = preempt_task;
				cp->task_id0 = (2);
				cp->task_id1 = (2);
				*isr_1_ptr = 1;
				*isr_0_ptr = 1;
				altera_avalon_mutex_unlock(mutex);

			}
			altera_avalon_mutex_unlock(mutex);
			OSTimeDlyHMSM(0, 0, 0, 300);

			altera_avalon_mutex_lock(mutex, 1);
				cp->critical = critical_task;
				cp->preempt = preempt_task;
				cp->task_id2 = (3);
				cp->task_id3 = (3);
				*isr_2_ptr = 1;
				*isr_3_ptr = 1;
			altera_avalon_mutex_unlock(mutex);

			altera_avalon_mutex_lock(mutex, 1);
			{
				cp->critical = critical_task;
				cp->preempt = preempt_task;
				cp->task_id4 = (4);
				cp->task_id5 = (4);
				*isr_4_ptr = 1;
				*isr_5_ptr = 1;
				altera_avalon_mutex_unlock(mutex);

			}
			altera_avalon_mutex_unlock(mutex);
			OSTimeDlyHMSM(0, 0, 0, 300);

			altera_avalon_mutex_lock(mutex, 1);
				cp->critical = critical_task;
				cp->preempt = preempt_task;
				cp->task_id6 = (5);
				cp->task_id7 = (5);
				*isr_6_ptr = 1;
				*isr_7_ptr = 1;
			altera_avalon_mutex_unlock(mutex);
			//OSTimeDlyHMSM(0, 1, 0, 0);
		}


	}
}
int endstate[10];
void print_status_task(void* pdata){
	while(1){
		OSSemPend(done, 0, &err);
		int i;
//		for(i = 2; i < 10; i++){
//
//			int success = endstate[i];
//
//			printf("task %d was ",i);
//			if(success == 1){
//				printf("successful ");
//			}
//			else if(success == -1){
//				printf("unsuccessful ");
//			}
//			else{
//				printf("not executed ");
//			}
//			unsigned long t = clock();
//			printf("at time %lu.\n",t);
//		}
//
//		for(i = 0; i < 10; i++){
//			endstate[i] = 0;
//		}
			dma_end();

	}
}

void print_execution_times(void* pdata){

	while(1){
		OSTimeDlyHMSM(0,0,1,0);
		printf("at time %d task 2 finished %d successful and %d failed executions\n",(int)clock(),count[0][0],count[0][1]);
		printf("at time %d task 3 finished %d successful and %d failed executions\n",(int)clock(),count[1][0],count[1][1]);
		printf("at time %d task 4 finished %d successful and %d failed executions\n",(int)clock(),count[2][0],count[2][1]);
		printf("at time %d task 5 finished %d successful and %d failed executions\n",(int)clock(),count[3][0],count[3][1]);




		count[0][0] = 0;
		count[0][1] = 0;
		count[1][0] = 0;
		count[1][1] = 0;
		count[2][0] = 0;
		count[2][1] = 0;
		count[3][0] = 0;
		count[3][1] = 0;

	}

}


static void handle_collision_interrupt(void* context) {
	Fprint_Status status;
	fprint_status(&status);

	int i;
	for(i = 0; i < 16; i++){
		if((status.successful_reg & (1 << i)) == (1 << i)){
			OSFlagPost (schedule_fgrp, 1 << i , OS_FLAG_SET, &err);
			endstate[i] = 1;
			if(i == 2)
				count[0][0]++;
			else if(i == 3)
				count[1][0]++;
			else if(i == 4)
				count[2][0]++;
			else if(i == 5)
				count[3][0]++;
		}
		if((status.failed_reg & (1 << i)) == (1 << i)){
			OSFlagPost (schedule_fgrp, 1 << i , OS_FLAG_SET, &err);
			endstate[i] = -1;
			if(i == 2)
				count[0][1]++;
			else if(i == 3)
				count[1][1]++;
			else if(i == 4)
				count[2][1]++;
			else if(i == 5)
				count[3][1]++;
		}
	}
	fprint_reset_irq();

	if( endstate[3] != 0 && endstate[2] != 0 && endstate[4] != 0 && endstate[5] != 0){
		endstate[3] = 0;
		endstate[2] = 0;
		endstate[4] = 0;
		endstate[5] = 0;
		OSSemPost(done);

	}
}

static void init_collision_isr(void) {
	alt_ic_isr_register(CFPU_0_CSR_IRQ_INTERRUPT_CONTROLLER_ID,
			CFPU_0_CSR_IRQ, handle_collision_interrupt, (void*) NULL,
			(void*) NULL);
}



static void done_dma (void* handle, void* data)
{
	int id = *(int*)handle;
	OS_FLAGS f = 1 << id;
	OSFlagPost (dma_fgrp, f , OS_FLAG_SET, &err);
}

static volatile int rx_done1 = 0;

static void done_dma1 (void* handle, void* data)
{
rx_done1++;
}
int answer[4][1024];
void dma_end(void){

	int size = 1024;
	int rc;
	void* tx_data = (void*) 0x8203000; /* pointer to data to send */

	int i;
	for(i = 0; i < 4; i++){
		void* rx_buffer = (void*)&answer[i] ; /* pointer to rx buffer */
		if ((rc = alt_dma_txchan_send (txchan[2*i],
		tx_data,
		size,
		NULL,
		NULL)) < 0){
			printf ("Failed to post transmit request, reason = %i\n", rc);

		}
		if ((rc = alt_dma_rxchan_prepare (rxchan[2*i],
				rx_buffer,
				size,
				done_dma1,
				NULL)) < 0){
					printf ("Failed to post read request, reason = %i\n", rc);

				}


	}
	while (rx_done1 < 4);
	rx_done1 = 0;
}
int x[] = {0,1,2,3,4,5,6,7};
void dma(void* tx_data, void* rx_buffer, int size){


	int rc;

int i;
	/* Create the transmit channel */

	for(i = 0; i < 8; i++){
		if ((rc = alt_dma_txchan_send (txchan[i],
		tx_data,
		size,
		NULL,
		NULL)) < 0){
			printf ("Failed to post transmit request, reason = %i\n", rc);

		}
		/* Post the receive request */
		if ((rc = alt_dma_rxchan_prepare (rxchan[i],
		rx_buffer,
		size,
		done_dma,
		&x[i])) < 0){
			printf ("Failed to post read request, reason = %i\n", rc);

		}
		/* wait for transfer to complete */

		//printf ("Transfer successful!\n");
	}
}

void dma_init(void){

	int i;
	char *dev = "/dev/processorX_0_dma_0";

	for(i = 0; i < 8; i++){
		sprintf(dev, "/dev/processor%d_0_dma_0",i);
		if ((txchan[i] = alt_dma_txchan_open(dev)) == NULL){
			printf ("Failed to open transmit channel\n");

		}
		/* Create the receive channel */
		if ((rxchan[i] = alt_dma_rxchan_open(dev)) == NULL){
			printf ("Failed to open receive channel\n");

		}
	}

}


//void set_memory_factor(int factor){
//	int* delay_reg_enable = (int*)MEMORY_0_MEMORY_DELAY_0_BASE;
//	int* delay_reg_count = (int*)(MEMORY_0_MEMORY_DELAY_0_BASE + 4);
//
//	*delay_reg_count = factor;
//	*delay_reg_enable = 1;
//}

int main(void) {

	dma_init();

	 printf("Hello from Nios II!\n");

//		set_memory_factor(1);
	int i,j;
	init_collision_isr();
	CriticalFunctionPointers* cp =
					(CriticalFunctionPointers*) SHARED_MEMORY_BASE;

	mutex = altera_avalon_mutex_open(MUTEX_0_NAME);	//Initialize the hardware mutex
	altera_avalon_mutex_lock(mutex, 1);
		{
			cp->critical = critical_task;
			cp->preempt = preempt_task;
			cp->init_complete = 1;
		}
	altera_avalon_mutex_unlock(mutex);


	OS_FLAGS f = 0x0;
	dma_fgrp = OSFlagCreate(f, &err);
	f = 0b111100;
	schedule_fgrp = OSFlagCreate(f, &err);


	done = OSSemCreate(0);
	start_schedule = OSSemCreate(1);

	//Initialize the message box
	//Initialize the directory for the fingerprinting unit:
	Directory_Init_Struct d;
//
//	if (alt_timestamp_star	t() < 0)
//
//	{
//		printf("No timestamp device available\n");
//	}
	for(i = 0; i < 2; i++){
		d.core_id = i;
		d.key = 2;
		d.start_ptr = 0;
		d.end_ptr = 5;
		set_task_directory(&d);


			d.key = 3;
			d.start_ptr = 50;
			d.end_ptr = 55;
			set_task_directory(&d);

			d.key = 4;
			d.start_ptr = 80;
			d.end_ptr = 85;
			set_task_directory(&d);

			d.key = 5;
			d.start_ptr = 111;
			d.end_ptr = 116;
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
	for(i = 0; i < CA_TABLE_MAX_REDUNDANCY; i++){
		for(j = 0; j < CA_TABLE_NUM_TASKS; j++){
			ca.table[i][j] = i;
		}
	}

	//correct table
	ca.table[0][2] = 0;
	ca.table[1][2] = 1;
	ca.table[0][3] = 2;
	ca.table[1][3] = 3;
	ca.table[0][4] = 4;
	ca.table[1][4] = 5;
	ca.table[0][5] = 6;
	ca.table[1][5] = 7;

	set_core_assignment_table(&ca);

	//Wait for both cores to be ready
	int p0 = 0, p1 = 0, p2 = 0, p3 = 0, p4 = 0, p5 = 0, p6 = 0, p7 = 0;
	while((p0 != 1) || (p1 != 1) || (p2 != 1) || (p3 != 1) \
			|| (p4 != 1) || (p5 != 1) || (p6 != 1) || (p7 != 1)){
		altera_avalon_mutex_lock(mutex, 1);
		{
			memcpy(&cpg,(void*)SHARED_MEMORY_BASE,sizeof(CriticalFunctionPointers));
			p0 = cpg.core0_ready;
			p1 = cpg.core1_ready;
			p2 = cpg.core2_ready;
			p3 = cpg.core3_ready;
			p4 = cpg.core4_ready;
			p5 = cpg.core5_ready;
			p6 = cpg.core6_ready;
			p7 = cpg.core7_ready;
		}
		altera_avalon_mutex_unlock(mutex);
	//	alt_busy_sleep(100000);

	}
//	FILE *fp;
//	char str[128];
//	fp = fopen("/mnt/rozipfs/file", "r");
//	while (fscanf(fp, "%s", str)!=EOF)
//	        printf("%s\n",str);

	int arg_5 = CRITICAL_TASK_PRIORITY;
	OSTaskCreateExt(schedule_task, &arg_5, &schedule_task_stk[TASK_STACKSIZE - 1],
			SCHEDULE_TASK_PRIORITY, SCHEDULE_TASK_PRIORITY,
			schedule_task_stk, TASK_STACKSIZE, NULL,OS_TASK_OPT_STK_CHK + OS_TASK_OPT_STK_CLR);
	OSTaskCreateExt(print_status_task, &arg_5, &print_status_stk[TASK_STACKSIZE - 1],
				PRINT_STATUS_PRIORITY, PRINT_STATUS_PRIORITY,
				print_status_stk, TASK_STACKSIZE, NULL,OS_TASK_OPT_STK_CHK + OS_TASK_OPT_STK_CLR);
	OSTaskCreateExt(print_execution_times, &arg_5, &print_execution_times_stk[TASK_STACKSIZE - 1],
				PRINT_EXECUTION_TIMES_PRIORITY, PRINT_EXECUTION_TIMES_PRIORITY,
				print_execution_times_stk, TASK_STACKSIZE, NULL,OS_TASK_OPT_STK_CHK + OS_TASK_OPT_STK_CLR);


	//Start operating system
	OSStart();

	return 0;
}
