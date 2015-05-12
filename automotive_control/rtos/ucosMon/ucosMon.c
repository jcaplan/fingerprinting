/*****************************************************************************
 * Includes
 *****************************************************************************/
#include <stdio.h> //for debugging only
#include <stddef.h>
#include "includes.h"
#include "sys/alt_dma.h"
#include <altera_avalon_mutex.h>
#include "fingerprint.h"
#include "critical.h"
#include "AirbagModel.h"
#include "CruiseControlSystem.h"
#include "Derivative.h"
#include "TractionControl.h"
#include "shared_mem.h"
#include "gp.h"

/*****************************************************************************
 * Defines
 *****************************************************************************/

//Nios IDE doesn't seem to recognize NULL
#define NULL ((void *)0)

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
#define CruiseControlSystem_STACKSIZE 		(156 + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR)
#define TransferResult_STACKSIZE 			(208 + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR)
#define TractionControl_STACKSIZE 			(136 + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR)

/*****************************************************************************
 * Task Priorities
 *****************************************************************************/
#define CruiseControlSystem_PRIORITY 		13
#define TransferResult_PRIORITY 			14
#define TractionControl_PRIORITY 			15

/*****************************************************************************
 * Task control flow conditions
 *****************************************************************************/
#define TransferResult_FLAG0_CONDITION 			0xf
#define TransferResult_FLAG0_WAITTYPE 			(OS_FLAG_WAIT_SET_ANY | OS_FLAG_CONSUME)
#define TransferResult_FLAG0_TIMEOUT 			0
#define TransferResult_FLAG0_SENDER1_BITMASK	((OS_FLAGS)0x1)
#define TransferResult_FLAG0_SENDER2_BITMASK	((OS_FLAGS)0x2)
#define TransferResult_FLAG0_SENDER3_BITMASK	((OS_FLAGS)0x4)
#define TransferResult_FLAG0_INITCOND			0

#define TractionControl_SEM0_INITCOND 			0

#define CORE0_SCRATCHPAD_STARTADDRESS 			((void*)0x4203000)
#define CORE1_SCRATCHPAD_STARTADDRESS 			((void*)0x4203000)

#define AirbagModel_COMPSTATUS_MASK 					0x1

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

/*****************************************************************************
 * Stack Declarations
 * Only tasks executing on this core require stacks
 *****************************************************************************/
OS_STK CruiseControlSystem_STACK[CruiseControlSystem_STACKSIZE] __attribute__ ((section (".critical")));
OS_STK TractionControl_STACK[TractionControl_STACKSIZE] __attribute__ ((section (".critical")));
OS_STK TransferResult_STACK[TransferResult_STACKSIZE] __attribute__ ((section (".critical")));

/*****************************************************************************
 * Control Flow declarations
 *****************************************************************************/
OS_FLAG_GRP *TransferResult_FLAG0;
OS_EVENT *TractionControl_SEM0;

/*****************************************************************************
 * Pointers to interrupt other cores
 *****************************************************************************/
int *core0_IRQ = (int *) PROCESSOR0_0_CPU_IRQ_0_BASE;
int *core1_IRQ = (int *) PROCESSOR1_0_CPU_IRQ_0_BASE;

/*****************************************************************************
 * DMA channels for each core
 *****************************************************************************/
alt_dma_txchan txchan[2];
alt_dma_rxchan rxchan[2];

typedef struct {
	int core;
	int code;
} HandleDMAStruct;

#define DMA_CODE_DERIVATIVE			0
#define DMA_CODE_AIRBAGMODEL		1
HandleDMAStruct handleDMAStruct_0 = { 0, 0 };
HandleDMAStruct handleDMAStruct_1 = { 1, 0 };
HandleDMAStruct handleDMAStruct_M = { 255, 0 };

typedef struct {
	AirbagModelStruct airbagModelStruct_0;
	DerivativeStruct derivativeStruct_0;
} DMAPackageStruct;

DMAPackageStruct dmaPackageStruct_0 __attribute__ ((section (".global_data")));

/*****************************************************************************
 * Hardware mutex
 *****************************************************************************/
alt_mutex_dev* mutex;


/*****************************************************************************
 * Function table for critical tasks
 *****************************************************************************/
SharedMemorySymbolTable shared_stab __attribute__ ((section (".shared")));
FunctionTable functionTable[2] __attribute__ ((section (".shared")));
CriticalFunctionData critFuncData[NUMCORES] __attribute__ ((section (".shared")));


/*****************************************************************************
 * CruiseControlSystem Task wrapper
 *****************************************************************************/
void CruiseControlSystem_TASK(void* pdata) {
	while (1) {
		printf("doing cruise control\n");
		CruiseControlSystem_step(&CruiseControlSystem_M, &CruiseControlSystem_U,
				&CruiseControlSystem_Y);
		INT8U perr = 0;
		OSFlagPost(TransferResult_FLAG0, TransferResult_FLAG0_SENDER1_BITMASK,
				OS_FLAG_SET, &perr);
		OSTimeDlyHMSM(0, 0, 0, 40);
	}
}

