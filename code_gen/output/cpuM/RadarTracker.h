/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: RadarTracker.h
 *
 * Code generated for Simulink model 'RadarTracker'.
 *
 * Model version                  : 1.106
 * Simulink Coder version         : 8.8 (R2015a) 09-Feb-2015
 * C/C++ source code generated on : Wed Aug  5 10:20:43 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Specified
 * Code generation objectives:
 *    1. MISRA-C:2004 guidelines
 *    2. Safety precaution
 * Validation result: All passed
 */

#ifndef RTW_HEADER_RadarTracker_h_
#define RTW_HEADER_RadarTracker_h_
#include <math.h>
#include <string.h>
#ifndef RadarTracker_COMMON_INCLUDES_
# define RadarTracker_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* RadarTracker_COMMON_INCLUDES_ */

#include "RadarTracker_types.h"
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
  real32_T P[16];                      /* '<Root>/RadarTracker' */
  real32_T xhat[4];                    /* '<Root>/RadarTracker' */
} DW_RadarTracker_T;

/* External inputs (root inport signals with auto storage) */
typedef struct {
  real32_T meas[2];                    /* '<Root>/meas' */
} ExtU_RadarTracker_T;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  real32_T residual[2];                /* '<Root>/residual' */
  real32_T xhatOut[4];                 /* '<Root>/xhatOut' */
} ExtY_RadarTracker_T;

/* Real-time Model Data Structure */
struct tag_RTM_RadarTracker_T {
  const char_T * volatile errorStatus;

  /*
   * ModelData:
   * The following substructure contains information regarding
   * the data used in the model.
   */
  struct {
    DW_RadarTracker_T *dwork;
  } ModelData;
};

/* Model entry point functions */
extern void RadarTracker_initialize(RT_MODEL_RadarTracker_T *const
  RadarTracker_M, ExtU_RadarTracker_T *RadarTracker_U, ExtY_RadarTracker_T
  *RadarTracker_Y);
extern void RadarTracker_step(RT_MODEL_RadarTracker_T *const RadarTracker_M,
  ExtU_RadarTracker_T *RadarTracker_U, ExtY_RadarTracker_T *RadarTracker_Y);
extern void RadarTracker_terminate(RT_MODEL_RadarTracker_T *const RadarTracker_M);

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Note that this particular code originates from a subsystem build,
 * and has its own system numbers different from the parent model.
 * Refer to the system hierarchy for this subsystem below, and use the
 * MATLAB hilite_system command to trace the generated code back
 * to the parent model.  For example,
 *
 * hilite_system('demo_aero_radar/RadarTracker')    - opens subsystem demo_aero_radar/RadarTracker
 * hilite_system('demo_aero_radar/RadarTracker/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'demo_aero_radar'
 * '<S1>'   : 'demo_aero_radar/RadarTracker'
 */
#endif                                 /* RTW_HEADER_RadarTracker_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
