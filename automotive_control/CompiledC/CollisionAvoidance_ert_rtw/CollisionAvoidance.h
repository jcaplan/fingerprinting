/*
 * File: CollisionAvoidance.h
 *
 * Code generated for Simulink model 'CollisionAvoidance'.
 *
 * Model version                  : 1.4
 * Simulink Coder version         : 8.6 (R2014a) 27-Dec-2013
 * C/C++ source code generated on : Mon Mar 23 13:28:59 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Generic->32-bit Embedded Processor
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_CollisionAvoidance_h_
#define RTW_HEADER_CollisionAvoidance_h_
#include "rtwtypes.h"
#include <stddef.h>
#include <string.h>
#ifndef CollisionAvoidance_COMMON_INCLUDES_
# define CollisionAvoidance_COMMON_INCLUDES_
#endif                                 /* CollisionAvoidance_COMMON_INCLUDES_ */

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Forward declaration for rtModel */
typedef struct tag_RTM_CollisionAvoidance_T RT_MODEL_CollisionAvoidance_T;

/* Block states (auto storage) for system '<Root>' */
typedef struct {
  real_T UD_DSTATE;                    /* '<S3>/UD' */
  real_T UD_DSTATE_g;                  /* '<S4>/UD' */
} DW_CollisionAvoidance_T;

/* Parameters (auto storage) */
struct P_CollisionAvoidance_T_ {
  real_T DiscreteDerivative_ICPrevScaled;/* Mask Parameter: DiscreteDerivative_ICPrevScaled
                                          * Referenced by: '<S3>/UD'
                                          */
  real_T DiscreteDerivative1_ICPrevScale;/* Mask Parameter: DiscreteDerivative1_ICPrevScale
                                          * Referenced by: '<S4>/UD'
                                          */
  real_T TSamp_WtEt;                   /* Computed Parameter: TSamp_WtEt
                                        * Referenced by: '<S3>/TSamp'
                                        */
  real_T TSamp_WtEt_m;                 /* Computed Parameter: TSamp_WtEt_m
                                        * Referenced by: '<S4>/TSamp'
                                        */
  real_T Constant_Value;               /* Expression: 0
                                        * Referenced by: '<S2>/Constant'
                                        */
};

/* Parameters (auto storage) */
typedef struct P_CollisionAvoidance_T_ P_CollisionAvoidance_T;

/* Real-time Model Data Structure */
struct tag_RTM_CollisionAvoidance_T {
  const char_T * volatile errorStatus;
};

/* Imported (extern) block signals */
extern boolean_T ca_light_on;          /* '<S2>/Compare' */
extern real_T ca_radar_sensor;         /* '<Root>/In1' */
extern real_T ca_acceleration;         /* '<Root>/In2' */

/* Block parameters (auto storage) */
extern P_CollisionAvoidance_T CollisionAvoidance_P;

/* Block states (auto storage) */
extern DW_CollisionAvoidance_T CollisionAvoidance_DW;

/* Model entry point functions */
extern void CollisionAvoidance_initialize(void);
extern void CollisionAvoidance_step(void);
extern void CollisionAvoidance_terminate(void);

/* Real-time Model object */
extern RT_MODEL_CollisionAvoidance_T *const CollisionAvoidance_M;

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
 * '<Root>' : 'CollisionAvoidance'
 * '<S1>'   : 'CollisionAvoidance/Collision Avoidance '
 * '<S2>'   : 'CollisionAvoidance/Collision Avoidance /Compare To Zero'
 * '<S3>'   : 'CollisionAvoidance/Collision Avoidance /Discrete Derivative'
 * '<S4>'   : 'CollisionAvoidance/Collision Avoidance /Discrete Derivative1'
 */
#endif                                 /* RTW_HEADER_CollisionAvoidance_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
