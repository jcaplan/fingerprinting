/**********************************
 * Includes
 **********************************/
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <unistd.h>
#include "includes.h"
#include "dma.h"
#include <altera_avalon_dma.h>
#include <altera_avalon_mutex.h>
#include "shared_mem.h"
#include "fingerprint.h"
#include "gp.h"
#include "cpuM.h"
#include "reset_monitor.h"
#include "runtimeMonitor.h"
#include "repos.h"
#include "os/alt_syscall.h"
#include "critical.h"
#include "for_loop_100000_0.h"
#include "for_loop_90000_0.h"
#include "for_loop_80000_0.h"
#include "for_loop_70000_0.h"
#include "for_loop_60000_0.h"





/**********************************
 * Global variable declarations
 **********************************/
/* dma*/
typedef struct {
	for_loop_100000_0Struct for_loop_100000_0_STRUCT;
} DMA_for_loop_100000_0PackageStruct;

DMA_for_loop_100000_0PackageStruct for_loop_100000_0PackageStruct __attribute__ ((section (".global_data")));



typedef struct {
	for_loop_90000_0Struct for_loop_90000_0_STRUCT;
} DMA_for_loop_90000_0PackageStruct;

DMA_for_loop_90000_0PackageStruct for_loop_90000_0PackageStruct __attribute__ ((section (".global_data")));



typedef struct {
	for_loop_80000_0Struct for_loop_80000_0_STRUCT;
} DMA_for_loop_80000_0PackageStruct;

DMA_for_loop_80000_0PackageStruct for_loop_80000_0PackageStruct __attribute__ ((section (".global_data")));



typedef struct {
	for_loop_70000_0Struct for_loop_70000_0_STRUCT;
} DMA_for_loop_70000_0PackageStruct;

DMA_for_loop_70000_0PackageStruct for_loop_70000_0PackageStruct __attribute__ ((section (".global_data")));



typedef struct {
	for_loop_60000_0Struct for_loop_60000_0_STRUCT;
} DMA_for_loop_60000_0PackageStruct;

DMA_for_loop_60000_0PackageStruct for_loop_60000_0PackageStruct __attribute__ ((section (".global_data")));



OS_EVENT *dmaQ;
OS_FLAG_GRP *dmaReadyFlag;
#define DMA_Q_SIZE 12
INT32U dmaQMem[DMA_Q_SIZE];







/*****************************************************************************
 * Stack Declarations
 *****************************************************************************/
OS_STK dma_STACK[DMA_STACKSIZE] __attribute__ ((section (".critical")));





/*****************************************************************************
 * Pointers to interrupt other cores
 *****************************************************************************/

int *core_IRQ[4] = { 
	(int *) PROCESSOR0_0_CPU_IRQ_0_BASE,
	(int *) PROCESSOR1_0_CPU_IRQ_0_BASE,
	(int *) PROCESSOR2_0_CPU_IRQ_0_BASE,
	(int *) PROCESSOR3_0_CPU_IRQ_0_BASE,
};

/*****************************************************************************
 * Hardware mutex
 *****************************************************************************/
alt_mutex_dev* mutex;

/*****************************************************************************
 * Task control flow conditions
 *****************************************************************************/

#define NUM_CRITICAL_TASKS 						5

bool coresReady = false;
bool taskFailed = false;
int failedTaskID = 0;






/*****************************************************************************
 * Shared memory interface with other cores
 *****************************************************************************/
SharedMemorySymbolTable shared_stab __attribute__ ((section (".shared")));
FunctionTable functionTable[NUM_CRITICAL_TASKS] __attribute__ ((section (".shared")));
CriticalFunctionData critFuncData[4] __attribute__ ((section (".shared")));






/*****************************************************************************
 * Pointer relocation functions
 *****************************************************************************/
void for_loop_100000_0UpdatePointers(INT32U baseAddress, RT_MODEL_for_loop_100000_0_T *for_loop_100000_0_M){
}

