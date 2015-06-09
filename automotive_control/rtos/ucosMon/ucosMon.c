/*****************************************************************************
 * Includes
 *****************************************************************************/
#include <stdio.h> //for debugging only
#include <stddef.h>
#include <stdbool.h>
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
#define CruiseControlSystem_STACKSIZE 		(200 + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR)
#define TransferResult_STACKSIZE 			(220 + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR)
#define TractionControl_STACKSIZE 			(200 + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR)
#define DMA_STACKSIZE 						(200 + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR)

/*****************************************************************************
 * Task Priorities
 *****************************************************************************/
#define CruiseControlSystem_PRIORITY 		13
#define TransferResult_PRIORITY 			14
#define TractionControl_PRIORITY 			17
#define DMA_PRIORITY 						15

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

#define CORE0_SCRATCHPAD_GLOBAL_ADDRESS 			((void*)0x4203000)
#define CORE1_SCRATCHPAD_GLOBAL_ADDRESS 			((void*)0x4203000)
#define CORE0_SCRATCHPAD_START_ADDRESS 			((void*)0x4200000)
#define CORE1_SCRATCHPAD_START_ADDRESS 			((void*)0x4200000)


#define AirbagModel_COMPSTATUS_MASK 					0x2
#define Derivative_COMPSTATUS_MASK 						0x1


#define dmaReady_FLAG0_INITCOND 				0
#define dmaReady_FLAG0_CORE0_M0_BITMASK 		0x1
#define dmaReady_FLAG0_CORE0_M1_BITMASK 		0x2
#define dmaReady_FLAG0_CORE0_M2_BITMASK 		0x4
#define dmaReady_FLAG0_CORE0_DMAREADY 			0x8

#define dmaReady_FLAG0_CORE1_M0_BITMASK 		0x10
#define dmaReady_FLAG0_CORE1_M1_BITMASK 		0x20
#define dmaReady_FLAG0_CORE1_M2_BITMASK 		0x40
#define dmaReady_FLAG0_CORE1_DMAREADY 			0x80

#define dmaReady_FLAG0_TIMEOUT 					0
#define dmaReady_FLAG0_WAITTYPE					(OS_FLAG_WAIT_SET_ANY )
#define dmaReady_FLAG0_CORE0_CONDITION 				0x0f
#define dmaReady_FLAG0_CORE1_CONDITION 				0xf0

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
OS_STK DMA_STACK[DMA_STACKSIZE] __attribute__ ((section (".critical")));

/*****************************************************************************
 * Control Flow declarations
 *****************************************************************************/
OS_FLAG_GRP *TransferResult_FLAG0;
OS_EVENT *TractionControl_SEM0;
OS_FLAG_GRP *dmaReady_FLAG0;

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
	int action;
	void* sourceAddress;
	void* destAddress;
	int size;
} HandleDMAStruct;

#define DMA_CODE_DERIVATIVE			0
#define DMA_CODE_AIRBAGMODEL		1
#define DMA_CODE_NOACTION			255

HandleDMAStruct handleDMAStruct_0[3] = { { 0, DMA_CODE_NOACTION, 0, 0, 0 }, { 0,
		DMA_CODE_NOACTION, 0, 0, 0 }, { 0, DMA_CODE_NOACTION, 0, 0, 0 } };
HandleDMAStruct handleDMAStruct_1[2] = { { 1, DMA_CODE_NOACTION, 0, 0, 0 }, { 1,
		DMA_CODE_NOACTION, 0, 0, 0 } };

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

void sendDMA(void* sourceAddress, void* destAddress, int size, void *handle);
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
		OSTimeDlyHMSM(0, 0, 0, 20);
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
		printf("did traction control %d\n",perr);
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

bool dmaReady[NUMCORES];
OS_FLAGS dmaReadyFlag = dmaReady_FLAG0_CORE0_CONDITION
		| dmaReady_FLAG0_CORE1_CONDITION;

