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

#define NUM_RUNS		1

#define MAX_LENGTH		1

#define DIR_MAX			50
#define DIR_MIN			50
#define DIR_STEP		5

#define BLK_MAX			0x4fff
#define BLK_MIN			0xff
#define BLK_STEP		256


#define SCHEDULE_TASK_PRIORITY      		1


/***********************************
 * Local Variables
 **********************************/
alt_mutex_dev* mutex;									//hardware mutex pointer
OS_FLAG_GRP* schedule_fgrp;

OS_STK schedule_task_stk[TASK_STACKSIZE] __attribute__ ((section (".critical")));

int *isr_0_ptr = (int *) PROCESSOR0_0_CPU_IRQ_0_BASE;
int *isr_1_ptr = (int *) PROCESSOR1_0_CPU_IRQ_0_BASE;
int *isr_2_ptr = (int *) PROCESSOR2_0_CPU_IRQ_0_BASE;
int *isr_3_ptr = (int *) PROCESSOR3_0_CPU_IRQ_0_BASE;
int *isr_4_ptr = (int *) PROCESSOR4_0_CPU_IRQ_0_BASE;
int *isr_5_ptr = (int *) PROCESSOR5_0_CPU_IRQ_0_BASE;
int *isr_6_ptr = (int *) PROCESSOR6_0_CPU_IRQ_0_BASE;
int *isr_7_ptr = (int *) PROCESSOR7_0_CPU_IRQ_0_BASE;


/*
 * Critical task. Only code prior to enable fingerprinting is
 * waiting for an activation message. This is necessary
 * to guarantee the stack matches on both cores.
 */
CriticalFunctionPointers cpg;
CriticalFunctionPointers* cp = (CriticalFunctionPointers*) SHARED_MEMORY_BASE;

Core_Assignment_Table ca;

void start_task(INT8U task_id, INT8U nmr, INT8U C0, INT8U C1, INT8U C2, INT8U length, INT8U fprint_enable, int blk_sz) {


	set_nmr(task_id,nmr);

	//correct table
	ca.table[0][task_id] = C0;
	ca.table[1][task_id] = C1;
	if(nmr) ca.table[2][task_id] = C2;

	set_core_assignment_table(&ca);

	altera_avalon_mutex_lock(mutex, 1);
		cp->task_id[C0] = task_id;
		cp->task_id[C1] = task_id;
		if(nmr) cp->task_id[C2] = task_id;
		cp->task_length[C0] = length;
		cp->task_length[C1] = length;
		if(nmr) cp->task_length[C2] = length;
		cp->fprint_enable[C0] = fprint_enable;
		cp->fprint_enable[C1] = fprint_enable;
		if(nmr) cp->fprint_enable[C2] = fprint_enable;
		cp->fprint_blocksize[C0] = blk_sz;
		cp->fprint_blocksize[C1] = blk_sz;
		if(nmr) cp->fprint_blocksize[C2] = blk_sz;
	altera_avalon_mutex_unlock(mutex);

	switch(C0) {

		case 0: *isr_0_ptr = 1;
				break;

		case 1: *isr_1_ptr = 1;
				break;

		case 2: *isr_2_ptr = 1;
				break;

		case 3: *isr_3_ptr = 1;
				break;

		case 4: *isr_4_ptr = 1;
				break;

		case 5: *isr_5_ptr = 1;
				break;

		case 6: *isr_6_ptr = 1;
				break;

		case 7: *isr_7_ptr = 1;
				break;

		default: break;
	}

	switch(C1) {

		case 0: *isr_0_ptr = 1;
				break;

		case 1: *isr_1_ptr = 1;
				break;

		case 2: *isr_2_ptr = 1;
				break;

		case 3: *isr_3_ptr = 1;
				break;

		case 4: *isr_4_ptr = 1;
				break;

		case 5: *isr_5_ptr = 1;
				break;

		case 6: *isr_6_ptr = 1;
				break;

		case 7: *isr_7_ptr = 1;
				break;

		default: break;
	}

	if(nmr) {
		switch(C2) {

			case 0: *isr_0_ptr = 1;
				break;

			case 1: *isr_1_ptr = 1;
				break;

			case 2: *isr_2_ptr = 1;
				break;

			case 3: *isr_3_ptr = 1;
				break;

			case 4: *isr_4_ptr = 1;
				break;

			case 5: *isr_5_ptr = 1;
				break;

			case 6: *isr_6_ptr = 1;
				break;

			case 7: *isr_7_ptr = 1;
				break;

			default: break;
		}
	}
}