void for_loop_90000_0UpdatePointers(INT32U baseAddress, RT_MODEL_for_loop_90000_0_T *for_loop_90000_0_M){
}

void for_loop_80000_0UpdatePointers(INT32U baseAddress, RT_MODEL_for_loop_80000_0_T *for_loop_80000_0_M){
}

void for_loop_70000_0UpdatePointers(INT32U baseAddress, RT_MODEL_for_loop_70000_0_T *for_loop_70000_0_M){
}

void for_loop_60000_0UpdatePointers(INT32U baseAddress, RT_MODEL_for_loop_60000_0_T *for_loop_60000_0_M){
}






/*****************************************************************************
 * DMA startup
 *****************************************************************************/
alt_dma_txchan txchan[4];
alt_dma_rxchan rxchan[4];
void initDMA(void) {
	txchan[0] = alt_dma_txchan_open("/dev/processor0_0_dma_0");
	rxchan[0] = alt_dma_rxchan_open("/dev/processor0_0_dma_0");

	txchan[1] = alt_dma_txchan_open("/dev/processor1_0_dma_0");
	rxchan[1] = alt_dma_rxchan_open("/dev/processor1_0_dma_0");

	txchan[2] = alt_dma_txchan_open("/dev/processor2_0_dma_0");
	rxchan[2] = alt_dma_rxchan_open("/dev/processor2_0_dma_0");

	txchan[3] = alt_dma_txchan_open("/dev/processor3_0_dma_0");
	rxchan[3] = alt_dma_rxchan_open("/dev/processor3_0_dma_0");

	dma_setTxRxPointers(txchan,rxchan);
	dma_setCoreIRQPointers(core_IRQ);
	dma_setCritFuncDataPointer(critFuncData);
}

//BROKEN!!!!!!!!!!!!
void resetDMA(){
	alt_avalon_dma_init (&txchan[0], &rxchan[0], (void*) PROCESSOR0_0_DMA_0_BASE,        
		PROCESSOR0_0_DMA_0_IRQ_INTERRUPT_CONTROLLER_ID, PROCESSOR0_0_DMA_0_IRQ);  
	alt_avalon_dma_init (&txchan[1], &rxchan[1], (void*) PROCESSOR1_0_DMA_0_BASE,        
		PROCESSOR1_0_DMA_0_IRQ_INTERRUPT_CONTROLLER_ID, PROCESSOR1_0_DMA_0_IRQ);  
	alt_avalon_dma_init (&txchan[2], &rxchan[2], (void*) PROCESSOR2_0_DMA_0_BASE,        
		PROCESSOR2_0_DMA_0_IRQ_INTERRUPT_CONTROLLER_ID, PROCESSOR2_0_DMA_0_IRQ);  
	alt_avalon_dma_init (&txchan[3], &rxchan[3], (void*) PROCESSOR3_0_DMA_0_BASE,        
		PROCESSOR3_0_DMA_0_IRQ_INTERRUPT_CONTROLLER_ID, PROCESSOR3_0_DMA_0_IRQ);  
}





/*****************************************************************************
 * Reset monitor interface
 *****************************************************************************/
void resetCores(void) {
	int* cpu0_reset = (int*) PROCESSOR0_0_SW_RESET_0_BASE;
	*cpu0_reset = 1;
	int* cpu1_reset = (int*) PROCESSOR1_0_SW_RESET_0_BASE;
	*cpu1_reset = 1;
	int* cpu2_reset = (int*) PROCESSOR2_0_SW_RESET_0_BASE;
	*cpu2_reset = 1;
	int* cpu3_reset = (int*) PROCESSOR3_0_SW_RESET_0_BASE;
	*cpu3_reset = 1;
	coresReady = false;
	taskFailed = true;
	resetMonitorEnable();
}
static void handleResetMonitor(void* context) {
	resetMonitorDisable();
	coresReady = true;
	if (taskFailed) {
		taskFailed = false;

		postDmaMessage(failedTaskID, true,false,true);

	}
}

