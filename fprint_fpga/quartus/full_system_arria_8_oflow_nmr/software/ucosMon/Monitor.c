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
/***********************************
 * Local Variables
 **********************************/
alt_mutex_dev* mutex;									//hardware mutex pointer




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




void schedule_task(void* pdata){
	//int i;

	printf("Monitor!\n");
	CriticalFunctionPointers* cp =
						(CriticalFunctionPointers*) SHARED_MEMORY_BASE;

	while(1){
		OSSemPend(start_schedule, 0, &err);
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

				cp->task_id2 = (3);
				cp->task_id3 = (3);
				*isr_2_ptr = 1;
				*isr_3_ptr = 1;

				cp->task_id4 = (4);
				cp->task_id5 = (4);
				*isr_4_ptr = 1;
				*isr_5_ptr = 1;

				cp->task_id6 = (5);
				cp->task_id7 = (5);
				*isr_6_ptr = 1;
				*isr_7_ptr = 1;


			}
			altera_avalon_mutex_unlock(mutex);
			OSTimeDlyHMSM(0, 1, 0, 0);
		}


	}
}
int endstate[10];
void print_status_task(void* pdata){
	while(1){
		OSSemPend(done, 0, &err);
		OSTimeDlyHMSM(0, 0, 1, 0);
		int i;
		for(i = 2; i < 10; i++){
			int task = endstate[i] & 0xf;

			int success = endstate[i];

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
			endstate[i] = 0;
		}
		CriticalFunctionPointers* cp =
								(CriticalFunctionPointers*) SHARED_MEMORY_BASE;
		for(i = 0; i < 8; i++){

			printf("core %d finished at time %llu\n",i,cp->core_time[i]);
		}
	}
}

void print_execution_times(void* pdata){
	CriticalFunctionPointers* cp =
							(CriticalFunctionPointers*) SHARED_MEMORY_BASE;
	while(1){
		OSTimeDlyHMSM(0,0,5,0);
		int i;
		for(i = 0; i < 8; i++){

			printf("core %d finished at time %llu\n",i,cp->core_time[i]);
		}
	}
}

int count = 0;
static void handle_collision_interrupt(void* context) {
	Fprint_Status status;
	fprint_status(&status);

	int i;
	for(i = 0; i < 16; i++){
		if((status.successful_reg & (1 << i)) == (1 << i)){
			endstate[i] = 1;
		}
		if((status.failed_reg & (1 << i)) == (1 << i)){
			endstate[i] = -1;
		}
	}
	fprint_reset_irq();
	if( endstate[5] != 0 && endstate[4] != 0 && endstate[3] != 0 && endstate[2] != 0){
	//	OSSemPost(start_schedule);
		OSSemPost(done);
		count = 0;
	}

}

static void init_collision_isr(void) {
	alt_ic_isr_register(CFPU_0_CSR_IRQ_INTERRUPT_CONTROLLER_ID,
			CFPU_0_CSR_IRQ, handle_collision_interrupt, (void*) NULL,
			(void*) NULL);
}

int main(void) {
	printf("Hello from Nios II!\n");

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
//	for(i = 0; i < 2; i++){
//		d.core_id = i;
//		d.key = 0;
//		d.start_ptr = 5;
//		d.end_ptr = 0xff;
//		set_task_directory(&d);
//
//		for(j = 2; j < 10; j++){
//			d.key = j;
//			d.start_ptr = 50*(j-2);
//			d.end_ptr = 50*(j-1)-1;
//			set_task_directory(&d);
//
//		}
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

		d.key = 4;
		d.start_ptr = 256;
		d.end_ptr = 383;
		set_task_directory(&d);

		d.key = 5;
		d.start_ptr = 384;
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
