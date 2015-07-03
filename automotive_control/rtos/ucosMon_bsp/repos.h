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

typedef enum task_status {
	PENDING,
	RUNNING,
	FAILED,
	SUCCEEDED
} task_status;

typedef enum task_kind {
	PERIODIC_K,
	EVENT_DRIVEN_K
} task_kind;


struct REPOS_task {
	task_status status;
	task_kind kind;
	INT32U core[2];

	void *dataAddressPhys;
	void *dataAddressSP[2];
	int dataSize;
	void *stackAddressPhys[2];
	void *stackAddressVirt[2];
	void *stackAddressSP[2];
	int stackSize;
	int numFuncs;
	int funcTableFirstIndex;
	INT32U runtime;
	INT32U deadline;
	INT32U taskID;
	INT32U fprintIDMask;
	INT32U fprintID;

	struct REPOS_task *next;
	union{
		struct{
			INT32U period;
			INT32U rest_time;
		}periodic;
		struct{
			INT32U flag;
		}event_driven;
	} data;
};


bool dmaReady[2];

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
	INT32U scratchpadRunning;
	INT32U scratchpadTask[2][2]; /* only two bins, each task consumes two pages (1 data one stack) */
	bool scratchpadValid[2][2]; /* what if it hasn't been used yet? */
	INT32U scratchpadLastUsed[2][2];
};
REPOS_core REPOSCoreTable[NUMCORES];


void REPOSUpdateTime(void);
void REPOSInit(void);
void REPOS_sendDMA(INT32U message);
void REPOSgetScratchpadPage(int coreID,REPOS_task *task);
void REPOStaskComplete(int taskID);

#define BOTH_CORES_LOADED 				0
#define CORE0_REQUIRES_TRANSFER 		1
#define CORE1_REQUIRES_TRANSFER 		2
#define BOTH_CORES_REQUIRE_TRANSFER 	3

bool REPOSAlreadyInScratchpad(REPOS_task *task, INT8U core);
int REPOSgetFreeFprintID(REPOS_task *task);
int REPOSgetTaskID(int fprintIDMask);
#endif /* REPOS_H_ */
