/**********************************
 * Includes
 **********************************/
#include <stdio.h>
#include <stddef.h>
#include "includes.h"
#include "shared_mem.h"
#include "fingerprint.h"
#include "gp.h"
#include "context_switch.h"
#include "tlb.h"
#include "mpu_utils.h"
#include "priv/alt_exception_handler_registry.h"
#include "mem_manager.h"
#include "cpu0.h"
#include "reset_monitor.h"
#include "runtimeMonitor.h"
#include "critical.h"
#include "AirbagModel.h"
#include "CruiseControlSystem.h"
#include "FuelSensor.h"
#include "TransmissionControl.h"
#include "RadarTracker.h"
#include "for_loop.h"





/**********************************
 * Global variable declarations
 **********************************/
/* AirbagModel*/

/* CruiseControlSystem*/

/* FuelSensor*/
static RT_MODEL_FuelSensor_T FuelSensor_M_;
static RT_MODEL_FuelSensor_T *const FuelSensor_M = &FuelSensor_M_;/* Real-time model */
static P_FuelSensor_T FuelSensor_P = {  3.0F,                                /* Mask Parameter: LowFuelSensor_const
                                        * Referenced by: '<S1>/Constant'
                                        */
  0                                    /* Computed Parameter: Lighton_InitialCondition
                                        * Referenced by: '<Root>/Light on  '
                                        */
};                                     /* Modifiable parameters */
static DW_FuelSensor_T FuelSensor_DW;  /* Observable states */
static ExtU_FuelSensor_T FuelSensor_U; /* External inputs */
static ExtY_FuelSensor_T FuelSensor_Y; /* External outputs */

/* TransmissionControl*/
static RT_MODEL_TransmissionControl_T TransmissionControl_M_;
static RT_MODEL_TransmissionControl_T *const TransmissionControl_M =  &TransmissionControl_M_;             /* Real-time model */
static P_TransmissionControl_T TransmissionControl_P = {  3.0F                                 /* Mask Parameter: LowFuelSensor_const
                                        * Referenced by: '<S1>/Constant'
                                        */
};                                     /* Modifiable parameters */
static ExtU_TransmissionControl_T TransmissionControl_U;/* External inputs */
static ExtY_TransmissionControl_T TransmissionControl_Y;/* External outputs */

/* RadarTracker*/

/* for_loop*/
static RT_MODEL_for_loop_T for_loop_M_;
static RT_MODEL_for_loop_T *const for_loop_M = &for_loop_M_;/* Real-time model */
static ExtU_for_loop_T for_loop_U;     /* External inputs */
static ExtY_for_loop_T for_loop_Y;     /* External outputs */






/*****************************************************************************
 * Stack Declarations
 *****************************************************************************/
OS_STK RadarTracker_STACK[RADARTRACKER_STACKSIZE] __attribute__ ((section (".stack_bin_0")));
OS_STK for_loop_STACK[FOR_LOOP_STACKSIZE];
OS_STK AirbagModel_STACK[AIRBAGMODEL_STACKSIZE] __attribute__ ((section (".stack_bin_1")));
OS_STK CruiseControlSystem_STACK[CRUISECONTROLSYSTEM_STACKSIZE] __attribute__ ((section (".stack_bin_1")));
OS_STK FuelSensor_STACK[FUELSENSOR_STACKSIZE];
OS_STK TransmissionControl_STACK[TRANSMISSIONCONTROL_STACKSIZE];





/*****************************************************************************
 * Execution time monitoring table
 *****************************************************************************/