void dma_TASK(void* pdata) {

	handleDMAStruct_0[0].action = DMA_CODE_NOACTION;
	handleDMAStruct_0[0].sourceAddress = &dmaPackageStruct_0;
	handleDMAStruct_0[0].destAddress = CORE0_SCRATCHPAD_GLOBAL_ADDRESS;
	handleDMAStruct_0[0].size = sizeof(DMAPackageStruct);

	handleDMAStruct_0[1].action = DMA_CODE_DERIVATIVE;
	handleDMAStruct_0[1].sourceAddress = (void *) (0x495000);
	handleDMAStruct_0[1].destAddress = CORE0_SCRATCHPAD_START_ADDRESS;
	handleDMAStruct_0[1].size = 4096;

	handleDMAStruct_0[2].action = DMA_CODE_AIRBAGMODEL;
	handleDMAStruct_0[2].sourceAddress = CORE0_SCRATCHPAD_GLOBAL_ADDRESS;
	handleDMAStruct_0[2].destAddress = &dmaPackageStruct_0;
	handleDMAStruct_0[2].size = sizeof(DMAPackageStruct);

	handleDMAStruct_1[0].action = DMA_CODE_NOACTION;
	handleDMAStruct_1[0].sourceAddress = &dmaPackageStruct_0;
	handleDMAStruct_1[0].destAddress = CORE1_SCRATCHPAD_GLOBAL_ADDRESS;
	handleDMAStruct_1[0].size = sizeof(DMAPackageStruct);

	handleDMAStruct_1[1].action = DMA_CODE_DERIVATIVE;
	handleDMAStruct_1[1].sourceAddress = (void *) (0x463000);
	handleDMAStruct_1[1].destAddress = CORE1_SCRATCHPAD_START_ADDRESS;
	handleDMAStruct_1[1].size = 4096;

	INT8U perr;
	HandleDMAStruct *messagePending[NUMCORES];
	int i;
	for (i = 0; i < NUMCORES; i++) {
		dmaReady[i] = true;
		messagePending[i] = NULL;
	}
	while (1) {
		printf("dma task\n");
		OS_FLAGS sender = OSFlagPend(dmaReady_FLAG0, dmaReadyFlag,
				dmaReady_FLAG0_WAITTYPE, dmaReady_FLAG0_TIMEOUT, &perr);
		HandleDMAStruct *message = NULL;

		//Two cores handled separately
		//----------------------------

		//Core 0
		//------
		if (sender & dmaReady_FLAG0_CORE0_M0_BITMASK) {
			message = &handleDMAStruct_0[0];
			OSFlagPost(dmaReady_FLAG0, dmaReady_FLAG0_CORE0_M0_BITMASK,
					OS_FLAG_CLR, &perr);
		} else if (sender & dmaReady_FLAG0_CORE0_M1_BITMASK) {
			message = &handleDMAStruct_0[1];
			OSFlagPost(dmaReady_FLAG0, dmaReady_FLAG0_CORE0_M1_BITMASK,
					OS_FLAG_CLR, &perr);
		} else if (sender & dmaReady_FLAG0_CORE0_M2_BITMASK) {
			message = &handleDMAStruct_0[2];
			OSFlagPost(dmaReady_FLAG0, dmaReady_FLAG0_CORE0_M2_BITMASK,
					OS_FLAG_CLR, &perr);
		} else if (sender & dmaReady_FLAG0_CORE0_DMAREADY) {
			OSFlagPost(dmaReady_FLAG0, dmaReady_FLAG0_CORE0_DMAREADY,
					OS_FLAG_CLR, &perr);
			if (messagePending[0] != NULL) {
				message = messagePending[0];
				sendDMA(message->sourceAddress, message->destAddress,
						message->size, message);
				messagePending[0] = NULL;
				message = NULL;
				dmaReadyFlag |= dmaReady_FLAG0_CORE0_CONDITION;
			}
		}

		//send message
		if (message != NULL) {
			if (dmaReady[0]) {
				sendDMA(message->sourceAddress, message->destAddress,
						message->size, message);
			} else {
				dmaReadyFlag &= 0xF0 | dmaReady_FLAG0_CORE0_DMAREADY;
				messagePending[0] = message;
			}
		}

		//Core 1
		//------
		message = NULL;

		if (sender & dmaReady_FLAG0_CORE1_M0_BITMASK) {
			message = &handleDMAStruct_1[0];
			OSFlagPost(dmaReady_FLAG0, dmaReady_FLAG0_CORE1_M0_BITMASK,
					OS_FLAG_CLR, &perr);
		} else if (sender & dmaReady_FLAG0_CORE1_M1_BITMASK) {
			message = &handleDMAStruct_1[1];
			OSFlagPost(dmaReady_FLAG0, dmaReady_FLAG0_CORE1_M1_BITMASK,
					OS_FLAG_CLR, &perr);
		} else if (sender & dmaReady_FLAG0_CORE1_DMAREADY) {
			OSFlagPost(dmaReady_FLAG0, dmaReady_FLAG0_CORE1_DMAREADY,
					OS_FLAG_CLR, &perr);
			if (messagePending[1] != NULL) {
				message = messagePending[1];
				sendDMA(message->sourceAddress, message->destAddress,
						message->size, message);
				messagePending[1] = NULL;
				message = NULL;
				dmaReadyFlag |= dmaReady_FLAG0_CORE1_CONDITION;
			}
		}

		//send message
		if (message != NULL) {
			if (dmaReady[0]) {
				sendDMA(message->sourceAddress, message->destAddress,
						message->size, message);
			} else {
				dmaReadyFlag &= 0xF0 | dmaReady_FLAG0_CORE0_DMAREADY;
				messagePending[0] = message;
			}
		}

		printf("after clearing flags = %x\n",
				OSFlagQuery(dmaReady_FLAG0, &perr));

	}
}

