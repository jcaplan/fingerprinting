/*
 * File: CollisionAvoidance.c
 *
 * Code generated for Simulink model 'CollisionAvoidance'.
 *
 * Model version                  : 1.4
 * Simulink Coder version         : 8.6 (R2014a) 27-Dec-2013
 * C/C++ source code generated on : Mon Mar 23 13:28:59 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Generic->32-bit Embedded Processor
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "CollisionAvoidance.h"

/* Block states (auto storage) */
DW_CollisionAvoidance_T CollisionAvoidance_DW;

/* Real-time model */
RT_MODEL_CollisionAvoidance_T CollisionAvoidance_M_;
RT_MODEL_CollisionAvoidance_T *const CollisionAvoidance_M =
  &CollisionAvoidance_M_;

/* Model step function */
void CollisionAvoidance_step(void)
{
  real_T rtb_TSamp;
  real_T rtb_TSamp_d;

  /* SampleTimeMath: '<S3>/TSamp' incorporates:
   *  Inport: '<Root>/In1'
   *
   * About '<S3>/TSamp':
   *  y = u * K where K = 1 / ( w * Ts )
   */
  rtb_TSamp = ca_radar_sensor * CollisionAvoidance_P.TSamp_WtEt;

  /* SampleTimeMath: '<S4>/TSamp' incorporates:
   *  Sum: '<S3>/Diff'
   *  UnitDelay: '<S3>/UD'
   *
   * About '<S4>/TSamp':
   *  y = u * K where K = 1 / ( w * Ts )
   */
  rtb_TSamp_d = (rtb_TSamp - CollisionAvoidance_DW.UD_DSTATE) *
    CollisionAvoidance_P.TSamp_WtEt_m;

  /* RelationalOperator: '<S2>/Compare' incorporates:
   *  Constant: '<S2>/Constant'
   *  Inport: '<Root>/In2'
   *  Sum: '<S1>/Subtract'
   *  Sum: '<S4>/Diff'
   *  UnitDelay: '<S4>/UD'
   */
  ca_light_on = ((rtb_TSamp_d - CollisionAvoidance_DW.UD_DSTATE_g) -
                 ca_acceleration <= CollisionAvoidance_P.Constant_Value);

  /* Update for UnitDelay: '<S3>/UD' */
  CollisionAvoidance_DW.UD_DSTATE = rtb_TSamp;

  /* Update for UnitDelay: '<S4>/UD' */
  CollisionAvoidance_DW.UD_DSTATE_g = rtb_TSamp_d;
}

/* Model initialize function */
void CollisionAvoidance_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(CollisionAvoidance_M, (NULL));

  /* states (dwork) */
  (void) memset((void *)&CollisionAvoidance_DW, 0,
                sizeof(DW_CollisionAvoidance_T));

  /* InitializeConditions for UnitDelay: '<S3>/UD' */
  CollisionAvoidance_DW.UD_DSTATE =
    CollisionAvoidance_P.DiscreteDerivative_ICPrevScaled;

  /* InitializeConditions for UnitDelay: '<S4>/UD' */
  CollisionAvoidance_DW.UD_DSTATE_g =
    CollisionAvoidance_P.DiscreteDerivative1_ICPrevScale;
}

/* Model terminate function */
void CollisionAvoidance_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
