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

/*****************************************************************************
 * Defines
 *****************************************************************************/

/*****************************************************************************
 * Defines
 *****************************************************************************/
#define CORE_ID 							1
int *core1_IRQ = (int *) PROCESSOR1_0_CPU_IRQ_0_BASE;


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
#define FuelSensor_STACKSIZE 				(156 + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR)
#define Derivative_AirbagModel_STACKSIZE	(156 + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR)



/*****************************************************************************
 * Task Priorities
 *****************************************************************************/
#define FuelSensor_PRIORITY 				13
#define Derivative_AirbagModel_PRIORITY     12

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
	if (critFuncData[1].tableIndex == DERIVATIVE_FUNC_TABLE_INDEX){
		//initiate the critical task execution
		//The stack is statically allocated in the scratchpad for now
		//main memory transfers may occur later
		//Use DMA, send pointers for tasks through shared memory to monitor
		//The monitor will move the task stacks into the scratchpad at the appropriate location
		//and send the translation info back to the core
		//The stacks will be packed as closely as possible into 4kB
		*core1_IRQ = 0;
		OSSemPost(derivative_AirbagModel_SEM0);
	}
}
static void initCpuIsr(void) {
	alt_ic_isr_register(PROCESSOR1_0_CPU_IRQ_0_IRQ_INTERRUPT_CONTROLLER_ID,
			PROCESSOR1_0_CPU_IRQ_0_IRQ, handleCPU, (void*) NULL,
			(void*) NULL);
}


/*****************************************************************************
 * Critical pair Task wrapper
 *****************************************************************************/
void Derivative_AirbagModel_TASK(void* pdata){
	void *gp = stab->gp_address;
	int derivative_blocksize = functionTable[DERIVATIVE_FUNC_TABLE_INDEX].blocksize;
	int airbagModel_blocksize = functionTable[AIRBAGMODEL_FUNC_TABLE_INDEX].blocksize;
	void (*derivativeFunc)(int ,DerivativeStruct*) = functionTable[DERIVATIVE_FUNC_TABLE_INDEX].address;
	void (*airbagModelFunc)(int ,AirbagModelStruct*) = functionTable[AIRBAGMODEL_FUNC_TABLE_INDEX].address;

	while(1){
		INT8U perr;
		OSSemPend(derivative_AirbagModel_SEM0,0,&perr);
		//Synchronize with partner
		//------------------------
		int done = 0, first = 0;
		int partnerCore = critFuncData[CORE_ID].partnerCore;
		while (done == 0) {
			if(first == 0){
				critFuncData[CORE_ID].checkout = 1;
				first = 1;
			}
			if( critFuncData[partnerCore].checkout == 1){
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
		//TODO The TLB must be appropriately configured


		set_cputable_entry(0, 0x00431000);
		set_spmtable_entry(0, 0x04203000);
		enableTlbLine(0);
		activateTlb();

		//Set the global pointer in case of compilation issues related
		//to global variables
		set_gp(gp);

		derivativeFunc(0,functionTable[DERIVATIVE_FUNC_TABLE_INDEX].args);
		//call the critical task
		//restore the original global pointer
		restore_gp();
		deactivateTlb();




		//Do the airbag part
		// Set default block size for fingerprinting
		fprint_set_block_size(airbagModel_blocksize);
		//TODO The TLB must be appropriately configured

		enableTlbLine(0);
		activateTlb();
		//Set the global pointer in case of compilation issues related
		//to global variables
		set_gp(gp);

		airbagModelFunc(1,functionTable[AIRBAGMODEL_FUNC_TABLE_INDEX].args);
		//call the critical task
		//restore the original global pointer
		restore_gp();
		deactivateTlb();

		//Restore the callee saved registers
		context_restore(registers);

	}
}

/*****************************************************************************
 * FuelSensor Task wrapper
 *****************************************************************************/
void FuelSensor_TASK(void* pdata){
	while(1){
		FuelSensor_step(&FuelSensor_M,&FuelSensor_U,&FuelSensor_Y);
		OSTimeDlyHMSM(0,0,0,150);
	}
}



/*****************************************************************************
 * Main entry point
 *****************************************************************************/


int main(){
	printf("starting core %d",CORE_ID);

	//Initialize interrupts
	//---------------------
	initCpuIsr();


	//Symbol table is at known location
	//---------------------------------
	stab = (SharedMemorySymbolTable*) SHARED_MEMORY_BASE;

	//Wait until the monitor has placed data into shared memory
	//---------------------------------------------------------
	while (stab->init_complete == 0);

	functionTable = stab->ftable;
	critFuncData  = stab->critFuncData;

	//Put the location of the stack for the task in shared memory
	//-----------------------------------------------------------
	functionTable[0].stackAddress[CORE_ID] = &Derivative_AirbagModel_STACK;
	functionTable[1].stackAddress[CORE_ID] = &Derivative_AirbagModel_STACK;


	//Initialize the Matlab tasks
	//---------------------------
	FuelSensor_initialize(&FuelSensor_M,&FuelSensor_U,&FuelSensor_Y);


	//Initialize the control flow data structures
	//-------------------------------------------
	derivative_AirbagModel_SEM0 = OSSemCreate(derivative_AirbagModel_SEM0_INITCOND);


	//Declare the OS tasks
	///-------------------

	OSTaskCreateExt(FuelSensor_TASK, NULL, &FuelSensor_STACK[FuelSensor_STACKSIZE - 1],
				FuelSensor_PRIORITY, FuelSensor_PRIORITY,
				FuelSensor_STACK, FuelSensor_STACKSIZE, NULL,OS_TASK_OPT_STK_CLR);

	OSTaskCreateExt(Derivative_AirbagModel_TASK, NULL, &Derivative_AirbagModel_STACK[Derivative_AirbagModel_STACKSIZE - 1],
			Derivative_AirbagModel_PRIORITY, Derivative_AirbagModel_PRIORITY,
			Derivative_AirbagModel_STACK, Derivative_AirbagModel_STACKSIZE, NULL,OS_TASK_OPT_STK_CLR);

	//Notify the monitor that startup is completed
	//--------------------------------------------
	stab->core_ready[CORE_ID] = 1;

	OSStart();
	return 0;
}
