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

alt_dma_txchan txchan[4];
alt_dma_rxchan rxchan[4];
static void done_dma_rx (void* handle);
static void done_dma_tx (void* handle);
int *isr_0_ptr = (int *) PROCESSOR0_0_CPU_IRQ_0_BASE;
int *isr_1_ptr = (int *) PROCESSOR1_0_CPU_IRQ_0_BASE;
int *isr_2_ptr = (int *) PROCESSOR2_0_CPU_IRQ_0_BASE;
int *isr_3_ptr = (int *) PROCESSOR3_0_CPU_IRQ_0_BASE;

#define   TASK_STACKSIZE       768						//Stack size for all tasks

OS_STK schedule_task_stk[TASK_STACKSIZE] __attribute__ ((section (".critical")));
OS_STK dma_task_stk[TASK_STACKSIZE] __attribute__ ((section (".critical")));
OS_STK print_status_stk[TASK_STACKSIZE] __attribute__ ((section (".critical")));
OS_STK print_execution_times_stk[TASK_STACKSIZE] __attribute__ ((section (".critical")));


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

int count[2][2];
int status;
char solutions[2][1024];
OS_FLAG_GRP* dma_fgrp;
OS_FLAG_GRP* schedule_fgrp;
OS_FLAGS dma_flag;
OS_FLAGS schedule_flag;


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



#define TASK2_DONE_MASK 0x4
#define TASK3_DONE_MASK 0x8
#define TASK2_DMA_DONE_MASK 0x40000
#define TASK3_DMA_DONE_MASK 0x80000
#define CORE0_TX_MASK 0x1
#define CORE1_TX_MASK 0x2
#define CORE2_TX_MASK 0x4
#define CORE3_TX_MASK 0x8
#define CORE0_RX_MASK 0x100
#define CORE1_RX_MASK 0x200
#define CORE2_RX_MASK 0x400
#define CORE3_RX_MASK 0x800

void schedule_task(void* pdata){
	printf("Monitor!\n");
	CriticalFunctionPointers* cp =
						(CriticalFunctionPointers*) SHARED_MEMORY_BASE;
	OS_FLAGS  flag;
	while(1){

		//Wait for either DMA to complete or an interrupt from Comparator
		flag = OSFlagPend(schedule_fgrp, 0xfffff, OS_FLAG_WAIT_SET_ANY + OS_FLAG_CONSUME, 0, &err);

		//4 possible scenarios

		if((flag & TASK2_DMA_DONE_MASK) == TASK2_DMA_DONE_MASK){
			altera_avalon_mutex_lock(mutex, 1);
			{
				cp->critical = critical_task;
				cp->preempt = preempt_task;
				cp->task_id0 = (2);
				cp->task_id1 = (2);
				*isr_1_ptr = 1;
				*isr_0_ptr = 1;
			}
			altera_avalon_mutex_unlock(mutex);

		}


		if((flag & TASK3_DMA_DONE_MASK) == TASK3_DMA_DONE_MASK){
			altera_avalon_mutex_lock(mutex, 1);
			{
				cp->critical = critical_task;
				cp->preempt = preempt_task;
				cp->task_id2 = (3);
				cp->task_id3 = (3);
				*isr_2_ptr = 1;
				*isr_3_ptr = 1;
			}
			altera_avalon_mutex_unlock(mutex);

		}

		if(((flag & TASK2_DONE_MASK) == TASK2_DONE_MASK) && ((flag & TASK3_DONE_MASK) == TASK3_DONE_MASK)){
			OSFlagPost (dma_fgrp, CORE0_RX_MASK + CORE2_RX_MASK, OS_FLAG_SET, &err);
		}
		else if((flag & TASK2_DONE_MASK) == TASK2_DONE_MASK){
			//Do a single DMA copy back here to a new buffer
			OSFlagPost (dma_fgrp, CORE0_RX_MASK, OS_FLAG_SET, &err);
		}
		else if((flag & TASK3_DONE_MASK) == TASK3_DONE_MASK){
			//Do a single DMA copy back here to a new buffer
			OSFlagPost (dma_fgrp, CORE2_RX_MASK, OS_FLAG_SET, &err);
		}
	}
}