static void initResetMonitorIsr(void) {
	alt_ic_isr_register(
			PROCESSORM_0_RESET_MONITOR_0_IRQ_INTERRUPT_CONTROLLER_ID,
			PROCESSORM_0_RESET_MONITOR_0_IRQ, handleResetMonitor, (void*) NULL,
			(void*) NULL);
}






/*****************************************************************************
 * Comparator ISR
 *****************************************************************************/

static void handleComp(void* context) {
	int result = 0;
	Fprint_Status status;
	comp_get_status(&status);

	//Assume static mapping of fingeprint tasks for now
	//There is only one possible task that can set this off in this example
	//---------------------------------------------------------------------
	if (status.failed_reg) {
		int i;
		for (i = 0; i < 16; i++) {
			INT32U mask;
			if (status.failed_reg & (mask = 1 << i)) {
				/* assume only one failure possible */
				failedTaskID = REPOSgetTaskID(i);
				REPOSTaskReset(failedTaskID);
				postDmaMessage(failedTaskID, true,false,true);
				break;
			}
		}
		// resetCores();
		// REPOSInit(); 
	}
	if ((result = status.successful_reg)) {

		//figure out what task is complete
		int i;
		int taskID = -1;
		for (i = 0; i < 32; i+=2) {
			INT32U mask;
			if (result & (mask = 3 << i)) {
				taskID = REPOSgetTaskID(i >> 1);
				/* Here we check that all the functions inside the task have executed,
				 * then we check that the task did not fail (since the same FID may have been used twice
				 * before the handler responds if the functions are tiny).
				 */
				if (!taskFailed || (taskFailed && taskID != failedTaskID)) { /*function can be decomposed into several chunks, so could be both cases */
					REPOSTaskTable[taskID].funcCompleteCount = 0;
					REPOSTaskComplete(taskID);
					int core0_safe = result & (1 << i);
					postDmaMessage(taskID, false, core0_safe,false);
				}
			}
		}
	}

	comp_reset_irq();
}

static void initCompIsr(void) {
	alt_ic_isr_register(CFPU_0_CSR_IRQ_INTERRUPT_CONTROLLER_ID, CFPU_0_CSR_IRQ,
			handleComp, (void*) NULL, (void*) NULL);
}





/*****************************************************************************
 * REPOS configuration functions
 *****************************************************************************/
REPOS_core REPOSCoreTable[4];

 void startHook(void *args) {
	postDmaMessage((int)args,true,false,false);
}


