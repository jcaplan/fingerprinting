/*
 * File: ShiftLogic_data.c
 *
 * Code generated for Simulink model 'ShiftLogic'.
 *
 * Model version                  : 1.255
 * Simulink Coder version         : 8.4 (R2013a) 13-Feb-2013
 * TLC version                    : 8.4 (Jan 18 2013)
 * C/C++ source code generated on : Sun Aug 25 20:36:14 2013
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: 32-bit Generic
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "ShiftLogic.h"
#include "ShiftLogic_private.h"

/* Block parameters (auto storage) */
P_ShiftLogic_T ShiftLogic_P = {
  0.0,                                 /* Expression: 0
                                        * Referenced by: '<S2>/down_th'
                                        */
  0.0,                                 /* Expression: 0
                                        * Referenced by: '<S2>/up_th'
                                        */

  /*  Expression: DOWN_TABLE
   * Referenced by: '<S2>/InterpDown'
   */
  { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 5.0, 5.0, 5.0, 5.0, 30.0, 30.0, 20.0, 20.0,
    25.0, 30.0, 50.0, 50.0, 35.0, 35.0, 40.0, 50.0, 80.0, 80.0 },

  /*  Expression: DOWN_TH_BP
   * Referenced by: '<S2>/InterpDown'
   */
  { 0.0, 5.0, 40.0, 50.0, 90.0, 100.0 },

  /*  Expression: [1:4]
   * Referenced by: '<S2>/InterpDown'
   */
  { 1.0, 2.0, 3.0, 4.0 },

  /*  Expression: UP_TABLE
   * Referenced by: '<S2>/InterpUp'
   */
  { 10.0, 10.0, 15.0, 23.0, 40.0, 40.0, 30.0, 30.0, 30.0, 41.0, 70.0, 70.0, 50.0,
    50.0, 50.0, 60.0, 100.0, 100.0, 1.0E+6, 1.0E+6, 1.0E+6, 1.0E+6, 1.0E+6,
    1.0E+6 },

  /*  Expression: UP_TH_BP
   * Referenced by: '<S2>/InterpUp'
   */
  { 0.0, 25.0, 35.0, 50.0, 90.0, 100.0 },

  /*  Expression: [1:4]
   * Referenced by: '<S2>/InterpUp'
   */
  { 1.0, 2.0, 3.0, 4.0 },
  2.0,                                 /* Expression: TWAIT
                                        * Referenced by: '<Root>/ShiftLogic'
                                        */

  /*  Computed Parameter: InterpDown_maxIndex
   * Referenced by: '<S2>/InterpDown'
   */
  { 5U, 3U },

  /*  Computed Parameter: InterpUp_maxIndex
   * Referenced by: '<S2>/InterpUp'
   */
  { 5U, 3U }
};

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