int handle[] = {0,1,2,3};
void dma_task(void* pdata){
	int size = 1024;
	void* tx_data; /* pointer to data to send */
	void* rx_buffer; /* pointer to rx buffer */
	OS_FLAGS  flag;
	while(1){
		flag = OSFlagPend(dma_fgrp, 0xffff, OS_FLAG_WAIT_SET_ANY + OS_FLAG_CONSUME, 0, &err);

		if((flag & CORE0_TX_MASK) == CORE0_TX_MASK){
			tx_data = (void*) vectors; /* pointer to data to send */
			rx_buffer = (void*) 0x8203000; /* pointer to rx buffer */
			alt_dma_txchan_send (txchan[0],
					tx_data,
					size,
					NULL,
					NULL);
			alt_dma_rxchan_prepare (rxchan[0],
								rx_buffer,
								size,
								done_dma_tx,
								&handle[0]);

		}

		if((flag & CORE1_TX_MASK) == CORE1_TX_MASK){
			tx_data = (void*) vectors; /* pointer to data to send */
			rx_buffer = (void*) 0x8203000; /* pointer to rx buffer */
			alt_dma_txchan_send (txchan[1],
					tx_data,
					size,
					NULL,
					NULL);
			alt_dma_rxchan_prepare (rxchan[1],
								rx_buffer,
								size,
								done_dma_tx,
								&handle[1]);
		}
		if((flag & CORE2_TX_MASK) == CORE2_TX_MASK){
			tx_data = (void*) vectors; /* pointer to data to send */
			rx_buffer = (void*) 0x8203000; /* pointer to rx buffer */
			alt_dma_txchan_send (txchan[2],
					tx_data,
					size,
					NULL,
					NULL);
			alt_dma_rxchan_prepare (rxchan[2],
								rx_buffer,
								size,
								done_dma_tx,
								&handle[2]);
		}
		if((flag & CORE3_TX_MASK) == CORE3_TX_MASK){
			tx_data = (void*) vectors; /* pointer to data to send */
			rx_buffer = (void*) 0x8203000; /* pointer to rx buffer */
			alt_dma_txchan_send (txchan[3],
					tx_data,
					size,
					NULL,
					NULL);
			alt_dma_rxchan_prepare (rxchan[3],
								rx_buffer,
								size,
								done_dma_tx,
								&handle[3]);
		}

		if((flag & CORE0_RX_MASK) == CORE0_RX_MASK){
			tx_data = (void*) 0x8203000; /* pointer to data to send */
			rx_buffer = (void*) &solutions[0]; /* pointer to rx buffer */
			alt_dma_txchan_send (txchan[0],
					tx_data,
					size,
					NULL,
					NULL);
			alt_dma_rxchan_prepare (rxchan[0],
								rx_buffer,
								size,
								done_dma_rx,
								&handle[0]);

		}


		if((flag & CORE1_RX_MASK) == CORE1_RX_MASK){
			tx_data = (void*) 0x8203000; /* pointer to data to send */
			rx_buffer = (void*) &solutions[0]; /* pointer to rx buffer */
			alt_dma_txchan_send (txchan[1],
					tx_data,
					size,
					NULL,
					NULL);
			alt_dma_rxchan_prepare (rxchan[1],
								rx_buffer,
								size,
								done_dma_rx,
								&handle[1]);
		}
		if((flag & CORE2_RX_MASK) == CORE2_RX_MASK){
			tx_data = (void*) 0x8203000; /* pointer to data to send */
			rx_buffer = (void*) &solutions[1]; /* pointer to rx buffer */
			alt_dma_txchan_send (txchan[2],
					tx_data,
					size,
					NULL,
					NULL);
			alt_dma_rxchan_prepare (rxchan[2],
								rx_buffer,
								size,
								done_dma_rx,
								&handle[2]);
		}
		if((flag & CORE3_RX_MASK) == CORE3_RX_MASK){
			tx_data = (void*) 0x8203000; /* pointer to data to send */
			rx_buffer = (void*) &solutions[1]; /* pointer to rx buffer */
			alt_dma_txchan_send (txchan[3],
					tx_data,
					size,
					NULL,
					NULL);
			alt_dma_rxchan_prepare (rxchan[3],
								rx_buffer,
								size,
								done_dma_rx,
								&handle[3]);
		}
	}
}


int endstate_p[10];

void copy_endstate(int* endstate){
	int i;
	for(i = 0; i < 10; i++){
		endstate_p[i] = endstate[i];
		endstate_p[i] = endstate[i];

	}
}




void print_status_task(void* pdata){
	while(1){
	//	OSSemPend(done, 0, &err);
		int i;
		for(i = 2; i < 10; i++){

			int success = endstate_p[i];

			printf("task %d was ",i);
			if(success == 1){
				printf("successful ");
			}
			else if(success == -1){
				printf("unsuccessful ");
			}
			else{
				printf("not executed ");
			}
			unsigned long t = clock();
			printf("at time %lu.\n",t);
		}

		for(i = 0; i < 10; i++){
			endstate_p[i] = 0;
		}
	}
}

void print_execution_times(void* pdata){

	while(1){
		OSTimeDlyHMSM(0,0,1,0);
		printf("at time %d task 2 finished %d successful and %d failed executions\n",(int)clock(),count[0][0],count[0][1]);
		printf("at time %d task 3 finished %d successful and %d failed executions\n",(int)clock(),count[1][0],count[1][1]);
		count[0][0] = 0;
		count[0][1] = 0;
		count[1][0] = 0;
		count[1][1] = 1;
	}
}


static void handle_collision_interrupt(void* context) {
	int endstate[10];
	Fprint_Status status;
	fprint_status(&status);
	OS_FLAGS f;
	int i;
	for(i = 0; i < 16; i++){
		if((status.successful_reg & (1 << i)) == (1 << i)){
			endstate[i] = 1;
			f |= (1 << i);
		}
		if((status.failed_reg & (1 << i)) == (1 << i)){
			endstate[i] = -1;
			f |= (1 << i);
		}
	}
	fprint_reset_irq();
	if( endstate[2] == 1){
		count[0][0]++;
	//	OSSemPost(done);
	}
	else if(endstate[2] == -1){
		count[0][1]++;
	}
	if (endstate[3] == 1){
		count[1][0]++;
	}
	else if(endstate[3] == -1){
		count[1][1]++;
	}
	//copy_endstate(endstate);
	INT8U err;
	OSFlagPost (schedule_fgrp, f, OS_FLAG_SET, &err);

}

