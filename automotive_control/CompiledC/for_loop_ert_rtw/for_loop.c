/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: for_loop.c
 *
 * Code generated for Simulink model 'for_loop'.
 *
 * Model version                  : 1.45
 * Simulink Coder version         : 8.8 (R2015a) 09-Feb-2015
 * C/C++ source code generated on : Wed Aug  5 18:02:50 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: 32-bit Generic
 * Code generation objectives:
 *    1. Safety precaution
 *    2. MISRA-C:2004 guidelines
 * Validation result: Not run
 */

#include "for_loop.h"
#include "for_loop_private.h"

/* Model step function */
void for_loop_step(RT_MODEL_for_loop_T *const for_loop_M, ExtU_for_loop_T
                   *for_loop_U, ExtY_for_loop_T *for_loop_Y)
{
  real32_T rtb_y;
  int32_T i;

  /* MATLAB Function: '<Root>/ForLoop' incorporates:
   *  Inport: '<Root>/In1'
   */
  /* MATLAB Function 'ForLoop': '<S1>:1' */
  /* '<S1>:1:2' */
  rtb_y = for_loop_U->In1;

  /* '<S1>:1:3' */
  for (i = 0; i < 1000000; i++) {
    /* '<S1>:1:3' */
    /* '<S1>:1:4' */
    rtb_y = (((real32_T)i) * 2.0F) + (((real32_T)i) * ((real32_T)i));

    /* '<S1>:1:3' */
  }

  /* End of MATLAB Function: '<Root>/ForLoop' */

  /* Outport: '<Root>/Out1' */
  for_loop_Y->Out1 = rtb_y;
  UNUSED_PARAMETER(for_loop_M);
}

/* Model initialize function */
void for_loop_initialize(RT_MODEL_for_loop_T *const for_loop_M, ExtU_for_loop_T *
  for_loop_U, ExtY_for_loop_T *for_loop_Y)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(for_loop_M, (NULL));

  /* external inputs */
  for_loop_U->In1 = 0.0F;

  /* external outputs */
  for_loop_Y->Out1 = 0.0F;
  UNUSED_PARAMETER(for_loop_M);
}

/* Model terminate function */
void for_loop_terminate(RT_MODEL_for_loop_T *const for_loop_M)
{
  /* (no terminate code required) */
  UNUSED_PARAMETER(for_loop_M);
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
