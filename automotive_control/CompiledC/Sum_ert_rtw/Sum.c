/*
 * File: Sum.c
 *
 * Code generated for Simulink model 'Sum'.
 *
 * Model version                  : 1.3
 * Simulink Coder version         : 8.6 (R2014a) 27-Dec-2013
 * C/C++ source code generated on : Sun Mar 22 13:52:58 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ASIC/FPGA->ASIC/FPGA
 * Emulation hardware selection:
 *    Differs from embedded hardware (Generic->32-bit Embedded Processor)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "Sum.h"

/* Real-time model */
RT_MODEL_Sum_T Sum_M_;
RT_MODEL_Sum_T *const Sum_M = &Sum_M_;

/* Model step function */
void Sum_step(void)
{
  /* Sum: '<S1>/Sum' incorporates:
   *  Inport: '<Root>/In1'
   *  Inport: '<Root>/In2'
   */
  sum_out = sum_in1 + sum_in2;
}

/* Model initialize function */
void Sum_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(Sum_M, (NULL));
}

/* Model terminate function */
void Sum_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
