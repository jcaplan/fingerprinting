#include "repos.h"
#include "includes.h"
#include "sys/alt_dma.h"
#include <stdio.h>
#include <string.h>

#ifndef NULL
#define NULL ((void *)0)
#endif

alt_dma_txchan txchan[NUMCORES];
alt_dma_rxchan rxchan[NUMCORES];


void REPOSUpdateTime(void){

	REPOS_task *task = firstTask;

	while(task != NULL){
		if(task->kind == PERIODIC_K){
			if(task->status == RUNNING){
				task->runtime++;
				//TODO: Check if overrun
			} else { /* task is not running */
				/* then it has rested for another millisecond */
				task->data.periodic.rest_time++;
				//TODO: Check if ready to go
			}
		}
		task = task->next;
	}
}


void REPOSInit(void){

	memset(REPOSCoreTable,0,NUMCORES*sizeof(REPOS_core));
	memset(REPOSTaskTable,0,OS_MAX_TASKS*sizeof(REPOS_task));

	REPOS_task *task = &REPOSTaskTable[0];
	firstTask = task;
	task->status = PENDING;
	task->kind = EVENT_DRIVEN_K; /* driven by task completion on monitor core */
	task->core[0] = 0;
	task->core[1] = 1;


	initDMA();
}


void send_dma(void){

}

void REPOS_sendDMA(INT32U message){
//    OS_CPU_SR  cpu_sr = 0;
//	OS_ENTER_CRITICAL();
//	switch(message){
//	case SEND_DERIVATIVE_AIRBAGMODEL:
//
//		//set the appropriate bits in the DMA message flag
//		dma bits |= derivative stack | derivative data
//		//call send DMA
//		send_dma();
//		//send DMA will work its way through the bits for each core
//		//when the DMA done happens, it will always call send dma.
//		//this continues until there are no bits left.
//
//	default:
//		break;
//	}
//	OS_EXIT_CRITICAL();
}
