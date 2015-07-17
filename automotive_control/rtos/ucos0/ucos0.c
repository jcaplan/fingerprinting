/**********************************
 * Includes
 **********************************/
#include <stdio.h> //for debugging only
#include <stddef.h>
#include "includes.h"
#include "TransmissionControl.h"
#include "CollisionAvoidance.h"
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


/*****************************************************************************
 * TransmissionControl
 *****************************************************************************/
RT_MODEL_TransmissionControl_T TransmissionControl_M;
ExtU_TransmissionControl_T TransmissionControl_U;
ExtY_TransmissionControl_T TransmissionControl_Y;
P_TransmissionControl_T transmissionControl_dfaultParam = { 3.0F /* Mask Parameter: LowFuelSensor_const
 * Referenced by: '<S1>/Constant'
 */
}; /* Modifiable parameters */

/*****************************************************************************
 * CollisionAvoidance
 *****************************************************************************/
RT_MODEL_CollisionAvoidance_T CollisionAvoidance_M;
ExtU_CollisionAvoidance_T CollisionAvoidance_U;
ExtY_CollisionAvoidance_T CollisionAvoidance_Y;
P_CollisionAvoidance_T collisionAvoidance_defaultParam = { 0.0, /* Mask Parameter: DiscreteDerivative_ICPrevScaled
 * Referenced by: '<S3>/UD'
 */
0.0, /* Mask Parameter: DiscreteDerivative1_ICPrevScale
 * Referenced by: '<S4>/UD'
 */
5.0, /* Computed Parameter: TSamp_WtEt
 * Referenced by: '<S3>/TSamp'
 */
5.0, /* Computed Parameter: TSamp_WtEt_m
 * Referenced by: '<S4>/TSamp'
 */
0.0 /* Expression: 0
 * Referenced by: '<S2>/Constant'
 */
}; /* Modifiable parameters */
DW_CollisionAvoidance_T collisionAvoidance_dwork;

/*****************************************************************************
 * Stack Declarations
 * Only tasks executing on this core require stacks
 *****************************************************************************/
OS_STK CollisionAvoidance_STACK[CollisionAvoidance_STACKSIZE];
OS_STK TransmissionControl_STACK[TransmissionControl_STACKSIZE];
OS_STK Derivative_AirbagModel_STACK[Derivative_AirbagModel_STACKSIZE] __attribute__ ((section (".stack_bin")));

/*****************************************************************************
 * Task control flow conditions
 *****************************************************************************/

#define derivative_AirbagModel_SEM0_INITCOND 			0

/*****************************************************************************
 * Shared memory interface with monitor
 *****************************************************************************/
FunctionTable *functionTable;
CriticalFunctionData *critFuncData;
SharedMemorySymbolTable *stab;

/*****************************************************************************
 * Control Flow declarations
 *****************************************************************************/
OS_EVENT *critical_SEM[2];


/*****************************************************************************
 * Interrupt from other cores
 *****************************************************************************/
