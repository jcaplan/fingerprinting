/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: for_loop_100000_0.c
 *
 * Code generated for Simulink model 'for_loop_100000_0'.
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

#include "for_loop_100000_0.h"
#include "for_loop_100000_0_private.h"
#include <math.h>
#include <stdlib.h>
#include "annot.h"

/* Model step function */
void for_loop_100000_0_step(RT_MODEL_for_loop_100000_0_T *const for_loop_100000_0_M, ExtU_for_loop_100000_0_T
                   *for_loop_100000_0_U, ExtY_for_loop_100000_0_T *for_loop_100000_0_Y)
{
  real32_T y;
  int32_T i;

  /* MATLAB Function: '<Root>/ForLoop' incorporates:
   *  Constant: '<Root>/Constant'
   *  Inport: '<Root>/In1'
   */
  /* MATLAB Function 'ForLoop': '<S1>:1' */
  /* '<S1>:1:2' */
  y = for_loop_100000_0_U->In1;

  /* '<S1>:1:3' */
  int32_T range = ((int32_T)(LOOP_LIMIT));
  for (i = 0; i < range; i++) {
		ANNOT_MAXITER(100000);
    /* '<S1>:1:3' */
    /* '<S1>:1:4' */
    y = (((real32_T)i) * 2.0F) + (((real32_T)i) * ((real32_T)i));

    /* '<S1>:1:3' */
  }

  /* Outport: '<Root>/Out1' incorporates:
   *  MATLAB Function: '<Root>/ForLoop'
   */
  for_loop_100000_0_Y->Out1 = y;
  UNUSED_PARAMETER(for_loop_100000_0_M);
}

/* Model initialize function */
void for_loop_100000_0_initialize(RT_MODEL_for_loop_100000_0_T *const for_loop_100000_0_M)
{
  /* (no initialization code required) */
  srand (RANDOM_SEED);
  UNUSED_PARAMETER(for_loop_100000_0_M);
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
