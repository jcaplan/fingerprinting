/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: Derivative.c
 *
 * Code generated for Simulink model 'Derivative'.
 *
 * Model version                  : 1.5
 * Simulink Coder version         : 8.8 (R2015a) 09-Feb-2015
 * C/C++ source code generated on : Mon May  4 13:58:04 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Generic->32-bit Embedded Processor
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "Derivative.h"

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
void Derivative_step(RT_MODEL_Derivative_T *const Derivative_M,
                     ExtU_Derivative_T *Derivative_U, ExtY_Derivative_T
                     *Derivative_Y)
{
  P_Derivative_T *Derivative_P = ((P_Derivative_T *)
    Derivative_M->ModelData.defaultParam);
  DW_Derivative_T *Derivative_DW = ((DW_Derivative_T *)
    Derivative_M->ModelData.dwork);
  real_T rtb_TSamp;

  /* SampleTimeMath: '<S2>/TSamp' incorporates:
   *  Inport: '<Root>/In1'
   *
   * About '<S2>/TSamp':
   *  y = u * K where K = 1 / ( w * Ts )
   */
  rtb_TSamp = Derivative_U->In1 * Derivative_P->TSamp_WtEt;

  /* Outport: '<Root>/Out1' incorporates:
   *  Sum: '<S2>/Diff'
   *  UnitDelay: '<S2>/UD'
   */
  Derivative_Y->Out1 = rtb_TSamp - Derivative_DW->UD_DSTATE;

  /* Update for UnitDelay: '<S2>/UD' */
  Derivative_DW->UD_DSTATE = rtb_TSamp;
}

/* Model initialize function */
void Derivative_initialize(RT_MODEL_Derivative_T *const Derivative_M,
  ExtU_Derivative_T *Derivative_U, ExtY_Derivative_T *Derivative_Y)
{
  P_Derivative_T *Derivative_P = ((P_Derivative_T *)
    Derivative_M->ModelData.defaultParam);
  DW_Derivative_T *Derivative_DW = ((DW_Derivative_T *)
    Derivative_M->ModelData.dwork);

  /* Registration code */

  /* states (dwork) */
  (void) memset((void *)Derivative_DW, 0,
                sizeof(DW_Derivative_T));

  /* external inputs */
  Derivative_U->In1 = 0.0;

  /* external outputs */
  Derivative_Y->Out1 = 0.0;

  /* InitializeConditions for UnitDelay: '<S2>/UD' */
  Derivative_DW->UD_DSTATE = Derivative_P->DiscreteDerivative_ICPrevScaled;
}

/* Model terminate function */
void Derivative_terminate(RT_MODEL_Derivative_T *const Derivative_M)
{
  /* (no terminate code required) */
  UNUSED_PARAMETER(Derivative_M);
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
