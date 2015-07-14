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
#include "cpu1.h"
#include "reset_monitor.h"
#include "FuelSensor.h"





/**********************************
 * Global variable declarations
 **********************************/
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





/*****************************************************************************
 * Stack Declarations
 *****************************************************************************/
OS_STK AirbagModel_STACK[AirbagModel_STACKSIZE];__attribute__ ((section (".stack_bin_0")))
OS_STK Derivative_STACK[Derivative_STACKSIZE];__attribute__ ((section (".stack_bin_0")))
OS_STK FuelSensor_STACK[FuelSensor_STACKSIZE];





/*****************************************************************************
 * Control Flow declarations
 *****************************************************************************/





/*****************************************************************************
 * Shared memory interface with monitor
 *****************************************************************************/
FunctionTable *functionTable;
CriticalFunctionData *critFuncData;
SharedMemorySymbolTable *stab;
