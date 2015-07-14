#include <stdio.h>
#include <stdlib.h>
#include "includes.h"
#include "fingerprint.h"
#include "shared_mem_testing.h"
#include <altera_avalon_pio_regs.h>
#include <altera_avalon_mutex.h>
#include <sys/alt_irq.h>
#include "critical.h"
#include <string.h>
#include "sys/alt_timestamp.h"

alt_mutex_dev* mutex;

void (*task_pt)(void*);
unsigned short fprint_blocksize;
INT8U FprintActive = 0;

int *isr_4_ptr = (int *) PROCESSOR4_0_CPU_IRQ_0_BASE;
int *isr_4_oflow_ptr = (int *) PROCESSOR4_0_CPU_OFLOW_IRQ_0_BASE;

int overflow;
alt_u64 t_oflow = 0;

CriticalFunctionPointers* cp = (CriticalFunctionPointers*)SHARED_MEMORY_BASE;

#define	  PHYSICAL_CORE_ID	   4
#define   TASK_STACKSIZE       1024

OS_STK execution_task_stk[TASK_STACKSIZE] __attribute__ ((section (".critical")));

#define EXECUTION_TASK_PRIORITY		1

OS_EVENT* execution_mbox;

unsigned short task_id;
unsigned short task_length;
unsigned short fprint_enable;

static void handle_cpu4_interrupt(void* context) {

	altera_avalon_mutex_lock(mutex, 1);
		task_id = cp->task_id[PHYSICAL_CORE_ID];
		task_length = cp->task_length[PHYSICAL_CORE_ID];
		fprint_enable = cp->fprint_enable[PHYSICAL_CORE_ID];
		fprint_blocksize = cp->fprint_blocksize[PHYSICAL_CORE_ID];
		*isr_4_ptr = 0;
	altera_avalon_mutex_unlock(mutex);

	OSSemPost(execution_mbox);

}

static void init_cpu4_isr(void) {
	alt_ic_isr_register(PROCESSOR4_0_CPU_IRQ_0_IRQ_INTERRUPT_CONTROLLER_ID,
			PROCESSOR4_0_CPU_IRQ_0_IRQ, handle_cpu4_interrupt, (void*) NULL,
			(void*) NULL);
}

static void handle_cpu4_oflow_interrupt(void *context) {

	altera_avalon_mutex_lock(mutex, 1);

		overflow = *isr_4_oflow_ptr & 0x100;

		if(overflow){
			OSTaskSuspend(EXECUTION_TASK_PRIORITY);

			t_oflow = alt_timestamp();
		} else {
			OSTaskResume(EXECUTION_TASK_PRIORITY);

			t_oflow = alt_timestamp() - t_oflow;
			cp->core_oflow_time[PHYSICAL_CORE_ID] += t_oflow;
			cp->oflow_count[PHYSICAL_CORE_ID] += 1;
		}

		*isr_4_oflow_ptr = 0;

	altera_avalon_mutex_unlock(mutex);
}

static void init_cpu4_oflow_isr(void) {
	alt_ic_isr_register(PROCESSOR4_0_CPU_OFLOW_IRQ_0_IRQ_INTERRUPT_CONTROLLER_ID,
			PROCESSOR4_0_CPU_OFLOW_IRQ_0_IRQ, handle_cpu4_oflow_interrupt, (void*) NULL,
			(void*) NULL);
}


INT8U err;
void execution_task(void* pdata){

	alt_u64 t = 0;

	while(1){
		OSSemPend(execution_mbox, 0, &err);
		int done = 0;
		int first = 0;
		//barrier function
		while (done == 0) {
			altera_avalon_mutex_lock(mutex, 1);
				if(first == 0){
					cp->checkout[PHYSICAL_CORE_ID] = 1;
					first = 1;
				}
				if( cp->checkout[PHYSICAL_CORE_ID] == 1){
					cp->checkout[PHYSICAL_CORE_ID] = 0;
					done = 1;
				}
				cp->core_total_time[PHYSICAL_CORE_ID] = 0;
				cp->core_oflow_time[PHYSICAL_CORE_ID] = 0;
				cp->oflow_count[PHYSICAL_CORE_ID] = 0;
			altera_avalon_mutex_unlock(mutex);
		}

		fprint_set_block_size(fprint_blocksize);
		int args[3] = {task_id,task_length,fprint_enable};

		FprintActive = 1;
		FprintTaskIDCurrent = task_id;
		t_oflow = 0;
		alt_timestamp_start();
		t = alt_timestamp();
			long registers[8];
			context_switch(registers);
				task_pt((void*)&args);
			context_restore(registers);
		t = alt_timestamp() - t;

		altera_avalon_mutex_lock(mutex, 1);
			cp->core_total_time[PHYSICAL_CORE_ID] = t;
		altera_avalon_mutex_unlock(mutex);
		FprintActive = 0;
	}
}

int main(void) {

	mutex = altera_avalon_mutex_open(MUTEX_0_NAME);
	execution_mbox = OSSemCreate(0);

	while(cp->init_complete == 0);

	altera_avalon_mutex_lock(mutex, 1);
		task_pt = cp->task_pt;
	altera_avalon_mutex_unlock(mutex);

	init_cpu4_isr();
	init_cpu4_oflow_isr();

	int arg_1 = EXECUTION_TASK_PRIORITY;
		OSTaskCreateExt(execution_task, (void*) &arg_1, (void *) &execution_task_stk[TASK_STACKSIZE - 1],
				EXECUTION_TASK_PRIORITY, EXECUTION_TASK_PRIORITY, execution_task_stk, TASK_STACKSIZE, NULL,0);

	altera_avalon_mutex_lock(mutex, 1);
		cp->core_ready[PHYSICAL_CORE_ID] = 1;
	altera_avalon_mutex_unlock(mutex);

	OSStart();

	return 0;
}
