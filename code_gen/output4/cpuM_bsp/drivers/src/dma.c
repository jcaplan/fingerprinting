/*
 * dma.c
 *
 *  Created on: 2015-07-15
 *      Author: jonah
 */

#include "dma.h"

#include <stdio.h>
/*****************************************************************************
 * DMA functions
 *****************************************************************************/
alt_dma_txchan *txchanP;
alt_dma_rxchan *rxchanP;
int **core_IRQp;
CriticalFunctionData *critFuncDataP;

void dma_setTxRxPointers(alt_dma_txchan *txp, alt_dma_rxchan *rxp){
	txchanP = txp;
	rxchanP = rxp;
}


void dma_setCoreIRQPointers(int **p){
	core_IRQp = p;
}


void dma_setCritFuncDataPointer(CriticalFunctionData *p){
	critFuncDataP = p;
}

void postDmaMessage(INT32U task, bool start, bool core_0_safe) {
	INT32U message = task;
	if (start) {
		message |= 1 << 31;
	}
	if(core_0_safe){
		message |= 1 << 30;
	}
	OSQPost(dmaQ, (void *) message);
}

void parseDmaMessage(INT32U message, bool *start, INT32U *task, bool *core_0_safe) {
	*start = message & (1 << 31);
	*task = message & (0x7FFFFFFF);
	*core_0_safe = message & (1 << 30);
}


void dma_TASK(void* pdata) {

	while (1) {
		INT8U perr;
		INT32U message = (INT32U) OSQPend(dmaQ, 0, &perr);
		bool start;
		INT32U taskID;
		bool core_0_safe;
		parseDmaMessage(message, &start, &taskID, &core_0_safe);

		// printf("dma task %lu\n", (unsigned long) taskID);

		REPOS_task *task = &REPOSTaskTable[taskID];

		//need to decide what to do with the task...

		//is it already located in the scratchpads??
		//for both cores
		int i, core;
		int numCores = (task->tmr ? 3 : 2);
		if (start) {
			for (i = 0; i < numCores; i++) {
				int core = task->core[i];

				REPOSCheckPreemption(core,taskID);


				bool alreadyInScratchpad = REPOSAlreadyInScratchpad(task, core);
				if (!alreadyInScratchpad) {

					/* does the code that's already there need to be copied back?? */


					REPOSgetScratchpadPage(core, i,task);
					//Once the scratchpad locations are known, it is possible to start the trans
					/* send to both cores */
					sendDMA(task->dataAddressPhys, task->dataAddressSP[i],
							task->dataSize, (void *) core);

					OSFlagPend(dmaReadyFlag, 1 << core, OS_FLAG_WAIT_SET_ALL, 0,
							&perr);

					sendDMA(task->stackAddressPhys[i],
							task->stackAddressSP[i], task->stackSize,
							(void *) core);
					OSFlagPend(dmaReadyFlag, 1 << core, OS_FLAG_WAIT_SET_ALL, 0,
							&perr);
				} else {
					sendDMA(task->dataAddressPhys, task->dataAddressSP[i],
							task->dataSize, (void *) core);

					OSFlagPend(dmaReadyFlag, 1 << core, OS_FLAG_WAIT_SET_ALL, 0,
							&perr);

				}
				printf("sent dma task %d core %d!\n",taskID,core);
			}
			REPOSgetFreeFprintID(task);
			REPOSBeginTask(task);
			/* now start the task */
			for (i = 0; i < numCores; i++) {
				core = task->core[i];
				critFuncDataP[core].fprintID = task->fprintID;
				if (critFuncDataP[core].fprintID < 0) {
					printf("big problem no free taskIDs!!!!!!!!!\n");
				}
				critFuncDataP[core].tableIndex = taskID;
				critFuncDataP[core].tlbDataAddressPhys =
						task->dataAddressSP[i];
				critFuncDataP[core].tlbDataAddressVirt = task->dataAddressVirt;
				critFuncDataP[core].tlbStackAddressPhys =
						task->stackAddressSP[i];
				/* both cores share the core 1 physical address */
				critFuncDataP[core].tlbStackAddressVirt =
						task->stackAddressVirt[i];
			}
			//TODO : check that cores have copied info !
			// while(....);

			//Notify both cores
			//-----------------
			*core_IRQp[task->core[0]] = 1;
			*core_IRQp[task->core[1]] = 1;
			if(numCores == 3){
				*core_IRQp[task->core[2]] = 1;
			}
			//wait until the message succeeds before returning to the top
		} else { /* retrieve from only core 0 */
			int safeCore = 0;
			if(task->tmr && !core_0_safe){
				safeCore = 1;
			}

			int core = task->core[safeCore];
			sendDMA(task->stackAddressSP[safeCore], task->stackAddressPhys[safeCore],
					task->stackSize, (void *) core);

			OSFlagPend(dmaReadyFlag, 1 << core, OS_FLAG_WAIT_SET_ALL, 0, &perr);
			printf("received from core %d!\n", core);
		}

	}

}

volatile int derivativeMsgCount = 0;

void handleDMA(void* handle, void* data) {

	int core = (int) handle;
	INT8U perr;
	if(!taskFailed)
		OSFlagPost(dmaReadyFlag, 1 << core, OS_FLAG_SET, &perr);
}

void sendDMA(void* sourceAddress, void* destAddress, int size, void *handle) {
	int rc;
	int core = (int) handle;
	
	 OS_CPU_SR  cpu_sr = 0u;
	if ((rc = alt_dma_txchan_send(txchanP[core], sourceAddress, size, NULL, NULL))
			< 0) {
		printf("dma failed!\n");
	}

	/* Post the receive request */
	if ((rc = alt_dma_rxchan_prepare(rxchanP[core], destAddress, size, handleDMA,
			handle)) < 0) {
		//Failure
	}
}
