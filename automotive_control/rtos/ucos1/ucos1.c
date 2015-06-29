/**********************************
 * Includes
 **********************************/
#include <stdio.h> //for debugging only
#include <stddef.h>
#include "includes.h"
#include "FuelSensor.h"
#include "shared_mem.h"
#include "fingerprint.h"
#include "gp.h"
#include "tlb.h"
#include "context_switch.h"
#include "critical.h"
#include "ucos1.h"
#include "reset_monitor.h"
#include "mem_manager.h"
#include "mpu_utils.h"
#include "priv/alt_exception_handler_registry.h"


/*****************************************************************************
 * Defines
 *****************************************************************************/
#define CORE_ID 							1
#define NULL ((void *)0)
int *core1_IRQ = (int *) PROCESSOR1_0_CPU_IRQ_0_BASE;

/*****************************************************************************
 * Task control flow conditions
 *****************************************************************************/

#define derivative_AirbagModel_SEM0_INITCOND 			0

/*****************************************************************************
 * The global variable declarations for each critical task
 *****************************************************************************/

/*****************************************************************************
 * Shared memory interface with monitor
 *****************************************************************************/
FunctionTable *functionTable;
CriticalFunctionData *critFuncData;
SharedMemorySymbolTable *stab;

/*****************************************************************************
 * FuelSensor
 *****************************************************************************/
RT_MODEL_FuelSensor_T FuelSensor_M;
ExtU_FuelSensor_T FuelSensor_U;
ExtY_FuelSensor_T FuelSensor_Y;
P_FuelSensor_T fuelSensor_defaultParam = { 3.0F, /* Mask Parameter: LowFuelSensor_const
 * Referenced by: '<S1>/Constant'
 */
0 /* Computed Parameter: Lighton_InitialCondition
 * Referenced by: '<Root>/Light on  '
 */
}; /* Modifiable parameters */
DW_FuelSensor_T fuelSensor_dwork;

/*****************************************************************************
 * Stack Declarations
 * Only tasks executing on this core require stacks
 *****************************************************************************/

OS_STK FuelSensor_STACK[FuelSensor_STACKSIZE];
OS_STK Derivative_AirbagModel_STACK[Derivative_AirbagModel_STACKSIZE] __attribute__ ((section (".stack_bin")));

/*****************************************************************************
 * Control Flow declarations
 *****************************************************************************/
OS_EVENT *derivative_AirbagModel_SEM0;

/*****************************************************************************
 * Interrupt from other cores
 *****************************************************************************/
static void handleCPU(void* context) {
	if (critFuncData[1].tableIndex == DERIVATIVE_FUNC_TABLE_INDEX) {
		//The monitor will provide a translation mapping for the stack
		//and for the global data... two translations received in data structure

		updateMemoryManagerTable(derivate_airbagModel_memoryTableIndex,
				&critFuncData[CORE_ID]);

		*core1_IRQ = 0;
		OSSemPost(derivative_AirbagModel_SEM0);
	}
}

