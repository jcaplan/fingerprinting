/*****************************************************************************
 * Includes
 *****************************************************************************/
#include <stdio.h> //for debugging only
#include <stddef.h>
#include <stdbool.h>
#include "includes.h"
#include <altera_avalon_mutex.h>
#include "fingerprint.h"
#include "critical.h"
#include "AirbagModel.h"
#include "CruiseControlSystem.h"
#include "Derivative.h"
#include "TractionControl.h"
#include "shared_mem.h"
#include "gp.h"
#include "reset_monitor.h"
#include "repos.h"
#include "dma.h"


/*****************************************************************************
 * Defines
 *****************************************************************************/

//Nios IDE doesn't seem to recognize NULL
#define NULL ((void *)0)

void initializeTaskTable(void);
/*****************************************************************************
 * Stack sizes
 *
 * The stack size is composed of three elements A + B + C:
 * A: Amount of room that function code requires to execute on stack
 *
 * B: empirically determined value for amount of stack used with function
 * body consisting of empty while loop
 *
 * C: Safety margin
 *
 * Note that OS_STK allocates 32 bit words so these numbers represent words
 *****************************************************************************/

#define STACKSIZE_MINOFFSET   				314
#define STACKSIZE_MARGINERROR 				15
#define CruiseControlSystem_STACKSIZE 		(1500 + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR)
#define DMA_STACKSIZE 						(1500 + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR)

/*****************************************************************************
 * Task Priorities
 *****************************************************************************/
#define CruiseControlSystem_PRIORITY 		13
#define DMA_PRIORITY 						12

/*****************************************************************************
 * Task control flow conditions
 *****************************************************************************/

#define NUM_CRITICAL_TASKS 						2

bool coresReady = false;
bool taskFailed = false;
int failedTaskID = 0;
/*****************************************************************************
 * The global variable declarations for each critical task
 *****************************************************************************/

/*****************************************************************************
 * AirbagModel
 * Since AirbagModel is executed with fingerprinting it is more convenient
 * to package all the data structures in a single struct for DMA
 *****************************************************************************/

//Instance declared in DMA struct since this task is executed on another core
/*****************************************************************************
 * CruiseControlSystem
 *****************************************************************************/
RT_MODEL_CruiseControlSystem_T CruiseControlSystem_M;
ExtU_CruiseControlSystem_T CruiseControlSystem_U;
ExtY_CruiseControlSystem_T CruiseControlSystem_Y;
P_CruiseControlSystem_T cruiseControlSystem_defaultParam;
DW_CruiseControlSystem_T cruiseControlSystem_dwork;
/*****************************************************************************
 * Derivative
 *****************************************************************************/

//Instance declared in DMA struct since this task is executed on another core
/*****************************************************************************
 * TractionControl
 *****************************************************************************/
RT_MODEL_TractionControl_T TractionControl_M;
ExtU_TractionControl_T TractionControl_U;
ExtY_TractionControl_T TractionControl_Y;
P_TractionControl_T tractionControl_defaultParam;

/*****************************************************************************
 * Stack Declarations
 * Only tasks executing on this core require stacks
 *****************************************************************************/
OS_STK CruiseControlSystem_STACK[CruiseControlSystem_STACKSIZE] __attribute__ ((section (".critical")));
OS_STK DMA_STACK[DMA_STACKSIZE] __attribute__ ((section (".critical")));

/*****************************************************************************
 * Control Flow declarations
 *****************************************************************************/


OS_EVENT *dmaQ;
OS_FLAG_GRP *dmaReadyFlag;
#define DMA_Q_SIZE 12
INT32U dmaQMem[DMA_Q_SIZE];



typedef struct {
	AirbagModelStruct AirbagModel_STRUCT;
	DerivativeStruct Derivative_STRUCT;
	P_Derivative_T Derivative_P;
	DW_Derivative_T Derivative_DW;
	P_AirbagModel_T AirbagModel_P;
	DW_AirbagModel_T AirbagModel_DW;
} DMAPackageStruct;

