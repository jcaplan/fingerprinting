/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: TractionControl.h
 *
 * Code generated for Simulink model 'TractionControl'.
 *
 * Model version                  : 1.11
 * Simulink Coder version         : 8.8 (R2015a) 09-Feb-2015
 * C/C++ source code generated on : Thu Jul 30 17:13:16 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ASIC/FPGA->ASIC/FPGA
 * Emulation hardware selection:
 *    Differs from embedded hardware (Generic->32-bit Embedded Processor)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_TractionControl_h_
#define RTW_HEADER_TractionControl_h_
#include "rtwtypes.h"
#include "multiword_types.h"
#include <string.h>
#ifndef TractionControl_COMMON_INCLUDES_
# define TractionControl_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* TractionControl_COMMON_INCLUDES_ */

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
typedef struct tag_RTM_TractionControl_T RT_MODEL_TractionControl_T;

/* External inputs (root inport signals with auto storage) */
typedef struct {
  real32_T CurrentSpeedPoweredAxle;    /* '<Root>/Current Speed(Powered Axle)' */
  real32_T SpeedUnpoweredAxle;         /* '<Root>/Speed(Unpowered Axle)' */
} ExtU_TractionControl_T;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  real32_T ControlFeedback;            /* '<Root>/Control Feedback' */
} ExtY_TractionControl_T;

/* Parameters (auto storage) */
struct P_TractionControl_T_ {
  real32_T CompareToConstant_const;    /* Mask Parameter: CompareToConstant_const
                                        * Referenced by: '<S1>/Constant'
                                        */
};

/* Parameters (auto storage) */
typedef struct P_TractionControl_T_ P_TractionControl_T;

/* Real-time Model Data Structure */
struct tag_RTM_TractionControl_T {
  const char_T * volatile errorStatus;

  /*
   * ModelData:
   * The following substructure contains information regarding
   * the data used in the model.
   */
  struct {
    P_TractionControl_T *defaultParam;
  } ModelData;
};

/* Model entry point functions */
extern void TractionControl_initialize(RT_MODEL_TractionControl_T *const
  TractionControl_M, ExtU_TractionControl_T *TractionControl_U,
  ExtY_TractionControl_T *TractionControl_Y);
extern void TractionControl_step(RT_MODEL_TractionControl_T *const
  TractionControl_M, ExtU_TractionControl_T *TractionControl_U,
  ExtY_TractionControl_T *TractionControl_Y);
extern void TractionControl_terminate(RT_MODEL_TractionControl_T *const
  TractionControl_M);

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
 * '<Root>' : 'TractionControl'
 * '<S1>'   : 'TractionControl/Compare To Constant'
 */
#endif                                 /* RTW_HEADER_TractionControl_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