void initializeTaskTable(void) {
	REPOS_task *task;

	task = &REPOSTaskTable[FOR_LOOP_100000_0_TABLE_INDEX];

	task->dataAddressPhys = &for_loop_100000_0PackageStruct;
	task->dataAddressVirt = (void *)((int)&for_loop_100000_0PackageStruct & 0x3FFFFF);
	task->stackAddressPhys[0] = (void *) (0x495000);
	task->stackAddressVirt[0] = (void *) (0x32000);
	task->stackAddressPhys[1] = (void *) (0x463000);
	task->stackAddressVirt[1] = (void *) (0x32000);
	task->dataSize = sizeof(for_loop_100000_0PackageStruct);
	task->stackSize = (FOR_LOOP_100000_0_STACKSIZE * 4);

	task = &REPOSTaskTable[FOR_LOOP_90000_0_TABLE_INDEX];

	task->dataAddressPhys = &for_loop_90000_0PackageStruct;
	task->dataAddressVirt = (void *)((int)&for_loop_90000_0PackageStruct & 0x3FFFFF);
	task->stackAddressPhys[0] = (void *) (0x494000);
	task->stackAddressVirt[0] = (void *) (0x33000);
	task->stackAddressPhys[1] = (void *) (0x462000);
	task->stackAddressVirt[1] = (void *) (0x33000);
	task->dataSize = sizeof(for_loop_90000_0PackageStruct);
	task->stackSize = (FOR_LOOP_90000_0_STACKSIZE * 4);

	task = &REPOSTaskTable[FOR_LOOP_80000_0_TABLE_INDEX];

	task->dataAddressPhys = &for_loop_80000_0PackageStruct;
	task->dataAddressVirt = (void *)((int)&for_loop_80000_0PackageStruct & 0x3FFFFF);
	task->stackAddressPhys[0] = (void *) (0x493000);
	task->stackAddressVirt[0] = (void *) (0x34000);
	task->stackAddressPhys[1] = (void *) (0x4c7000);
	task->stackAddressVirt[1] = (void *) (0x34000);
	task->dataSize = sizeof(for_loop_80000_0PackageStruct);
	task->stackSize = (FOR_LOOP_80000_0_STACKSIZE * 4);

	task = &REPOSTaskTable[FOR_LOOP_70000_0_TABLE_INDEX];

	task->dataAddressPhys = &for_loop_70000_0PackageStruct;
	task->dataAddressVirt = (void *)((int)&for_loop_70000_0PackageStruct & 0x3FFFFF);
	task->stackAddressPhys[0] = (void *) (0x4c6000);
	task->stackAddressVirt[0] = (void *) (0x35000);
	task->stackAddressPhys[1] = (void *) (0x4f9000);
	task->stackAddressVirt[1] = (void *) (0x35000);
	task->dataSize = sizeof(for_loop_70000_0PackageStruct);
	task->stackSize = (FOR_LOOP_70000_0_STACKSIZE * 4);

	task = &REPOSTaskTable[FOR_LOOP_60000_0_TABLE_INDEX];

	task->dataAddressPhys = &for_loop_60000_0PackageStruct;
	task->dataAddressVirt = (void *)((int)&for_loop_60000_0PackageStruct & 0x3FFFFF);
	task->stackAddressPhys[0] = (void *) (0x4f8000);
	task->stackAddressVirt[0] = (void *) (0x36000);
	task->stackAddressPhys[1] = (void *) (0x461000);
	task->stackAddressVirt[1] = (void *) (0x36000);
	task->dataSize = sizeof(for_loop_60000_0PackageStruct);
	task->stackSize = (FOR_LOOP_60000_0_STACKSIZE * 4);

}

