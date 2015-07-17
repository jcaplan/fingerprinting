/**********************************
 * Includes
 **********************************/
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include "includes.h"
#include "dma.h"
#include <altera_avalon_mutex.h>
#include "shared_mem.h"
#include "fingerprint.h"
#include "gp.h"
#include "critical.h"
#include "cpuM.h"
#include "reset_monitor.h"
#include "repos.h"
#include "AirbagModel.h"
#include "CruiseControlSystem.h"
#include "Derivative.h"
#include "TractionControl.h"





/**********************************
 * Global variable declarations
 **********************************/
/* Derivative*/
static RT_MODEL_Derivative_T Derivative_M_;
static RT_MODEL_Derivative_T *const Derivative_M = &Derivative_M_;/* Real-time model */
static P_Derivative_T Derivative_P = {  0.0,                                 /* Mask Parameter: DiscreteDerivative_ICPrevScaled
                                        * Referenced by: '<S2>/UD'
                                        */
  5.0                                  /* Computed Parameter: TSamp_WtEt
                                        * Referenced by: '<S2>/TSamp'
                                        */
};                                     /* Modifiable parameters */
static DW_Derivative_T Derivative_DW;  /* Observable states */
static ExtU_Derivative_T Derivative_U; /* External inputs */
static ExtY_Derivative_T Derivative_Y; /* External outputs */

/* dma*/
typedef struct {
	AirbagModelStruct AirbagModel_STRUCT;
	P_AirbagModel_T AirbagModel_P;
	DW_AirbagModel_T AirbagModel_DW;
} DMA_AirbagModelPackageStruct;

DMA_AirbagModelPackageStruct AirbagModelPackageStruct __attribute__ ((section (".global_data")));



typedef struct {
	CruiseControlSystemStruct CruiseControlSystem_STRUCT;
	P_CruiseControlSystem_T CruiseControlSystem_P;
	DW_CruiseControlSystem_T CruiseControlSystem_DW;
} DMA_CruiseControlSystemPackageStruct;

DMA_CruiseControlSystemPackageStruct CruiseControlSystemPackageStruct __attribute__ ((section (".global_data")));



typedef struct {
	TractionControlStruct TractionControl_STRUCT;
	P_TractionControl_T TractionControl_P;
} DMA_TractionControlPackageStruct;

DMA_TractionControlPackageStruct TractionControlPackageStruct __attribute__ ((section (".global_data")));



OS_EVENT *dmaQ;
OS_FLAG_GRP *dmaReadyFlag;
#define DMA_Q_SIZE 12
INT32U dmaQMem[DMA_Q_SIZE];







/*****************************************************************************
 * Stack Declarations
 *****************************************************************************/
OS_STK dma_STACK[dma_STACKSIZE] __attribute__ ((section (".critical")));
OS_STK Derivative_STACK[Derivative_STACKSIZE] __attribute__ ((section (".critical")));





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
 * Task control flow conditions
 *****************************************************************************/

#define NUM_CRITICAL_TASKS 						2

bool coresReady = false;
bool taskFailed = false;
int failedTaskID = 0;






/*****************************************************************************
 * Shared memory interface with other cores
 *****************************************************************************/
SharedMemorySymbolTable shared_stab __attribute__ ((section (".shared")));
FunctionTable functionTable[3] __attribute__ ((section (".shared")));
CriticalFunctionData critFuncData[NUMCORES] __attribute__ ((section (".shared")));






/*****************************************************************************
 * Reset monitor interface
 *****************************************************************************/
void resetCores(void) {
	OSTaskDel(dma_PRIORITY);
	int* cpu0_reset = (int*) PROCESSOR0_0_SW_RESET_0_BASE;
	int* cpu1_reset = (int*) PROCESSOR1_0_SW_RESET_0_BASE;
	*cpu0_reset = 1;
	*cpu1_reset = 1;
	coresReady = false;
	taskFailed = true;
	resetMonitorEnable();
}
static void handleResetMonitor(void* context) {
	resetMonitorDisable();
	coresReady = true;
	if (taskFailed) {
		taskFailed = false;

		postDmaMessage(failedTaskID, true);

		OSTaskCreateExt(dma_TASK, NULL, &dma_STACK[dma_STACKSIZE - 1], dma_PRIORITY,
				dma_PRIORITY, dma_STACK, dma_STACKSIZE, NULL, OS_TASK_OPT_NONE);

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
			handleComp, (void*) NULL, (void*) NULL);
}





/*****************************************************************************
 * REPOS configuration functions
 *****************************************************************************/

 void startHook(void *args) {
	postDmaMessage((int)args,true);
}


