/*
 * File: CruiseControlSystem.c
 *
 * Code generated for Simulink model 'CruiseControlSystem'.
 *
 * Model version                  : 1.8
 * Simulink Coder version         : 8.6 (R2014a) 27-Dec-2013
 * C/C++ source code generated on : Mon Mar 23 13:31:51 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Generic->32-bit Embedded Processor
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "CruiseControlSystem.h"

/* Block states (auto storage) */
DW_CruiseControlSystem_T CruiseControlSystem_DW;

/* Real-time model */
RT_MODEL_CruiseControlSystem_T CruiseControlSystem_M_;
RT_MODEL_CruiseControlSystem_T *const CruiseControlSystem_M =
  &CruiseControlSystem_M_;

/* Model step function */
void CruiseControlSystem_step(void)
{
  /* DiscreteIntegrator: '<S2>/Discrete-Time Integrator' */
  cc_current_speed = CruiseControlSystem_DW.DiscreteTimeIntegrator_DSTATE;

  /* Update for DiscreteIntegrator: '<S2>/Discrete-Time Integrator' incorporates:
   *  DiscreteIntegrator: '<S1>/Discrete-Time Integrator'
   *  Gain: '<S1>/Ki'
   *  Gain: '<S1>/Kp'
   *  Gain: '<S2>/Inertia '
   *  Gain: '<S2>/damping'
   *  Sum: '<S1>/Sum1'
   *  Sum: '<S2>/Sum'
   */
  CruiseControlSystem_DW.DiscreteTimeIntegrator_DSTATE +=
    ((CruiseControlSystem_P.Kp_Gain *
      CruiseControlSystem_DW.DiscreteTimeIntegrator_DSTATE_o +
      CruiseControlSystem_P.Ki_Gain *
      CruiseControlSystem_DW.DiscreteTimeIntegrator_DSTATE_o) -
     CruiseControlSystem_P.damping_Gain * cc_current_speed) *
    CruiseControlSystem_P.Inertia_Gain *
    CruiseControlSystem_P.DiscreteTimeIntegrator_gainval;

  /* Update for DiscreteIntegrator: '<S1>/Discrete-Time Integrator' incorporates:
   *  Update for Inport: '<Root>/In1'
   *  Sum: '<S1>/Sum'
   */
  CruiseControlSystem_DW.DiscreteTimeIntegrator_DSTATE_o += (cc_desired_speed -
    cc_current_speed) * CruiseControlSystem_P.DiscreteTimeIntegrator_gainva_l;
}

/* Model initialize function */
void CruiseControlSystem_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(CruiseControlSystem_M, (NULL));

  /* states (dwork) */
  (void) memset((void *)&CruiseControlSystem_DW, 0,
                sizeof(DW_CruiseControlSystem_T));

  /* InitializeConditions for DiscreteIntegrator: '<S2>/Discrete-Time Integrator' */
  CruiseControlSystem_DW.DiscreteTimeIntegrator_DSTATE =
    CruiseControlSystem_P.DiscreteTimeIntegrator_IC;

  /* InitializeConditions for DiscreteIntegrator: '<S1>/Discrete-Time Integrator' */
  CruiseControlSystem_DW.DiscreteTimeIntegrator_DSTATE_o =
    CruiseControlSystem_P.DiscreteTimeIntegrator_IC_n;
}

/* Model terminate function */
void CruiseControlSystem_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
