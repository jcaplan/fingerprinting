/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: Derivative.h
 *
 * Code generated for Simulink model 'Derivative'.
 *
 * Model version                  : 1.6
 * Simulink Coder version         : 8.8 (R2015a) 09-Feb-2015
 * C/C++ source code generated on : Thu Jul 30 17:12:19 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Generic->32-bit Embedded Processor
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_Derivative_h_
#define RTW_HEADER_Derivative_h_
#include "rtwtypes.h"
#include <string.h>
#ifndef Derivative_COMMON_INCLUDES_
# define Derivative_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* Derivative_COMMON_INCLUDES_ */

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Forward declaration for rtModel */
typedef struct tag_RTM_Derivative_T RT_MODEL_Derivative_T;

/* Block states (auto storage) for system '<Root>' */
typedef struct {
  real32_T UD_DSTATE;                  /* '<S2>/UD' */
} DW_Derivative_T;

/* External inputs (root inport signals with auto storage) */
typedef struct {
  real32_T In1;                        /* '<Root>/In1' */
} ExtU_Derivative_T;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  real32_T Out1;                       /* '<Root>/Out1' */
} ExtY_Derivative_T;

/* Parameters (auto storage) */
struct P_Derivative_T_ {
  real32_T DiscreteDerivative_ICPrevScaled;/* Mask Parameter: DiscreteDerivative_ICPrevScaled
                                            * Referenced by: '<S2>/UD'
                                            */
  real32_T TSamp_WtEt;                 /* Computed Parameter: TSamp_WtEt
                                        * Referenced by: '<S2>/TSamp'
                                        */
};

/* Parameters (auto storage) */
typedef struct P_Derivative_T_ P_Derivative_T;

/* Real-time Model Data Structure */
struct tag_RTM_Derivative_T {
  const char_T * volatile errorStatus;

  /*
   * ModelData:
   * The following substructure contains information regarding
   * the data used in the model.
   */
  struct {
    P_Derivative_T *defaultParam;
    DW_Derivative_T *dwork;
  } ModelData;
};

/* Model entry point functions */
extern void Derivative_initialize(RT_MODEL_Derivative_T *const Derivative_M,
  ExtU_Derivative_T *Derivative_U, ExtY_Derivative_T *Derivative_Y);
extern void Derivative_step(RT_MODEL_Derivative_T *const Derivative_M,
  ExtU_Derivative_T *Derivative_U, ExtY_Derivative_T *Derivative_Y);
extern void Derivative_terminate(RT_MODEL_Derivative_T *const Derivative_M);

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
 * '<Root>' : 'Derivative'
 * '<S1>'   : 'Derivative/Derivative'
 * '<S2>'   : 'Derivative/Derivative/Discrete Derivative'
 */
#endif                                 /* RTW_HEADER_Derivative_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
