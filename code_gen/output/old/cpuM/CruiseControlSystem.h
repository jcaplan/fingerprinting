/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: CruiseControlSystem.h
 *
 * Code generated for Simulink model 'CruiseControlSystem'.
 *
 * Model version                  : 1.12
 * Simulink Coder version         : 8.8 (R2015a) 09-Feb-2015
 * C/C++ source code generated on : Thu Jul 30 17:10:07 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Generic->32-bit Embedded Processor
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_CruiseControlSystem_h_
#define RTW_HEADER_CruiseControlSystem_h_
#include "rtwtypes.h"
#include <string.h>
#ifndef CruiseControlSystem_COMMON_INCLUDES_
# define CruiseControlSystem_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* CruiseControlSystem_COMMON_INCLUDES_ */

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Forward declaration for rtModel */
typedef struct tag_RTM_CruiseControlSystem_T RT_MODEL_CruiseControlSystem_T;

/* Block states (auto storage) for system '<Root>' */
typedef struct {
  real32_T DiscreteTimeIntegrator_DSTATE;/* '<S2>/Discrete-Time Integrator' */
  real32_T DiscreteTimeIntegrator_DSTATE_o;/* '<S1>/Discrete-Time Integrator' */
} DW_CruiseControlSystem_T;

/* External inputs (root inport signals with auto storage) */
typedef struct {
  real32_T In1;                        /* '<Root>/In1' */
  real32_T In2;                        /* '<Root>/In2' */
} ExtU_CruiseControlSystem_T;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  real32_T Out1;                       /* '<Root>/Out1' */
} ExtY_CruiseControlSystem_T;

/* Parameters (auto storage) */
struct P_CruiseControlSystem_T_ {
  real32_T DiscreteTimeIntegrator_gainval;/* Computed Parameter: DiscreteTimeIntegrator_gainval
                                           * Referenced by: '<S2>/Discrete-Time Integrator'
                                           */
  real32_T DiscreteTimeIntegrator_IC;  /* Computed Parameter: DiscreteTimeIntegrator_IC
                                        * Referenced by: '<S2>/Discrete-Time Integrator'
                                        */
  real32_T DiscreteTimeIntegrator_gainva_l;/* Computed Parameter: DiscreteTimeIntegrator_gainva_l
                                            * Referenced by: '<S1>/Discrete-Time Integrator'
                                            */
  real32_T DiscreteTimeIntegrator_IC_n;/* Computed Parameter: DiscreteTimeIntegrator_IC_n
                                        * Referenced by: '<S1>/Discrete-Time Integrator'
                                        */
  real32_T Ki_Gain;                    /* Computed Parameter: Ki_Gain
                                        * Referenced by: '<S1>/Ki'
                                        */
  real32_T Kp_Gain;                    /* Computed Parameter: Kp_Gain
                                        * Referenced by: '<S1>/Kp'
                                        */
  real32_T damping_Gain;               /* Computed Parameter: damping_Gain
                                        * Referenced by: '<S2>/damping'
                                        */
  real32_T Inertia_Gain;               /* Computed Parameter: Inertia_Gain
                                        * Referenced by: '<S2>/Inertia '
                                        */
};

/* Parameters (auto storage) */
typedef struct P_CruiseControlSystem_T_ P_CruiseControlSystem_T;

/* Real-time Model Data Structure */
struct tag_RTM_CruiseControlSystem_T {
  const char_T * volatile errorStatus;

  /*
   * ModelData:
   * The following substructure contains information regarding
   * the data used in the model.
   */
  struct {
    P_CruiseControlSystem_T *defaultParam;
    DW_CruiseControlSystem_T *dwork;
  } ModelData;
};

/* Model entry point functions */
extern void CruiseControlSystem_initialize(RT_MODEL_CruiseControlSystem_T *const
  CruiseControlSystem_M, ExtU_CruiseControlSystem_T *CruiseControlSystem_U,
  ExtY_CruiseControlSystem_T *CruiseControlSystem_Y);
extern void CruiseControlSystem_step(RT_MODEL_CruiseControlSystem_T *const
  CruiseControlSystem_M, ExtU_CruiseControlSystem_T *CruiseControlSystem_U,
  ExtY_CruiseControlSystem_T *CruiseControlSystem_Y);
extern void CruiseControlSystem_terminate(RT_MODEL_CruiseControlSystem_T *const
  CruiseControlSystem_M);

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
 * '<Root>' : 'CruiseControlSystem'
 * '<S1>'   : 'CruiseControlSystem/Subsystem'
 * '<S2>'   : 'CruiseControlSystem/Subsystem/Plant Model '
 */
#endif                                 /* RTW_HEADER_CruiseControlSystem_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