INT8U err;
void DMR_SOLO() {
	int num_runs;
	int i;

	INT8U task_id;
	INT8U nmr;
	INT8U C0;
	INT8U C1;
	INT8U C2;
	INT8U length;
	INT8U fprint_en;
	int blk_sz;

	OS_FLAGS f = 0;

	alt_u64 core_total_time[NUM_CORES];
	alt_u64 core_oflow_time[NUM_CORES];
	int oflow_count[NUM_CORES];

	length = 1;
	fprint_en = 1;

	while(length <= MAX_LENGTH) {

		blk_sz = BLK_MIN;
		while(blk_sz <= BLK_MAX) {

			for (i = 0; i < NUM_CORES; i++) {
				core_total_time[i] = 0;
				core_oflow_time[i] = 0;
				oflow_count[i] = 0;

				altera_avalon_mutex_lock(mutex, 1);
					cp->oflow_count[i] = 0;
					cp->core_oflow_time[i] = 0;
					cp->core_total_time[i] = 0;
				altera_avalon_mutex_unlock(mutex);
			}

			num_runs = 1;

			while(num_runs <= NUM_RUNS){

				task_id = 0;
				nmr = 0;
				f |= 1<<task_id;
				C0 = 0;
				C1 = 1;
				C2 = 16;
				start_task(task_id, nmr, C0, C1, C2, length, fprint_en, blk_sz);


				OSFlagPend(schedule_fgrp, f, OS_FLAG_WAIT_SET_ALL + OS_FLAG_CONSUME, 0, &err);

				for (i = 0; i < NUM_CORES; i++) {
					core_total_time[i] += cp->core_total_time[i];
					core_oflow_time[i] += cp->core_oflow_time[i];
					oflow_count[i] += cp->oflow_count[i];
					OSTimeDlyHMSM(0,0,0,10);
				}
				num_runs++;
			}

			printf("%d\t", blk_sz);
			for (i = 0; i < 2; i++) {
				printf("%llu\t%llu\t%d\t", core_total_time[i]/NUM_RUNS, core_oflow_time[i]/NUM_RUNS, oflow_count[i]/NUM_RUNS);
				OSTimeDlyHMSM(0,0,0,10);
			}
			printf("\n");
			blk_sz += BLK_STEP;
		}
		length++;
	}
}

void DMR_ALL() {
	int num_runs;
	int i;

	INT8U task_id;
	INT8U nmr;
	INT8U C0;
	INT8U C1;
	INT8U C2;
	INT8U length;
	INT8U fprint_en;
	int blk_sz;

	OS_FLAGS f = 0;

	alt_u64 core_total_time[NUM_CORES];
	alt_u64 core_oflow_time[NUM_CORES];
	int oflow_count[NUM_CORES];

	length = 1;
	fprint_en = 1;

	while(length <= MAX_LENGTH) {

		blk_sz = BLK_MIN;
		while(blk_sz <= BLK_MAX) {

			for (i = 0; i < NUM_CORES; i++) {
				core_total_time[i] = 0;
				core_oflow_time[i] = 0;
				oflow_count[i] = 0;


				altera_avalon_mutex_lock(mutex, 1);
					cp->oflow_count[i] = 0;
					cp->core_oflow_time[i] = 0;
					cp->core_total_time[i] = 0;
				altera_avalon_mutex_unlock(mutex);
			}

			num_runs = 1;

			while(num_runs <= NUM_RUNS){

				task_id = 0;
				nmr = 0;
				f |= 1<<task_id;
				C0 = 0;
				C1 = 1;
				C2 = 16;
				start_task(task_id, nmr, C0, C1, C2, length, fprint_en, blk_sz);


				task_id = 1;
				nmr = 0;
				f |= 1<<task_id;
				C0 = 2;
				C1 = 3;
				C2 = 16;
				start_task(task_id, nmr, C0, C1, C2, length, fprint_en, blk_sz);


				task_id = 2;
				nmr = 0;
				f |= 1<<task_id;
				C0 = 4;
				C1 = 5;
				C2 = 16;
				start_task(task_id, nmr, C0, C1, C2, length, fprint_en, blk_sz);


				task_id = 3;
				nmr = 0;
				f |= 1<<task_id;
				C0 = 6;
				C1 = 7;
				C2 = 16;
				start_task(task_id, nmr, C0, C1, C2, length, fprint_en, blk_sz);


				OSFlagPend(schedule_fgrp, f, OS_FLAG_WAIT_SET_ALL + OS_FLAG_CONSUME, 0, &err);

				for (i = 0; i < NUM_CORES; i++) {
					core_total_time[i] += cp->core_total_time[i];
					core_oflow_time[i] += cp->core_oflow_time[i];
					oflow_count[i] += cp->oflow_count[i];
					OSTimeDlyHMSM(0,0,0,10);
				}
				num_runs++;
			}

			printf("%d\t", blk_sz);
			for (i = 0; i < NUM_CORES; i++) {
				printf("%llu\t%llu\t%d\t", core_total_time[i]/NUM_RUNS, core_oflow_time[i]/NUM_RUNS, oflow_count[i]/NUM_RUNS);
				OSTimeDlyHMSM(0,0,0,10);
			}
			printf("\n");
			blk_sz += BLK_STEP;
		}
		length++;
	}
}

