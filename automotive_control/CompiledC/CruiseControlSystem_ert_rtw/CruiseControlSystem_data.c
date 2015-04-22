/*
 * File: CruiseControlSystem_data.c
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

/* Block parameters (auto storage) */
P_CruiseControlSystem_T CruiseControlSystem_P = {
  1.0,                                 /* Computed Parameter: DiscreteTimeIntegrator_gainval
                                        * Referenced by: '<S2>/Discrete-Time Integrator'
                                        */
  0.0,                                 /* Expression: 0
                                        * Referenced by: '<S2>/Discrete-Time Integrator'
                                        */
  1.0,                                 /* Computed Parameter: DiscreteTimeIntegrator_gainva_l
                                        * Referenced by: '<S1>/Discrete-Time Integrator'
                                        */
  0.0,                                 /* Expression: 0
                                        * Referenced by: '<S1>/Discrete-Time Integrator'
                                        */
  1.0,                                 /* Expression: 1.0
                                        * Referenced by: '<S1>/Ki'
                                        */
  1.0,                                 /* Expression: 1.0
                                        * Referenced by: '<S1>/Kp'
                                        */
  50.0,                                /* Expression: 50
                                        * Referenced by: '<S2>/damping'
                                        */
  0.001                                /* Expression: 0.001
                                        * Referenced by: '<S2>/Inertia '
                                        */
};

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