static void initCpuIsr(void) {
	alt_ic_isr_register(PROCESSOR1_0_CPU_IRQ_0_IRQ_INTERRUPT_CONTROLLER_ID,
			PROCESSOR1_0_CPU_IRQ_0_IRQ, handleCPU, (void*) NULL, (void*) NULL);
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

	while (1) {
		INT8U perr;
		OSSemPend(derivative_AirbagModel_SEM0, 0, &perr);
		//Synchronize with partner
		//------------------------
		int done = 0, first = 0;
		int partnerCore = critFuncData[CORE_ID].partnerCore;
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

		//Context switch is necessary to clear the callee saved registers
		long registers[8];
		context_switch(registers);

		//Do the derivative part
		// Set default block size for fingerprinting
		fprint_set_block_size(derivative_blocksize);
		int priority = critFuncData->priority;

		//set the flag for the OS context switch
		FprintActive = 1;

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


		//Do the airbag part
		// Set default block size for fingerprinting
		fprint_set_block_size(airbagModel_blocksize);

		//set the flag for the OS context switch
		FprintActive = 1;

		//Set the global pointer in case of compilation issues related
		//to global variables
		args = functionTable[AIRBAGMODEL_FUNC_TABLE_INDEX].args;
		set_gp(gp);

		airbagModelFunc(priority + 1, args);
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
 * FuelSensor Task wrapper
 *****************************************************************************/
void FuelSensor_TASK(void* pdata) {
	while (1) {
		FuelSensor_step(&FuelSensor_M, &FuelSensor_U, &FuelSensor_Y);
		OSTimeDlyHMSM(0, 0, 0, 150);
	}
}




/*****************************************************************************
 * MPU stuff
 *****************************************************************************/

alt_exception_result exc_handler(alt_exception_cause cause,
		alt_u32 exception_pc, alt_u32 badaddr) {
	//TODO: Notify monitor to reset core immediately!!
	int *coreM_IRQ = (int *) PROCESSORM_0_CPU_IRQ_0_BASE;
	*coreM_IRQ = 1;
	while (1)
		;
	return 0;
}

void nios2_mpu_data_init() {
	//Data region is scratchpads + this core's main memory region.
	Nios2MPURegion region[NIOS2_MPU_NUM_DATA_REGIONS];
	//jtag_uart.
	region[0].index = 0x0;
	region[0].base = MEMORY_0_ONCHIP_MEMORYMAIN_BEFORE_RESET_REGION_BASE/64;
	region[0].mask = (0x431000)/64;
	region[0].c = 0;
	region[0].perm = MPU_DATA_PERM_SUPER_NONE_USER_NONE;

	//0x431-0x432 -> global data
	region[1].index = 0x1;
	region[1].base = 0x464000/64;
	region[1].mask = (0x496000)/64;
	region[1].c = 0;
	region[1].perm = MPU_DATA_PERM_SUPER_NONE_USER_NONE;


	int index;
	for (index = 2; index < NIOS2_MPU_NUM_DATA_REGIONS; index++) {
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
	FuelSensor_M.ModelData.defaultParam = &fuelSensor_defaultParam;
	FuelSensor_M.ModelData.dwork = &fuelSensor_dwork;
	FuelSensor_initialize(&FuelSensor_M, &FuelSensor_U, &FuelSensor_Y);

	//Initialize the control flow data structures
	//-------------------------------------------
	derivative_AirbagModel_SEM0 = OSSemCreate(
			derivative_AirbagModel_SEM0_INITCOND);



	//Start up the MPU
	//----------------

	// Register exception handler.
	alt_instruction_exception_register(&exc_handler);
	// Initialize and start the MPU.
	nios2_mpu_data_init();
	nios2_mpu_inst_init();
//	nios2_mpu_enable();



	mem_manager_init();

	//Declare the OS tasks
	///-------------------

	OSTaskCreateExt(FuelSensor_TASK, NULL,
			&FuelSensor_STACK[FuelSensor_STACKSIZE - 1], FuelSensor_PRIORITY,
			FuelSensor_PRIORITY, FuelSensor_STACK, FuelSensor_STACKSIZE, NULL,
			OS_TASK_OPT_STK_CLR);

	OSTaskCreateExt(Derivative_AirbagModel_TASK, NULL,
			&Derivative_AirbagModel_STACK[Derivative_AirbagModel_STACKSIZE - 1],
			Derivative_AirbagModel_PRIORITY, Derivative_AirbagModel_PRIORITY,
			Derivative_AirbagModel_STACK, Derivative_AirbagModel_STACKSIZE,
			NULL, OS_TASK_OPT_STK_CLR);

	//Notify the monitor that startup is completed
	//--------------------------------------------
	resetMonitorCoreReg(1);

	OSStart();
	return 0;
}
