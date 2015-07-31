/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: ert_main.c
 *
 * Code generated for Simulink model 'RadarTracker'.
 *
 * Model version                  : 1.106
 * Simulink Coder version         : 8.8 (R2015a) 09-Feb-2015
 * C/C++ source code generated on : Fri Jul 31 11:31:30 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Specified
 * Code generation objectives:
 *    1. MISRA-C:2004 guidelines
 *    2. Safety precaution
 * Validation result: Not run
 */

#include <stddef.h>
#include <stdio.h>                     /* This ert_main.c example uses printf/fflush */
#include "RadarTracker.h"              /* Model's header file */
#include "rtwtypes.h"

static RT_MODEL_RadarTracker_T RadarTracker_M_;
static RT_MODEL_RadarTracker_T *const RadarTracker_M = &RadarTracker_M_;/* Real-time model */
static DW_RadarTracker_T RadarTracker_DW;/* Observable states */
static ExtU_RadarTracker_T RadarTracker_U;/* External inputs */
static ExtY_RadarTracker_T RadarTracker_Y;/* External outputs */

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
void rt_OneStep(RT_MODEL_RadarTracker_T *const RadarTracker_M);
void rt_OneStep(RT_MODEL_RadarTracker_T *const RadarTracker_M)
{
  static boolean_T OverrunFlag = false;

  /* Disable interrupts here */

  /* Check for overrun */
  if (OverrunFlag) {
    rtmSetErrorStatus(RadarTracker_M, "Overrun");
    return;
  }

  OverrunFlag = true;

  /* Save FPU context here (if necessary) */
  /* Re-enable timer or interrupt here */
  /* Set model inputs here */

  /* Step the model */
  RadarTracker_step(RadarTracker_M, &RadarTracker_U, &RadarTracker_Y);

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
  RadarTracker_M->ModelData.dwork = &RadarTracker_DW;

  /* Initialize model */
  RadarTracker_initialize(RadarTracker_M, &RadarTracker_U, &RadarTracker_Y);

  /* Attach rt_OneStep to a timer or interrupt service routine with
   * period 1.0 seconds (the model's base sample time) here.  The
   * call syntax for rt_OneStep is
   *
   *  rt_OneStep(RadarTracker_M);
   */
  printf("Warning: The simulation will run forever. "
         "Generated ERT main won't simulate model step behavior. "
         "To change this behavior select the 'MAT-file logging' option.\n");
  fflush((NULL));
  while (rtmGetErrorStatus(RadarTracker_M) == (NULL)) {
    /*  Perform other application tasks here */
  }

  /* Disable rt_OneStep() here */

  /* Terminate model */
  RadarTracker_terminate(RadarTracker_M);
  return 0;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