void REPOSInit(void) {

	memset(REPOSCoreTable, 0, 4 * sizeof(REPOS_core));
	memset(REPOSTaskTable, 0, OS_MAX_TASKS * sizeof(REPOS_task));

	REPOS_task *task;
	task = &REPOSTaskTable[FOR_LOOP_100000_0_TABLE_INDEX];
	task->taskRunning = false;
	task->kind = PERIODIC_K;
	task->data.periodic.period = FOR_LOOP_100000_0_PERIOD;
	task->data.periodic.countdown = FOR_LOOP_100000_0_PERIOD;
	task->data.periodic.deadline = FOR_LOOP_100000_0_PERIOD; /* Deadline not specified, assume deadline = period */
	task->tmr = false;
	task->core[0] = 0;
	task->core[1] = 1;
	task->numFuncs = 1;
	task->funcTableFirstIndex = 0;
	task->taskID = FOR_LOOP_100000_0_TABLE_INDEX;
	task->startHook = startHook;
	task->startHookArgs = (void*)FOR_LOOP_100000_0_TABLE_INDEX;

	task = &REPOSTaskTable[FOR_LOOP_90000_0_TABLE_INDEX];
	task->taskRunning = false;
	task->kind = PERIODIC_K;
	task->data.periodic.period = FOR_LOOP_90000_0_PERIOD;
	task->data.periodic.countdown = FOR_LOOP_90000_0_PERIOD;
	task->data.periodic.deadline = FOR_LOOP_90000_0_PERIOD; /* Deadline not specified, assume deadline = period */
	task->tmr = false;
	task->core[0] = 0;
	task->core[1] = 1;
	task->numFuncs = 1;
	task->funcTableFirstIndex = 0;
	task->taskID = FOR_LOOP_90000_0_TABLE_INDEX;
	task->startHook = startHook;
	task->startHookArgs = (void*)FOR_LOOP_90000_0_TABLE_INDEX;

	task = &REPOSTaskTable[FOR_LOOP_80000_0_TABLE_INDEX];
	task->taskRunning = false;
	task->kind = PERIODIC_K;
	task->data.periodic.period = FOR_LOOP_80000_0_PERIOD;
	task->data.periodic.countdown = FOR_LOOP_80000_0_PERIOD;
	task->data.periodic.deadline = FOR_LOOP_80000_0_PERIOD; /* Deadline not specified, assume deadline = period */
	task->tmr = false;
	task->core[0] = 0;
	task->core[1] = 2;
	task->numFuncs = 1;
	task->funcTableFirstIndex = 0;
	task->taskID = FOR_LOOP_80000_0_TABLE_INDEX;
	task->startHook = startHook;
	task->startHookArgs = (void*)FOR_LOOP_80000_0_TABLE_INDEX;

	task = &REPOSTaskTable[FOR_LOOP_70000_0_TABLE_INDEX];
	task->taskRunning = false;
	task->kind = PERIODIC_K;
	task->data.periodic.period = FOR_LOOP_70000_0_PERIOD;
	task->data.periodic.countdown = FOR_LOOP_70000_0_PERIOD;
	task->data.periodic.deadline = FOR_LOOP_70000_0_PERIOD; /* Deadline not specified, assume deadline = period */
	task->tmr = false;
	task->core[0] = 2;
	task->core[1] = 3;
	task->numFuncs = 1;
	task->funcTableFirstIndex = 0;
	task->taskID = FOR_LOOP_70000_0_TABLE_INDEX;
	task->startHook = startHook;
	task->startHookArgs = (void*)FOR_LOOP_70000_0_TABLE_INDEX;

	task = &REPOSTaskTable[FOR_LOOP_60000_0_TABLE_INDEX];
	task->taskRunning = false;
	task->kind = PERIODIC_K;
	task->data.periodic.period = FOR_LOOP_60000_0_PERIOD;
	task->data.periodic.countdown = FOR_LOOP_60000_0_PERIOD;
	task->data.periodic.deadline = FOR_LOOP_60000_0_PERIOD; /* Deadline not specified, assume deadline = period */
	task->tmr = false;
	task->core[0] = 3;
	task->core[1] = 1;
	task->numFuncs = 1;
	task->funcTableFirstIndex = 0;
	task->taskID = FOR_LOOP_60000_0_TABLE_INDEX;
	task->startHook = startHook;
	task->startHookArgs = (void*)FOR_LOOP_60000_0_TABLE_INDEX;


	fprintIDFreeList = 0xFFFF;
	int i,j;
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
	REPOSCoreTableP = REPOSCoreTable;
}












/*****************************************************************************
 * Main entry point
 *****************************************************************************/
