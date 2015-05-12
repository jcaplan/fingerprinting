/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: FuelSensor.h
 *
 * Code generated for Simulink model 'FuelSensor'.
 *
 * Model version                  : 1.8
 * Simulink Coder version         : 8.8 (R2015a) 09-Feb-2015
 * C/C++ source code generated on : Mon May  4 13:58:52 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Generic->32-bit Embedded Processor
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_FuelSensor_h_
#define RTW_HEADER_FuelSensor_h_
#include "rtwtypes.h"
#include <string.h>
#ifndef FuelSensor_COMMON_INCLUDES_
# define FuelSensor_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* FuelSensor_COMMON_INCLUDES_ */

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Forward declaration for rtModel */
typedef struct tag_RTM_FuelSensor_T RT_MODEL_FuelSensor_T;

/* Block states (auto storage) for system '<Root>' */
typedef struct {
  boolean_T Lighton_DSTATE;            /* '<Root>/Light on  ' */
} DW_FuelSensor_T;

/* External inputs (root inport signals with auto storage) */
typedef struct {
  real32_T FuelLevel;                  /* '<Root>/Fuel Level' */
} ExtU_FuelSensor_T;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  real32_T FuelDisplay;                /* '<Root>/Fuel Display' */
  boolean_T WarningLight;              /* '<Root>/Warning Light' */
} ExtY_FuelSensor_T;

/* Parameters (auto storage) */
struct P_FuelSensor_T_ {
  real32_T LowFuelSensor_const;        /* Mask Parameter: LowFuelSensor_const
                                        * Referenced by: '<S1>/Constant'
                                        */
  boolean_T Lighton_InitialCondition;  /* Computed Parameter: Lighton_InitialCondition
                                        * Referenced by: '<Root>/Light on  '
                                        */
};

/* Parameters (auto storage) */
typedef struct P_FuelSensor_T_ P_FuelSensor_T;

/* Real-time Model Data Structure */
struct tag_RTM_FuelSensor_T {
  const char_T * volatile errorStatus;

  /*
   * ModelData:
   * The following substructure contains information regarding
   * the data used in the model.
   */
  struct {
    P_FuelSensor_T *defaultParam;
    DW_FuelSensor_T *dwork;
  } ModelData;
};

/* Model entry point functions */
extern void FuelSensor_initialize(RT_MODEL_FuelSensor_T *const FuelSensor_M,
  ExtU_FuelSensor_T *FuelSensor_U, ExtY_FuelSensor_T *FuelSensor_Y);
extern void FuelSensor_step(RT_MODEL_FuelSensor_T *const FuelSensor_M,
  ExtU_FuelSensor_T *FuelSensor_U, ExtY_FuelSensor_T *FuelSensor_Y);
extern void FuelSensor_terminate(RT_MODEL_FuelSensor_T *const FuelSensor_M);

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
 * '<Root>' : 'FuelSensor'
 * '<S1>'   : 'FuelSensor/Low Fuel Sensor'
 */
#endif                                 /* RTW_HEADER_FuelSensor_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
