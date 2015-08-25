/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: AirbagModel.h
 *
 * Code generated for Simulink model 'AirbagModel'.
 *
 * Model version                  : 1.13
 * Simulink Coder version         : 8.8 (R2015a) 09-Feb-2015
 * C/C++ source code generated on : Thu Jul 30 17:09:53 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Generic->32-bit Embedded Processor
 * Emulation hardware selection:
 *    Differs from embedded hardware (MATLAB Host)
 * Code generation objectives:
 *    1. MISRA-C:2004 guidelines
 *    2. Safety precaution
 * Validation result: Not run
 */

#ifndef RTW_HEADER_AirbagModel_h_
#define RTW_HEADER_AirbagModel_h_
#include <string.h>
#ifndef AirbagModel_COMMON_INCLUDES_
# define AirbagModel_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* AirbagModel_COMMON_INCLUDES_ */

#include "AirbagModel_types.h"
#include "rt_defines.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Block states (auto storage) for system '<Root>' */
typedef struct {
  real32_T DiscreteTimeIntegrator1_DSTATE;/* '<Root>/Discrete-Time Integrator1' */
  real32_T DiscreteTimeIntegrator_DSTATE;/* '<Root>/Discrete-Time Integrator' */
} DW_AirbagModel_T;

/* External inputs (root inport signals with auto storage) */
typedef struct {
  real32_T Force;                      /* '<Root>/Force' */
} ExtU_AirbagModel_T;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  boolean_T ActivateAirbag;            /* '<Root>/Activate Airbag ' */
} ExtY_AirbagModel_T;

/* Parameters (auto storage) */
struct P_AirbagModel_T_ {
  real32_T CompareToConstant_const;    /* Mask Parameter: CompareToConstant_const
                                        * Referenced by: '<S1>/Constant'
                                        */
  real32_T DiscreteTimeIntegrator1_gainval;/* Computed Parameter: DiscreteTimeIntegrator1_gainval
                                            * Referenced by: '<Root>/Discrete-Time Integrator1'
                                            */
  real32_T DiscreteTimeIntegrator1_IC; /* Computed Parameter: DiscreteTimeIntegrator1_IC
                                        * Referenced by: '<Root>/Discrete-Time Integrator1'
                                        */
  real32_T DiscreteTimeIntegrator_gainval;/* Computed Parameter: DiscreteTimeIntegrator_gainval
                                           * Referenced by: '<Root>/Discrete-Time Integrator'
                                           */
  real32_T DiscreteTimeIntegrator_IC;  /* Computed Parameter: DiscreteTimeIntegrator_IC
                                        * Referenced by: '<Root>/Discrete-Time Integrator'
                                        */
  real32_T Gain_Gain;                  /* Computed Parameter: Gain_Gain
                                        * Referenced by: '<S2>/Gain'
                                        */
  real32_T Gain_Gain_b;                /* Computed Parameter: Gain_Gain_b
                                        * Referenced by: '<Root>/Gain'
                                        */
  real32_T Dividemass_Gain;            /* Computed Parameter: Dividemass_Gain
                                        * Referenced by: '<Root>/Divide mass'
                                        */
};

/* Real-time Model Data Structure */
struct tag_RTM_AirbagModel_T {
  const char_T * volatile errorStatus;

  /*
   * ModelData:
   * The following substructure contains information regarding
   * the data used in the model.
   */
  struct {
    P_AirbagModel_T *defaultParam;
    DW_AirbagModel_T *dwork;
  } ModelData;
};

/* Model entry point functions */
extern void AirbagModel_initialize(RT_MODEL_AirbagModel_T *const AirbagModel_M,
  ExtU_AirbagModel_T *AirbagModel_U, ExtY_AirbagModel_T *AirbagModel_Y);
extern void AirbagModel_step(RT_MODEL_AirbagModel_T *const AirbagModel_M,
  ExtU_AirbagModel_T *AirbagModel_U, ExtY_AirbagModel_T *AirbagModel_Y);
extern void AirbagModel_terminate(RT_MODEL_AirbagModel_T *const AirbagModel_M);

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
 * '<Root>' : 'AirbagModel'
 * '<S1>'   : 'AirbagModel/Compare To Constant'
 * '<S2>'   : 'AirbagModel/Gain1'
 */
#endif                                 /* RTW_HEADER_AirbagModel_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
