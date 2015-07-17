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
#include "critical.h"
#include "mpu_utils.h"
#include "priv/alt_exception_handler_registry.h"
#include "mem_manager.h"
#include "cpu0.h"
#include "reset_monitor.h"
#include "FuelSensor.h"
#include "TransmissionControl.h"





/**********************************
 * Global variable declarations
 **********************************/
/* AirbagModel*/

/* CruiseControlSystem*/

/* TractionControl*/

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






/*****************************************************************************
 * Stack Declarations
 *****************************************************************************/
OS_STK CruiseControlSystem_STACK[CruiseControlSystem_STACKSIZE] __attribute__ ((section (".stack_bin_0")));
OS_STK AirbagModel_STACK[AirbagModel_STACKSIZE] __attribute__ ((section (".stack_bin_0")));
OS_STK TractionControl_STACK[TractionControl_STACKSIZE] __attribute__ ((section (".stack_bin_1")));
OS_STK FuelSensor_STACK[FuelSensor_STACKSIZE];
OS_STK TransmissionControl_STACK[TransmissionControl_STACKSIZE];





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
	int AirbagModel_blocksize =
			functionTable[AIRBAGMODEL_TABLE_INDEX].blocksize;
	void (*AirbagModelFunc)(int,
			AirbagModelStruct*) = functionTable[AIRBAGMODEL_TABLE_INDEX].address;
	int partnerCore = 1; /* static variable */

	while (1) {
		INT8U perr;
		OSSemPend(critical_SEM[AIRBAGMODEL_TABLE_INDEX], 0, &perr);

		waitForPartnerCore(partnerCore);
		//Context switch is necessary to clear the callee saved registers
		long registers[8];
		context_switch(registers);

		//Do the derivative part
		// Set default block size for fingerprinting
		fprint_set_block_size(AirbagModel_blocksize);

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

	}
}

/*****************************************************************************
 * CruiseControlSystemTask wrapper
 *****************************************************************************/
void CruiseControlSystem_TASK(void* pdata) {
	void *gp = stab->gp_address;
	int CruiseControlSystem_blocksize =
			functionTable[CRUISECONTROLSYSTEM_TABLE_INDEX].blocksize;
	void (*CruiseControlSystemFunc)(int,
			CruiseControlSystemStruct*) = functionTable[CRUISECONTROLSYSTEM_TABLE_INDEX].address;
	int partnerCore = 1; /* static variable */

	while (1) {
		INT8U perr;
		OSSemPend(critical_SEM[CRUISECONTROLSYSTEM_TABLE_INDEX], 0, &perr);

		waitForPartnerCore(partnerCore);
		//Context switch is necessary to clear the callee saved registers
		long registers[8];
		context_switch(registers);

		//Do the derivative part
		// Set default block size for fingerprinting
		fprint_set_block_size(CruiseControlSystem_blocksize);

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

	}
}

/*****************************************************************************
 * TractionControlTask wrapper
 *****************************************************************************/
void TractionControl_TASK(void* pdata) {
	void *gp = stab->gp_address;
	int TractionControl_blocksize =
			functionTable[TRACTIONCONTROL_TABLE_INDEX].blocksize;
	void (*TractionControlFunc)(int,
			TractionControlStruct*) = functionTable[TRACTIONCONTROL_TABLE_INDEX].address;
	int partnerCore = 1; /* static variable */

	while (1) {
		INT8U perr;
		OSSemPend(critical_SEM[TRACTIONCONTROL_TABLE_INDEX], 0, &perr);

		waitForPartnerCore(partnerCore);
		//Context switch is necessary to clear the callee saved registers
		long registers[8];
		context_switch(registers);

		//Do the derivative part
		// Set default block size for fingerprinting
		fprint_set_block_size(TractionControl_blocksize);

		int priority = critFuncData->priority;

		//set the flag for the OS context switch
		FprintActive = 1;
		FprintTaskIDCurrent = priority;

		//Retrieve the arguments before changing the GP

		void *args = functionTable[TRACTIONCONTROL_TABLE_INDEX].args;
		//Set the global pointer in case of compilation issues related
		//to global variables
		set_gp(gp);

		TractionControlFunc(priority, args);
		//call the critical task
		//restore the original global pointer
		restore_gp();

		//set the flag for the OS context switch
		FprintActive = 0;

	}
}

/*****************************************************************************
 * FuelSensorTask wrapper
 *****************************************************************************/
void FuelSensor_TASK(void* pdata) {
	while (1) {
		FuelSensor_step(FuelSensor_M, &FuelSensor_U,
			&FuelSensor_Y);
		OSTimeDlyHMSM(0, 0, 0, 75);
	}
}

/*****************************************************************************
 * TransmissionControlTask wrapper
 *****************************************************************************/
