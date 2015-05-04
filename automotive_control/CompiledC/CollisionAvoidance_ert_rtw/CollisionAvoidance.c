/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: CollisionAvoidance.c
 *
 * Code generated for Simulink model 'CollisionAvoidance'.
 *
 * Model version                  : 1.6
 * Simulink Coder version         : 8.8 (R2015a) 09-Feb-2015
 * C/C++ source code generated on : Mon May  4 13:02:04 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Generic->32-bit Embedded Processor
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "CollisionAvoidance.h"

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
void CollisionAvoidance_step(RT_MODEL_CollisionAvoidance_T *const
  CollisionAvoidance_M, ExtU_CollisionAvoidance_T *CollisionAvoidance_U,
  ExtY_CollisionAvoidance_T *CollisionAvoidance_Y)
{
  P_CollisionAvoidance_T *CollisionAvoidance_P = ((P_CollisionAvoidance_T *)
    CollisionAvoidance_M->ModelData.defaultParam);
  DW_CollisionAvoidance_T *CollisionAvoidance_DW = ((DW_CollisionAvoidance_T *)
    CollisionAvoidance_M->ModelData.dwork);
  real_T rtb_TSamp;
  real_T rtb_TSamp_d;

  /* SampleTimeMath: '<S3>/TSamp' incorporates:
   *  Inport: '<Root>/In1'
   *
   * About '<S3>/TSamp':
   *  y = u * K where K = 1 / ( w * Ts )
   */
  rtb_TSamp = CollisionAvoidance_U->In1 * CollisionAvoidance_P->TSamp_WtEt;

  /* SampleTimeMath: '<S4>/TSamp' incorporates:
   *  Sum: '<S3>/Diff'
   *  UnitDelay: '<S3>/UD'
   *
   * About '<S4>/TSamp':
   *  y = u * K where K = 1 / ( w * Ts )
   */
  rtb_TSamp_d = (rtb_TSamp - CollisionAvoidance_DW->UD_DSTATE) *
    CollisionAvoidance_P->TSamp_WtEt_m;

  /* Outport: '<Root>/Out1' incorporates:
   *  Constant: '<S2>/Constant'
   *  Inport: '<Root>/In2'
   *  RelationalOperator: '<S2>/Compare'
   *  Sum: '<S1>/Subtract'
   *  Sum: '<S4>/Diff'
   *  UnitDelay: '<S4>/UD'
   */
  CollisionAvoidance_Y->Out1 = ((rtb_TSamp_d -
    CollisionAvoidance_DW->UD_DSTATE_g) - CollisionAvoidance_U->In2 <=
    CollisionAvoidance_P->Constant_Value);

  /* Update for UnitDelay: '<S3>/UD' */
  CollisionAvoidance_DW->UD_DSTATE = rtb_TSamp;

  /* Update for UnitDelay: '<S4>/UD' */
  CollisionAvoidance_DW->UD_DSTATE_g = rtb_TSamp_d;
}

/* Model initialize function */
void CollisionAvoidance_initialize(RT_MODEL_CollisionAvoidance_T *const
  CollisionAvoidance_M, ExtU_CollisionAvoidance_T *CollisionAvoidance_U,
  ExtY_CollisionAvoidance_T *CollisionAvoidance_Y)
{
  P_CollisionAvoidance_T *CollisionAvoidance_P = ((P_CollisionAvoidance_T *)
    CollisionAvoidance_M->ModelData.defaultParam);
  DW_CollisionAvoidance_T *CollisionAvoidance_DW = ((DW_CollisionAvoidance_T *)
    CollisionAvoidance_M->ModelData.dwork);

  /* Registration code */

  /* states (dwork) */
  (void) memset((void *)CollisionAvoidance_DW, 0,
                sizeof(DW_CollisionAvoidance_T));

  /* external inputs */
  (void) memset((void *)CollisionAvoidance_U, 0,
                sizeof(ExtU_CollisionAvoidance_T));

  /* external outputs */
  CollisionAvoidance_Y->Out1 = false;

  /* InitializeConditions for UnitDelay: '<S3>/UD' */
  CollisionAvoidance_DW->UD_DSTATE =
    CollisionAvoidance_P->DiscreteDerivative_ICPrevScaled;

  /* InitializeConditions for UnitDelay: '<S4>/UD' */
  CollisionAvoidance_DW->UD_DSTATE_g =
    CollisionAvoidance_P->DiscreteDerivative1_ICPrevScale;
}

/* Model terminate function */
void CollisionAvoidance_terminate(RT_MODEL_CollisionAvoidance_T *const
  CollisionAvoidance_M)
{
  /* (no terminate code required) */
  UNUSED_PARAMETER(CollisionAvoidance_M);
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
