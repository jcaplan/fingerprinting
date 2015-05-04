/*
 * File: Gain.c
 *
 * Code generated for Simulink model 'Gain'.
 *
 * Model version                  : 1.1
 * Simulink Coder version         : 8.6 (R2014a) 27-Dec-2013
 * C/C++ source code generated on : Sun Mar 22 13:58:47 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Generic->32-bit Embedded Processor
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "Gain.h"

/* Real-time model */
RT_MODEL_Gain_T Gain_M_;
RT_MODEL_Gain_T *const Gain_M = &Gain_M_;

/* Model step function */
void Gain_step(void)
{
  /* Gain: '<S1>/Gain' incorporates:
   *  Inport: '<Root>/In1'
   */
  gain_out = Gain_P.Gain_Gain * gain_in;
}

/* Model initialize function */
void Gain_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(Gain_M, (NULL));
}

/* Model terminate function */
void Gain_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
