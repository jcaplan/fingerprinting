/*
 * File: Derivative.c
 *
 * Code generated for Simulink model 'Derivative'.
 *
 * Model version                  : 1.3
 * Simulink Coder version         : 8.6 (R2014a) 27-Dec-2013
 * C/C++ source code generated on : Mon Mar 23 13:34:32 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Generic->32-bit Embedded Processor
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "Derivative.h"

/* Block states (auto storage) */
DW_Derivative_T Derivative_DW;

/* Real-time model */
RT_MODEL_Derivative_T Derivative_M_;
RT_MODEL_Derivative_T *const Derivative_M = &Derivative_M_;

/* Model step function */
void Derivative_step(void)
{
  real_T rtb_TSamp;

  /* SampleTimeMath: '<S2>/TSamp' incorporates:
   *  Inport: '<Root>/In1'
   *
   * About '<S2>/TSamp':
   *  y = u * K where K = 1 / ( w * Ts )
   */
  rtb_TSamp = deriv_in * Derivative_P.TSamp_WtEt;

  /* Sum: '<S2>/Diff' incorporates:
   *  UnitDelay: '<S2>/UD'
   */
  deriv_out = rtb_TSamp - Derivative_DW.UD_DSTATE;

  /* Update for UnitDelay: '<S2>/UD' */
  Derivative_DW.UD_DSTATE = rtb_TSamp;
}

/* Model initialize function */
void Derivative_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(Derivative_M, (NULL));

  /* states (dwork) */
  (void) memset((void *)&Derivative_DW, 0,
                sizeof(DW_Derivative_T));

  /* InitializeConditions for UnitDelay: '<S2>/UD' */
  Derivative_DW.UD_DSTATE = Derivative_P.DiscreteDerivative_ICPrevScaled;
}

/* Model terminate function */
void Derivative_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