/*****************************************************************************
 * TractionControl Task wrapper
 *****************************************************************************/
void TractionControl_TASK(void* pdata) {
	while (1) {
		INT8U perr;
		OSSemPend(TractionControl_SEM0, 0, &perr);
		TractionControl_step(&TractionControl_M, &TractionControl_U,
				&TractionControl_Y);
		OSFlagPost(TransferResult_FLAG0, TransferResult_FLAG0_SENDER2_BITMASK,
				OS_FLAG_SET, &perr);
		printf("did traction control\n");
	}
}

/*****************************************************************************
 * DMA functions
 *****************************************************************************/

void initDMA(void) {

	txchan[0] = alt_dma_txchan_open("/dev/processor0_0_dma_0");
	rxchan[0] = alt_dma_rxchan_open("/dev/processor0_0_dma_0");

	txchan[1] = alt_dma_txchan_open("/dev/processor1_0_dma_0");
	rxchan[1] = alt_dma_rxchan_open("/dev/processor1_0_dma_0");

}

volatile int derivativeMsgCount = 0;

void handleDMA(void* handle, void* data) {
	HandleDMAStruct *message = (HandleDMAStruct*) handle;
	int code = message->code;
	switch (code) {
	case DMA_CODE_DERIVATIVE:
		if (++derivativeMsgCount == 2) {
			int i;
			//Prepare information for core 0 and core 1
			//-----------------------
			for (i = 0; i < NUMCORES; i++) {
				critFuncData[i].priority = 0;
				critFuncData[i].tableIndex = DERIVATIVE_FUNC_TABLE_INDEX;
				critFuncData[i].tlbTranslationPhys = CORE0_SCRATCHPAD_STARTADDRESS;
				critFuncData[i].tlbTranslationVirt = (void *)GLOBAL_DATA_REGION_SPAN;

			}

			critFuncData[0].partnerCore = 1;
			critFuncData[1].partnerCore = 0;
			//Notify both cores
			//-----------------
			*core0_IRQ = 1;
			*core1_IRQ = 1;


			//Reset the counter
			//-----------------
			derivativeMsgCount = 0;
		}
		break;
	case DMA_CODE_AIRBAGMODEL:
		//No action required
		//------------------
		break;
	default:
		//shouldn't arrive here;
		break;
	}
}

void sendDMA(void* sourceAddress, void* destAddress, int size, void *handle) {
	int rc;
	HandleDMAStruct *message = (HandleDMAStruct*) handle;
	if ((rc = alt_dma_txchan_send(txchan[(int)message->core], sourceAddress, size,
			NULL, NULL)) < 0) {
		//failure
	}

	/* Post the receive request */
	if ((rc = alt_dma_rxchan_prepare(rxchan[message->core], destAddress, size,
			handleDMA, handle)) < 0) {
		//Failure
	}
}

/*****************************************************************************
 * TransferResult Task wrapper
 *
 * This task handles all critical task dataflow.
 *****************************************************************************/
void TransferResult_TASK(void* pdata) {
	while (1) {
		printf("hi\n");
		INT8U perr;
		OS_FLAGS sender = OSFlagPend(TransferResult_FLAG0,
				TransferResult_FLAG0_CONDITION, TransferResult_FLAG0_WAITTYPE,
				TransferResult_FLAG0_TIMEOUT, &perr);
		printf("bye\n");
		switch (sender) {
		case TransferResult_FLAG0_SENDER1_BITMASK:

			//CruiseControlSystem -> Derivative
			//---------------------------------
			dmaPackageStruct_0.derivativeStruct_0.Derivative_U.In1 =
					CruiseControlSystem_Y.Out1;

			//CruiseControlSystem -> TractionControl
			//--------------------------------------
			TractionControl_U.CurrentSpeedPoweredAxle =
					CruiseControlSystem_Y.Out1;

			//Derivative,Airbag -> Core 0
			//Starting assumption: no instruction scratchpad
			//Move only the data
			//Only one critical task in scratchpad at a time
			//--------------------
			//TODO the stack must also be moved
			handleDMAStruct_0.code = DMA_CODE_DERIVATIVE;
			sendDMA(&dmaPackageStruct_0, CORE0_SCRATCHPAD_STARTADDRESS,
					sizeof(DMAPackageStruct), &handleDMAStruct_0);

			sendDMA((void *)(0x495000), CORE0_SCRATCHPAD_STARTADDRESS + 1024,
					sizeof(DMAPackageStruct), &handleDMAStruct_0);


			//Derivative -> Core 1
			//--------------------
			handleDMAStruct_1.code = DMA_CODE_DERIVATIVE;
			sendDMA(&dmaPackageStruct_0, CORE1_SCRATCHPAD_STARTADDRESS,
					sizeof(DMAPackageStruct), &handleDMAStruct_1);
			printf("sender 1\n");
			OSSemPost(TractionControl_SEM0);
			break;
		case TransferResult_FLAG0_SENDER2_BITMASK:
			//TractionControl -> CruiseControl
			//--------------------------------
			CruiseControlSystem_U.In2 = TractionControl_Y.ControlFeedback;
			printf("sender 2\n");
			break;
		case TransferResult_FLAG0_SENDER3_BITMASK:
			//Core 0 -> AirbagModel_Y.output
			//------------------------------
			handleDMAStruct_M.code = DMA_CODE_AIRBAGMODEL;
			sendDMA(CORE0_SCRATCHPAD_STARTADDRESS, &dmaPackageStruct_0,
					sizeof(DMAPackageStruct), &handleDMAStruct_M);
			//TODO AirbagModel_address... needs to be assigned at startup
			printf("sender 3\n");
			break;
		}
	}
}