volatile int derivativeMsgCount = 0;

void handleDMA(void* handle, void* data) {
	HandleDMAStruct *message = (HandleDMAStruct*) handle;
	int code = message->action;
	switch (code) {
	case DMA_CODE_DERIVATIVE:
		if (++derivativeMsgCount == 2) {
			int i;
			//Prepare information for core 0 and core 1
			//-----------------------
			for (i = 0; i < NUMCORES; i++) {
				critFuncData[i].priority = 0;
				critFuncData[i].tableIndex = DERIVATIVE_FUNC_TABLE_INDEX;
				critFuncData[i].tlbDataAddressPhys =
						CORE0_SCRATCHPAD_GLOBAL_ADDRESS;
				critFuncData[i].tlbDataAddressVirt =
						(void *) GLOBAL_DATA_REGION_BASE;
				critFuncData[i].tlbStackAddressPhys = CORE0_SCRATCHPAD_START_ADDRESS;
				critFuncData[i].tlbStackAddressVirt = (void *) (0x463000);
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

	int core = message->core;
	dmaReady[core] = true;
	INT8U perr;
	if (core == 0)
		OSFlagPost(dmaReady_FLAG0, dmaReady_FLAG0_CORE0_DMAREADY, OS_FLAG_SET,
				&perr);
	else if (core == 1)
		OSFlagPost(dmaReady_FLAG0, dmaReady_FLAG0_CORE1_DMAREADY, OS_FLAG_SET,
				&perr);
}

void sendDMA(void* sourceAddress, void* destAddress, int size, void *handle) {
	int rc;
	HandleDMAStruct *message = (HandleDMAStruct*) handle;
	if ((rc = alt_dma_txchan_send(txchan[(int) message->core], sourceAddress,
			size, NULL, NULL)) < 0) {
		printf("dma failed!\n");
	}

	/* Post the receive request */
	if ((rc = alt_dma_rxchan_prepare(rxchan[message->core], destAddress, size,
			handleDMA, handle)) < 0) {
		//Failure
	}
}
int result = 0;
/*****************************************************************************
 * TransferResult Task wrapper
 *
 * This task handles all critical task dataflow.
 *****************************************************************************/
void TransferResult_TASK(void* pdata) {

	while (1) {
		INT8U perr;
		OS_FLAGS sender = OSFlagPend(TransferResult_FLAG0,
				TransferResult_FLAG0_CONDITION, TransferResult_FLAG0_WAITTYPE,
				TransferResult_FLAG0_TIMEOUT, &perr);
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
			OSFlagPost(dmaReady_FLAG0,
					dmaReady_FLAG0_CORE0_M0_BITMASK
							| dmaReady_FLAG0_CORE0_M1_BITMASK, OS_FLAG_SET,
					&perr);
			//Derivative -> Core 1
			//--------------------
			OSFlagPost(dmaReady_FLAG0,
					dmaReady_FLAG0_CORE1_M0_BITMASK
							| dmaReady_FLAG0_CORE1_M1_BITMASK, OS_FLAG_SET,
					&perr);

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
			OSFlagPost(dmaReady_FLAG0, dmaReady_FLAG0_CORE0_M2_BITMASK, OS_FLAG_SET, &perr);
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
		result = status.successful_reg;
	}
	if (status.failed_reg) {
		int* cpu0_reset = (int*)PROCESSOR0_0_SW_RESET_0_BASE;
		int* cpu1_reset = (int*)PROCESSOR1_0_SW_RESET_0_BASE;
		*cpu0_reset = 1;
		*cpu1_reset = 1;
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
	dmaReady_FLAG0 = OSFlagCreate(dmaReady_FLAG0_INITCOND, &perr);

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

	OSTaskCreateExt(dma_TASK, NULL, &DMA_STACK[DMA_STACKSIZE - 1], DMA_PRIORITY,
			DMA_PRIORITY, DMA_STACK, DMA_STACKSIZE, NULL, OS_TASK_OPT_STK_CLR);
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
