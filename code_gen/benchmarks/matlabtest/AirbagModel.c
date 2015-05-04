/*
 * File: AirbagModel.c
 *
 * Code generated for Simulink model 'AirbagModel'.
 *
 * Model version                  : 1.8
 * Simulink Coder version         : 8.6 (R2014a) 27-Dec-2013
 * C/C++ source code generated on : Sat Apr  4 15:17:57 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Generic->32-bit Embedded Processor
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "AirbagModel.h"

/* Block states (auto storage) */
DW_AirbagModel_T AirbagModel_DW;

/* Real-time model */
RT_MODEL_AirbagModel_T AirbagModel_M_;
RT_MODEL_AirbagModel_T *const AirbagModel_M = &AirbagModel_M_;

/* Model step function */
void AirbagModel_step(void)
{
  /* DiscreteIntegrator: '<Root>/Discrete-Time Integrator1' */
  ab_sensor_displacement = AirbagModel_DW.DiscreteTimeIntegrator1_DSTATE;

  /* Update for DiscreteIntegrator: '<Root>/Discrete-Time Integrator1' incorporates:
   *  DiscreteIntegrator: '<Root>/Discrete-Time Integrator'
   */
  AirbagModel_DW.DiscreteTimeIntegrator1_DSTATE +=
    AirbagModel_P.DiscreteTimeIntegrator1_gainval *
    AirbagModel_DW.DiscreteTimeIntegrator_DSTATE;

  /* Update for DiscreteIntegrator: '<Root>/Discrete-Time Integrator' incorporates:
   *  Gain: '<Root>/Divide mass'
   *  Gain: '<Root>/Gain'
   *  Update for Inport: '<Root>/Force'
   *  Sum: '<Root>/Sum1'
   */
  AirbagModel_DW.DiscreteTimeIntegrator_DSTATE += (ab_force -
    AirbagModel_P.Gain_Gain * ab_sensor_displacement) *
    AirbagModel_P.Dividemass_Gain * AirbagModel_P.DiscreteTimeIntegrator_gainval;
}

/* Model initialize function */
void AirbagModel_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(AirbagModel_M, (NULL));

  /* states (dwork) */
  (void) memset((void *)&AirbagModel_DW, 0,
                sizeof(DW_AirbagModel_T));

  /* InitializeConditions for DiscreteIntegrator: '<Root>/Discrete-Time Integrator1' */
  AirbagModel_DW.DiscreteTimeIntegrator1_DSTATE =
    AirbagModel_P.DiscreteTimeIntegrator1_IC;

  /* InitializeConditions for DiscreteIntegrator: '<Root>/Discrete-Time Integrator' */
  AirbagModel_DW.DiscreteTimeIntegrator_DSTATE =
    AirbagModel_P.DiscreteTimeIntegrator_IC;
}

/* Model terminate function */
void AirbagModel_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