void TransmissionControl_TASK(void* pdata) {
	while (1) {
		TransmissionControl_step(TransmissionControl_M, &TransmissionControl_U,
			&TransmissionControl_Y);
		OSTimeDlyHMSM(0, 0, 0, 15);
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
	entry->taskPriority = AirbagModel_PRIORITY;
	entry->tlbDataLine = 0;
	entry->tlbStackLine = 1;
	entry->stackPhysicalAddress = (void*)0x495000;
	entry->stackVirtualAddress = (void*)0x463000;
	entry->dataVirtualAddress = 0; /*get from monitor at interrupt time*/
	entry->dataPhysicalAddress = 0; /*get from monitor at interrupt time*/

	managerEnableTask(entry);
	// CruiseControlSystem
	entry = &memoryManagerTable[CRUISECONTROLSYSTEM_TABLE_INDEX];
	entry->disablePending = false;
	entry->disablePendSource = 0;
	entry->taskPriority = CruiseControlSystem_PRIORITY;
	entry->tlbDataLine = 2;
	entry->tlbStackLine = 3;
	entry->stackPhysicalAddress = (void*)0x495000;
	entry->stackVirtualAddress = (void*)0x463000;
	entry->dataVirtualAddress = 0; /*get from monitor at interrupt time*/
	entry->dataPhysicalAddress = 0; /*get from monitor at interrupt time*/

	managerEnableTask(entry);
	// TractionControl
	entry = &memoryManagerTable[TRACTIONCONTROL_TABLE_INDEX];
	entry->disablePending = false;
	entry->disablePendSource = 0;
	entry->taskPriority = TractionControl_PRIORITY;
	entry->tlbDataLine = 4;
	entry->tlbStackLine = 5;
	entry->stackPhysicalAddress = (void*)0x494000;
	entry->stackVirtualAddress = (void*)0x462000;
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
		disable_fprint_task(FprintTaskIDCurrent);
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
	region[0].mask = (0x431000) / 64;
	region[0].c = 0;
	region[0].perm = MPU_DATA_PERM_SUPER_NONE_USER_NONE;

	//other core's region - stack
	region[1].index = 0x1;
	region[1].base = 0x432000/ 64;
	region[1].mask = 0x462000/ 64;
	region[1].c = 0;
	region[1].perm = MPU_DATA_PERM_SUPER_NONE_USER_NONE;

	//catch null pointers
	region[2].index = 0x2;
	region[2].base = 0x0;
	region[2].mask = (MEMORY_0_ONCHIP_MEMORYMAIN_BEFORE_RESET_REGION_BASE) / 64;
	region[2].c = 0;
	region[2].perm = MPU_DATA_PERM_SUPER_NONE_USER_NONE;

	int index;
	for (index = 3; index < NIOS2_MPU_NUM_DATA_REGIONS; index++) {
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
	printf("starting core %d", CORE_ID);

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
	functionTable[CRUISECONTROLSYSTEM_TABLE_INDEX].stackAddress[CORE_ID] = &CruiseControlSystem_STACK;
	functionTable[TRACTIONCONTROL_TABLE_INDEX].stackAddress[CORE_ID] = &TractionControl_STACK;

	FuelSensor_M->ModelData.defaultParam = &FuelSensor_P;
	FuelSensor_M->ModelData.dwork = &FuelSensor_DW;
	FuelSensor_initialize(FuelSensor_M, &FuelSensor_U, &FuelSensor_Y);

	TransmissionControl_M->ModelData.defaultParam = &TransmissionControl_P;
	TransmissionControl_initialize(TransmissionControl_M, &TransmissionControl_U,
	    &TransmissionControl_Y);


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
	// Declare the OS tasks
	// -------------------

	OSTaskCreateExt(AirbagModel_TASK, NULL,
			0x463f8c,
			AirbagModel_PRIORITY, AirbagModel_PRIORITY,
			0x4637cc, AirbagModel_STACKSIZE, NULL,
			OS_TASK_OPT_STK_CLR);
	OSTaskCreateExt(CruiseControlSystem_TASK, NULL,
			0x4637c8,
			CruiseControlSystem_PRIORITY, CruiseControlSystem_PRIORITY,
			0x463000, CruiseControlSystem_STACKSIZE, NULL,
			OS_TASK_OPT_STK_CLR);
	OSTaskCreateExt(TractionControl_TASK, NULL,
			0x4627b4,
			TractionControl_PRIORITY, TractionControl_PRIORITY,
			0x462000, TractionControl_STACKSIZE, NULL,
			OS_TASK_OPT_STK_CLR);
	OSTaskCreateExt(FuelSensor_TASK, NULL,
			&FuelSensor_STACK[FuelSensor_STACKSIZE - 1],
			FuelSensor_PRIORITY, FuelSensor_PRIORITY,
			FuelSensor_STACK, FuelSensor_STACKSIZE, NULL,
			OS_TASK_OPT_STK_CLR);
	OSTaskCreateExt(TransmissionControl_TASK, NULL,
			&TransmissionControl_STACK[TransmissionControl_STACKSIZE - 1],
			TransmissionControl_PRIORITY, TransmissionControl_PRIORITY,
			TransmissionControl_STACK, TransmissionControl_STACKSIZE, NULL,
			OS_TASK_OPT_STK_CLR);

	resetMonitorCoreReg(CORE_ID);

	OSStart();
	return 0;
}

