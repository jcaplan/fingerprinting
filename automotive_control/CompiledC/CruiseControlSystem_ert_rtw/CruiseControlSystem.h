/*
 * File: CruiseControlSystem.h
 *
 * Code generated for Simulink model 'CruiseControlSystem'.
 *
 * Model version                  : 1.8
 * Simulink Coder version         : 8.6 (R2014a) 27-Dec-2013
 * C/C++ source code generated on : Mon Mar 23 13:31:51 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Generic->32-bit Embedded Processor
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_CruiseControlSystem_h_
#define RTW_HEADER_CruiseControlSystem_h_
#include "rtwtypes.h"
#include <stddef.h>
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
  real_T DiscreteTimeIntegrator_DSTATE;/* '<S2>/Discrete-Time Integrator' */
  real_T DiscreteTimeIntegrator_DSTATE_o;/* '<S1>/Discrete-Time Integrator' */
} DW_CruiseControlSystem_T;

/* Parameters (auto storage) */
struct P_CruiseControlSystem_T_ {
  real_T DiscreteTimeIntegrator_gainval;/* Computed Parameter: DiscreteTimeIntegrator_gainval
                                         * Referenced by: '<S2>/Discrete-Time Integrator'
                                         */
  real_T DiscreteTimeIntegrator_IC;    /* Expression: 0
                                        * Referenced by: '<S2>/Discrete-Time Integrator'
                                        */
  real_T DiscreteTimeIntegrator_gainva_l;/* Computed Parameter: DiscreteTimeIntegrator_gainva_l
                                          * Referenced by: '<S1>/Discrete-Time Integrator'
                                          */
  real_T DiscreteTimeIntegrator_IC_n;  /* Expression: 0
                                        * Referenced by: '<S1>/Discrete-Time Integrator'
                                        */
  real_T Ki_Gain;                      /* Expression: 1.0
                                        * Referenced by: '<S1>/Ki'
                                        */
  real_T Kp_Gain;                      /* Expression: 1.0
                                        * Referenced by: '<S1>/Kp'
                                        */
  real_T damping_Gain;                 /* Expression: 50
                                        * Referenced by: '<S2>/damping'
                                        */
  real_T Inertia_Gain;                 /* Expression: 0.001
                                        * Referenced by: '<S2>/Inertia '
                                        */
};

/* Parameters (auto storage) */
typedef struct P_CruiseControlSystem_T_ P_CruiseControlSystem_T;

/* Real-time Model Data Structure */
struct tag_RTM_CruiseControlSystem_T {
  const char_T * volatile errorStatus;
};

/* Imported (extern) block signals */
extern real_T cc_current_speed;        /* '<S2>/Discrete-Time Integrator' */
extern real_T cc_desired_speed;        /* '<Root>/In1' */

/* Block parameters (auto storage) */
extern P_CruiseControlSystem_T CruiseControlSystem_P;

/* Block states (auto storage) */
extern DW_CruiseControlSystem_T CruiseControlSystem_DW;

/* Model entry point functions */
extern void CruiseControlSystem_initialize(void);
extern void CruiseControlSystem_step(void);
extern void CruiseControlSystem_terminate(void);

/* Real-time Model object */
extern RT_MODEL_CruiseControlSystem_T *const CruiseControlSystem_M;

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
