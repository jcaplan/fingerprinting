/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: AirbagModel.c
 *
 * Code generated for Simulink model 'AirbagModel'.
 *
 * Model version                  : 1.13
 * Simulink Coder version         : 8.8 (R2015a) 09-Feb-2015
 * C/C++ source code generated on : Thu Jul 30 17:09:53 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Generic->32-bit Embedded Processor
 * Emulation hardware selection:
 *    Differs from embedded hardware (MATLAB Host)
 * Code generation objectives:
 *    1. MISRA-C:2004 guidelines
 *    2. Safety precaution
 * Validation result: Not run
 */

#include "AirbagModel.h"
#include "AirbagModel_private.h"

/* Model step function */
void AirbagModel_step(RT_MODEL_AirbagModel_T *const AirbagModel_M,
                      ExtU_AirbagModel_T *AirbagModel_U, ExtY_AirbagModel_T
                      *AirbagModel_Y)
{
  P_AirbagModel_T *AirbagModel_P = ((P_AirbagModel_T *)
    AirbagModel_M->ModelData.defaultParam);
  DW_AirbagModel_T *AirbagModel_DW = ((DW_AirbagModel_T *)
    AirbagModel_M->ModelData.dwork);
  real32_T rtb_Sum1;
  real32_T DiscreteTimeIntegrator1_DSTATE;
  real32_T DiscreteTimeIntegrator_DSTATE;

  /* Update for DiscreteIntegrator: '<Root>/Discrete-Time Integrator' */
  DiscreteTimeIntegrator_DSTATE = AirbagModel_DW->DiscreteTimeIntegrator_DSTATE;

  /* Update for DiscreteIntegrator: '<Root>/Discrete-Time Integrator1' */
  DiscreteTimeIntegrator1_DSTATE =
    AirbagModel_DW->DiscreteTimeIntegrator1_DSTATE;

  /* Outport: '<Root>/Activate Airbag ' incorporates:
   *  Constant: '<S1>/Constant'
   *  RelationalOperator: '<S1>/Compare'
   */
  AirbagModel_Y->ActivateAirbag = (DiscreteTimeIntegrator1_DSTATE <=
    AirbagModel_P->CompareToConstant_const);

  /* Sum: '<Root>/Sum1' incorporates:
   *  Gain: '<Root>/Gain'
   *  Gain: '<S2>/Gain'
   *  Inport: '<Root>/Force'
   */
  rtb_Sum1 = AirbagModel_P->Gain_Gain * AirbagModel_U->Force -
    AirbagModel_P->Gain_Gain_b * DiscreteTimeIntegrator1_DSTATE;

  /* Update for DiscreteIntegrator: '<Root>/Discrete-Time Integrator1' */
  DiscreteTimeIntegrator1_DSTATE +=
    AirbagModel_P->DiscreteTimeIntegrator1_gainval *
    DiscreteTimeIntegrator_DSTATE;

  /* Update for DiscreteIntegrator: '<Root>/Discrete-Time Integrator' incorporates:
   *  Gain: '<Root>/Divide mass'
   */
  DiscreteTimeIntegrator_DSTATE += AirbagModel_P->Dividemass_Gain * rtb_Sum1 *
    AirbagModel_P->DiscreteTimeIntegrator_gainval;

  /* Update for DiscreteIntegrator: '<Root>/Discrete-Time Integrator1' */
  AirbagModel_DW->DiscreteTimeIntegrator1_DSTATE =
    DiscreteTimeIntegrator1_DSTATE;

  /* Update for DiscreteIntegrator: '<Root>/Discrete-Time Integrator' */
  AirbagModel_DW->DiscreteTimeIntegrator_DSTATE = DiscreteTimeIntegrator_DSTATE;
}

/* Model initialize function */
void AirbagModel_initialize(RT_MODEL_AirbagModel_T *const AirbagModel_M,
  ExtU_AirbagModel_T *AirbagModel_U, ExtY_AirbagModel_T *AirbagModel_Y)
{
  P_AirbagModel_T *AirbagModel_P = ((P_AirbagModel_T *)
    AirbagModel_M->ModelData.defaultParam);
  DW_AirbagModel_T *AirbagModel_DW = ((DW_AirbagModel_T *)
    AirbagModel_M->ModelData.dwork);

  /* Registration code */

  /* states (dwork) */
  (void) memset((void *)AirbagModel_DW, 0,
                sizeof(DW_AirbagModel_T));

  /* external inputs */
  AirbagModel_U->Force = 0.0F;

  /* external outputs */
  AirbagModel_Y->ActivateAirbag = false;

  /* InitializeConditions for DiscreteIntegrator: '<Root>/Discrete-Time Integrator1' */
  AirbagModel_DW->DiscreteTimeIntegrator1_DSTATE =
    AirbagModel_P->DiscreteTimeIntegrator1_IC;

  /* InitializeConditions for DiscreteIntegrator: '<Root>/Discrete-Time Integrator' */
  AirbagModel_DW->DiscreteTimeIntegrator_DSTATE =
    AirbagModel_P->DiscreteTimeIntegrator_IC;
}

/* Model terminate function */
void AirbagModel_terminate(RT_MODEL_AirbagModel_T *const AirbagModel_M)
{
  /* (no terminate code required) */
  UNUSED_PARAMETER(AirbagModel_M);
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
