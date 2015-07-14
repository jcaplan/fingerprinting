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
#include "CollisionAvoidance.h"
#include "TransmissionControl.h"





/**********************************
 * Global variable declarations
 **********************************/
static RT_MODEL_CollisionAvoidance_T CollisionAvoidance_M_;
static RT_MODEL_CollisionAvoidance_T *const CollisionAvoidance_M =  &CollisionAvoidance_M_;              /* Real-time model */
static P_CollisionAvoidance_T CollisionAvoidance_P = {  0.0,                                 /* Mask Parameter: DiscreteDerivative_ICPrevScaled
                                        * Referenced by: '<S3>/UD'
                                        */
  0.0,                                 /* Mask Parameter: DiscreteDerivative1_ICPrevScale
                                        * Referenced by: '<S4>/UD'
                                        */
  5.0,                                 /* Computed Parameter: TSamp_WtEt
                                        * Referenced by: '<S3>/TSamp'
                                        */
  5.0,                                 /* Computed Parameter: TSamp_WtEt_m
                                        * Referenced by: '<S4>/TSamp'
                                        */
  0.0                                  /* Expression: 0
                                        * Referenced by: '<S2>/Constant'
                                        */
};                                     /* Modifiable parameters */
static DW_CollisionAvoidance_T CollisionAvoidance_DW;/* Observable states */
static ExtU_CollisionAvoidance_T CollisionAvoidance_U;/* External inputs */
static ExtY_CollisionAvoidance_T CollisionAvoidance_Y;/* External outputs */
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
OS_STK AirbagModel_STACK[AirbagModel_STACKSIZE];__attribute__ ((section (".stack_bin_0")))
OS_STK Derivative_STACK[Derivative_STACKSIZE];__attribute__ ((section (".stack_bin_0")))
OS_STK CollisionAvoidance_STACK[CollisionAvoidance_STACKSIZE];
OS_STK TransmissionControl_STACK[TransmissionControl_STACKSIZE];





/*****************************************************************************
 * Control Flow declarations
 *****************************************************************************/





/*****************************************************************************
 * Shared memory interface with monitor
 *****************************************************************************/
FunctionTable *functionTable;
CriticalFunctionData *critFuncData;
SharedMemorySymbolTable *stab;