void TMR_SOLO() {
	int num_runs;
	int i;

	INT8U task_id;
	INT8U nmr;
	INT8U C0;
	INT8U C1;
	INT8U C2;
	INT8U length;
	INT8U fprint_en;
	int blk_sz;

	OS_FLAGS f = 0;

	alt_u64 core_total_time[NUM_CORES];
	alt_u64 core_oflow_time[NUM_CORES];
	int oflow_count[NUM_CORES];

	length = 1;
	fprint_en = 1;

	while(length <= MAX_LENGTH) {

		blk_sz = BLK_MIN;
		while(blk_sz <= BLK_MAX) {

			for (i = 0; i < NUM_CORES; i++) {
				core_total_time[i] = 0;
				core_oflow_time[i] = 0;
				oflow_count[i] = 0;

				altera_avalon_mutex_lock(mutex, 1);
					cp->oflow_count[i] = 0;
					cp->core_oflow_time[i] = 0;
					cp->core_total_time[i] = 0;
				altera_avalon_mutex_unlock(mutex);
			}

			num_runs = 1;

			while(num_runs <= NUM_RUNS){

				task_id = 0;
				nmr = 1;
				f |= 1<<task_id;
				C0 = 0;
				C1 = 1;
				C2 = 2;
				start_task(task_id, nmr, C0, C1, C2, length, fprint_en, blk_sz);


				OSFlagPend(schedule_fgrp, f, OS_FLAG_WAIT_SET_ALL + OS_FLAG_CONSUME, 0, &err);

				for (i = 0; i < NUM_CORES; i++) {
					core_total_time[i] += cp->core_total_time[i];
					core_oflow_time[i] += cp->core_oflow_time[i];
					oflow_count[i] += cp->oflow_count[i];
					OSTimeDlyHMSM(0,0,0,10);
				}
				num_runs++;
			}

			printf("%d\t", blk_sz);
			for (i = 0; i < 3; i++) {
				printf("%llu\t%llu\t%d\t", core_total_time[i]/NUM_RUNS, core_oflow_time[i]/NUM_RUNS, oflow_count[i]/NUM_RUNS);
				OSTimeDlyHMSM(0,0,0,10);
			}
			printf("\n");
			blk_sz += BLK_STEP;
		}
		length++;
	}
}

void TMR_ALL() {
	int num_runs;
	int i;

	INT8U task_id;
	INT8U nmr;
	INT8U C0;
	INT8U C1;
	INT8U C2;
	INT8U length;
	INT8U fprint_en;
	int blk_sz;

	OS_FLAGS f = 0;

	alt_u64 core_total_time[NUM_CORES];
	alt_u64 core_oflow_time[NUM_CORES];
	int oflow_count[NUM_CORES];

	length = 1;
	fprint_en = 1;

	while(length <= MAX_LENGTH) {

		blk_sz = BLK_MIN;
		while(blk_sz <= BLK_MAX) {

			for (i = 0; i < NUM_CORES; i++) {
				core_total_time[i] = 0;
				core_oflow_time[i] = 0;
				oflow_count[i] = 0;

				altera_avalon_mutex_lock(mutex, 1);
					cp->oflow_count[i] = 0;
					cp->core_oflow_time[i] = 0;
					cp->core_total_time[i] = 0;
				altera_avalon_mutex_unlock(mutex);
			}

			num_runs = 1;

			while(num_runs <= NUM_RUNS){

				task_id = 0;
				nmr = 1;
				f |= 1<<task_id;
				C0 = 0;
				C1 = 1;
				C2 = 2;
				start_task(task_id, nmr, C0, C1, C2, length, fprint_en, blk_sz);


				task_id = 1;
				nmr = 1;
				f |= 1<<task_id;
				C0 = 3;
				C1 = 4;
				C2 = 5;
				start_task(task_id, nmr, C0, C1, C2, length, fprint_en, blk_sz);


				task_id = 2;
				nmr = 0;
				f |= 1<<task_id;
				C0 = 6;
				C1 = 7;
				C2 = 16;
				start_task(task_id, nmr, C0, C1, C2, length, fprint_en, blk_sz);


				OSFlagPend(schedule_fgrp, f, OS_FLAG_WAIT_SET_ALL + OS_FLAG_CONSUME, 0, &err);

				for (i = 0; i < NUM_CORES; i++) {
					core_total_time[i] += cp->core_total_time[i];
					core_oflow_time[i] += cp->core_oflow_time[i];
					oflow_count[i] += cp->oflow_count[i];
					OSTimeDlyHMSM(0,0,0,10);
				}
				num_runs++;
			}

			printf("%d\t", blk_sz);
			for (i = 0; i < NUM_CORES; i++) {
				printf("%llu\t%llu\t%d\t", core_total_time[i]/NUM_RUNS, core_oflow_time[i]/NUM_RUNS, oflow_count[i]/NUM_RUNS);
				OSTimeDlyHMSM(0,0,0,10);
			}
			printf("\n");
			blk_sz += BLK_STEP;
		}
		length++;
	}
}


