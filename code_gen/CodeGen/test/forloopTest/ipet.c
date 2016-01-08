/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: ert_main.c
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

#include <stddef.h>
#include <stdio.h>                     /* This ert_main.c example uses printf/fflush */
#include "for_loop_100000_0.h"                  /* Model's header file */
#include "rtwtypes.h"

static RT_MODEL_for_loop_100000_0_T for_loop_100000_0_M_;
static RT_MODEL_for_loop_100000_0_T *const for_loop_100000_0_M = &for_loop_100000_0_M_;/* Real-time model */
static ExtU_for_loop_100000_0_T for_loop_100000_0_U;     /* External inputs */
static ExtY_for_loop_100000_0_T for_loop_100000_0_Y;     /* External outputs */

/*
 * The example "main" function illustrates what is required by your
 * application code to initialize, execute, and terminate the generated code.
 * Attaching rt_OneStep to a real-time clock is target specific.  This example
 * illustates how you do this relative to initializing the model.
 */
int_T main(int_T argc, const char *argv[])
{
  /* Unused arguments */
  (void)(argc);
  (void)(argv);

  /* Pack model data into RTM */

  /* Initialize model */
  for_loop_100000_0_initialize(for_loop_100000_0_M);
  for_loop_100000_0_step(for_loop_100000_0_M, &for_loop_100000_0_U, &for_loop_100000_0_Y);

  return 0;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