DMAPackageStruct dmaPackageStruct_0 __attribute__ ((section (".global_data")));

/*****************************************************************************
 * Pointers to interrupt other cores
 *****************************************************************************/

int *core_IRQ[NUMCORES] = { (int *) PROCESSOR0_0_CPU_IRQ_0_BASE,
		(int *) PROCESSOR1_0_CPU_IRQ_0_BASE };




/*****************************************************************************
 * Hardware mutex
 *****************************************************************************/
alt_mutex_dev* mutex;

/*****************************************************************************
 * Function table for critical tasks
 *****************************************************************************/
SharedMemorySymbolTable shared_stab __attribute__ ((section (".shared")));
FunctionTable functionTable[NUM_CRITICAL_TASKS] __attribute__ ((section (".shared")));
CriticalFunctionData critFuncData[NUMCORES] __attribute__ ((section (".shared")));



/*****************************************************************************
 * REPOS configuration functions
 *****************************************************************************/



void initializeTaskTable(void) {
	REPOS_task *task = &REPOSTaskTable[DERIVATIVE_AIRBAGMODEL_INDEX];

	task->dataAddressPhys = &dmaPackageStruct_0;

	task->stackAddressPhys[0] = (void *) (0x495000);
	task->stackAddressPhys[1] = (void *) (0x463000);

	task->stackAddressVirt[0] = (void *) (0x463000);
	task->stackAddressVirt[1] = (void *) (0x463000);

	task->dataSize = sizeof(DMAPackageStruct);
	task->stackSize = 4096;
}

