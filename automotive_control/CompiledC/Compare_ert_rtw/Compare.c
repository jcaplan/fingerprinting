/*
 * File: Compare.c
 *
 * Code generated for Simulink model 'Compare'.
 *
 * Model version                  : 1.2
 * Simulink Coder version         : 8.6 (R2014a) 27-Dec-2013
 * C/C++ source code generated on : Sun Mar 22 14:33:06 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Generic->32-bit Embedded Processor
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "Compare.h"

/* Real-time model */
RT_MODEL_Compare_T Compare_M_;
RT_MODEL_Compare_T *const Compare_M = &Compare_M_;

/* Model step function */
void Compare_step(void)
{
  /* RelationalOperator: '<S2>/Compare' incorporates:
   *  Constant: '<S2>/Constant'
   *  Inport: '<Root>/In1'
   */
  compare_out = (compare_in <= Compare_P.CompareToConstant_const);
}

/* Model initialize function */
void Compare_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(Compare_M, (NULL));
}

/* Model terminate function */
void Compare_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