rtMonitor_task rtMonTaskTable[NUM_TASKS] = {
	{ AIRBAGMODEL_PRIORITY, 0, AIRBAGMODEL_PERIOD, false, true, "AirbagModel" },
	{ CRUISECONTROLSYSTEM_PRIORITY, 0, CRUISECONTROLSYSTEM_PERIOD, false, true, "CruiseControlSystem" },
	{ FUELSENSOR_PRIORITY, 0, FUELSENSOR_PERIOD, false, false, "FuelSensor" },
	{ TRANSMISSIONCONTROL_PRIORITY, 0, TRANSMISSIONCONTROL_PERIOD, false, false, "TransmissionControl" },
	{ RADARTRACKER_PRIORITY, 0, RADARTRACKER_PERIOD, false, true, "RadarTracker" },
	{ FOR_LOOP_PRIORITY, 0, FOR_LOOP_PERIOD, false, false, "for_loop" }
};





/*****************************************************************************
 * Control Flow declarations
 *****************************************************************************/
OS_EVENT *critical_SEM[3];





/*****************************************************************************
 * Shared memory interface with monitor
 *****************************************************************************/
FunctionTable *functionTable;
CriticalFunctionData *critFuncData;
SharedMemorySymbolTable *stab;





/*****************************************************************************
 * Interrupt from other cores
 *****************************************************************************/
static void handleCPU(void* context) {
	int taskIndex = critFuncData[CORE_ID].tableIndex;
	updateMemoryManagerTable(taskIndex,
			&critFuncData[CORE_ID]);

	int *PROCESSOR0_IRQ = (int *) PROCESSOR0_0_CPU_IRQ_0_BASE;
	*PROCESSOR0_IRQ = 0;
	OSSemPost(critical_SEM[taskIndex]);
}

static void initCpuIsr(void) {
	alt_ic_isr_register(PROCESSOR0_0_CPU_IRQ_0_IRQ_INTERRUPT_CONTROLLER_ID,
			PROCESSOR0_0_CPU_IRQ_0_IRQ, handleCPU, (void*) NULL, (void*) NULL);
}

void waitForPartnerCore(int partnerCore) {
	//Synchronize with partner
	//------------------------
	int done = 0, first = 0;
	while (done == 0) {
		if (first == 0) {
			critFuncData[CORE_ID].checkout = 1;
			first = 1;
		}
		if (critFuncData[partnerCore].checkout == 1) {
			critFuncData[partnerCore].checkout = 0;
			done = 1;
		}
	}
}





/*****************************************************************************
 * AirbagModelTask wrapper
 *****************************************************************************/
void AirbagModel_TASK(void* pdata) {
	void *gp = stab->gp_address;
	void (*AirbagModelFunc)(int,
			AirbagModelStruct*) = functionTable[AIRBAGMODEL_TABLE_INDEX].address;
	int partnerCore = 1; /* static variable */

	while (1) {
		INT8U perr;
		OSSemPend(critical_SEM[AIRBAGMODEL_TABLE_INDEX], 0, &perr);

		rtMonitorStartTask(AIRBAGMODEL_RT_PRIO);

		waitForPartnerCore(partnerCore);
		//Context switch is necessary to clear the callee saved registers
		long registers[8];
		context_switch(registers);

		//Do the derivative part

		int priority = critFuncData->priority;

		//set the flag for the OS context switch
		FprintActive = 1;
		FprintTaskIDCurrent = priority;

		//Retrieve the arguments before changing the GP

		void *args = functionTable[AIRBAGMODEL_TABLE_INDEX].args;
		//Set the global pointer in case of compilation issues related
		//to global variables
		set_gp(gp);

		AirbagModelFunc(priority, args);
		//call the critical task
		//restore the original global pointer
		restore_gp();

		//set the flag for the OS context switch
		FprintActive = 0;


		rtMonitorEndTask(AIRBAGMODEL_RT_PRIO);
	}
}

/*****************************************************************************
 * CruiseControlSystemTask wrapper
 *****************************************************************************/
