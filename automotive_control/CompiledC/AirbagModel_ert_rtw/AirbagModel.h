/*
 * File: AirbagModel.h
 *
 * Code generated for Simulink model 'AirbagModel'.
 *
 * Model version                  : 1.8
 * Simulink Coder version         : 8.6 (R2014a) 27-Dec-2013
 * C/C++ source code generated on : Sat Apr  4 15:17:57 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Generic->32-bit Embedded Processor
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_AirbagModel_h_
#define RTW_HEADER_AirbagModel_h_
#include "rtwtypes.h"
#include <stddef.h>
#include <string.h>
#ifndef AirbagModel_COMMON_INCLUDES_
# define AirbagModel_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* AirbagModel_COMMON_INCLUDES_ */

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Forward declaration for rtModel */
typedef struct tag_RTM_AirbagModel_T RT_MODEL_AirbagModel_T;

/* Block states (auto storage) for system '<Root>' */
typedef struct {
  real_T DiscreteTimeIntegrator1_DSTATE;/* '<Root>/Discrete-Time Integrator1' */
  real_T DiscreteTimeIntegrator_DSTATE;/* '<Root>/Discrete-Time Integrator' */
} DW_AirbagModel_T;

/* Parameters (auto storage) */
struct P_AirbagModel_T_ {
  real_T DiscreteTimeIntegrator1_gainval;/* Computed Parameter: DiscreteTimeIntegrator1_gainval
                                          * Referenced by: '<Root>/Discrete-Time Integrator1'
                                          */
  real_T DiscreteTimeIntegrator1_IC;   /* Expression: 0
                                        * Referenced by: '<Root>/Discrete-Time Integrator1'
                                        */
  real_T DiscreteTimeIntegrator_gainval;/* Computed Parameter: DiscreteTimeIntegrator_gainval
                                         * Referenced by: '<Root>/Discrete-Time Integrator'
                                         */
  real_T DiscreteTimeIntegrator_IC;    /* Expression: 0
                                        * Referenced by: '<Root>/Discrete-Time Integrator'
                                        */
  real_T Gain_Gain;                    /* Expression: 2.0
                                        * Referenced by: '<Root>/Gain'
                                        */
  real_T Dividemass_Gain;              /* Expression: -0.2
                                        * Referenced by: '<Root>/Divide mass'
                                        */
};

/* Parameters (auto storage) */
typedef struct P_AirbagModel_T_ P_AirbagModel_T;

/* Real-time Model Data Structure */
struct tag_RTM_AirbagModel_T {
  const char_T * volatile errorStatus;
};

/* Imported (extern) block signals */
real_T ab_sensor_displacement;  /* '<Root>/Discrete-Time Integrator1' */
real_T ab_force;                /* '<Root>/Force' */

/* Block parameters (auto storage) */
P_AirbagModel_T AirbagModel_P;

/* Block states (auto storage) */
DW_AirbagModel_T AirbagModel_DW;

/* Model entry point functions */
void AirbagModel_initialize(void);
void AirbagModel_step(void);
void AirbagModel_terminate(void);

/* Real-time Model object */
RT_MODEL_AirbagModel_T *const AirbagModel_M;

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
 */
#endif                                 /* RTW_HEADER_AirbagModel_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
