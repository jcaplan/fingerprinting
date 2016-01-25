/*
 * repos.h
 *
 *  Created on: 2015-06-20
 *      Author: jonah
 */



#ifndef REPOS_H_
#define REPOS_H_
#include <ucos_ii.h>
#include "shared_mem.h"
#include "system.h"
#include <stdbool.h>


#define DERIVATIVE_AIRBAGMODEL_INDEX 0
#define NUM_SCRATCHPADS 2
#define NUM_SP_PARTITIONS 2
typedef struct REPOS_task REPOS_task;
typedef struct REPOS_core REPOS_core;
/*
 * What are the things that need to happen?
 *
 * 1. Define data structs
 * 2. function definitions
 */

//List of all the pages
INT32U pageTable[2][4];

typedef enum task_kind {
	null,
	PERIODIC_K,
	EVENT_DRIVEN_K
} task_kind;


struct REPOS_task {
	bool taskRunning;
	task_kind kind;
	INT32U core[3];
	bool tmr;
	void *dataAddressPhys;
	void *dataAddressVirt;
	void *dataAddressSP[3];
	int dataSize;
	void *stackAddressPhys[3];
	void *stackAddressVirt[3];
	void *stackAddressSP[3];
	int stackSize;
	int numFuncs;
	int funcTableFirstIndex;
	int funcCompleteCount;
	INT32U runtime;
	INT32U taskID;
	INT32U fprintID;
	INT32U currentSP;
	INT32U currentSPbin;
	void (*startHook)(void *args);
	void *startHookArgs;
	union{
		struct{
			INT32U period;
			INT32U countdown;
			INT32U deadline;
		}periodic;
		struct{
			INT32U flag;
		}event_driven;
	} data;
};



/*
 * Task searching will used a linked list method
 * but max capacity for list is statically assigned
 */
REPOS_task REPOSTaskTable[OS_MAX_TASKS];
REPOS_task *firstTask;
INT16U fprintIDFreeList;

struct REPOS_core {
	bool   coreRunningCriticalTask;
	INT32U currentTaskID;
	INT32U currentScratchpad;
	INT32U scratchpadTask[2][2]; /* only two bins, each task consumes two pages (1 data one stack) */
	bool scratchpadValid[2][2]; /* what if it hasn't been used yet? */
	bool scratchpadActive[2][2];
	INT32U scratchpadLastUsed[2][2];
	INT32U preemptedTaskID[3];
	INT32U preemptedTaskSP[3];
	int preemptedIndex;

};
REPOS_core *REPOSCoreTableP;

extern void initializeTaskTable(void);
void REPOSUpdateTime(void);
extern void REPOSInit(void);
void REPOS_sendDMA(INT32U message);
void REPOSgetScratchpadPage(int coreID, int logicalCoreID, REPOS_task *task);
void REPOSTaskComplete(int taskID);
void REPOSTaskReset(int taskID);

#define BOTH_CORES_LOADED 				0
#define CORE0_REQUIRES_TRANSFER 		1
#define CORE1_REQUIRES_TRANSFER 		2
#define BOTH_CORES_REQUIRE_TRANSFER 	3

bool REPOSAlreadyInScratchpad(REPOS_task *task, INT8U core);
int REPOSgetFreeFprintID(REPOS_task *task);
int REPOSgetTaskID(int fprintID);
void REPOSBeginTask(REPOS_task *task);
void REPOSCheckPreemption(int coreID, int newTask);

#endif /* REPOS_H_ */
