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
#include "cpuM.h"
#include "reset_monitor.h"
#include "runtimeMonitor.h"
#include "repos.h"
#include "critical.h"
#include "Derivative.h"
#include "TractionControl.h"
#include "AirbagModel.h"
#include "CruiseControlSystem.h"
#include "RadarTracker.h"





/**********************************
 * Global variable declarations
 **********************************/
/* Derivative*/
static RT_MODEL_Derivative_T Derivative_M_;
static RT_MODEL_Derivative_T *const Derivative_M = &Derivative_M_;/* Real-time model */
static P_Derivative_T Derivative_P = {  0.0F,                                /* Mask Parameter: DiscreteDerivative_ICPrevScaled
                                        * Referenced by: '<S2>/UD'
                                        */
  5.0F                                 /* Computed Parameter: TSamp_WtEt
                                        * Referenced by: '<S2>/TSamp'
                                        */
};                                     /* Modifiable parameters */
static DW_Derivative_T Derivative_DW;  /* Observable states */
static ExtU_Derivative_T Derivative_U; /* External inputs */
static ExtY_Derivative_T Derivative_Y; /* External outputs */

/* TractionControl*/
static RT_MODEL_TractionControl_T TractionControl_M_;
static RT_MODEL_TractionControl_T *const TractionControl_M = &TractionControl_M_;/* Real-time model */
static P_TractionControl_T TractionControl_P = {  3.0F                                 /* Mask Parameter: CompareToConstant_const
                                        * Referenced by: '<S1>/Constant'
                                        */
};                                     /* Modifiable parameters */
static ExtU_TractionControl_T TractionControl_U;/* External inputs */
static ExtY_TractionControl_T TractionControl_Y;/* External outputs */

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
	RadarTrackerStruct RadarTracker_STRUCT;
	DW_RadarTracker_T RadarTracker_DW;
} DMA_RadarTrackerPackageStruct;

DMA_RadarTrackerPackageStruct RadarTrackerPackageStruct __attribute__ ((section (".global_data")));



OS_EVENT *dmaQ;
OS_FLAG_GRP *dmaReadyFlag;
#define DMA_Q_SIZE 12
INT32U dmaQMem[DMA_Q_SIZE];







/*****************************************************************************
 * Stack Declarations
 *****************************************************************************/
OS_STK dma_STACK[DMA_STACKSIZE] __attribute__ ((section (".critical")));
OS_STK Derivative_STACK[DERIVATIVE_STACKSIZE] __attribute__ ((section (".critical")));
OS_STK TractionControl_STACK[TRACTIONCONTROL_STACKSIZE] __attribute__ ((section (".critical")));





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

#define NUM_CRITICAL_TASKS 						3

bool coresReady = false;
bool taskFailed = false;
int failedTaskID = 0;






/*****************************************************************************
 * Shared memory interface with other cores
 *****************************************************************************/
SharedMemorySymbolTable shared_stab __attribute__ ((section (".shared")));
FunctionTable functionTable[NUM_CRITICAL_TASKS] __attribute__ ((section (".shared")));
CriticalFunctionData critFuncData[NUMCORES] __attribute__ ((section (".shared")));






/*****************************************************************************
 * Pointer relocation functions
 *****************************************************************************/
void AirbagModelUpdatePointers(INT32U baseAddress, RT_MODEL_AirbagModel_T *AirbagModel_M){
	AirbagModel_M->ModelData.defaultParam = (P_AirbagModel_T *)(baseAddress + sizeof(AirbagModelStruct));
	AirbagModel_M->ModelData.dwork = (DW_AirbagModel_T *)(baseAddress + sizeof(AirbagModelStruct) + sizeof(P_AirbagModel_T));
}

void CruiseControlSystemUpdatePointers(INT32U baseAddress, RT_MODEL_CruiseControlSystem_T *CruiseControlSystem_M){
	CruiseControlSystem_M->ModelData.defaultParam = (P_CruiseControlSystem_T *)(baseAddress + sizeof(CruiseControlSystemStruct));
	CruiseControlSystem_M->ModelData.dwork = (DW_CruiseControlSystem_T *)(baseAddress + sizeof(CruiseControlSystemStruct) + sizeof(P_CruiseControlSystem_T));
}

void RadarTrackerUpdatePointers(INT32U baseAddress, RT_MODEL_RadarTracker_T *RadarTracker_M){
	RadarTracker_M->ModelData.dwork = (DW_RadarTracker_T *)(baseAddress + sizeof(RadarTrackerStruct));
}






/*****************************************************************************
 * Reset monitor interface
 *****************************************************************************/
void resetCores(void) {
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
				failedTaskID = REPOSgetTaskID(mask);
				REPOSTaskReset(failedTaskID);				postDmaMessage(failedTaskID, true);
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
		for (i = 0; i < 16; i++) {
			INT32U mask;
			if (result & (mask = 1 << i)) {
				taskID = REPOSgetTaskID(mask);
				/* Here we check that all the functions inside the task have executed,
				 * then we check that the task did not fail (since the same FID may have been used twice
				 * before the handler responds if the functions are tiny).
				 */
				if (!taskFailed || (taskFailed && taskID != failedTaskID)) { /*function can be decomposed into several chunks, so could be both cases */
					REPOSTaskTable[taskID].funcCompleteCount = 0;
					REPOSTaskComplete(taskID);
					postDmaMessage(taskID, false);
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

 void startHook(void *args) {
	postDmaMessage((int)args,true);
}


void initializeTaskTable(void) {
	REPOS_task *task;

	task = &REPOSTaskTable[AIRBAGMODEL_TABLE_INDEX];

	task->dataAddressPhys = &AirbagModelPackageStruct;
	task->dataAddressVirt = (void *)((int)&AirbagModelPackageStruct & 0x3FFFFF);
	task->stackAddressPhys[0] = (void *) (0x494000);
	task->stackAddressPhys[1] = (void *) (0x462000);

	task->stackAddressVirt[0] = (void *) (0x62000);
	task->stackAddressVirt[1] = (void *) (0x62000);

	task->dataSize = sizeof(AirbagModelPackageStruct);
	task->stackSize = (AIRBAGMODEL_STACKSIZE * 4);

	task = &REPOSTaskTable[CRUISECONTROLSYSTEM_TABLE_INDEX];

	task->dataAddressPhys = &CruiseControlSystemPackageStruct;
	task->dataAddressVirt = (void *)((int)&CruiseControlSystemPackageStruct & 0x3FFFFF);
	task->stackAddressPhys[0] = (void *) (0x494768);
	task->stackAddressPhys[1] = (void *) (0x462768);

	task->stackAddressVirt[0] = (void *) (0x62768);
	task->stackAddressVirt[1] = (void *) (0x62768);

	task->dataSize = sizeof(CruiseControlSystemPackageStruct);
	task->stackSize = (CRUISECONTROLSYSTEM_STACKSIZE * 4);

	task = &REPOSTaskTable[RADARTRACKER_TABLE_INDEX];

	task->dataAddressPhys = &RadarTrackerPackageStruct;
	task->dataAddressVirt = (void *)((int)&RadarTrackerPackageStruct & 0x3FFFFF);
	task->stackAddressPhys[0] = (void *) (0x495000);
	task->stackAddressPhys[1] = (void *) (0x463000);

	task->stackAddressVirt[0] = (void *) (0x63000);
	task->stackAddressVirt[1] = (void *) (0x63000);

	task->dataSize = sizeof(RadarTrackerPackageStruct);
	task->stackSize = (RADARTRACKER_STACKSIZE * 4);

}

void REPOSInit(void) {

	memset(REPOSCoreTable, 0, NUMCORES * sizeof(REPOS_core));
	memset(REPOSTaskTable, 0, OS_MAX_TASKS * sizeof(REPOS_task));

	REPOS_task *task;
	task = &REPOSTaskTable[AIRBAGMODEL_TABLE_INDEX];
	task->taskRunning = false;
	task->kind = PERIODIC_K;
	task->data.periodic.period = AIRBAGMODEL_PERIOD;
	task->data.periodic.countdown = AIRBAGMODEL_PERIOD;
	task->data.periodic.deadline = AIRBAGMODEL_PERIOD; /* Deadline not specified, assume deadline = period */
	task->core[0] = 0;
	task->core[1] = 1;
	task->numFuncs = 1;
	task->funcTableFirstIndex = 0;
	task->taskID = AIRBAGMODEL_TABLE_INDEX;
	task->startHook = startHook;
	task->startHookArgs = (void*)AIRBAGMODEL_TABLE_INDEX;

	task = &REPOSTaskTable[CRUISECONTROLSYSTEM_TABLE_INDEX];
	task->taskRunning = false;
	task->kind = PERIODIC_K;
	task->data.periodic.period = CRUISECONTROLSYSTEM_PERIOD;
	task->data.periodic.countdown = CRUISECONTROLSYSTEM_PERIOD;
	task->data.periodic.deadline = CRUISECONTROLSYSTEM_PERIOD; /* Deadline not specified, assume deadline = period */
	task->core[0] = 0;
	task->core[1] = 1;
	task->numFuncs = 1;
	task->funcTableFirstIndex = 0;
	task->taskID = CRUISECONTROLSYSTEM_TABLE_INDEX;
	task->startHook = startHook;
	task->startHookArgs = (void*)CRUISECONTROLSYSTEM_TABLE_INDEX;

	task = &REPOSTaskTable[RADARTRACKER_TABLE_INDEX];
	task->taskRunning = false;
	task->kind = PERIODIC_K;
	task->data.periodic.period = RADARTRACKER_PERIOD;
	task->data.periodic.countdown = RADARTRACKER_PERIOD;
	task->data.periodic.deadline = RADARTRACKER_PERIOD; /* Deadline not specified, assume deadline = period */
	task->core[0] = 0;
	task->core[1] = 1;
	task->numFuncs = 1;
	task->funcTableFirstIndex = 0;
	task->taskID = RADARTRACKER_TABLE_INDEX;
	task->startHook = startHook;
	task->startHookArgs = (void*)RADARTRACKER_TABLE_INDEX;


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
		OSTimeDlyHMSM(0, 0, 0, DERIVATIVE_PERIOD);
	}
}

/*****************************************************************************
 * TractionControlTask wrapper
 *****************************************************************************/
void TractionControl_TASK(void* pdata) {
	while (1) {
		TractionControl_step(TractionControl_M, &TractionControl_U,
			&TractionControl_Y);
		OSTimeDlyHMSM(0, 0, 0, TRACTIONCONTROL_PERIOD);
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
	functionTable[AIRBAGMODEL_TABLE_INDEX].args =  (void *)((int)&AirbagModelPackageStruct & 0x3FFFFF);
	functionTable[AIRBAGMODEL_TABLE_INDEX].blocksize = 0xfff;
	functionTable[CRUISECONTROLSYSTEM_TABLE_INDEX].args =  (void *)((int)&CruiseControlSystemPackageStruct & 0x3FFFFF);
	functionTable[CRUISECONTROLSYSTEM_TABLE_INDEX].blocksize = 0xfff;
	functionTable[RADARTRACKER_TABLE_INDEX].args =  (void *)((int)&RadarTrackerPackageStruct & 0x3FFFFF);
	functionTable[RADARTRACKER_TABLE_INDEX].blocksize = 0xfff;
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
	ca.table[0][0] = 0;
	ca.table[1][0] = 1;
	comp_set_core_assignment_table(&ca);

	initCompIsr();

	//Initialize the DMA hardware
	//---------------------------
	initDMA();

	Derivative_M->ModelData.defaultParam = &Derivative_P;
	Derivative_M->ModelData.dwork = &Derivative_DW;
	Derivative_initialize(Derivative_M, &Derivative_U, &Derivative_Y);

	TractionControl_M->ModelData.defaultParam = &TractionControl_P;
	TractionControl_initialize(TractionControl_M, &TractionControl_U,
	    &TractionControl_Y);



	RT_MODEL_AirbagModel_T *AirbagModel_M =
			&AirbagModelPackageStruct.AirbagModel_STRUCT.AirbagModel_M;
	ExtU_AirbagModel_T *AirbagModel_U =
			&AirbagModelPackageStruct.AirbagModel_STRUCT.AirbagModel_U;
	ExtY_AirbagModel_T *AirbagModel_Y =
			&AirbagModelPackageStruct.AirbagModel_STRUCT.AirbagModel_Y;
	AirbagModelUpdatePointers((INT32U)&AirbagModelPackageStruct, AirbagModel_M);
	AirbagModel_initialize(AirbagModel_M, AirbagModel_U, AirbagModel_Y);
	AirbagModelUpdatePointers((INT32U)&AirbagModelPackageStruct & 0x3FFFFF, AirbagModel_M);

	RT_MODEL_CruiseControlSystem_T *CruiseControlSystem_M =
			&CruiseControlSystemPackageStruct.CruiseControlSystem_STRUCT.CruiseControlSystem_M;
	ExtU_CruiseControlSystem_T *CruiseControlSystem_U =
			&CruiseControlSystemPackageStruct.CruiseControlSystem_STRUCT.CruiseControlSystem_U;
	ExtY_CruiseControlSystem_T *CruiseControlSystem_Y =
			&CruiseControlSystemPackageStruct.CruiseControlSystem_STRUCT.CruiseControlSystem_Y;
	CruiseControlSystemUpdatePointers((INT32U)&CruiseControlSystemPackageStruct, CruiseControlSystem_M);
	CruiseControlSystem_initialize(CruiseControlSystem_M, CruiseControlSystem_U, CruiseControlSystem_Y);
	CruiseControlSystemUpdatePointers((INT32U)&CruiseControlSystemPackageStruct & 0x3FFFFF, CruiseControlSystem_M);

	RT_MODEL_RadarTracker_T *RadarTracker_M =
			&RadarTrackerPackageStruct.RadarTracker_STRUCT.RadarTracker_M;
	ExtU_RadarTracker_T *RadarTracker_U =
			&RadarTrackerPackageStruct.RadarTracker_STRUCT.RadarTracker_U;
	ExtY_RadarTracker_T *RadarTracker_Y =
			&RadarTrackerPackageStruct.RadarTracker_STRUCT.RadarTracker_Y;
	RadarTrackerUpdatePointers((INT32U)&RadarTrackerPackageStruct, RadarTracker_M);
	RadarTracker_initialize(RadarTracker_M, RadarTracker_U, RadarTracker_Y);
	RadarTrackerUpdatePointers((INT32U)&RadarTrackerPackageStruct & 0x3FFFFF, RadarTracker_M);

	//-------------------------------------------
	INT8U perr;
	dmaReadyFlag = OSFlagCreate(0, &perr); /* 0x3 = both cores are ready */
	dmaQ = OSQCreate((void**) dmaQMem, DMA_Q_SIZE);



	//Declare the OS tasks
	//-------------------
	OSTaskCreateExt(Derivative_TASK, NULL,
			&Derivative_STACK[DERIVATIVE_STACKSIZE - 1],
			DERIVATIVE_PRIORITY, DERIVATIVE_PRIORITY,
			Derivative_STACK, DERIVATIVE_STACKSIZE, NULL,
			OS_TASK_OPT_STK_CLR);
	OSTaskNameSet(DERIVATIVE_PRIORITY, (INT8U *)"Derivative", &perr);
	OSTaskCreateExt(TractionControl_TASK, NULL,
			&TractionControl_STACK[TRACTIONCONTROL_STACKSIZE - 1],
			TRACTIONCONTROL_PRIORITY, TRACTIONCONTROL_PRIORITY,
			TractionControl_STACK, TRACTIONCONTROL_STACKSIZE, NULL,
			OS_TASK_OPT_STK_CLR);
	OSTaskNameSet(TRACTIONCONTROL_PRIORITY, (INT8U *)"TractionControl", &perr);
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

	//Start the OS
	//------------
	OSStart();

	//Never get here
	//--------------
	return 0;
}