static void handleCPU(void* context) {

		//The monitor will provide a translation mapping for the stack
		//and for the global data... two translations received in data structure

		//Dont set anything here explicitly
		//Set the entries in the memory manager
		int taskIndex = critFuncData[CORE_ID].tableIndex;
		updateMemoryManagerTable(taskIndex,
				&critFuncData[CORE_ID]);

		int *core0_IRQ = (int *) PROCESSOR0_0_CPU_IRQ_0_BASE;
		*core0_IRQ = 0;
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
 * CollisionAvoidance Task wrapper
 *****************************************************************************/
void CollisionAvoidance_TASK(void* pdata) {
	while (1) {
		CollisionAvoidance_step(&CollisionAvoidance_M, &CollisionAvoidance_U,
				&CollisionAvoidance_Y);
		OSTimeDlyHMSM(0, 0, 0, 150);
	}
}

/*****************************************************************************
 * TransmissionControl Task wrapper
 *****************************************************************************/
void TransmissionControl_TASK(void* pdata) {
	while (1) {
		TransmissionControl_step(&TransmissionControl_M, &TransmissionControl_U,
				&TransmissionControl_Y);
		OSTimeDlyHMSM(0, 0, 0, 150);
	}
}


/*****************************************************************************
 * Critical pair Task wrapper
 *****************************************************************************/
void Derivative_AirbagModel_TASK(void* pdata) {
	void *gp = stab->gp_address;
	int derivative_blocksize =
			functionTable[DERIVATIVE_FUNC_TABLE_INDEX].blocksize;
	int airbagModel_blocksize =
			functionTable[AIRBAGMODEL_FUNC_TABLE_INDEX].blocksize;
	void (*derivativeFunc)(int,
			DerivativeStruct*) = functionTable[DERIVATIVE_FUNC_TABLE_INDEX].address;
	void (*airbagModelFunc)(int,
			AirbagModelStruct*) = functionTable[AIRBAGMODEL_FUNC_TABLE_INDEX].address;
	int partnerCore = 1; /* static variable */
	while (1) {
		INT8U perr;
		OSSemPend(critical_SEM[0], 0, &perr);

		waitForPartnerCore(partnerCore);
		//Context switch is necessary to clear the callee saved registers
		long registers[8];
		context_switch(registers);

		//Do the derivative part
		// Set default block size for fingerprinting
		fprint_set_block_size(derivative_blocksize);

		int priority = critFuncData->priority;

		//set the flag for the OS context switch
		FprintActive = 1;
		FprintTaskIDCurrent = priority;

		//Retrieve the arguments before changing the GP

		void *args = functionTable[DERIVATIVE_FUNC_TABLE_INDEX].args;
		//Set the global pointer in case of compilation issues related
		//to global variables
		set_gp(gp);

		derivativeFunc(priority, args);
		//call the critical task
		//restore the original global pointer
		restore_gp();

		//set the flag for the OS context switch
		FprintActive = 0;

		//Transfer result
		DerivativeStruct *output = (DerivativeStruct *)args;
		AirbagModelStruct *input = functionTable[AIRBAGMODEL_FUNC_TABLE_INDEX].args;
		input->AirbagModel_U.Force = output->Derivative_Y.Out1;
		CollisionAvoidance_U.In2 = output->Derivative_Y.Out1;

		//Do the airbag part
		// Set default block size for fingerprinting
		fprint_set_block_size(airbagModel_blocksize);

		//set the flag for the OS context switch
		FprintActive = 1;

		waitForPartnerCore(partnerCore);

		//Set the global pointer in case of compilation issues related
		//to global variables
		args = functionTable[AIRBAGMODEL_FUNC_TABLE_INDEX].args;
		set_gp(gp);

		airbagModelFunc(priority, args);
		//call the critical task
		//restore the original global pointer
		restore_gp();

		//restore the flag for the OS context switch
		FprintActive = 0;
		//Restore the callee saved registers
		context_restore(registers);

	}
}

/*****************************************************************************
 * MPU stuff
 *****************************************************************************/
void mem_manager_init(void){
	//For each critical task set up a position in the table

	//AirbagModel + Derivative
	MemoryManagerStruct *entry = &memoryManagerTable[derivate_airbagModel_memoryTableIndex];
	entry->disablePending = false;
	entry->disablePendSource = 0;
	entry->taskPriority = Derivative_AirbagModel_PRIORITY;
	entry->tlbDataLine = 0;
	entry->tlbStackLine = 1;
	entry->stackPhysicalAddress = (void*)0x00495000;
	entry->stackVirtualAddress = (void*)0x00463000;
	entry->dataVirtualAddress = 0; /*get from monitor at interrupt time*/
	entry->dataPhysicalAddress = 0; /*get from monitor at interrupt time*/

	//The core 0 critical task stack must appear to be at the same address as for core 1
	set_cputable_entry(entry->tlbStackLine, entry->stackVirtualAddress);
	set_spmtable_entry(entry->tlbStackLine, entry->stackPhysicalAddress);
	enableTlbLine(entry->tlbStackLine);


	activateTlb();
}

alt_exception_result handleMPUexception(alt_exception_cause cause,
		alt_u32 exception_pc, alt_u32 badaddr) {
	//TODO: Notify monitor to reset core immediately!!
	int *coreM_IRQ = (int *) PROCESSORM_0_CPU_IRQ_0_BASE;
	if(FprintActive){
		OSTaskSuspend(OSPrioCur);
		disable_fprint_task(FprintTaskIDCurrent);
	}
	*coreM_IRQ = 1;
	return 0;
}


void nios2_mpu_data_init() {
	//Data region is scratchpads + this core's main memory region.
	Nios2MPURegion region[NIOS2_MPU_NUM_DATA_REGIONS];
	//jtag_uart.
	//global data region allowed
	region[0].index = 0x0;
	region[0].base = 0x0;
	region[0].mask = (MEMORY_0_ONCHIP_MEMORYMAIN_BEFORE_RESET_REGION_BASE)/64;
	region[0].c = 0;
	region[0].perm = MPU_DATA_PERM_SUPER_NONE_USER_NONE;

	/* global data region */
	region[1].index = 0x1;
	region[1].base = 0x431000/64;
	region[1].mask = (0x432000)/64;
	region[1].c = 0;
	region[1].perm = MPU_DATA_PERM_SUPER_RW_USER_RW;


	region[2].index = 0x2;
	region[2].base = MEMORY_0_ONCHIP_MEMORYMAIN_BEFORE_RESET_REGION_BASE/64;
	region[2].mask = (0x463000)/64;
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

void nios2_mpu_inst_init() {

	Nios2MPURegion region[NIOS2_MPU_NUM_INST_REGIONS];


		int index;
		for (index = 0; index < NIOS2_MPU_NUM_INST_REGIONS; index++) {
			region[index].base = 0x0;
			region[index].index = index;
			region[index].mask = 0x2000000;
			region[index].c = 0;
			region[index].perm = MPU_INST_PERM_SUPER_EXEC_USER_EXEC; //No access for user and supervisor
		}

	nios2_mpu_load_region(region, NIOS2_MPU_NUM_INST_REGIONS, INST_REGION);
}
/*****************************************************************************
 * Main entry point
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
	functionTable[0].stackAddress[CORE_ID] = &Derivative_AirbagModel_STACK;
	functionTable[1].stackAddress[CORE_ID] = &Derivative_AirbagModel_STACK;

	//Initialize the Matlab tasks
	//---------------------------

	CollisionAvoidance_M.ModelData.defaultParam =
			&collisionAvoidance_defaultParam;
	CollisionAvoidance_M.ModelData.dwork = &collisionAvoidance_dwork;
	CollisionAvoidance_initialize(&CollisionAvoidance_M, &CollisionAvoidance_U,
			&CollisionAvoidance_Y);

	TransmissionControl_M.ModelData.defaultParam =
			&transmissionControl_dfaultParam;
	TransmissionControl_initialize(&TransmissionControl_M,
			&TransmissionControl_U, &TransmissionControl_Y);

	//Initialize the control flow data structures
	//-------------------------------------------
	critical_SEM[0] = OSSemCreate(
			derivative_AirbagModel_SEM0_INITCOND);

	//Start up the MPU
	//----------------

	// Register exception handler.
	alt_instruction_exception_register(&handleMPUexception);
	// Initialize and start the MPU.
	nios2_mpu_data_init();
	nios2_mpu_inst_init();
	nios2_mpu_enable();



	//Start up the software memory manager
	mem_manager_init();

	//Declare the OS tasks
	///-------------------
	OSTaskCreateExt(CollisionAvoidance_TASK, NULL,
			&CollisionAvoidance_STACK[CollisionAvoidance_STACKSIZE - 1],
			CollisionAvoidance_PRIORITY, CollisionAvoidance_PRIORITY,
			CollisionAvoidance_STACK, CollisionAvoidance_STACKSIZE, NULL,
			OS_TASK_OPT_STK_CLR);

	OSTaskCreateExt(TransmissionControl_TASK, NULL,
			&TransmissionControl_STACK[TransmissionControl_STACKSIZE - 1],
			TransmissionControl_PRIORITY, TransmissionControl_PRIORITY,
			TransmissionControl_STACK, TransmissionControl_STACKSIZE, NULL,
			OS_TASK_OPT_STK_CLR);

	//The stack must look like it matches
	OSTaskCreateExt(Derivative_AirbagModel_TASK, NULL,
			(OS_STK*) (0x00463000 + 4 * (Derivative_AirbagModel_STACKSIZE - 1)),
			Derivative_AirbagModel_PRIORITY, Derivative_AirbagModel_PRIORITY,
			(OS_STK *) 0x00463000, Derivative_AirbagModel_STACKSIZE, NULL,
			OS_TASK_OPT_STK_CLR);


	//Notify the monitor that startup is completed
	//--------------------------------------------
	resetMonitorCoreReg(0);

	OSStart();
	return 0;
}