void CruiseControlSystem_TASK(void* pdata) {
	void *gp = stab->gp_address;
	void (*CruiseControlSystemFunc)(int,
			CruiseControlSystemStruct*) = functionTable[CRUISECONTROLSYSTEM_TABLE_INDEX].address;
	int partnerCore = 1; /* static variable */

	while (1) {
		INT8U perr;
		OSSemPend(critical_SEM[CRUISECONTROLSYSTEM_TABLE_INDEX], 0, &perr);

		rtMonitorStartTask(CRUISECONTROLSYSTEM_RT_PRIO);

		waitForPartnerCore(partnerCore);
		//Context switch is necessary to clear the callee saved registers
		long registers[8];
		context_switch(registers);

		//Do the derivative part

		int priority = critFuncData->priority;

		//set the flag for the OS context switch
		FprintActive = 1;
		FprintTaskIDCurrent = priority;

		//Retrieve the arguments before changing the GP

		void *args = functionTable[CRUISECONTROLSYSTEM_TABLE_INDEX].args;
		//Set the global pointer in case of compilation issues related
		//to global variables
		set_gp(gp);

		CruiseControlSystemFunc(priority, args);
		//call the critical task
		//restore the original global pointer
		restore_gp();

		//set the flag for the OS context switch
		FprintActive = 0;


		rtMonitorEndTask(CRUISECONTROLSYSTEM_RT_PRIO);
	}
}

/*****************************************************************************
 * FuelSensorTask wrapper
 *****************************************************************************/
void FuelSensor_TASK(void* pdata) {

	while (1) {
		rtMonitorStartTask(FUELSENSOR_RT_PRIO);
		FuelSensor_step(FuelSensor_M, &FuelSensor_U,
			&FuelSensor_Y);
		rtMonitorEndTask(FUELSENSOR_RT_PRIO);
		OSTimeDlyHMSM(0, 0, 0, FUELSENSOR_PERIOD);
	}
}

/*****************************************************************************
 * TransmissionControlTask wrapper
 *****************************************************************************/
void TransmissionControl_TASK(void* pdata) {

	while (1) {
		rtMonitorStartTask(TRANSMISSIONCONTROL_RT_PRIO);
		TransmissionControl_step(TransmissionControl_M, &TransmissionControl_U,
			&TransmissionControl_Y);
		rtMonitorEndTask(TRANSMISSIONCONTROL_RT_PRIO);
		OSTimeDlyHMSM(0, 0, 0, TRANSMISSIONCONTROL_PERIOD);
	}
}

/*****************************************************************************
 * RadarTrackerTask wrapper
 *****************************************************************************/
void RadarTracker_TASK(void* pdata) {
	void *gp = stab->gp_address;
	void (*RadarTrackerFunc)(int,
			RadarTrackerStruct*) = functionTable[RADARTRACKER_TABLE_INDEX].address;
	int partnerCore = 1; /* static variable */

	while (1) {
		INT8U perr;
		OSSemPend(critical_SEM[RADARTRACKER_TABLE_INDEX], 0, &perr);

		rtMonitorStartTask(RADARTRACKER_RT_PRIO);

		waitForPartnerCore(partnerCore);
		//Context switch is necessary to clear the callee saved registers
		long registers[8];
		context_switch(registers);

		//Do the derivative part

		int priority = critFuncData->priority;

		//set the flag for the OS context switch
		FprintActive = 1;
		FprintTaskIDCurrent = priority;

		//Retrieve the arguments before changing the GP

		void *args = functionTable[RADARTRACKER_TABLE_INDEX].args;
		//Set the global pointer in case of compilation issues related
		//to global variables
		set_gp(gp);

		RadarTrackerFunc(priority, args);
		//call the critical task
		//restore the original global pointer
		restore_gp();

		//set the flag for the OS context switch
		FprintActive = 0;


		rtMonitorEndTask(RADARTRACKER_RT_PRIO);
	}
}

/*****************************************************************************
 * for_loopTask wrapper
 *****************************************************************************/
