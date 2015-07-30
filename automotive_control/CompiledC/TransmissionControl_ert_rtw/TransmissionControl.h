/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: TransmissionControl.h
 *
 * Code generated for Simulink model 'TransmissionControl'.
 *
 * Model version                  : 1.8
 * Simulink Coder version         : 8.8 (R2015a) 09-Feb-2015
 * C/C++ source code generated on : Thu Jul 30 17:01:34 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ASIC/FPGA->ASIC/FPGA
 * Emulation hardware selection:
 *    Differs from embedded hardware (Generic->32-bit Embedded Processor)
 * Code generation objectives:
 *    1. Safety precaution
 *    2. MISRA-C:2004 guidelines
 * Validation result: Not run
 */

#ifndef RTW_HEADER_TransmissionControl_h_
#define RTW_HEADER_TransmissionControl_h_
#include "rtwtypes.h"
#include "multiword_types.h"
#ifndef TransmissionControl_COMMON_INCLUDES_
# define TransmissionControl_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* TransmissionControl_COMMON_INCLUDES_ */

/* Shared type includes */
#include "multiword_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Forward declaration for rtModel */
typedef struct tag_RTM_TransmissionControl_T RT_MODEL_TransmissionControl_T;

/* External inputs (root inport signals with auto storage) */
typedef struct {
  real32_T FluidSensor;                /* '<Root>/Fluid Sensor' */
} ExtU_TransmissionControl_T;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  boolean_T WarningLight;              /* '<Root>/Warning Light' */
} ExtY_TransmissionControl_T;

/* Parameters (auto storage) */
struct P_TransmissionControl_T_ {
  real32_T LowFuelSensor_const;        /* Mask Parameter: LowFuelSensor_const
                                        * Referenced by: '<S1>/Constant'
                                        */
};

/* Parameters (auto storage) */
typedef struct P_TransmissionControl_T_ P_TransmissionControl_T;

/* Real-time Model Data Structure */
struct tag_RTM_TransmissionControl_T {
  const char_T * volatile errorStatus;

  /*
   * ModelData:
   * The following substructure contains information regarding
   * the data used in the model.
   */
  struct {
    P_TransmissionControl_T *defaultParam;
  } ModelData;
};

/* Model entry point functions */
extern void TransmissionControl_initialize(RT_MODEL_TransmissionControl_T *const
  TransmissionControl_M, ExtU_TransmissionControl_T *TransmissionControl_U,
  ExtY_TransmissionControl_T *TransmissionControl_Y);
extern void TransmissionControl_step(RT_MODEL_TransmissionControl_T *const
  TransmissionControl_M, ExtU_TransmissionControl_T *TransmissionControl_U,
  ExtY_TransmissionControl_T *TransmissionControl_Y);
extern void TransmissionControl_terminate(RT_MODEL_TransmissionControl_T *const
  TransmissionControl_M);

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'TransmissionControl'
 * '<S1>'   : 'TransmissionControl/Low Fuel Sensor'
 */
#endif                                 /* RTW_HEADER_TransmissionControl_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
