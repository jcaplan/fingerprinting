/*
 * File: TractionControl.c
 *
 * Code generated for Simulink model 'TractionControl'.
 *
 * Model version                  : 1.6
 * Simulink Coder version         : 8.6 (R2014a) 27-Dec-2013
 * C/C++ source code generated on : Sun Mar 22 13:37:13 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ASIC/FPGA->ASIC/FPGA
 * Emulation hardware selection:
 *    Differs from embedded hardware (Generic->32-bit Embedded Processor)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "TractionControl.h"

/* Real-time model */
RT_MODEL_TractionControl_T TractionControl_M_;
RT_MODEL_TractionControl_T *const TractionControl_M = &TractionControl_M_;

/* Model step function */
void TractionControl_step(void)
{
  real_T rtb_Subtract;

  /* Sum: '<Root>/Subtract' incorporates:
   *  Inport: '<Root>/Current Speed(Powered Axle)'
   *  Inport: '<Root>/Speed(Unpowered Axle)'
   */
  rtb_Subtract = current_speed_powered - current_speed_unpowered;

  /* Product: '<Root>/Product' incorporates:
   *  Constant: '<S1>/Constant'
   *  RelationalOperator: '<S1>/Compare'
   */
  traction_control_feedback = (real_T)(rtb_Subtract <=
    TractionControl_P.CompareToConstant_const) * rtb_Subtract;
}

/* Model initialize function */
void TractionControl_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(TractionControl_M, (NULL));
}

/* Model terminate function */
void TractionControl_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
