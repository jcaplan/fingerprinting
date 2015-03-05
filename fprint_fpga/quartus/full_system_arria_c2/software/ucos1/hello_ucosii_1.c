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
#include "gp.h"
#include "tlb.h"

alt_mutex_dev* mutex;									//hardware mutex pointer
void (*ct)(int);

int *isr_1_ptr = (int *) PROCESSOR1_0_CPU_IRQ_0_BASE;	//Pointer to IRQ Register
								//(CPU0 -> CPU1)

#define   TASK_STACKSIZE       1024						//Stack size for all tasks


OS_STK critical_task_stk[TASK_STACKSIZE] __attribute__ ((section (".critical")));

/* Definition of Task Priorities */


#define CRITICAL_TASK_PRIORITY      5

OS_EVENT* mbox;
/*
 * Interrupt Handler and Initializer
 */
int count = 0;

/*
 * If CPU1 interrupt goes off, we assume that it has been sent
 * by the monitor for now and that a task must be executed.
 * The identity of the task is retrieved from the shared_memory
 * using the CriticalFunctionPointers data structure. The task is then executed
 */
static void handle_cpu1_interrupt(void* context) {
	unsigned short task_id;
	altera_avalon_mutex_lock(mutex, 1);
	{

		CriticalFunctionPointers* cp = (CriticalFunctionPointers*)SHARED_MEMORY_BASE;
		task_id = cp->task_id1;
		*isr_1_ptr = 0;

	}
	altera_avalon_mutex_unlock(mutex);
	if(task_id == CRITICAL_TASK_PRIORITY)
		OSSemPost(mbox);

}

static void init_cpu1_isr(void) {
	alt_ic_isr_register(PROCESSOR1_0_CPU_IRQ_0_IRQ_INTERRUPT_CONTROLLER_ID,
			PROCESSOR1_0_CPU_IRQ_0_IRQ, handle_cpu1_interrupt, (void*) NULL,
			(void*) NULL);
}

/*
 * Critical task. Only code prior to enable fingerprinting is
 * waiting for an activation message. This is necessary
 * to guarantee the stack matches on both cores.
 */

INT8U err;

/*
 * This is the wrapper for the task that executes rendundantly on both cores
 * There is one VERY important thing to note. When the critical task begins executing
 * the value of the stack pointer MUST be the same on both cores. This means that
 * the wrapper must have the same number of variables declared within its scope (i.e.
 * onto its stack) before calling the critical task (pt() in this example)
 */
void preemption_task(void* pdata){
	int* p = pdata;

	//Some ugly pointer formatting to make sure there are no gp related problems
	//when calling the task
	int priority = *p;
	void (*pt)(int) = (void*)*(p + 1);

	while(1){

		//Wait for the interrupt to begin transfer
		OSSemPend(mbox, 0, &err);

		int done = 0;
		int first = 0;
		//barrier function
		CriticalFunctionPointers* cp =
				(CriticalFunctionPointers*) SHARED_MEMORY_BASE;

		//This is a crude way of synchronizing the beginning of the task
		//on both cores
		while (done == 0) {
			altera_avalon_mutex_lock(mutex, 1); //Acquire the hardware mutex
			{
				if(first == 0){
					cp->checkout1 = 1;
					first = 1;
				}
				if( cp->checkout0 == 1){
					cp->checkout0 = 0;
					done = 1;
				}

			}
			altera_avalon_mutex_unlock(mutex);
		}

		//measure the execution time on each core
		if (alt_timestamp_start() < 0)
			{
			printf ("No timestamp device available\n");
			}

		//Context switch is necessary to clear the callee saved registers
		long registers[8];
		context_switch(registers);

		//Set the global pointer in case of compilation issues related
		//to global variables
		set_gp();
		//call the critical task
		pt(priority);
		//restore the original global pointer
		restore_gp();
		//Restore the callee saved registers
		context_restore(registers);
		//Get the end time
		alt_u64 t = alt_timestamp();
		//store the end time
		cp->core_time[1] = t;
	}
}


//Initialize the TLB. It uses 4k pages so only one line is necessary
void init_tlb(){
	set_cputable_entry(1, 0x00431000);
	set_spmtable_entry(1, 0x04203000);

	//Enable these 4 translations
	set_enable(0x2);

}



/*
 * Main
 */
int main(void) {
	//start up the TLB. This could be done in the preemption_task wrapper but we never need it off in this
	//example
	init_tlb();
	enable_tlb();
	void (*pt)(int);
	printf("Hello from Nios II!\n");
		mutex = altera_avalon_mutex_open(MUTEX_0_NAME);			//Initialize the hardware mutex
		mbox = OSSemCreate(0);				//Initialize the message box
		CriticalFunctionPointers* cp = (CriticalFunctionPointers*)SHARED_MEMORY_BASE;

		//Wait for monitor to be done initialization of shared variables before retrieving their values
		while(cp->init_complete == 0);
			altera_avalon_mutex_lock(mutex, 1);				//Acquire the hardware mutex
			{

				//retrieve shared values from monitor
				ct = cp->critical;
				pt = cp->preempt;
			}
			altera_avalon_mutex_unlock(mutex);				//Memory


	init_cpu1_isr();										//Initialize the ISR

	//Set default block size for fingerprinting
	fprint_set_block_size(0x3ff);


	//Set the task(only one in this example)
	void* arg_5[2] = {(void*)CRITICAL_TASK_PRIORITY,pt};
			OSTaskCreateExt(preemption_task, &arg_5, &critical_task_stk[TASK_STACKSIZE - 1],
					CRITICAL_TASK_PRIORITY, CRITICAL_TASK_PRIORITY,
					critical_task_stk, TASK_STACKSIZE, NULL,0);


	//Signal that the core has finished initializing
	altera_avalon_mutex_lock(mutex, 1);				//Acquire the hardware mutex
	{
		cp->core1_ready = 1;
	}
	altera_avalon_mutex_unlock(mutex);				//Memory
	//Start OS
	OSStart();
	return 0;
}
