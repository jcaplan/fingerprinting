/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: ert_main.c
 *
 * Code generated for Simulink model 'for_loop_60000_30000'.
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
#include "for_loop_60000_30000.h"                  /* Model's header file */
#include "rtwtypes.h"

static RT_MODEL_for_loop_60000_30000_T for_loop_60000_30000_M_;
static RT_MODEL_for_loop_60000_30000_T *const for_loop_60000_30000_M = &for_loop_60000_30000_M_;/* Real-time model */
static ExtU_for_loop_60000_30000_T for_loop_60000_30000_U;     /* External inputs */
static ExtY_for_loop_60000_30000_T for_loop_60000_30000_Y;     /* External outputs */

/*
 * Associating rt_OneStep with a real-time clock or interrupt service routine
 * is what makes the generated code "real-time".  The function rt_OneStep is
 * always associated with the base rate of the model.  Subrates are managed
 * by the base rate from inside the generated code.  Enabling/disabling
 * interrupts and floating point context switches are target specific.  This
 * example code indicates where these should take place relative to executing
 * the generated code step function.  Overrun behavior should be tailored to
 * your application needs.  This example simply sets an error status in the
 * real-time model and returns from rt_OneStep.
 */
void rt_OneStep(RT_MODEL_for_loop_60000_30000_T *const for_loop_60000_30000_M);
void rt_OneStep(RT_MODEL_for_loop_60000_30000_T *const for_loop_60000_30000_M)
{
  static boolean_T OverrunFlag = false;

  /* Disable interrupts here */

  /* Check for overrun */
  if (OverrunFlag) {
    return;
  }

  OverrunFlag = true;

  /* Save FPU context here (if necessary) */
  /* Re-enable timer or interrupt here */
  /* Set model inputs here */

  /* Step the model */
  for_loop_60000_30000_step(for_loop_60000_30000_M, &for_loop_60000_30000_U, &for_loop_60000_30000_Y);

  /* Get model outputs here */

  /* Indicate task complete */
  OverrunFlag = false;

  /* Disable interrupts here */
  /* Restore FPU context here (if necessary) */
  /* Enable interrupts here */
}

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
  for_loop_60000_30000_initialize(for_loop_60000_30000_M);

  /* Attach rt_OneStep to a timer or interrupt service routine with
   * period 0.2 seconds (the model's base sample time) here.  The
   * call syntax for rt_OneStep is
   *
   *  rt_OneStep(for_loop_60000_30000_M);
   */
  printf("Warning: The simulation will run forever. "
         "Generated ERT main won't simulate model step behavior. "
         "To change this behavior select the 'MAT-file logging' option.\n");
  fflush((NULL));
  while (((void*) 0) == (NULL)) {
    /*  Perform other application tasks here */
  }

  /* The option 'Suppress error status in real-time model data structure'
   * is selected, therefore the following code does not need to execute.
   */
#if 0

  /* Disable rt_OneStep() here */
#endif

  return 0;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
