/*
 * File: TransmissionControl.c
 *
 * Code generated for Simulink model 'TransmissionControl'.
 *
 * Model version                  : 1.4
 * Simulink Coder version         : 8.6 (R2014a) 27-Dec-2013
 * C/C++ source code generated on : Sun Mar 22 13:54:15 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ASIC/FPGA->ASIC/FPGA
 * Emulation hardware selection:
 *    Differs from embedded hardware (Generic->32-bit Embedded Processor)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "TransmissionControl.h"

/* Real-time model */
RT_MODEL_TransmissionControl_T TransmissionControl_M_;
RT_MODEL_TransmissionControl_T *const TransmissionControl_M =
  &TransmissionControl_M_;

/* Model step function */
void TransmissionControl_step(void)
{
  /* RelationalOperator: '<S1>/Compare' incorporates:
   *  Constant: '<S1>/Constant'
   *  Inport: '<Root>/Fluid Sensor'
   */
  warning_light = (fluid_sensor <= TransmissionControl_P.LowFuelSensor_const);
}

/* Model initialize function */
void TransmissionControl_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(TransmissionControl_M, (NULL));
}

/* Model terminate function */
void TransmissionControl_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