void for_loop_TASK(void* pdata) {
    srand(RANDOM_SEED);
	while (1) {
		rtMonitorStartTask(FOR_LOOP_RT_PRIO);
		INT32U time = OSTimeGet();
		for_loop_step(for_loop_M, &for_loop_U,
			&for_loop_Y);
		time = OSTimeGet() - time;
		printf("runtime task %s: %u\n",rtMonitorGetTaskName(FOR_LOOP_RT_PRIO),time);
		rtMonitorEndTask(FOR_LOOP_RT_PRIO);
		OSTimeDlyHMSM(0, 0, 0, FOR_LOOP_PERIOD);
	}
}






/*****************************************************************************
 * Memory Manager 
 *****************************************************************************/
void mem_manager_init(void) {
// For each critical task set up a position in the table
	MemoryManagerStruct *entry;

	// AirbagModel
	entry = &memoryManagerTable[AIRBAGMODEL_TABLE_INDEX];
	entry->disablePending = false;
	entry->disablePendSource = 0;
	entry->taskPriority = AIRBAGMODEL_PRIORITY;
	entry->tlbDataLine = 0;
	entry->tlbStackLine = 1;
	entry->stackPhysicalAddress = (void*)0x494000;
	entry->stackVirtualAddress = (void*)0x62000;
	entry->dataVirtualAddress = 0; /*get from monitor at interrupt time*/
	entry->dataPhysicalAddress = 0; /*get from monitor at interrupt time*/

	managerEnableTask(entry);
	// CruiseControlSystem
	entry = &memoryManagerTable[CRUISECONTROLSYSTEM_TABLE_INDEX];
	entry->disablePending = false;
	entry->disablePendSource = 0;
	entry->taskPriority = CRUISECONTROLSYSTEM_PRIORITY;
	entry->tlbDataLine = 2;
	entry->tlbStackLine = 3;
	entry->stackPhysicalAddress = (void*)0x494000;
	entry->stackVirtualAddress = (void*)0x62000;
	entry->dataVirtualAddress = 0; /*get from monitor at interrupt time*/
	entry->dataPhysicalAddress = 0; /*get from monitor at interrupt time*/

	managerEnableTask(entry);
	// RadarTracker
	entry = &memoryManagerTable[RADARTRACKER_TABLE_INDEX];
	entry->disablePending = false;
	entry->disablePendSource = 0;
	entry->taskPriority = RADARTRACKER_PRIORITY;
	entry->tlbDataLine = 4;
	entry->tlbStackLine = 5;
	entry->stackPhysicalAddress = (void*)0x495000;
	entry->stackVirtualAddress = (void*)0x63000;
	entry->dataVirtualAddress = 0; /*get from monitor at interrupt time*/
	entry->dataPhysicalAddress = 0; /*get from monitor at interrupt time*/

	managerEnableTask(entry);
}





/*****************************************************************************
 * MPU Initialization 
 *****************************************************************************/
alt_exception_result handleMPUException(alt_exception_cause cause,
		alt_u32 exception_pc, alt_u32 badaddr) {
	//TODO: Notify monitor to reset core immediately!!
	int *coreM_IRQ = (int *) PROCESSORM_0_CPU_IRQ_0_BASE;
	if (FprintActive) {
		fprint_disable_task(FprintTaskIDCurrent);
	}
	*coreM_IRQ = 1;
	while (1)
		;
	return 0;
}