int main(void) {
	printf("starting monitor\n");

	//Initialize the hardware mutex
	//-----------------------------
	mutex = altera_avalon_mutex_open(MUTEX_0_NAME);

	//Initialize the shared memory symbol table
	//-----------------------------------------
	shared_stab.ftable = functionTable;
	shared_stab.critFuncData = critFuncData;

	//Initialize the Function Table
	//-----------------------------
	functionTable[FOR_LOOP_100000_0_TABLE_INDEX].args =  (void *)((int)&for_loop_100000_0PackageStruct & 0x3FFFFF);
	functionTable[FOR_LOOP_100000_0_TABLE_INDEX].blocksize = 0xfff;
	functionTable[FOR_LOOP_90000_0_TABLE_INDEX].args =  (void *)((int)&for_loop_90000_0PackageStruct & 0x3FFFFF);
	functionTable[FOR_LOOP_90000_0_TABLE_INDEX].blocksize = 0xfff;
	functionTable[FOR_LOOP_80000_0_TABLE_INDEX].args =  (void *)((int)&for_loop_80000_0PackageStruct & 0x3FFFFF);
	functionTable[FOR_LOOP_80000_0_TABLE_INDEX].blocksize = 0xfff;
	functionTable[FOR_LOOP_70000_0_TABLE_INDEX].args =  (void *)((int)&for_loop_70000_0PackageStruct & 0x3FFFFF);
	functionTable[FOR_LOOP_70000_0_TABLE_INDEX].blocksize = 0xfff;
	functionTable[FOR_LOOP_60000_0_TABLE_INDEX].args =  (void *)((int)&for_loop_60000_0PackageStruct & 0x3FFFFF);
	functionTable[FOR_LOOP_60000_0_TABLE_INDEX].blocksize = 0xfff;
	//Initialize the runtime interface
	REPOSInit();

	// Initialize the reset monitor
	resetMonitorMonReg(15);
	resetMonitorEnable();
	initResetMonitorIsr();

	//Pass information to processing cores and notify them to begin their startup
	//---------------------------------------------------------------------------
	altera_avalon_mutex_lock(mutex, 1);
	{
		//Synchronize clocks
		//Give other cores the global pointer offset
		//Notify them to continue startup
		//------------------------------------------
		OSTimeSet(0);
		shared_stab.gp_address = get_gp();
		shared_stab.init_complete = 1;
	}
	altera_avalon_mutex_unlock(mutex);

	//Set up the fingerprinting hardware
	//Only one task is being fingerprinted
	//Assign FID=0
	//------------------------------------
	int i,j;

	for (i = 0; i < CA_TABLE_NUM_TASKS; i++){
		comp_set_success_maxcount_value(i,1);
	}	Core_Assignment_Table ca;
	//Default table
	for (i = 0; i < CA_TABLE_MAX_REDUNDANCY; i++) {
		for (j = 0; j < CA_TABLE_NUM_TASKS; j++) {
			ca.table[i][j] = i;
		}
	}
	comp_set_core_assignment_table(&ca);
	for(i = 0; i < CA_TABLE_NUM_TASKS; i++){
		comp_set_nmr(i,0);
	}
	initCompIsr();

	//Initialize the DMA hardware
	//---------------------------
	initDMA();


	RT_MODEL_for_loop_100000_0_T *for_loop_100000_0_M =
			&for_loop_100000_0PackageStruct.for_loop_100000_0_STRUCT.for_loop_100000_0_M;
	ExtU_for_loop_100000_0_T *for_loop_100000_0_U =
			&for_loop_100000_0PackageStruct.for_loop_100000_0_STRUCT.for_loop_100000_0_U;
	ExtY_for_loop_100000_0_T *for_loop_100000_0_Y =
			&for_loop_100000_0PackageStruct.for_loop_100000_0_STRUCT.for_loop_100000_0_Y;
	for_loop_100000_0UpdatePointers((INT32U)&for_loop_100000_0PackageStruct, for_loop_100000_0_M);
	for_loop_100000_0_initialize(for_loop_100000_0_M);
	for_loop_100000_0UpdatePointers((INT32U)&for_loop_100000_0PackageStruct & 0x3FFFFF, for_loop_100000_0_M);

	RT_MODEL_for_loop_90000_0_T *for_loop_90000_0_M =
			&for_loop_90000_0PackageStruct.for_loop_90000_0_STRUCT.for_loop_90000_0_M;
	ExtU_for_loop_90000_0_T *for_loop_90000_0_U =
			&for_loop_90000_0PackageStruct.for_loop_90000_0_STRUCT.for_loop_90000_0_U;
	ExtY_for_loop_90000_0_T *for_loop_90000_0_Y =
			&for_loop_90000_0PackageStruct.for_loop_90000_0_STRUCT.for_loop_90000_0_Y;
	for_loop_90000_0UpdatePointers((INT32U)&for_loop_90000_0PackageStruct, for_loop_90000_0_M);
	for_loop_90000_0_initialize(for_loop_90000_0_M);
	for_loop_90000_0UpdatePointers((INT32U)&for_loop_90000_0PackageStruct & 0x3FFFFF, for_loop_90000_0_M);

	RT_MODEL_for_loop_80000_0_T *for_loop_80000_0_M =
			&for_loop_80000_0PackageStruct.for_loop_80000_0_STRUCT.for_loop_80000_0_M;
	ExtU_for_loop_80000_0_T *for_loop_80000_0_U =
			&for_loop_80000_0PackageStruct.for_loop_80000_0_STRUCT.for_loop_80000_0_U;
	ExtY_for_loop_80000_0_T *for_loop_80000_0_Y =
			&for_loop_80000_0PackageStruct.for_loop_80000_0_STRUCT.for_loop_80000_0_Y;
	for_loop_80000_0UpdatePointers((INT32U)&for_loop_80000_0PackageStruct, for_loop_80000_0_M);
	for_loop_80000_0_initialize(for_loop_80000_0_M);
	for_loop_80000_0UpdatePointers((INT32U)&for_loop_80000_0PackageStruct & 0x3FFFFF, for_loop_80000_0_M);

	RT_MODEL_for_loop_70000_0_T *for_loop_70000_0_M =
			&for_loop_70000_0PackageStruct.for_loop_70000_0_STRUCT.for_loop_70000_0_M;
	ExtU_for_loop_70000_0_T *for_loop_70000_0_U =
			&for_loop_70000_0PackageStruct.for_loop_70000_0_STRUCT.for_loop_70000_0_U;
	ExtY_for_loop_70000_0_T *for_loop_70000_0_Y =
			&for_loop_70000_0PackageStruct.for_loop_70000_0_STRUCT.for_loop_70000_0_Y;
	for_loop_70000_0UpdatePointers((INT32U)&for_loop_70000_0PackageStruct, for_loop_70000_0_M);
	for_loop_70000_0_initialize(for_loop_70000_0_M);
	for_loop_70000_0UpdatePointers((INT32U)&for_loop_70000_0PackageStruct & 0x3FFFFF, for_loop_70000_0_M);

	RT_MODEL_for_loop_60000_0_T *for_loop_60000_0_M =
			&for_loop_60000_0PackageStruct.for_loop_60000_0_STRUCT.for_loop_60000_0_M;
	ExtU_for_loop_60000_0_T *for_loop_60000_0_U =
			&for_loop_60000_0PackageStruct.for_loop_60000_0_STRUCT.for_loop_60000_0_U;
	ExtY_for_loop_60000_0_T *for_loop_60000_0_Y =
			&for_loop_60000_0PackageStruct.for_loop_60000_0_STRUCT.for_loop_60000_0_Y;
	for_loop_60000_0UpdatePointers((INT32U)&for_loop_60000_0PackageStruct, for_loop_60000_0_M);
	for_loop_60000_0_initialize(for_loop_60000_0_M);
	for_loop_60000_0UpdatePointers((INT32U)&for_loop_60000_0PackageStruct & 0x3FFFFF, for_loop_60000_0_M);

	//-------------------------------------------
	INT8U perr;
	dmaReadyFlag = OSFlagCreate(0, &perr); /* 0x3 = both cores are ready */
	dmaQ = OSQCreate((void**) dmaQMem, DMA_Q_SIZE);



	//Declare the OS tasks
	//-------------------
	OSTaskCreateExt(dma_TASK, NULL,
			&dma_STACK[DMA_STACKSIZE - 1],
			DMA_PRIORITY, DMA_PRIORITY,
			dma_STACK, DMA_STACKSIZE, NULL,
			OS_TASK_OPT_STK_CLR);
	OSTaskNameSet(DMA_PRIORITY, (INT8U *)"dma", &perr);


	//Wait for confirmation that other cores have completed their startup routines
	//----------------------------------------------------------------------------
	while (!coresReady)
		;

	//Let task wrappers warm up
	//-------------------------
	ALT_USLEEP(225000);

	//Start the OS
	//------------
	OSStart();

	//Never get here
	//--------------
	return 0;
}






