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


#define SEND_DERIVATIVE_AIRBAGMODEL 0
#define SEND_DERIVATIVE_AIRBAGMODEL_DATA 1
#define SEND_DERIVATIVE_AIRBAGMODEL_STACK 2


typedef struct REPOS_task REPOS_task;
typedef struct REPOS_core REPOS_core;
/*
 * What are the things that need to happen?
 *
 * 1. Define data structs
 * 2. function definitions
 */


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
	INT32U runtime;
	INT32U deadline;
	INT32U priority;
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

struct REPOS_core {
	INT32U taskRunning;
	INT32U scratchpadTask[2];
};
REPOS_core REPOSCoreTable[NUMCORES];


void REPOSUpdateTime(void);
void REPOSInit(void);
void REPOS_sendDMA(INT32U message);
//Flag collects a flag, plus a waitlist of tasks waiting on the flag
//task has conditions on which it is waiting...

//Interrupts/tasks/timers will post to REPOS ONLY IF there is no initial task on the monitor.
//Otherwise use the UCOS facilities to initiate the task on the monitor core.




#endif /* REPOS_H_ */
