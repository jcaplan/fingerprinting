/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: AirbagModel.c
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

#include "AirbagModel.h"

/*===========*
 * Constants *
 *===========*/
#define RT_PI                          3.14159265358979323846
#define RT_PIF                         3.1415927F
#define RT_LN_10                       2.30258509299404568402
#define RT_LN_10F                      2.3025851F
#define RT_LOG10E                      0.43429448190325182765
#define RT_LOG10EF                     0.43429449F
#define RT_E                           2.7182818284590452354
#define RT_EF                          2.7182817F

/*
 * UNUSED_PARAMETER(x)
 *   Used to specify that a function parameter (argument) is required but not
 *   accessed by the function body.
 */
#ifndef UNUSED_PARAMETER
# if defined(__LCC__)
#   define UNUSED_PARAMETER(x)                                   /* do nothing */
# else

/*
 * This is the semi-ANSI standard way of indicating that an
 * unused function parameter is required.
 */
#   define UNUSED_PARAMETER(x)         (void) (x)
# endif
#endif

/* Model step function */
void AirbagModel_step(RT_MODEL_AirbagModel_T *const AirbagModel_M,
                      ExtU_AirbagModel_T *AirbagModel_U, ExtY_AirbagModel_T
                      *AirbagModel_Y)
{
  P_AirbagModel_T *AirbagModel_P = ((P_AirbagModel_T *)
    AirbagModel_M->ModelData.defaultParam);
  DW_AirbagModel_T *AirbagModel_DW = ((DW_AirbagModel_T *)
    AirbagModel_M->ModelData.dwork);
  real_T rtb_Sum1;

  /* Outport: '<Root>/Activate Airbag ' incorporates:
   *  Constant: '<S1>/Constant'
   *  DiscreteIntegrator: '<Root>/Discrete-Time Integrator1'
   *  RelationalOperator: '<S1>/Compare'
   */
  AirbagModel_Y->ActivateAirbag =
    (AirbagModel_DW->DiscreteTimeIntegrator1_DSTATE <=
     AirbagModel_P->CompareToConstant_const);

  /* Sum: '<Root>/Sum1' incorporates:
   *  DiscreteIntegrator: '<Root>/Discrete-Time Integrator1'
   *  Gain: '<Root>/Gain'
   *  Inport: '<Root>/Force'
   */
  rtb_Sum1 = AirbagModel_U->Force - AirbagModel_P->Gain_Gain *
    AirbagModel_DW->DiscreteTimeIntegrator1_DSTATE;

  /* Update for DiscreteIntegrator: '<Root>/Discrete-Time Integrator1' incorporates:
   *  DiscreteIntegrator: '<Root>/Discrete-Time Integrator'
   */
  AirbagModel_DW->DiscreteTimeIntegrator1_DSTATE +=
    AirbagModel_P->DiscreteTimeIntegrator1_gainval *
    AirbagModel_DW->DiscreteTimeIntegrator_DSTATE;

  /* Update for DiscreteIntegrator: '<Root>/Discrete-Time Integrator' incorporates:
   *  Gain: '<Root>/Divide mass'
   */
  AirbagModel_DW->DiscreteTimeIntegrator_DSTATE +=
    AirbagModel_P->Dividemass_Gain * rtb_Sum1 *
    AirbagModel_P->DiscreteTimeIntegrator_gainval;
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
  AirbagModel_U->Force = 0.0;

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
