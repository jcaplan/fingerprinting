/****************************************************************************
 * Copyright (c) 2013 Jonah Caplan, McGill University.      				*
 * All rights reserved. All use of this software and documentation is     	*
 * subject to the License Agreement located at the end of this file below.	*
 ****************************************************************************
 * Description:                                                           	*
 * This software is one half of a program that demonstrates how				*
 * fingerprinting can be controlled from software using the ucos RTOS.		*
 * This is the slave core. This is the second processor, it executes		*
 * Task 2 until an interrupt from the main processor triggers critical		*
 * execution. When multiple tasks or argument passing occur	a data 			*
 * structure in shared on chip memory can be used to pass arguments.		*
 ***************************************************************************/

/**********************************
 * Includes
 **********************************/
#include <stdio.h>
#include <stdlib.h>
#include "includes.h"
#include "fingerprint.h"
#include "shared_mem.h"
#include <altera_avalon_pio_regs.h>
#include <altera_avalon_mutex.h>
#include <sys/alt_irq.h>
#include "critical.h"
#include <string.h>
#include "sys/alt_timestamp.h"
#include "basicmath.h"
alt_mutex_dev* mutex; //hardware mutex pointer
void (*ct)(int);
void (*pt)(void*);
int *isr_0_ptr = (int *) PROCESSOR0_0_CPU_IRQ_0_BASE; //Pointer to IRQ Register
//(CPU0 -> CPU1)

#define   TASK_STACKSIZE       1024						//Stack size for all tasks

OS_STK preempt_task_stk_3[TASK_STACKSIZE];
//OS_STK preempt_task_stk_2[TASK_STACKSIZE] __attribute__ ((section (".critical")));
//OS_STK preempt_task_stk[TASK_STACKSIZE] __attribute__ ((section (".critical")));
OS_STK critical_task_stk[TASK_STACKSIZE] __attribute__ ((section (".critical")));

/* Definition of Task Priorities */

#define PREEMPT_TASK_PRIORITY_2		2
#define PREEMPT_TASK_PRIORITY_3		3
#define PREEMPT_TASK_PRIORITY		4
#define CRITICAL_TASK_PRIORITY      5
#define TASK2_PRIORITY      		25

OS_EVENT* mbox; //Message box to initiate
//OS_EVENT* preempt_sem;														//critical task
//OS_EVENT* preempt_sem_2;
//OS_EVENT* preempt_sem_3;
/*
 * Interrupt Handler and Initializer
 */
int count = 0;

void basicmathtime(int time){
		int loopvalues[4] = { time*7, 2, 7, 0 };
		BasicmathSol *solution = (BasicmathSol*)0x8203000;
		basicmath_small(loopvalues,solution);
}
int time[4];
void noncritical_task(void* pdata){
	while(1){
		time[0] = OSTimeGet();
		basicmathtime(7);
		time[1] = OSTimeGet();
		OSTimeDly(70-time[1]);
		time[2] = OSTimeGet();
		basicmathtime(6);
		time[3] = OSTimeGet();
		CriticalFunctionPointers* cp =
					(CriticalFunctionPointers*) SHARED_MEMORY_BASE;
		int i;
		for(i = 0; i < 4; i++){
			cp->timelog[0][i] = time[i];
		}
		OSTimeDlyHMSM(0,4,0,0);
	}

}
static void handle_cpu0_interrupt(void* context) {
	unsigned short priority;
	altera_avalon_mutex_lock(mutex, 1);
	{

		CriticalFunctionPointers* cp =
				(CriticalFunctionPointers*) SHARED_MEMORY_BASE;
		priority = cp->priority[0];
		*isr_0_ptr = 0;

	}
	altera_avalon_mutex_unlock(mutex);
	if (priority == CRITICAL_TASK_PRIORITY)
		OSSemPost(mbox);
//	else if(task_id == PREEMPT_TASK_PRIORITY)
//		OSSemPost(preempt_sem);
//	else if(task_id == PREEMPT_TASK_PRIORITY_2)
//		OSSemPost(preempt_sem_2);
//	else if(task_id == PREEMPT_TASK_PRIORITY_3)
//		OSSemPost(preempt_sem_3);
}

