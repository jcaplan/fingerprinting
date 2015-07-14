#include "repos.h"
#include "includes.h"
#include "sys/alt_dma.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef NULL
#define NULL ((void *)0)
#endif


void REPOSUpdateTime(void) {

	int i;
	for(i = 0; i < OS_MAX_TASKS; i++){
		REPOS_task *task = &REPOSTaskTable[i];
		if(task->kind == PERIODIC_K){
			if(task->data.periodic.countdown-- == 0){
				//reset the timer
				task->data.periodic.countdown = task->data.periodic.period-1;
				task->startHook(task->startHookArgs);
			}
		}
	}
}



void REPOSgetScratchpadPage(int coreID,REPOS_task *task) {
	// 1. Choose a scratchpad.
	// 	a. If a task is running then must be the other scratchpad
	//  b. Otherwise consider both scratchpads
	//  c. Use any invalid pages
	//  d. if all pages have been used , choose the least recently used page

	REPOS_core *core = &REPOSCoreTable[coreID];
	int bin = -1, sp = -1;
	int i;
	bool invalidLine = false;
	int priority = -1;
	for (i = 0; i < NUM_SCRATCHPADS; i++) {
		if (!core->coreRunningCriticalTask || !(core->currentScratchpad == i)) {
			/* check that the scratchpad does not contain an active critical task */
			/* get least recently used */

			int j;
			for (j = 0; j < NUM_SP_PARTITIONS; j++) {
				/* first use an invalid page */
				if (!core->scratchpadValid[i][j]) {
					bin = j;
					sp = i;
					invalidLine = true;
					core->scratchpadValid[i][j] = true;
					break;
				} else if(!core->scratchpadActive[i][j]){ /* otherwise find the lowest priority task (highest index) */
					int newPriority;
					if((newPriority = REPOSTaskTable[core->scratchpadTask[i][j]].taskID) > priority){
						sp = i;
						bin = j;
						priority = newPriority;
					}

				}
			}
			if(invalidLine){ /* if an invalid line was found the search is over */
				break;
			}
		}

	}

	/* was a scratchpad found? */
	if(bin < 0) {
		printf("no scratchpad found!!!\n");
		exit(EXIT_FAILURE);
	}



	int tableIndex = bin*2;
	task->dataAddressSP[coreID] = (void *)pageTable[sp][tableIndex];
	task->stackAddressSP[coreID] = (void *)pageTable[sp][tableIndex+1];
	core->scratchpadTask[sp][bin] = task->taskID;
	core->scratchpadActive[sp][bin] = true;
	core->currentScratchpad = sp;
	task->currentSP = sp;
	task->currentSPbin = bin;

}

bool REPOSAlreadyInScratchpad(REPOS_task *task, INT8U core) {
	//if numcores is greater than 2, check which two cores are necessary....

	int i;
	bool core_has_task = false;
	for (i = 0; i < 2; i++) {
		if ((REPOSCoreTable[core].scratchpadValid[0][i]
				&& REPOSCoreTable[core].scratchpadTask[0][i] == task->taskID)
				|| (REPOSCoreTable[core].scratchpadValid[1][i]
						&& REPOSCoreTable[core].scratchpadTask[1][i]
								== task->taskID)) {
			core_has_task = true;
		}
	}

	return core_has_task;
}

void REPOSTaskComplete(int taskID){
	REPOS_task *task = &REPOSTaskTable[taskID];

	int i;
	for (i = 0; i < 2; i++){
		REPOS_core *core = &REPOSCoreTable[task->core[i]];
		core->coreRunningCriticalTask = false;
		int j;
		for(j = 0; j < 2; j++){
			if(core->scratchpadTask[core->currentScratchpad][j] == taskID){
				core->scratchpadActive[core->currentScratchpad][j] = false;
			}
		}
		if(core->preemptedIndex > 0){

			core->preemptedIndex--;
			core->currentTaskID = core->preemptedTaskID[core->preemptedIndex];
			core->currentScratchpad = core->preemptedTaskSP[core->preemptedIndex];
			printf("resuming task %d\n",(int)core->currentTaskID);
			core->coreRunningCriticalTask = true;
		}
	}
	fprintIDFreeList |= 1 << taskID;
}

int REPOSgetFreeFprintID(REPOS_task *task) {
	int i;
	for(i = 0; i < 16; i++){
		INT32U mask = 0;
		if(fprintIDFreeList & (mask = 1 << i)){
			fprintIDFreeList &= !(mask);
			task->fprintIDMask = mask;
			task->fprintID = i;
			return i;
		}
	}
	return -1;
}

int REPOSgetTaskID(int fprintIDMask){
	int i;
	for(i = 0; i < OS_MAX_TASKS; i++){
		REPOS_task *task = &REPOSTaskTable[i];
		if(task->fprintIDMask == fprintIDMask){
			return task->taskID;
		}
	}
	return -1;
}

void REPOSBeginTask(REPOS_task *task){
	int i;
	for(i = 0; i < 2; i++){
		REPOS_core *core = &REPOSCoreTable[i];
		core->coreRunningCriticalTask=true;
		core->currentTaskID = task->taskID;
		core->currentScratchpad = task->currentSP;
		core->scratchpadActive[core->currentScratchpad][task->currentSPbin] = true;
	}
}

void REPOSCheckPreemption(int coreID) {
	REPOS_core *core = &REPOSCoreTable[coreID];
	if (core->coreRunningCriticalTask) { /* task is being preempted */
		core->coreRunningCriticalTask = false;
		core->preemptedTaskID[core->preemptedIndex] = core->currentTaskID;
		core->preemptedTaskSP[core->preemptedIndex] = core->currentScratchpad;
		core->preemptedIndex++;
		printf("task %d was preempted on core %d\n", (int) core->currentTaskID,
				coreID);
	}
}
