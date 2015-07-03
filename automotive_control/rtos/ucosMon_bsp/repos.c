#include "repos.h"
#include "includes.h"
#include "sys/alt_dma.h"
#include <stdio.h>
#include <string.h>

#ifndef NULL
#define NULL ((void *)0)
#endif


void REPOSUpdateTime(void) {

	REPOS_task *task = firstTask;

	while (task != NULL) {
		if (task->kind == PERIODIC_K) {
			if (task->status == RUNNING) {
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

void REPOSInit(void) {

	memset(REPOSCoreTable, 0, NUMCORES * sizeof(REPOS_core));
	memset(REPOSTaskTable, 0, OS_MAX_TASKS * sizeof(REPOS_task));

	REPOS_task *task = &REPOSTaskTable[0];
	firstTask = task;
	task->status = PENDING;
	task->kind = EVENT_DRIVEN_K; /* driven by task completion on monitor core */
	task->core[0] = 0;
	task->core[1] = 1;
	int i, j;
	for (i = 0; i < 2; i++) {
		for (j = 0; j < 4; j++) {
			pageTable[i][j] = 0x4200000 + ((i * 4 + j) << 12);
		}
	}
	/*
	 * pageTable
	 * = {
	 {
	 0x4200000,
	 0x4201000,
	 0x4202000,
	 0x4203000
	 },
	 {
	 0x4204000,
	 0x4205000,
	 0x4206000,
	 0x4207000
	 }
	 };
	 */
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
	int time = -1;
	for (i = 0; i < NUM_SCRATCHPADS; i++) {
		if (!core->coreRunningCriticalTask || !(core->scratchpadRunning == i)) {
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
				}
			}
			if(invalidLine){ /* if an invalid line was found the search is over */
				break;
			}

			if (bin < 0) {
				for (j = 0; j < NUM_SP_PARTITIONS; j++) {
					/* first use an invalid page */
					int newTime = 0;
					if ((newTime = core->scratchpadLastUsed[i][j]) > time) {
						sp = i;
						bin = j;
						time = newTime;
					}
				}

			}
		}

	}

	/* was a scratchpad found? */
	if(bin < 0) {
		printf("no scratchpad found!!!n");
		exit(1);
	}

	task->dataAddressSP[coreID] = (void *)pageTable[sp][bin];
	task->stackAddressSP[coreID] = (void *)pageTable[sp][bin+1];
	core->coreRunningCriticalTask=true;
	core->scratchpadTask[sp][bin] = task->priority;
	core->scratchpadTask[sp][bin+1] = task->priority;

}

bool REPOSAlreadyInScratchpad(REPOS_task *task, INT8U core) {
	//if numcores is greater than 2, check which two cores are necessary....

	int i;
	bool core_has_task = false;
	for (i = 0; i < 2; i++) {
		if ((REPOSCoreTable[core].scratchpadValid[0][i]
				&& REPOSCoreTable[core].scratchpadTask[0][i] == task->priority)
				|| (REPOSCoreTable[core].scratchpadValid[1][i]
						&& REPOSCoreTable[core].scratchpadTask[1][i]
								== task->priority)) {
			core_has_task = true;
		}
	}

	return core_has_task;
}

void REPOStaskComplete(int taskID){
	REPOS_task *task = &REPOSTaskTable[taskID];

	int i;
	for (i = 0; i < 2; i++){
		REPOS_core *core = &REPOSCoreTable[task->core[i]];
		core->coreRunningCriticalTask = false;
	}
}
