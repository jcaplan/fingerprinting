#include "repos.h"
#include "test_includes.h"
#include "stdio.h"
#include <assert.h>

#include <string.h>
#include <stdlib.h>
#include <time.h>

int testTime;

void checkoutTask(int taskID) {
	int i;
	REPOS_task *task = &REPOSTaskTable[taskID];

	for (i = 0; i < 2; i++) {
		int coreID = task->core[i];

		REPOSCheckPreemption(coreID);


		if (!REPOSAlreadyInScratchpad(task, coreID)) {
			REPOSgetScratchpadPage(coreID, task);
			printf("core %d retrieves SP pages (%p,%p) for task %lu\n", coreID,
					task->dataAddressSP[coreID], task->stackAddressSP[coreID],
					(unsigned long) task->taskID);
		} else {
			printf("task %d already in sp\n", taskID);
		}
		/* send dma */

	}

	REPOSgetFreeFprintID(task);
	REPOSBeginTask(task);
}

void checkinTask(int i) {
	REPOSTaskComplete(i);
}

void checkCoreRunning(int coreID) {
	REPOS_core *core = &REPOSCoreTable[coreID];
	assert(core->coreRunningCriticalTask);
	printf("core %d is running task %d\n", coreID,
			(int) REPOSCoreTable[0].currentTaskID);

}

void initializeTaskTable(void) {
	REPOS_task *task = &REPOSTaskTable[DERIVATIVE_AIRBAGMODEL_INDEX];

	task->dataAddressPhys = (void *) 0xcafebabe;

	task->stackAddressPhys[0] = (void *) (0x495000);
	task->stackAddressPhys[1] = (void *) (0x463000);

	task->stackAddressVirt[0] = (void *) (0x463000);
	task->stackAddressVirt[1] = (void *) (0x463000);

	task->dataSize = sizeof(void *);
	task->stackSize = 4096;
}

int rand_lim(int limit) {
/* return a random number between 0 and limit inclusive.
 */

    int divisor = RAND_MAX/(limit+1);
    int retval;

    do {
        retval = rand() / divisor;
    } while (retval > limit);

    return retval;
}

void startHook(void *args){
	printf("started task %d at time %d\n",(int )args,testTime);

	checkoutTask((int)args);
	checkinTask((int)args);
}

void REPOSInit(void) {

	memset(REPOSCoreTable, 0, NUMCORES * sizeof(REPOS_core));
	memset(REPOSTaskTable, 0, OS_MAX_TASKS * sizeof(REPOS_task));
	srand(time(NULL));
	int i;
	for (i = 0; i < OS_MAX_TASKS; i++) {

		REPOS_task *task = &REPOSTaskTable[i];
		task->status = PENDING;
		task->kind = PERIODIC_K; /* driven by task completion on monitor core */
		task->data.periodic.period = rand_lim(50)+1;
		task->core[0] = 0;
		task->core[1] = 1;
		task->numFuncs = 2; /* two functions run in the given task */
		task->funcTableFirstIndex = 0;
		task->taskID = i;
		task->startHook = startHook;
		task->startHookArgs = i;
	}

	fprintIDFreeList = 0xFFFF;
	int j;
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

	initializeTaskTable();
}

void checkPrintTable() {
	int i, j;
	for (i = 0; i < 2; i++) {
		for (j = 0; j < 4; j++) {
			printf("page_table[%d][%d]: %x\n", i, j,
					(unsigned int) pageTable[i][j]);
		}
	}
}

void checkNoScratchpadsActive() {
	int i, j;
	for (i = 0; i < 2; i++) {
		for (j = 0; j < 2; j++) {
			assert(!REPOSCoreTable[0].scratchpadActive[i][j]);
			assert(!REPOSCoreTable[1].scratchpadActive[i][j]);
		}
	}
	printf("***PASS: all scratchpads contain inactive tasks\n");
}

void test1(void){

	REPOSInit();
	checkPrintTable();
	/* do a DMA task */
	//first get scratchpad pages for each core
	int i;
	for (i = 0; i < 8; i++) {
		checkoutTask(i);
		checkCoreRunning(0);
		checkCoreRunning(1);
		checkinTask(i);
	}

	checkNoScratchpadsActive();

	checkoutTask(0);
	checkoutTask(1); /* 1 preempts 0 */
	checkoutTask(2);
	checkoutTask(3);
	checkinTask(3);
	checkinTask(2);
	checkoutTask(4);
	checkinTask(4);
	checkinTask(1);
	checkinTask(0);



	checkNoScratchpadsActive();

	for (i = 0; i < 5; i++) {
		checkoutTask(i);
		checkCoreRunning(0);
		checkCoreRunning(1);
	}
}

void printTaskPeriods(void){
	int i;
	for(i = 0; i < OS_MAX_TASKS; i++){
		printf("task %d, period %d\n",i,(int)REPOSTaskTable[i].data.periodic.period);
	}
}

void test2(void ){

	REPOSInit();
	printTaskPeriods();
	testTime = 0;
	int i;
	for(i = 0; i < 1000; i++){
		REPOSUpdateTime();
		testTime++;
	}


}
int main() {

//	test1();

	test2();


	return 0;
}
