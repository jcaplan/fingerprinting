/*
 * File: FuelSensor.c
 *
 * Code generated for Simulink model 'FuelSensor'.
 *
 * Model version                  : 1.5
 * Simulink Coder version         : 8.6 (R2014a) 27-Dec-2013
 * C/C++ source code generated on : Sun Mar 22 14:02:44 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Generic->32-bit Embedded Processor
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "FuelSensor.h"

/* Block states (auto storage) */
DW_FuelSensor_T FuelSensor_DW;

/* Real-time model */
RT_MODEL_FuelSensor_T FuelSensor_M_;
RT_MODEL_FuelSensor_T *const FuelSensor_M = &FuelSensor_M_;

/* Model step function */
void FuelSensor_step(void)
{
  /* UnitDelay: '<Root>/Light on  ' */
  fs_warning_light = FuelSensor_DW.Lighton_DSTATE;

  /* Update for UnitDelay: '<Root>/Light on  ' incorporates:
   *  Constant: '<S1>/Constant'
   *  Update for Inport: '<Root>/Fuel Level'
   *  RelationalOperator: '<S1>/Compare'
   */
  FuelSensor_DW.Lighton_DSTATE = (fs_fuel_level <=
    FuelSensor_P.LowFuelSensor_const);
}

/* Model initialize function */
void FuelSensor_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(FuelSensor_M, (NULL));

  /* states (dwork) */
  (void) memset((void *)&FuelSensor_DW, 0,
                sizeof(DW_FuelSensor_T));

  /* InitializeConditions for UnitDelay: '<Root>/Light on  ' */
  FuelSensor_DW.Lighton_DSTATE = FuelSensor_P.Lighton_InitialCondition;
}

/* Model terminate function */
void FuelSensor_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
