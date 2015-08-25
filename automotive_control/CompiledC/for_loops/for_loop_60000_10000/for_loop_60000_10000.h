/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: for_loop_60000_10000.h
 *
 * Code generated for Simulink model 'for_loop_60000_10000'.
 *
 * Model version                  : 1.85
 * Simulink Coder version         : 8.8 (R2015a) 09-Feb-2015
 * C/C++ source code generated on : Tue Aug 11 14:08:16 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Generic->32-bit Embedded Processor
 * Code generation objectives:
 *    1. Safety precaution
 *    2. MISRA-C:2004 guidelines
 *    3. RAM efficiency
 *    4. ROM efficiency
 * Validation result: Not run
 */

#ifndef RTW_HEADER_for_loop_60000_10000_h_
#define RTW_HEADER_for_loop_60000_10000_h_
#ifndef for_loop_60000_10000_COMMON_INCLUDES_
# define for_loop_60000_10000_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* for_loop_60000_10000_COMMON_INCLUDES_ */

#include "for_loop_60000_10000_types.h"
#include "rt_defines.h"

/* Macros for accessing real-time model data structure */

/* Exported data define */

/* Definition for custom storage class: Define */
#define LOOP_LIMIT                     60000
#define RANDOM_RANGE                   10000
#define RANDOM_SEED                    157
/* External inputs (root inport signals with auto storage) */
typedef struct {
  real32_T In1;                        /* '<Root>/In1' */
} ExtU_for_loop_60000_10000_T;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  real32_T Out1;                       /* '<Root>/Out1' */
} ExtY_for_loop_60000_10000_T;

/* Real-time Model Data Structure */
struct tag_RTM_for_loop_60000_10000_T {
  boolean_T *dummy;
};

/* Model entry point functions */
extern void for_loop_60000_10000_initialize(RT_MODEL_for_loop_60000_10000_T *const for_loop_60000_10000_M);
extern void for_loop_60000_10000_step(RT_MODEL_for_loop_60000_10000_T *const for_loop_60000_10000_M, ExtU_for_loop_60000_10000_T
  *for_loop_60000_10000_U, ExtY_for_loop_60000_10000_T *for_loop_60000_10000_Y);

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
 * '<Root>' : 'for_loop_60000_10000'
 * '<S1>'   : 'for_loop_60000_10000/ForLoop'
 */
#endif                                 /* RTW_HEADER_for_loop_60000_10000_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