/*****************************************************************************
 * Comparator ISR
 *****************************************************************************/

static void handleCompISR(void* context) {
	Fprint_Status status;
	fprint_status(&status);

	//Assume static mapping of fingeprint tasks for now
	//There is only one possible task that can set this off in this example
	//---------------------------------------------------------------------
	INT8U perr;
	if (status.successful_reg & AirbagModel_COMPSTATUS_MASK) {
		OSFlagPost(TransferResult_FLAG0, TransferResult_FLAG0_SENDER3_BITMASK,
				OS_FLAG_SET, &perr);
	} else if (status.failed_reg & AirbagModel_COMPSTATUS_MASK) {
		//take failed action
		printf("failed\n");
	} else {
		//wtf
	}

	fprint_reset_irq();
}

static void initCompIsr(void) {
	alt_ic_isr_register(CFPU_0_CSR_IRQ_INTERRUPT_CONTROLLER_ID, CFPU_0_CSR_IRQ,
			handleCompISR, (void*) NULL, (void*) NULL);
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
	functionTable[0].address = Derivative_CT;
	functionTable[0].args = &dmaPackageStruct_0.derivativeStruct_0;
	functionTable[0].blocksize = 0xfff;
	functionTable[1].address = AirbagModel_CT;
	functionTable[1].args = &dmaPackageStruct_0.airbagModelStruct_0;
	functionTable[1].blocksize = 0xfff;

	//Initialize the runtime interface


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
	CruiseControlSystem_initialize(&CruiseControlSystem_M,
			&CruiseControlSystem_U, &CruiseControlSystem_Y);
	TractionControl_initialize(&TractionControl_M, &TractionControl_U,
			&TractionControl_Y);
	AirbagModelStruct airbagModelStruct_0 =
			dmaPackageStruct_0.airbagModelStruct_0;
	AirbagModel_initialize(&airbagModelStruct_0.AirbagModel_M,
			&airbagModelStruct_0.AirbagModel_U,
			&airbagModelStruct_0.AirbagModel_Y);
	DerivativeStruct derivativeStruct_0 = dmaPackageStruct_0.derivativeStruct_0;
	Derivative_initialize(&derivativeStruct_0.Derivative_M,
			&derivativeStruct_0.Derivative_U, &derivativeStruct_0.Derivative_Y);

	//Initialize the control flow data structures
	//-------------------------------------------
	INT8U perr;
	TransferResult_FLAG0 = OSFlagCreate(TransferResult_FLAG0_INITCOND, &perr);
	TractionControl_SEM0 = OSSemCreate(TractionControl_SEM0_INITCOND);

	//Declare the OS tasks
	///-------------------
	OSTaskCreateExt(CruiseControlSystem_TASK, NULL,
			&CruiseControlSystem_STACK[CruiseControlSystem_STACKSIZE - 1],
			CruiseControlSystem_PRIORITY, CruiseControlSystem_PRIORITY,
			CruiseControlSystem_STACK, CruiseControlSystem_STACKSIZE, NULL,
			OS_TASK_OPT_STK_CLR);

	OSTaskCreateExt(TractionControl_TASK, NULL,
			&TractionControl_STACK[TractionControl_STACKSIZE - 1],
			TractionControl_PRIORITY, TractionControl_PRIORITY,
			TractionControl_STACK, TractionControl_STACKSIZE, NULL,
			OS_TASK_OPT_STK_CLR);

	OSTaskCreateExt(TransferResult_TASK, NULL,
			&TransferResult_STACK[TransferResult_STACKSIZE - 1],
			TransferResult_PRIORITY, TransferResult_PRIORITY,
			TransferResult_STACK, TransferResult_STACKSIZE, NULL,
			OS_TASK_OPT_STK_CLR);


	//Wait for confirmation that other cores have completed their startup routines
	//----------------------------------------------------------------------------
	int p0 = 0, p1 = 0;
	while ((p0 != 1) || (p1 != 1)) {
		altera_avalon_mutex_lock(mutex, 1);
		{
			p0 = shared_stab.core_ready[0];
			p1 = shared_stab.core_ready[1];
		}
		altera_avalon_mutex_unlock(mutex);
	}

	//Start the OS
	//------------
	OSStart();

	//Never get here
	//--------------
	return 0;
}
