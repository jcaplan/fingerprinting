/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: ert_main.c
 *
 * Code generated for Simulink model 'AirbagModel'.
 *
 * Model version                  : 1.11
 * Simulink Coder version         : 8.8 (R2015a) 09-Feb-2015
 * C/C++ source code generated on : Mon May  4 13:22:36 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Generic->32-bit Embedded Processor
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include <stddef.h>
#include <stdio.h>                     /* This ert_main.c example uses printf/fflush */
#include "AirbagModel.h"               /* Model's header file */
#include "rtwtypes.h"

static RT_MODEL_AirbagModel_T AirbagModel_M_;
static RT_MODEL_AirbagModel_T *const AirbagModel_M = &AirbagModel_M_;/* Real-time model */
static P_AirbagModel_T AirbagModel_P = {
  3.0,                                 /* Mask Parameter: CompareToConstant_const
                                        * Referenced by: '<S1>/Constant'
                                        */
  1.0,                                 /* Computed Parameter: DiscreteTimeIntegrator1_gainval
                                        * Referenced by: '<Root>/Discrete-Time Integrator1'
                                        */
  0.0,                                 /* Expression: 0
                                        * Referenced by: '<Root>/Discrete-Time Integrator1'
                                        */
  1.0,                                 /* Computed Parameter: DiscreteTimeIntegrator_gainval
                                        * Referenced by: '<Root>/Discrete-Time Integrator'
                                        */
  0.0,                                 /* Expression: 0
                                        * Referenced by: '<Root>/Discrete-Time Integrator'
                                        */
  2.0,                                 /* Expression: 2.0
                                        * Referenced by: '<Root>/Gain'
                                        */
  -0.2                                 /* Expression: -0.2
                                        * Referenced by: '<Root>/Divide mass'
                                        */
};                                     /* Modifiable parameters */

static DW_AirbagModel_T AirbagModel_DW;/* Observable states */
static ExtU_AirbagModel_T AirbagModel_U;/* External inputs */
static ExtY_AirbagModel_T AirbagModel_Y;/* External outputs */

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
void rt_OneStep(RT_MODEL_AirbagModel_T *const AirbagModel_M);
void rt_OneStep(RT_MODEL_AirbagModel_T *const AirbagModel_M)
{
  static boolean_T OverrunFlag = false;

  /* Disable interrupts here */

  /* Check for overrun */
  if (OverrunFlag) {
    rtmSetErrorStatus(AirbagModel_M, "Overrun");
    return;
  }

  OverrunFlag = true;

  /* Save FPU context here (if necessary) */
  /* Re-enable timer or interrupt here */
  /* Set model inputs here */

  /* Step the model */
  AirbagModel_step(AirbagModel_M, &AirbagModel_U, &AirbagModel_Y);

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
  AirbagModel_M->ModelData.defaultParam = &AirbagModel_P;
  AirbagModel_M->ModelData.dwork = &AirbagModel_DW;

  /* Initialize model */
  AirbagModel_initialize(AirbagModel_M, &AirbagModel_U, &AirbagModel_Y);

  /* Attach rt_OneStep to a timer or interrupt service routine with
   * period 1.0 seconds (the model's base sample time) here.  The
   * call syntax for rt_OneStep is
   *
   *  rt_OneStep(AirbagModel_M);
   */
  printf("Warning: The simulation will run forever. "
         "Generated ERT main won't simulate model step behavior. "
         "To change this behavior select the 'MAT-file logging' option.\n");
  fflush((NULL));
  while (rtmGetErrorStatus(AirbagModel_M) == (NULL)) {
    /*  Perform other application tasks here */
  }

  /* Disable rt_OneStep() here */

  /* Terminate model */
  AirbagModel_terminate(AirbagModel_M);
  return 0;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