void nios2_mpu_inst_init() {

	Nios2MPURegion region[NIOS2_MPU_NUM_INST_REGIONS];

	//catch null pointers
	region[0].index = 0x0;
	region[0].base = 0x0;
	region[0].mask = 0x1000 / 64;
	region[0].c = 0;
	region[0].perm = MPU_DATA_PERM_SUPER_NONE_USER_NONE;


	int index;
	for (index = 1; index < NIOS2_MPU_NUM_INST_REGIONS; index++) {
		region[index].base = 0x0;
		region[index].index = index;
		region[index].mask = 0x2000000;
		region[index].c = 0;
		region[index].perm = MPU_INST_PERM_SUPER_EXEC_USER_EXEC; //No access for user and supervisor
	}

	nios2_mpu_load_region(region, NIOS2_MPU_NUM_INST_REGIONS, INST_REGION);
}
void nios2_mpu_data_init() {
	//Data region is scratchpads + this core's main memory region.
	Nios2MPURegion region[NIOS2_MPU_NUM_DATA_REGIONS];
	//monitor core - global_data
	region[0].index = 0x0;
	region[0].base = MEMORY_0_ONCHIP_MEMORYMAIN_BEFORE_RESET_REGION_BASE / 64;
	region[0].mask = (0x432000) / 64;
	region[0].c = 0;
	region[0].perm = MPU_DATA_PERM_SUPER_NONE_USER_NONE;

	//other core's region - stack
	region[1].index = 0x1;
	region[1].base = 0x432000/ 64;
	region[1].mask = 0x464000/ 64;
	region[1].c = 0;
	region[1].perm = MPU_DATA_PERM_SUPER_RD_USER_RD;

	//catch null pointers
	region[2].index = 0x2;
	region[2].base = 0x0;
	region[2].mask = 0x31000 / 64;
	region[2].c = 0;
	region[2].perm = MPU_DATA_PERM_SUPER_NONE_USER_NONE;

	//no scratchpad physical address
	region[3].index = 0x3;
	region[3].base = 0x4200000 / 64;
	region[3].mask = 0x4208000 / 64;
	region[3].c = 0;
	region[3].perm = MPU_DATA_PERM_SUPER_NONE_USER_NONE;

	int index;
	for (index = 4; index < NIOS2_MPU_NUM_DATA_REGIONS; index++) {
		region[index].base = 0x0;
		region[index].index = index;
		region[index].mask = 0x2000000;
		region[index].c = 0;
		region[index].perm = MPU_DATA_PERM_SUPER_RW_USER_RW; //No access for user and supervisor
	}

	nios2_mpu_load_region(region, NIOS2_MPU_NUM_DATA_REGIONS, DATA_REGION);
}





/*****************************************************************************
 * Main
 *****************************************************************************/

int main() {
	printf("starting core %d\n", CORE_ID);

	//Initialize interrupts
	//---------------------
	initCpuIsr();

	//Initialize OS Context switch flag
	FprintActive = 0;

	//Symbol table is at known location
	//---------------------------------
	stab = (SharedMemorySymbolTable*) SHARED_MEMORY_BASE;

	//Wait until the monitor has placed data into shared memory
	//---------------------------------------------------------
	while (stab->init_complete == 0)
		;

	functionTable = stab->ftable;
	critFuncData = stab->critFuncData;

	//Put the location of the stack for the task in shared memory
	//-----------------------------------------------------------
	functionTable[AIRBAGMODEL_TABLE_INDEX].stackAddress[CORE_ID] = &AirbagModel_STACK;
	functionTable[AIRBAGMODEL_TABLE_INDEX].address = AirbagModel_CT;
	functionTable[CRUISECONTROLSYSTEM_TABLE_INDEX].stackAddress[CORE_ID] = &CruiseControlSystem_STACK;
	functionTable[CRUISECONTROLSYSTEM_TABLE_INDEX].address = CruiseControlSystem_CT;
	functionTable[RADARTRACKER_TABLE_INDEX].stackAddress[CORE_ID] = &RadarTracker_STACK;
	functionTable[RADARTRACKER_TABLE_INDEX].address = RadarTracker_CT;

	FuelSensor_M->ModelData.defaultParam = &FuelSensor_P;
	FuelSensor_M->ModelData.dwork = &FuelSensor_DW;
	FuelSensor_initialize(FuelSensor_M, &FuelSensor_U, &FuelSensor_Y);


	TransmissionControl_M->ModelData.defaultParam = &TransmissionControl_P;
	TransmissionControl_initialize(TransmissionControl_M, &TransmissionControl_U,
	    &TransmissionControl_Y);



	for_loop_initialize(for_loop_M);


	critical_SEM[0] = OSSemCreate(0);
	critical_SEM[1] = OSSemCreate(0);
	critical_SEM[2] = OSSemCreate(0);
	//Start up the MPU
	//----------------

	// Register exception handler.
	alt_instruction_exception_register(&handleMPUException);
	// Initialize and start the MPU.
	nios2_mpu_data_init();
	nios2_mpu_inst_init();
	nios2_mpu_enable();

	//Start up the software memory manager
	mem_manager_init();

	// Initialize executime time monitor
	// ---------------------------------
	rtMonitorInit(&rtMonTaskTable[0],NUM_TASKS);

	// Declare the OS tasks
	// -------------------

	INT8U perr;	OSTaskCreateExt(AirbagModel_TASK, NULL,
			(OS_STK *)0x62764,
			AIRBAGMODEL_PRIORITY, AIRBAGMODEL_PRIORITY,
			(OS_STK *)0x62000, AIRBAGMODEL_STACKSIZE, NULL,
			OS_TASK_OPT_STK_CLR);
	OSTaskNameSet(AIRBAGMODEL_PRIORITY, (INT8U *)"AirbagModel", &perr);
	OSTaskCreateExt(CruiseControlSystem_TASK, NULL,
			(OS_STK *)0x62ec4,
			CRUISECONTROLSYSTEM_PRIORITY, CRUISECONTROLSYSTEM_PRIORITY,
			(OS_STK *)0x62768, CRUISECONTROLSYSTEM_STACKSIZE, NULL,
			OS_TASK_OPT_STK_CLR);
	OSTaskNameSet(CRUISECONTROLSYSTEM_PRIORITY, (INT8U *)"CruiseControlSystem", &perr);
	OSTaskCreateExt(FuelSensor_TASK, NULL,
			&FuelSensor_STACK[FUELSENSOR_STACKSIZE - 1],
			FUELSENSOR_PRIORITY, FUELSENSOR_PRIORITY,
			FuelSensor_STACK, FUELSENSOR_STACKSIZE, NULL,
			OS_TASK_OPT_STK_CLR);
	OSTaskNameSet(FUELSENSOR_PRIORITY, (INT8U *)"FuelSensor", &perr);
	OSTaskCreateExt(TransmissionControl_TASK, NULL,
			&TransmissionControl_STACK[TRANSMISSIONCONTROL_STACKSIZE - 1],
			TRANSMISSIONCONTROL_PRIORITY, TRANSMISSIONCONTROL_PRIORITY,
			TransmissionControl_STACK, TRANSMISSIONCONTROL_STACKSIZE, NULL,
			OS_TASK_OPT_STK_CLR);
	OSTaskNameSet(TRANSMISSIONCONTROL_PRIORITY, (INT8U *)"TransmissionControl", &perr);
	OSTaskCreateExt(RadarTracker_TASK, NULL,
			(OS_STK *)0x63c68,
			RADARTRACKER_PRIORITY, RADARTRACKER_PRIORITY,
			(OS_STK *)0x63000, RADARTRACKER_STACKSIZE, NULL,
			OS_TASK_OPT_STK_CLR);
	OSTaskNameSet(RADARTRACKER_PRIORITY, (INT8U *)"RadarTracker", &perr);
	OSTaskCreateExt(for_loop_TASK, NULL,
			&for_loop_STACK[FOR_LOOP_STACKSIZE - 1],
			FOR_LOOP_PRIORITY, FOR_LOOP_PRIORITY,
			for_loop_STACK, FOR_LOOP_STACKSIZE, NULL,
			OS_TASK_OPT_STK_CLR);
	OSTaskNameSet(FOR_LOOP_PRIORITY, (INT8U *)"for_loop", &perr);

	resetMonitorCoreReg(CORE_ID);

	OSStart();
	return 0;
}






