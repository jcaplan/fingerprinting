/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: ert_main.c
 *
 * Code generated for Simulink model 'TractionControl'.
 *
 * Model version                  : 1.8
 * Simulink Coder version         : 8.8 (R2015a) 09-Feb-2015
 * C/C++ source code generated on : Mon May  4 13:55:05 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ASIC/FPGA->ASIC/FPGA
 * Emulation hardware selection:
 *    Differs from embedded hardware (Generic->32-bit Embedded Processor)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include <stddef.h>
#include <stdio.h>                     /* This ert_main.c example uses printf/fflush */
#include "TractionControl.h"           /* Model's header file */
#include "rtwtypes.h"
#include "multiword_types.h"

static RT_MODEL_TractionControl_T TractionControl_M_;
static RT_MODEL_TractionControl_T *const TractionControl_M = &TractionControl_M_;/* Real-time model */
static P_TractionControl_T TractionControl_P = {
  3.0                                  /* Mask Parameter: CompareToConstant_const
                                        * Referenced by: '<S1>/Constant'
                                        */
};                                     /* Modifiable parameters */

static ExtU_TractionControl_T TractionControl_U;/* External inputs */
static ExtY_TractionControl_T TractionControl_Y;/* External outputs */

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
void rt_OneStep(RT_MODEL_TractionControl_T *const TractionControl_M);
void rt_OneStep(RT_MODEL_TractionControl_T *const TractionControl_M)
{
  static boolean_T OverrunFlag = false;

  /* Disable interrupts here */

  /* Check for overrun */
  if (OverrunFlag) {
    // rtmSetErrorStatus(TractionControl_M, "Overrun");
    return;
  }

  OverrunFlag = true;

  /* Save FPU context here (if necessary) */
  /* Re-enable timer or interrupt here */
  /* Set model inputs here */

  /* Step the model */
  TractionControl_step(TractionControl_M, &TractionControl_U, &TractionControl_Y);

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
  TractionControl_M->ModelData.defaultParam = &TractionControl_P;

  /* Initialize model */
  TractionControl_initialize(TractionControl_M, &TractionControl_U,
    &TractionControl_Y);

  /* Attach rt_OneStep to a timer or interrupt service routine with
   * period 0.2 seconds (the model's base sample time) here.  The
   * call syntax for rt_OneStep is
   *
   *  rt_OneStep(TractionControl_M);
   */
  // printf("Warning: The simulation will run forever. "
  //        "Generated ERT main won't simulate model step behavior. "
  //        "To change this behavior select the 'MAT-file logging' option.\n");
  // fflush((NULL));
  while (rtmGetErrorStatus(TractionControl_M) == (NULL)) {
    /*  Perform other application tasks here */
  }

  /* Disable rt_OneStep() here */

  /* Terminate model */
  TractionControl_terminate(TractionControl_M);
  return 0;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */