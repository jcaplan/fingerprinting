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
#include "shared_mem_testing.h"
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

#define TASK_STACKSIZE	1024		//Stack size for all tasks
#define CLKS_PER_SEC	50000000
#define FPRINT_ISR_EN	1
#define TASK_NAME		testing_task
#define length			20
#define blk_sz			0xfff
#define pren			1
#define NUM_RUNS		10
#define D				printf("\nD: %d\n", dbg++);

int dbg = 0;

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

/***********************************
 * Local Variables
 **********************************/
alt_mutex_dev* mutex;									//hardware mutex pointer
OS_FLAG_GRP* schedule_fgrp;
OS_FLAGS schedule_flag;

OS_STK schedule_task_stk[TASK_STACKSIZE] __attribute__ ((section (".critical")));

OS_EVENT* done;
OS_EVENT* task_sem[16];

int *isr_0_ptr = (int *) PROCESSOR0_0_CPU_IRQ_0_BASE;
int *isr_1_ptr = (int *) PROCESSOR1_0_CPU_IRQ_0_BASE;
int *isr_2_ptr = (int *) PROCESSOR2_0_CPU_IRQ_0_BASE;
int *isr_3_ptr = (int *) PROCESSOR3_0_CPU_IRQ_0_BASE;
int *isr_4_ptr = (int *) PROCESSOR4_0_CPU_IRQ_0_BASE;
int *isr_5_ptr = (int *) PROCESSOR5_0_CPU_IRQ_0_BASE;
int *isr_6_ptr = (int *) PROCESSOR6_0_CPU_IRQ_0_BASE;
int *isr_7_ptr = (int *) PROCESSOR7_0_CPU_IRQ_0_BASE;


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
CriticalFunctionPointers* cp = (CriticalFunctionPointers*) SHARED_MEMORY_BASE;

INT8U err;

void schedule_task(void* pdata){
	int num_runs = 0;
	int i;

	printf("Monitor!\n");

	alt_u64 core_total_time[NUM_CORES];
	alt_u64 core_oflow_time[NUM_CORES];

	for (i = 0; i < NUM_CORES; i++) {
		core_total_time[i] = 0;
		core_oflow_time[i] = 0;
	}

	OSFlagPend(schedule_fgrp, 0b111100, OS_FLAG_WAIT_SET_ALL + OS_FLAG_CONSUME, 0, &err);

	while(num_runs <= NUM_RUNS){

		while(getchar() != 'c');
		printf("Scheduling %d!\n", num_runs);

		altera_avalon_mutex_lock(mutex, 1);
			//cp->task_pt = testing_task;
			cp->task_id[0] = 2;
			cp->task_id[1] = 2;
			cp->task_length[0] = length;
			cp->task_length[1] = length;
			cp->fprint_enable[0] = 1;
			cp->fprint_enable[1] = 1;
			//*isr_1_ptr = 1;
			//*isr_0_ptr = 1;
		altera_avalon_mutex_unlock(mutex);

		//OSTimeDlyHMSM(0, 0, 0, 10);

		altera_avalon_mutex_lock(mutex, 1);
			//cp->task_pt = testing_task;
			cp->task_id[2] = 3;
			cp->task_id[3] = 3;
			cp->task_length[2] = length;
			cp->task_length[3] = length;
			cp->fprint_enable[2] = 1;
			cp->fprint_enable[3] = 1;
			//*isr_2_ptr = 1;
			//*isr_3_ptr = 1;
		altera_avalon_mutex_unlock(mutex);

		//OSTimeDlyHMSM(0, 0, 0, 10);


		altera_avalon_mutex_lock(mutex, 1);
			//cp->task_pt = testing_task;
			cp->task_id[4] = 4;
			cp->task_id[5] = 4;
			cp->task_length[4] = length;
			cp->task_length[5] = length;
			cp->fprint_enable[4] = 1;
			cp->fprint_enable[5] = 1;
			*isr_4_ptr = 1;
			*isr_5_ptr = 1;
		altera_avalon_mutex_unlock(mutex);

		//OSTimeDlyHMSM(0, 0, 0, 10);

		altera_avalon_mutex_lock(mutex, 1);
			//cp->task_pt = testing_task;
			cp->task_id[6] = 5;
			cp->task_id[7] = 5;
			cp->task_length[6] = length;
			cp->task_length[7] = length;
			cp->fprint_enable[6] = 1;
			cp->fprint_enable[7] = 1;
			*isr_6_ptr = 1;
			*isr_7_ptr = 1;
		altera_avalon_mutex_unlock(mutex);

		printf("sch done\n");

		OSFlagPend(schedule_fgrp, 0b110000, OS_FLAG_WAIT_SET_ALL + OS_FLAG_CONSUME, 0, &err);

		for (i = 0; i < NUM_CORES; i++) {
			core_total_time[i] += cp->core_total_time[i];
			core_oflow_time[i] += cp->core_oflow_time[i];
			if(pren)
				printf("core %d took %llu total, %llu oflow\n", i, cp->core_total_time[i], cp->core_oflow_time[i]);
		}

		num_runs++;

	}

	printf("Stopping\n\n\n");
	//OSTimeDlyHMSM(0, 0, 5, 0);
	printf("Avg times\n\n");
	for (i = 0; i < NUM_CORES; i++) {
		printf("%llu\n%llu\n", core_total_time[i]/NUM_RUNS, core_oflow_time[i]/NUM_RUNS);
	}

	while(1){
		OSTimeDlyHMSM(0, 0, 5, 0);
	}
}


