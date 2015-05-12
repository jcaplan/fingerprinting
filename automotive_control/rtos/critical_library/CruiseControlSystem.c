/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: CruiseControlSystem.c
 *
 * Code generated for Simulink model 'CruiseControlSystem'.
 *
 * Model version                  : 1.10
 * Simulink Coder version         : 8.8 (R2015a) 09-Feb-2015
 * C/C++ source code generated on : Mon May  4 13:53:44 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Generic->32-bit Embedded Processor
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "CruiseControlSystem.h"

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
void CruiseControlSystem_step(RT_MODEL_CruiseControlSystem_T *const
  CruiseControlSystem_M, ExtU_CruiseControlSystem_T *CruiseControlSystem_U,
  ExtY_CruiseControlSystem_T *CruiseControlSystem_Y)
{
  P_CruiseControlSystem_T *CruiseControlSystem_P = ((P_CruiseControlSystem_T *)
    CruiseControlSystem_M->ModelData.defaultParam);
  DW_CruiseControlSystem_T *CruiseControlSystem_DW = ((DW_CruiseControlSystem_T *)
    CruiseControlSystem_M->ModelData.dwork);
  real_T rtb_Sum_n;

  /* Outport: '<Root>/Out1' incorporates:
   *  DiscreteIntegrator: '<S2>/Discrete-Time Integrator'
   */
  CruiseControlSystem_Y->Out1 =
    CruiseControlSystem_DW->DiscreteTimeIntegrator_DSTATE;

  /* Sum: '<S1>/Sum' incorporates:
   *  DiscreteIntegrator: '<S2>/Discrete-Time Integrator'
   *  Inport: '<Root>/In1'
   */
  rtb_Sum_n = CruiseControlSystem_U->In1 -
    CruiseControlSystem_DW->DiscreteTimeIntegrator_DSTATE;

  /* Update for DiscreteIntegrator: '<S2>/Discrete-Time Integrator' incorporates:
   *  DiscreteIntegrator: '<S1>/Discrete-Time Integrator'
   *  DiscreteIntegrator: '<S2>/Discrete-Time Integrator'
   *  Gain: '<S1>/Ki'
   *  Gain: '<S1>/Kp'
   *  Gain: '<S2>/Inertia '
   *  Gain: '<S2>/damping'
   *  Sum: '<S1>/Sum1'
   *  Sum: '<S2>/Sum'
   */
  CruiseControlSystem_DW->DiscreteTimeIntegrator_DSTATE +=
    ((CruiseControlSystem_P->Kp_Gain *
      CruiseControlSystem_DW->DiscreteTimeIntegrator_DSTATE_o +
      CruiseControlSystem_P->Ki_Gain *
      CruiseControlSystem_DW->DiscreteTimeIntegrator_DSTATE_o) -
     CruiseControlSystem_P->damping_Gain *
     CruiseControlSystem_DW->DiscreteTimeIntegrator_DSTATE) *
    CruiseControlSystem_P->Inertia_Gain *
    CruiseControlSystem_P->DiscreteTimeIntegrator_gainval;

  /* Update for DiscreteIntegrator: '<S1>/Discrete-Time Integrator' */
  CruiseControlSystem_DW->DiscreteTimeIntegrator_DSTATE_o +=
    CruiseControlSystem_P->DiscreteTimeIntegrator_gainva_l * rtb_Sum_n;
}

/* Model initialize function */
void CruiseControlSystem_initialize(RT_MODEL_CruiseControlSystem_T *const
  CruiseControlSystem_M, ExtU_CruiseControlSystem_T *CruiseControlSystem_U,
  ExtY_CruiseControlSystem_T *CruiseControlSystem_Y)
{
  P_CruiseControlSystem_T *CruiseControlSystem_P = ((P_CruiseControlSystem_T *)
    CruiseControlSystem_M->ModelData.defaultParam);
  DW_CruiseControlSystem_T *CruiseControlSystem_DW = ((DW_CruiseControlSystem_T *)
    CruiseControlSystem_M->ModelData.dwork);

  /* Registration code */

  /* states (dwork) */
  (void) memset((void *)CruiseControlSystem_DW, 0,
                sizeof(DW_CruiseControlSystem_T));

  /* external inputs */
  (void) memset((void *)CruiseControlSystem_U, 0,
                sizeof(ExtU_CruiseControlSystem_T));

  /* external outputs */
  CruiseControlSystem_Y->Out1 = 0.0;

  /* InitializeConditions for DiscreteIntegrator: '<S2>/Discrete-Time Integrator' */
  CruiseControlSystem_DW->DiscreteTimeIntegrator_DSTATE =
    CruiseControlSystem_P->DiscreteTimeIntegrator_IC;

  /* InitializeConditions for DiscreteIntegrator: '<S1>/Discrete-Time Integrator' */
  CruiseControlSystem_DW->DiscreteTimeIntegrator_DSTATE_o =
    CruiseControlSystem_P->DiscreteTimeIntegrator_IC_n;
}

/* Model terminate function */
void CruiseControlSystem_terminate(RT_MODEL_CruiseControlSystem_T *const
  CruiseControlSystem_M)
{
  /* (no terminate code required) */
  UNUSED_PARAMETER(CruiseControlSystem_M);
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