static void init_cpu0_isr(void) {
	alt_ic_isr_register(PROCESSOR0_0_CPU_IRQ_0_IRQ_INTERRUPT_CONTROLLER_ID,
			PROCESSOR0_0_CPU_IRQ_0_IRQ, handle_cpu0_interrupt, (void*) NULL,
			(void*) NULL);
}

/*
 * Critical task. Only code prior to enable fingerprinting is
 * waiting for an activation message. This is necessary
 * to guarantee the stack matches on both cores.
 */

INT8U err;
void preemption_task(void* pdata) {

	while (1) {

		OSSemPend(mbox, 0, &err);

		//barrier function
		CriticalFunctionPointers* cp =
				(CriticalFunctionPointers*) SHARED_MEMORY_BASE;

		if (alt_timestamp_start() < 0)
			printf("No timestamp device available\n");

		fprint_set_block_size(cp->blocksize[0]);
		pt = cp->task[0];
		long registers[8];
		context_switch(registers);
		pt(cp->args[0]);
		context_restore(registers);
		alt_u64 t = alt_timestamp();
		cp->core_time[0] = t;
	}
}

/*
 * Main
 */
int main(void) {
	printf("Hello from Nios II!\n");
	mutex = altera_avalon_mutex_open(MUTEX_0_NAME); //Initialize the hardware mutex
	mbox = OSSemCreate(0); //Initialize the message box
	init_cpu0_isr();
//		preempt_sem = OSSemCreate(0);
//		preempt_sem_2 = OSSemCreate(0);
//		preempt_sem_3 = OSSemCreate(0);
	CriticalFunctionPointers* cp =
			(CriticalFunctionPointers*) SHARED_MEMORY_BASE;
	while (cp->init_complete == 0)
		;
	OSTimeSet(0);
	int arg_5 = CRITICAL_TASK_PRIORITY;
	OSTaskCreateExt(preemption_task, &arg_5,
			&critical_task_stk[TASK_STACKSIZE - 1], CRITICAL_TASK_PRIORITY,
			CRITICAL_TASK_PRIORITY, critical_task_stk, TASK_STACKSIZE, NULL, 0);
//	int arg = PREEMPT_TASK_PRIORITY;
//		OSTaskCreateExt(preemption_task,(void*) &arg, (void *) &preempt_task_stk[TASK_STACKSIZE - 1],
//				PREEMPT_TASK_PRIORITY, PREEMPT_TASK_PRIORITY, preempt_task_stk, TASK_STACKSIZE, NULL,0);
//	int arg_2 = PREEMPT_TASK_PRIORITY_2;
//		OSTaskCreateExt(preemption_task, (void*) &arg_2, (void *) &preempt_task_stk_2[TASK_STACKSIZE - 1],
//				PREEMPT_TASK_PRIORITY_2, PREEMPT_TASK_PRIORITY_2, preempt_task_stk_2, TASK_STACKSIZE, NULL,0);
	int arg_3 = PREEMPT_TASK_PRIORITY_3;
		OSTaskCreateExt(noncritical_task, (void*) &arg_3, (void *) &preempt_task_stk_3[TASK_STACKSIZE - 1],
				PREEMPT_TASK_PRIORITY_3, PREEMPT_TASK_PRIORITY_3, preempt_task_stk_3, TASK_STACKSIZE, NULL,0);
	//Start OS
	altera_avalon_mutex_lock(mutex, 1); //Acquire the hardware mutex
	{
		cp->core_ready[0] = 1;
	}
	altera_avalon_mutex_unlock(mutex); //Memory

	OSStart();
	return 0;
}