void REPOSInit(void) {

	memset(REPOSCoreTable, 0, NUMCORES * sizeof(REPOS_core));
	memset(REPOSTaskTable, 0, OS_MAX_TASKS * sizeof(REPOS_task));

	int i;
	for (i = 0; i < OS_MAX_TASKS; i++) {

		REPOS_task *task = &REPOSTaskTable[i];
		task->status = PENDING;
		task->kind = EVENT_DRIVEN_K; /* driven by task completion on monitor core */
		task->core[0] = 0;
		task->core[1] = 1;
		task->numFuncs = 2; /* two functions run in the given task */
		task->funcTableFirstIndex = 0;
		task->taskID = i;

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


/*****************************************************************************
 * CruiseControlSystem Task wrapper
 *****************************************************************************/
void CruiseControlSystem_TASK(void* pdata) {
	while (1) {
		printf("doing cruise control\n");
		CruiseControlSystem_step(&CruiseControlSystem_M, &CruiseControlSystem_U,
				&CruiseControlSystem_Y);

		//---------------------------------
		dmaPackageStruct_0.Derivative_STRUCT.Derivative_U.In1 =
				CruiseControlSystem_Y.Out1;

		//CruiseControlSystem -> TractionControl
		//--------------------------------------
		TractionControl_U.CurrentSpeedPoweredAxle = CruiseControlSystem_Y.Out1;

		//Derivative,Airbag -> Core 0
		//Derivative,Airbag -> Core 1
		//--------------------
		postDmaMessage(DERIVATIVE_AIRBAGMODEL_INDEX, true);

		TractionControl_step(&TractionControl_M, &TractionControl_U,
				&TractionControl_Y);

		CruiseControlSystem_U.In2 = TractionControl_Y.ControlFeedback;

		printf("did traction control\n");

		OSTimeDlyHMSM(0, 0, 0, 10);
	}
}

int result = 0;

/*****************************************************************************
 * Comparator ISR
 *****************************************************************************/
void resetCores(void) {
	OSTaskDel(DMA_PRIORITY);
	int* cpu0_reset = (int*) PROCESSOR0_0_SW_RESET_0_BASE;
	int* cpu1_reset = (int*) PROCESSOR1_0_SW_RESET_0_BASE;
	*cpu0_reset = 1;
	*cpu1_reset = 1;
	coresReady = false;
	taskFailed = true;
	resetMonitorEnable();
}

static void handleCompISR(void* context) {
	Fprint_Status status;
	fprint_status(&status);

	//Assume static mapping of fingeprint tasks for now
	//There is only one possible task that can set this off in this example
	//---------------------------------------------------------------------
	if (status.failed_reg) {
		int i;
		for (i = 0; i < 16; i++) {
			INT32U mask;
			if (status.failed_reg & (mask = 1 << i)) {
				/* assume only one failure possible */
				failedTaskID = REPOSgetTaskID(mask);
				fprint_get_task_count(i); /* make sure the counter resets */
				break;
			}
		}
		resetCores();
		REPOSInit(); /* ORDER MATTERS */
	}

	if ((result = status.successful_reg)) {

		//figure out what task is complete
		int i;
		int taskID = -1;
		for (i = 0; i < 16; i++) {
			INT32U mask;
			if (result & (mask = 1 << i)) {
				taskID = REPOSgetTaskID(mask);
				int numFuncs = REPOSTaskTable[taskID].numFuncs;
				/* Here we check that all the functions inside the task have executed,
				 * then we check that the task did not fail (since the same FID may have been used twice
				 * before the handler responds if the functions are tiny).
				 */
				if (((REPOSTaskTable[taskID].funcCompleteCount += fprint_get_task_count(i)) == numFuncs)
						&& (!taskFailed
								|| (taskFailed && taskID != failedTaskID))) { /*function can be decomposed into several chunks, so could be both cases */
					REPOSTaskTable[taskID].funcCompleteCount = 0;
					REPOSTaskComplete(taskID);
					postDmaMessage(taskID, false);
				}
			}
		}
	}

	fprint_reset_irq();
}

static void initCompIsr(void) {
	alt_ic_isr_register(CFPU_0_CSR_IRQ_INTERRUPT_CONTROLLER_ID, CFPU_0_CSR_IRQ,
			handleCompISR, (void*) NULL, (void*) NULL);
}



/*****************************************************************************
 * Reset monitor interface
 *****************************************************************************/

static void handleResetMonitor(void* context) {
	resetMonitorDisable();
	coresReady = true;
	if (taskFailed) {
		taskFailed = false;

		postDmaMessage(failedTaskID, true);

//		OSTaskResume(DMA_PRIORITY);
		OSTaskCreateExt(dma_TASK, NULL, &DMA_STACK[DMA_STACKSIZE - 1], DMA_PRIORITY,
				DMA_PRIORITY, DMA_STACK, DMA_STACKSIZE, NULL, OS_TASK_OPT_NONE);

	}
}

static void initResetMonitorIsr(void) {
	alt_ic_isr_register(
			PROCESSORM_0_RESET_MONITOR_0_IRQ_INTERRUPT_CONTROLLER_ID,
			PROCESSORM_0_RESET_MONITOR_0_IRQ, handleResetMonitor, (void*) NULL,
			(void*) NULL);
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
	functionTable[DERIVATIVE_FUNC_TABLE_INDEX].address = Derivative_CT;
	functionTable[DERIVATIVE_FUNC_TABLE_INDEX].args =
			&dmaPackageStruct_0.Derivative_STRUCT;
	functionTable[DERIVATIVE_FUNC_TABLE_INDEX].blocksize = 0xfff;
	functionTable[AIRBAGMODEL_FUNC_TABLE_INDEX].address = AirbagModel_CT;
	functionTable[AIRBAGMODEL_FUNC_TABLE_INDEX].args =
			&dmaPackageStruct_0.AirbagModel_STRUCT;
	functionTable[AIRBAGMODEL_FUNC_TABLE_INDEX].blocksize = 0xfff;

	//Initialize the runtime interface
	REPOSInit(); /* ORDER MATTERS */

	// Initialize the reset monitor
	resetMonitorMonReg(3);
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
	Directory_Init_Struct d;
	int i, j;
	for (i = 0; i < 2; i++) {
		d.core_id = i;
		d.key = 0;
		d.start_ptr = 0;
		d.end_ptr = 255;
		set_task_directory(&d);

		d.key = 1;
		d.start_ptr = 256;
		d.end_ptr = 511;
		set_task_directory(&d);
	}

	Core_Assignment_Table ca;
	//Default table
	for (i = 0; i < CA_TABLE_MAX_REDUNDANCY; i++) {
		for (j = 0; j < CA_TABLE_NUM_TASKS; j++) {
			ca.table[i][j] = i;
		}
	}
	ca.table[0][0] = 0;
	ca.table[1][0] = 1;
	set_core_assignment_table(&ca);

	initCompIsr();

	//Initialize the DMA hardware
	//---------------------------
	initDMA();

	//Initialize the Matlab tasks
	//---------------------------
	CruiseControlSystem_M.ModelData.defaultParam =
			&cruiseControlSystem_defaultParam;
	CruiseControlSystem_M.ModelData.dwork = &cruiseControlSystem_dwork;
	CruiseControlSystem_initialize(&CruiseControlSystem_M,
			&CruiseControlSystem_U, &CruiseControlSystem_Y);

	TractionControl_M.ModelData.defaultParam = &tractionControl_defaultParam;
	TractionControl_initialize(&TractionControl_M, &TractionControl_U,
			&TractionControl_Y);

	AirbagModelStruct *airbagModelStruct_0 =
			&dmaPackageStruct_0.AirbagModel_STRUCT;
	airbagModelStruct_0->AirbagModel_M.ModelData.defaultParam =
			&dmaPackageStruct_0.AirbagModel_P;
	airbagModelStruct_0->AirbagModel_M.ModelData.dwork =
			&dmaPackageStruct_0.AirbagModel_DW;
	AirbagModel_initialize(&airbagModelStruct_0->AirbagModel_M,
			&airbagModelStruct_0->AirbagModel_U,
			&airbagModelStruct_0->AirbagModel_Y);

	DerivativeStruct *derivativeStruct_0 =
			&dmaPackageStruct_0.Derivative_STRUCT;
	derivativeStruct_0->Derivative_M.ModelData.defaultParam =
			&dmaPackageStruct_0.Derivative_P;
	derivativeStruct_0->Derivative_M.ModelData.dwork =
			&dmaPackageStruct_0.Derivative_DW;
	Derivative_initialize(&derivativeStruct_0->Derivative_M,
			&derivativeStruct_0->Derivative_U,
			&derivativeStruct_0->Derivative_Y);

	//Initialize the control flow data structures
	//-------------------------------------------
	INT8U perr;
	dmaReadyFlag = OSFlagCreate(0, &perr); /* 0x3 = both cores are ready */
	dmaQ = OSQCreate((void**) dmaQMem, DMA_Q_SIZE);
	//Declare the OS tasks
	///-------------------
	OSTaskCreateExt(CruiseControlSystem_TASK, NULL,
			&CruiseControlSystem_STACK[CruiseControlSystem_STACKSIZE - 1],
			CruiseControlSystem_PRIORITY, CruiseControlSystem_PRIORITY,
			CruiseControlSystem_STACK, CruiseControlSystem_STACKSIZE, NULL,
			OS_TASK_OPT_STK_CLR);

	OSTaskCreateExt(dma_TASK, NULL, &DMA_STACK[DMA_STACKSIZE - 1], DMA_PRIORITY,
			DMA_PRIORITY, DMA_STACK, DMA_STACKSIZE, NULL, OS_TASK_OPT_STK_CLR);

	//Wait for confirmation that other cores have completed their startup routines
	//----------------------------------------------------------------------------
	while (!coresReady)
		;

	//Start the OS
	//------------
	OSStart();

	//Never get here
	//--------------
	return 0;
}
