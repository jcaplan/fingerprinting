/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: TractionControl.c
 *
 * Code generated for Simulink model 'TractionControl'.
 *
 * Model version                  : 1.11
 * Simulink Coder version         : 8.8 (R2015a) 09-Feb-2015
 * C/C++ source code generated on : Thu Jul 30 17:13:16 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ASIC/FPGA->ASIC/FPGA
 * Emulation hardware selection:
 *    Differs from embedded hardware (Generic->32-bit Embedded Processor)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "TractionControl.h"

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
void TractionControl_step(RT_MODEL_TractionControl_T *const TractionControl_M,
  ExtU_TractionControl_T *TractionControl_U, ExtY_TractionControl_T
  *TractionControl_Y)
{
  P_TractionControl_T *TractionControl_P = ((P_TractionControl_T *)
    TractionControl_M->ModelData.defaultParam);
  real32_T rtb_Subtract;

  /* Sum: '<Root>/Subtract' incorporates:
   *  Inport: '<Root>/Current Speed(Powered Axle)'
   *  Inport: '<Root>/Speed(Unpowered Axle)'
   */
  rtb_Subtract = TractionControl_U->CurrentSpeedPoweredAxle -
    TractionControl_U->SpeedUnpoweredAxle;

  /* Outport: '<Root>/Control Feedback' incorporates:
   *  Constant: '<S1>/Constant'
   *  Product: '<Root>/Product'
   *  RelationalOperator: '<S1>/Compare'
   */
  TractionControl_Y->ControlFeedback = (real32_T)(rtb_Subtract <=
    TractionControl_P->CompareToConstant_const) * rtb_Subtract;
}

/* Model initialize function */
void TractionControl_initialize(RT_MODEL_TractionControl_T *const
  TractionControl_M, ExtU_TractionControl_T *TractionControl_U,
  ExtY_TractionControl_T *TractionControl_Y)
{
  /* Registration code */

  /* external inputs */
  (void) memset((void *)TractionControl_U, 0,
                sizeof(ExtU_TractionControl_T));

  /* external outputs */
  TractionControl_Y->ControlFeedback = 0.0F;
  UNUSED_PARAMETER(TractionControl_M);
}

/* Model terminate function */
void TractionControl_terminate(RT_MODEL_TractionControl_T *const
  TractionControl_M)
{
  /* (no terminate code required) */
  UNUSED_PARAMETER(TractionControl_M);
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
