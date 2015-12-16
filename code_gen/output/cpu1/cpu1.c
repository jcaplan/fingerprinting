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
#include "cpu1.h"
#include "reset_monitor.h"
#include "runtimeMonitor.h"





/**********************************
 * Global variable declarations
 **********************************/
/* for_loop_100000_0*/






/*****************************************************************************
 * Stack Declarations
 *****************************************************************************/
OS_STK for_loop_100000_0_STACK[FOR_LOOP_100000_0_STACKSIZE] __attribute__ ((section (".stack_bin_0")));





/*****************************************************************************
 * Execution time monitoring table
 *****************************************************************************/
rtMonitor_task rtMonTaskTable[NUM_TASKS] = {
	{ FOR_LOOP_100000_0_PRIORITY, 0, FOR_LOOP_100000_0_WCET_LOWERBOUND, false, true, "for_loop_100000_0" }
};





/*****************************************************************************
 * Control Flow declarations
 *****************************************************************************/
OS_EVENT *critical_SEM[1];





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

	int *PROCESSOR1_IRQ = (int *) PROCESSOR1_0_CPU_IRQ_0_BASE;
	*PROCESSOR1_IRQ = 0;
	OSSemPost(critical_SEM[taskIndex]);
}

static void initCpuIsr(void) {
	alt_ic_isr_register(PROCESSOR1_0_CPU_IRQ_0_IRQ_INTERRUPT_CONTROLLER_ID,
			PROCESSOR1_0_CPU_IRQ_0_IRQ, handleCPU, (void*) NULL, (void*) NULL);
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
 * for_loop_100000_0Task wrapper
 *****************************************************************************/
void for_loop_100000_0_TASK(void* pdata) {
	void *gp = stab->gp_address;
	void (*for_loop_100000_0Func)(int,
			for_loop_100000_0Struct*) = functionTable[FOR_LOOP_100000_0_TABLE_INDEX].address;
	int partnerCore = 0; /* static variable */

	while (1) {
		INT8U perr;
		OSSemPend(critical_SEM[FOR_LOOP_100000_0_TABLE_INDEX], 0, &perr);

		rtMonitorStartTask(FOR_LOOP_100000_0_RT_PRIO);

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

		void *args = functionTable[FOR_LOOP_100000_0_TABLE_INDEX].args;
		//Set the global pointer in case of compilation issues related
		//to global variables
		set_gp(gp);

		for_loop_100000_0Func(priority, args);
		//call the critical task
		//restore the original global pointer
		restore_gp();

		//set the flag for the OS context switch
		FprintActive = 0;


		rtMonitorEndTask(FOR_LOOP_100000_0_RT_PRIO);
	}
}






/*****************************************************************************
 * Memory Manager 
 *****************************************************************************/
void mem_manager_init(void) {
// For each critical task set up a position in the table
	MemoryManagerStruct *entry;

	// for_loop_100000_0
	entry = &memoryManagerTable[FOR_LOOP_100000_0_TABLE_INDEX];
	entry->disablePending = false;
	entry->disablePendSource = 0;
	entry->taskPriority = FOR_LOOP_100000_0_PRIORITY;
	entry->tlbDataLine = 0;
	entry->tlbStackLine = 1;
	entry->stackPhysicalAddress = (void*)0x463000;
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
	region[1].base = 0x464000/ 64;
	region[1].mask = 0x496000/ 64;
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
	functionTable[FOR_LOOP_100000_0_TABLE_INDEX].stackAddress[CORE_ID] = &for_loop_100000_0_STACK;

	critical_SEM[0] = OSSemCreate(0);
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

	INT8U perr;	OSTaskCreateExt(for_loop_100000_0_TASK, NULL,
			(OS_STK *)0x63794,
			FOR_LOOP_100000_0_PRIORITY, FOR_LOOP_100000_0_PRIORITY,
			(OS_STK *)0x63000, FOR_LOOP_100000_0_STACKSIZE, NULL,
			OS_TASK_OPT_STK_CLR);
	OSTaskNameSet(FOR_LOOP_100000_0_PRIORITY, (INT8U *)"for_loop_100000_0", &perr);

	resetMonitorCoreReg(CORE_ID);

	OSStart();
	return 0;
}