void test_func() {

	int i,j;
	int dir_size;

	Directory_Init_Struct d;


	dir_size = DIR_MIN;

	while(dir_size <= DIR_MAX) {

		for(j = 0; j < 4; j++)
			for(i = 0; i < CA_TABLE_MAX_REDUNDANCY; i++) {
				d.core_id = i;
				d.key = j;
				d.start_ptr = j*dir_size;
				d.end_ptr = d.start_ptr+dir_size-1;
				set_task_directory(&d);
			}

		for(i = 0; i < 4; i++)
			for(j = 0; j < CA_TABLE_MAX_REDUNDANCY; j++)
				set_maxcount_value(i,j,dir_size);


		printf("# %d\n", dir_size);

		//DMR_SOLO();
		//DMR_ALL();
		//TMR_SOLO();
		TMR_ALL();

		dir_size += DIR_STEP;
	}
}

void schedule_task(void* pdata){
	printf("Monitor!\n");
	test_func();
}


static void handle_collision_interrupt(void* context) {
	Fprint_Status status;
	fprint_status(&status);
	int i;
	//printf("Interrupt\n");
	for(i = 0; i < 16; i++) {
		if((status.successful_reg & (1 << i)) == (1 << i)) {
			OSFlagPost (schedule_fgrp, 1 << i , OS_FLAG_SET, &err);
			//printf("success %d\t%x\n",i,status.successful_reg);
		}
		if((status.failed_reg & (3 << 2*i)) != (3 << 2*i)) {
			printf("failure %d\t%x\n",i,status.failed_reg);
			OSFlagPost (schedule_fgrp, 1 << i , OS_FLAG_SET, &err);
		}
	}

	fprint_reset_irq();
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
			cp->init_complete = 1;
		}
	altera_avalon_mutex_unlock(mutex);



	OS_FLAGS f = 0;
	schedule_fgrp = OSFlagCreate(f, &err);


	//Initialize the default directory for the fingerprinting unit:
	Directory_Init_Struct d;

	for(j = 0; j < CA_TABLE_NUM_TASKS; j++){
		for(i = 0; i < CA_TABLE_MAX_REDUNDANCY; i++) {
			d.core_id = i;
			d.key = j;
			d.start_ptr = j*20;
			d.end_ptr = d.start_ptr+20-1;
			set_task_directory(&d);
		}
	}

	//set default maxcount values for all tasks and logical cores
	for(i = 0; i < CA_TABLE_NUM_TASKS; i++){
		for(j = 0; j < CA_TABLE_MAX_REDUNDANCY; j++) {
			set_maxcount_value(i,j,20);
		}
	}

	//Default table
	for(i = 0; i < CA_TABLE_MAX_REDUNDANCY; i++){
		for(j = 0; j < CA_TABLE_NUM_TASKS; j++){
			ca.table[i][j] = 0;
		}
	}

	//set default nmr for all tasks
	for(i = 0; i < CA_TABLE_NUM_TASKS; i++){
		set_nmr(i,0);
	}

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

	//set memory delay
	//enable_memory_delay();
	//set_memory_factor(8);

	//create the tasks
	int arg_5 = SCHEDULE_TASK_PRIORITY;
	OSTaskCreateExt(schedule_task, &arg_5, &schedule_task_stk[TASK_STACKSIZE - 1],
			SCHEDULE_TASK_PRIORITY, SCHEDULE_TASK_PRIORITY,
			schedule_task_stk, TASK_STACKSIZE, NULL,OS_TASK_OPT_STK_CHK + OS_TASK_OPT_STK_CLR);;

	//Start operating system
	OSStart();

	return 0;
}
