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
#include "mem_manager.h"
#include "ucos0.h"

/*****************************************************************************
 * TransmissionControl
 *****************************************************************************/
RT_MODEL_TransmissionControl_T TransmissionControl_M;
ExtU_TransmissionControl_T TransmissionControl_U;
ExtY_TransmissionControl_T TransmissionControl_Y;
P_TransmissionControl_T transmissionControl_dfaultParam;

/*****************************************************************************
 * CollisionAvoidance
 *****************************************************************************/
RT_MODEL_CollisionAvoidance_T CollisionAvoidance_M;
ExtU_CollisionAvoidance_T CollisionAvoidance_U;
ExtY_CollisionAvoidance_T CollisionAvoidance_Y;
P_CollisionAvoidance_T collisionAvoidance_defaultParam;
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
OS_EVENT *derivative_AirbagModel_SEM0;


int *core0_IRQ = (int *) PROCESSOR0_0_CPU_IRQ_0_BASE;

/*****************************************************************************
 * Interrupt from other cores
 *****************************************************************************/
static void handleCPU(void* context) {
	if (critFuncData[CORE_ID].tableIndex == DERIVATIVE_FUNC_TABLE_INDEX){
		//The monitor will provide a translation mapping for the stack
		//and for the global data... two translations received in data structure


		//Dont set anything here explicitly
		//Set the entries in the memory manager

		updateMemoryManagerTable(derivate_airbagModel_memoryTableIndex, &critFuncData[CORE_ID]);


		*core0_IRQ = 0;
		OSSemPost(derivative_AirbagModel_SEM0);
	}
}

static void initCpuIsr(void) {
	alt_ic_isr_register(PROCESSOR0_0_CPU_IRQ_0_IRQ_INTERRUPT_CONTROLLER_ID,
			PROCESSOR0_0_CPU_IRQ_0_IRQ, handleCPU, (void*) NULL,
			(void*) NULL);
}

/*****************************************************************************
 * CollisionAvoidance Task wrapper
 *****************************************************************************/
void CollisionAvoidance_TASK(void* pdata){
	while(1){
		CollisionAvoidance_step(&CollisionAvoidance_M,&CollisionAvoidance_U,&CollisionAvoidance_Y);
		OSTimeDlyHMSM(0,0,0,150);
	}
}

/*****************************************************************************
 * TransmissionControl Task wrapper
 *****************************************************************************/
void TransmissionControl_TASK(void* pdata){
	while(1){
		TransmissionControl_step(&TransmissionControl_M,&TransmissionControl_U,&TransmissionControl_Y);
		OSTimeDlyHMSM(0,0,0,150);
	}
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


		int priority = critFuncData->priority;

		//Retrieve the arguments before changing the GP

		void *args = functionTable[DERIVATIVE_FUNC_TABLE_INDEX].args;
		//Set the global pointer in case of compilation issues related
		//to global variables
		set_gp(gp);

		derivativeFunc(priority,args);
		//call the critical task
		//restore the original global pointer
		restore_gp();


		//Do the airbag part
		// Set default block size for fingerprinting
		fprint_set_block_size(airbagModel_blocksize);
		//TODO The TLB must be appropriately configured

		//Set the global pointer in case of compilation issues related
		//to global variables
		args = functionTable[AIRBAGMODEL_FUNC_TABLE_INDEX].args;
		set_gp(gp);

		airbagModelFunc(priority+1,args);
		//call the critical task
		//restore the original global pointer
		restore_gp();
		disableTlbLine(0);
		//Restore the callee saved registers
		context_restore(registers);

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

	CollisionAvoidance_M.ModelData.defaultParam = &collisionAvoidance_defaultParam;
	CollisionAvoidance_M.ModelData.dwork = &collisionAvoidance_dwork;
	CollisionAvoidance_initialize(&CollisionAvoidance_M, &CollisionAvoidance_U, &CollisionAvoidance_Y);

	TransmissionControl_M.ModelData.defaultParam = &transmissionControl_dfaultParam;
	TransmissionControl_initialize(&TransmissionControl_M,&TransmissionControl_U,&TransmissionControl_Y);

	//Initialize the control flow data structures
	//-------------------------------------------
	derivative_AirbagModel_SEM0 = OSSemCreate(derivative_AirbagModel_SEM0_INITCOND);





	//Start up the MPU
	//----------------
	mem_manager_init();

//	nios2_mpu_data_init();
//	nios2_mpu_inst_init();




	//Declare the OS tasks
	///-------------------
	OSTaskCreateExt(CollisionAvoidance_TASK, NULL, &CollisionAvoidance_STACK[CollisionAvoidance_STACKSIZE - 1],
				CollisionAvoidance_PRIORITY, CollisionAvoidance_PRIORITY,
				CollisionAvoidance_STACK, CollisionAvoidance_STACKSIZE, NULL,OS_TASK_OPT_STK_CLR);

	OSTaskCreateExt(TransmissionControl_TASK, NULL, &TransmissionControl_STACK[TransmissionControl_STACKSIZE - 1],
				TransmissionControl_PRIORITY, TransmissionControl_PRIORITY,
				TransmissionControl_STACK, TransmissionControl_STACKSIZE, NULL,OS_TASK_OPT_STK_CLR);


	//The stack must look like it matches
	OSTaskCreateExt(Derivative_AirbagModel_TASK, NULL, (OS_STK*)(0x00463000 + 4*(Derivative_AirbagModel_STACKSIZE - 2)),
				Derivative_AirbagModel_PRIORITY, Derivative_AirbagModel_PRIORITY,
				(OS_STK *)0x00463000, Derivative_AirbagModel_STACKSIZE, NULL,OS_TASK_OPT_STK_CLR);
//	OSTaskCreateExt(Derivative_AirbagModel_TASK, NULL, &Derivative_AirbagModel_STACK[Derivative_AirbagModel_STACKSIZE - 1],
//				Derivative_AirbagModel_PRIORITY, Derivative_AirbagModel_PRIORITY,
//				Derivative_AirbagModel_STACK, Derivative_AirbagModel_STACKSIZE, NULL,OS_TASK_OPT_STK_CLR);

	//Notify the monitor that startup is completed
	//--------------------------------------------
	stab->core_ready[CORE_ID] = 1;

	OSStart();
	return 0;
}