void initializeTaskTable(void) {
	REPOS_task *task;

	task = &REPOSTaskTable[AIRBAGMODEL_TABLE_INDEX];

	task->dataAddressPhys = &AirbagModelPackageStruct;

	task->stackAddressPhys[0] = (void *) (0x4957cc);
	task->stackAddressPhys[1] = (void *) (0x4637cc);

	task->stackAddressVirt[0] = (void *) (0x4637cc);
	task->stackAddressVirt[1] = (void *) (0x4637cc);

	task->dataSize = sizeof(AirbagModelPackageStruct);
	task->stackSize = (AirbagModel_STACKSIZE * 4);

	task = &REPOSTaskTable[CRUISECONTROLSYSTEM_TABLE_INDEX];

	task->dataAddressPhys = &CruiseControlSystemPackageStruct;

	task->stackAddressPhys[0] = (void *) (0x495000);
	task->stackAddressPhys[1] = (void *) (0x463000);

	task->stackAddressVirt[0] = (void *) (0x463000);
	task->stackAddressVirt[1] = (void *) (0x463000);

	task->dataSize = sizeof(CruiseControlSystemPackageStruct);
	task->stackSize = (CruiseControlSystem_STACKSIZE * 4);

	task = &REPOSTaskTable[TRACTIONCONTROL_TABLE_INDEX];

	task->dataAddressPhys = &TractionControlPackageStruct;

	task->stackAddressPhys[0] = (void *) (0x494000);
	task->stackAddressPhys[1] = (void *) (0x462000);

	task->stackAddressVirt[0] = (void *) (0x462000);
	task->stackAddressVirt[1] = (void *) (0x462000);

	task->dataSize = sizeof(TractionControlPackageStruct);
	task->stackSize = (TractionControl_STACKSIZE * 4);

}

void REPOSInit(void) {

	memset(REPOSCoreTable, 0, NUMCORES * sizeof(REPOS_core));
	memset(REPOSTaskTable, 0, OS_MAX_TASKS * sizeof(REPOS_task));

	REPOS_task *task;
	task = &REPOSTaskTable[AIRBAGMODEL_TABLE_INDEX];
	task->status = PENDING;
	task->kind = PERIODIC_K;
	task->data.periodic.period = AIRBAGMODEL_PERIOD;
	task->core[0] = 0;
	task->core[1] = 1;
	task->numFuncs = 1;
	task->funcTableFirstIndex = 0;
	task->taskID = AIRBAGMODEL_TABLE_INDEX;
	task->startHook = startHook;
	task->startHookArgs = (void*)AIRBAGMODEL_TABLE_INDEX;

	task = &REPOSTaskTable[CRUISECONTROLSYSTEM_TABLE_INDEX];
	task->status = PENDING;
	task->kind = PERIODIC_K;
	task->data.periodic.period = CRUISECONTROLSYSTEM_PERIOD;
	task->core[0] = 0;
	task->core[1] = 1;
	task->numFuncs = 1;
	task->funcTableFirstIndex = 0;
	task->taskID = CRUISECONTROLSYSTEM_TABLE_INDEX;
	task->startHook = startHook;
	task->startHookArgs = (void*)CRUISECONTROLSYSTEM_TABLE_INDEX;

	task = &REPOSTaskTable[TRACTIONCONTROL_TABLE_INDEX];
	task->status = PENDING;
	task->kind = PERIODIC_K;
	task->data.periodic.period = TRACTIONCONTROL_PERIOD;
	task->core[0] = 0;
	task->core[1] = 1;
	task->numFuncs = 1;
	task->funcTableFirstIndex = 0;
	task->taskID = TRACTIONCONTROL_TABLE_INDEX;
	task->startHook = startHook;
	task->startHookArgs = (void*)TRACTIONCONTROL_TABLE_INDEX;


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
}






/*****************************************************************************
 * DerivativeTask wrapper
 *****************************************************************************/