static void handle_collision_interrupt(void* context) {
	Fprint_Status status;
	fprint_status(&status);
	int i;
	//printf("Interrupt\n");
	for(i = 0; i < 16; i++) {
		if((status.successful_reg & (1 << i)) == (1 << i)) {
			OSFlagPost (schedule_fgrp, 1 << i , OS_FLAG_SET, &err);
			//printf("success %d\n",i);
		}
		if((status.failed_reg & (1 << i)) == (1 << i)) {
			printf("failure %d\n",i);
			OSFlagPost (schedule_fgrp, 1 << i , OS_FLAG_SET, &err);
		}
	}

	fprint_reset_irq();
	//OSSemPost(done);
}

static void init_collision_isr(void) {
	alt_ic_isr_register(CFPU_0_CSR_IRQ_INTERRUPT_CONTROLLER_ID,
			CFPU_0_CSR_IRQ, handle_collision_interrupt, (void*) NULL,
			(void*) NULL);
}

void enable_memory_delay() {
	int* delay_reg_enable = (int*)MEMORY_0_MEMORY_DELAY_0_BASE;
	*delay_reg_enable = 1;
}

void disable_memory_delay() {
	int* delay_reg_enable = (int*)MEMORY_0_MEMORY_DELAY_0_BASE;
	*delay_reg_enable = 1;
}

void set_memory_factor(int factor){
	int* delay_reg_count = (int*)(MEMORY_0_MEMORY_DELAY_0_BASE + 4);
	*delay_reg_count = factor;
}


int main(void) {
	int i,j;

	printf("Hello from Nios II!\n");

	if(sizeof(CriticalFunctionPointers) > 1024){
		printf("error");
				exit(1);
	}
	if(FPRINT_ISR_EN) {
		init_collision_isr();
	}



	mutex = altera_avalon_mutex_open(MUTEX_0_NAME);	//Initialize the hardware mutex
	altera_avalon_mutex_lock(mutex, 1);
		{
			cp->task_pt = TASK_NAME;
			for(i=0;i<8;i++) {
				cp->fprint_blocksize[i] = blk_sz;
			}
			cp->init_complete = 1;
		}
	altera_avalon_mutex_unlock(mutex);



	OS_FLAGS f = 0b111100;
	schedule_fgrp = OSFlagCreate(f, &err);
	done = OSSemCreate(0);
	//for(i=0 ; i<16 ; i++) {
		//task_sem[i] = OSSemCreate(0);
	//}

	//Initialize the directory for the fingerprinting unit:
	Directory_Init_Struct d;

	int dir_size = 40;
	for(i = 0; i < 2; i++){
		d.core_id = i;
		d.key = 2;
		d.start_ptr = 0;
		d.end_ptr = d.start_ptr+dir_size;
		set_task_directory(&d);

		d.key = 3;
		d.start_ptr = 50;
		d.end_ptr = d.start_ptr+dir_size;
		set_task_directory(&d);

		d.key = 4;
		d.start_ptr = 100;
		d.end_ptr = d.start_ptr+dir_size;
		set_task_directory(&d);

		d.key = 5;
		d.start_ptr = 150;
		d.end_ptr = d.start_ptr+dir_size;
		set_task_directory(&d);
	}



	Core_Assignment_Table ca;

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



	//set maxcount values for tasks
	set_maxcount_value(2,10);
	set_maxcount_value(3,10);
	set_maxcount_value(4,10);
	set_maxcount_value(5,10);

	D

	//Wait for both cores to be ready
	int p0 = 0, p1 = 0, p2 = 0, p3 = 0, p4 = 0, p5 = 0, p6 = 0, p7 = 0;
	while((p0 != 1) || (p1 != 1) || (p2 != 1) || (p3 != 1) \
			|| (p4 != 1) || (p5 != 1) || (p6 != 1) || (p7 != 1)){
		altera_avalon_mutex_lock(mutex, 1);
		{
			memcpy(&cpg,(void*)SHARED_MEMORY_BASE,sizeof(CriticalFunctionPointers));
			p0 = cpg.core_ready[0];
			p1 = cpg.core_ready[1];
			p2 = cpg.core_ready[2];
			p3 = cpg.core_ready[3];
			p4 = cpg.core_ready[4];
			p5 = cpg.core_ready[5];
			p6 = cpg.core_ready[6];
			p7 = cpg.core_ready[7];
		}
		altera_avalon_mutex_unlock(mutex);

	}

	D


	//set memory delay
	//enable_memory_delay();
	//set_memory_factor(8);

	//create the tasks
	int arg_5 = CRITICAL_TASK_PRIORITY;
	OSTaskCreateExt(schedule_task, &arg_5, &schedule_task_stk[TASK_STACKSIZE - 1],
			SCHEDULE_TASK_PRIORITY, SCHEDULE_TASK_PRIORITY,
			schedule_task_stk, TASK_STACKSIZE, NULL,OS_TASK_OPT_STK_CHK + OS_TASK_OPT_STK_CLR);;

	//Start operating system
	OSStart();

	return 0;
}