static void init_collision_isr(void) {
	alt_ic_isr_register(CFPU_0_CSR_IRQ_INTERRUPT_CONTROLLER_ID,
			CFPU_0_CSR_IRQ, handle_collision_interrupt, (void*) NULL,
			(void*) NULL);
}

static volatile int rx_state[4];
static void done_dma_rx (void* handle)
{
	rx_state[*(int*)handle] = 1;


	//When the old answer is done being transfered back
	//only one answer is copied back
	if((rx_state[0] == 1 || rx_state[1] == 1)){
		INT8U err;
		rx_state[0] = 0;
		rx_state[1] = 0;
		OSFlagPost (dma_fgrp, CORE0_TX_MASK + CORE1_TX_MASK, OS_FLAG_SET,  &err);
	}
	else if((rx_state[2] == 1 || rx_state[3] == 1)){
		INT8U err;
		rx_state[2] = 0;
		rx_state[3] = 0;
		OSFlagPost (dma_fgrp, CORE2_TX_MASK + CORE3_TX_MASK, OS_FLAG_SET,  &err);
	}
}



static volatile int tx_state[4];
static void done_dma_tx (void* handle)
{
	tx_state[*(int*)handle] = 1;
	if((tx_state[0] == 1 && tx_state[1] == 1)){
		INT8U err;
		tx_state[0] = 0;
		tx_state[1] = 0;
		OSFlagPost (schedule_fgrp, TASK2_DMA_DONE_MASK , OS_FLAG_SET,  &err);
	}
	else if((tx_state[2] == 1 && tx_state[3] == 1)){
		INT8U err;
		tx_state[2] = 0;
		tx_state[3] = 0;
		OSFlagPost (schedule_fgrp, TASK3_DMA_DONE_MASK, OS_FLAG_SET,  &err);
	}
}
void dma_init(void){

	int i;
	char *dev = "/dev/processorX_0_dma_0";

	for(i = 0; i < 4; i++){
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
	 printf("Hello from Nios II!\n");


	// set_memory_factor(1);

	int i,j;
	init_collision_isr();
	CriticalFunctionPointers* cp =
					(CriticalFunctionPointers*) SHARED_MEMORY_BASE;

	mutex = altera_avalon_mutex_open(MUTEX_0_NAME);	//Initialize the hardware mutex
	altera_avalon_mutex_lock(mutex, 1);
		{
			cp->critical = critical_task;
			cp->preempt = qsort_test;
			cp->init_complete = 1;
		}
	altera_avalon_mutex_unlock(mutex);


	OS_FLAGS f = 0x0;
	INT8U err;
	dma_fgrp = OSFlagCreate(f, &err);
	f = 0b1100;
	schedule_fgrp = OSFlagCreate(f, &err);



	dma_init();





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
		d.end_ptr = 127;
		set_task_directory(&d);


			d.key = 3;
			d.start_ptr = 128;
			d.end_ptr = 255;
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

	set_core_assignment_table(&ca);

	//Wait for both cores to be ready
	int p0 = 0, p1 = 0, p2 = 0, p3 = 0;
	while((p0 != 1) || (p1 != 1) || (p2 != 1) || (p3 != 1) ){
		altera_avalon_mutex_lock(mutex, 1);
		{
			memcpy(&cpg,(void*)SHARED_MEMORY_BASE,sizeof(CriticalFunctionPointers));
			p0 = cpg.core0_ready;
			p1 = cpg.core1_ready;
			p2 = cpg.core2_ready;
			p3 = cpg.core3_ready;
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
	OSTaskCreateExt(dma_task, &arg_5, &dma_task_stk[TASK_STACKSIZE - 1],
				6, 6,
				dma_task_stk, TASK_STACKSIZE, NULL,OS_TASK_OPT_STK_CHK + OS_TASK_OPT_STK_CLR);
//	OSTaskCreateExt(print_status_task, &arg_5, &print_status_stk[TASK_STACKSIZE - 1],
//				PRINT_STATUS_PRIORITY, PRINT_STATUS_PRIORITY,
//				print_status_stk, TASK_STACKSIZE, NULL,OS_TASK_OPT_STK_CHK + OS_TASK_OPT_STK_CLR);
	OSTaskCreateExt(print_execution_times, &arg_5, &print_execution_times_stk[TASK_STACKSIZE - 1],
				PRINT_EXECUTION_TIMES_PRIORITY, PRINT_EXECUTION_TIMES_PRIORITY,
				print_execution_times_stk, TASK_STACKSIZE, NULL,OS_TASK_OPT_STK_CHK + OS_TASK_OPT_STK_CLR);


	//Start operating system
	OSStart();

	return 0;
}