void Derivative_TASK(void* pdata) {
	while (1) {
		Derivative_step(Derivative_M, &Derivative_U,
			&Derivative_Y);
		OSTimeDlyHMSM(0, 0, 0, 12);
	}
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
	functionTable[AIRBAGMODEL_TABLE_INDEX].address = AirbagModel_CT;
	functionTable[AIRBAGMODEL_TABLE_INDEX].args = &AirbagModelPackageStruct;
	functionTable[AIRBAGMODEL_TABLE_INDEX].blocksize = 0xfff;
	functionTable[CRUISECONTROLSYSTEM_TABLE_INDEX].address = CruiseControlSystem_CT;
	functionTable[CRUISECONTROLSYSTEM_TABLE_INDEX].args = &CruiseControlSystemPackageStruct;
	functionTable[CRUISECONTROLSYSTEM_TABLE_INDEX].blocksize = 0xfff;
	functionTable[TRACTIONCONTROL_TABLE_INDEX].address = TractionControl_CT;
	functionTable[TRACTIONCONTROL_TABLE_INDEX].args = &TractionControlPackageStruct;
	functionTable[TRACTIONCONTROL_TABLE_INDEX].blocksize = 0xfff;
	//Initialize the runtime interface
	REPOSInit();

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
	int i,j;

	d.core_id = 0;
	d.key = 0;
	d.start_ptr = 0;
	d.end_ptr = 169;
	set_task_directory(&d);

	d.core_id = 1;
	set_task_directory(&d);
	d.core_id = 0;
	d.key = 1;
	d.start_ptr = 170;
	d.end_ptr = 339;
	set_task_directory(&d);

	d.core_id = 1;
	set_task_directory(&d);
	d.core_id = 0;
	d.key = 2;
	d.start_ptr = 340;
	d.end_ptr = 509;
	set_task_directory(&d);

	d.core_id = 1;
	set_task_directory(&d);
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

	Derivative_M->ModelData.defaultParam = &Derivative_P;
	Derivative_M->ModelData.dwork = &Derivative_DW;
	Derivative_initialize(Derivative_M, &Derivative_U, &Derivative_Y);
	RT_MODEL_AirbagModel_T *AirbagModel_M =
			&AirbagModelPackageStruct.AirbagModel_STRUCT.AirbagModel_M;
	ExtU_AirbagModel_T *AirbagModel_U =
			&AirbagModelPackageStruct.AirbagModel_STRUCT.AirbagModel_U;
	ExtY_AirbagModel_T *AirbagModel_Y =
			&AirbagModelPackageStruct.AirbagModel_STRUCT.AirbagModel_Y;
	P_AirbagModel_T *AirbagModel_P =
			&AirbagModelPackageStruct.AirbagModel_P;
	DW_AirbagModel_T *AirbagModel_DW =
			&AirbagModelPackageStruct.AirbagModel_DW;
	AirbagModel_M->ModelData.defaultParam = AirbagModel_P;
	AirbagModel_M->ModelData.dwork = AirbagModel_DW;
	AirbagModel_initialize(AirbagModel_M, AirbagModel_U, AirbagModel_Y);

	RT_MODEL_CruiseControlSystem_T *CruiseControlSystem_M =
			&CruiseControlSystemPackageStruct.CruiseControlSystem_STRUCT.CruiseControlSystem_M;
	ExtU_CruiseControlSystem_T *CruiseControlSystem_U =
			&CruiseControlSystemPackageStruct.CruiseControlSystem_STRUCT.CruiseControlSystem_U;
	ExtY_CruiseControlSystem_T *CruiseControlSystem_Y =
			&CruiseControlSystemPackageStruct.CruiseControlSystem_STRUCT.CruiseControlSystem_Y;
	P_CruiseControlSystem_T *CruiseControlSystem_P =
			&CruiseControlSystemPackageStruct.CruiseControlSystem_P;
	DW_CruiseControlSystem_T *CruiseControlSystem_DW =
			&CruiseControlSystemPackageStruct.CruiseControlSystem_DW;
	CruiseControlSystem_M->ModelData.defaultParam = CruiseControlSystem_P;
	CruiseControlSystem_M->ModelData.dwork = CruiseControlSystem_DW;
	CruiseControlSystem_initialize(CruiseControlSystem_M, CruiseControlSystem_U,
	    CruiseControlSystem_Y);


	RT_MODEL_TractionControl_T *TractionControl_M =
			&TractionControlPackageStruct.TractionControl_STRUCT.TractionControl_M;
	ExtU_TractionControl_T *TractionControl_U =
			&TractionControlPackageStruct.TractionControl_STRUCT.TractionControl_U;
	ExtY_TractionControl_T *TractionControl_Y =
			&TractionControlPackageStruct.TractionControl_STRUCT.TractionControl_Y;
	P_TractionControl_T *TractionControl_P =
			&TractionControlPackageStruct.TractionControl_P;
	TractionControl_M->ModelData.defaultParam = TractionControl_P;
	TractionControl_initialize(TractionControl_M, TractionControl_U,
	    TractionControl_Y);


	//-------------------------------------------
	INT8U perr;
	dmaReadyFlag = OSFlagCreate(0, &perr); /* 0x3 = both cores are ready */
	dmaQ = OSQCreate((void**) dmaQMem, DMA_Q_SIZE);



	//Declare the OS tasks
	//-------------------
	OSTaskCreateExt(Derivative_TASK, NULL,
			&Derivative_STACK[Derivative_STACKSIZE - 1],
			Derivative_PRIORITY, Derivative_PRIORITY,
			Derivative_STACK, Derivative_STACKSIZE, NULL,
			OS_TASK_OPT_STK_CLR);
	OSTaskCreateExt(dma_TASK, NULL,
			&dma_STACK[dma_STACKSIZE - 1],
			dma_PRIORITY, dma_PRIORITY,
			dma_STACK, dma_STACKSIZE, NULL,
			OS_TASK_OPT_STK_CLR);


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

