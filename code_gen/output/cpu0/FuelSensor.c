/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: FuelSensor.c
 *
 * Code generated for Simulink model 'FuelSensor'.
 *
 * Model version                  : 1.9
 * Simulink Coder version         : 8.8 (R2015a) 09-Feb-2015
 * C/C++ source code generated on : Thu Jul 30 17:04:31 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Generic->32-bit Embedded Processor
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "FuelSensor.h"

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
void FuelSensor_step(RT_MODEL_FuelSensor_T *const FuelSensor_M,
                     ExtU_FuelSensor_T *FuelSensor_U, ExtY_FuelSensor_T
                     *FuelSensor_Y)
{
  P_FuelSensor_T *FuelSensor_P = ((P_FuelSensor_T *)
    FuelSensor_M->ModelData.defaultParam);
  DW_FuelSensor_T *FuelSensor_DW = ((DW_FuelSensor_T *)
    FuelSensor_M->ModelData.dwork);

  /* Outport: '<Root>/Fuel Display' incorporates:
   *  Inport: '<Root>/Fuel Level'
   */
  FuelSensor_Y->FuelDisplay = FuelSensor_U->FuelLevel;

  /* Outport: '<Root>/Warning Light' incorporates:
   *  UnitDelay: '<Root>/Light on  '
   */
  FuelSensor_Y->WarningLight = FuelSensor_DW->Lighton_DSTATE;

  /* Update for UnitDelay: '<Root>/Light on  ' incorporates:
   *  Constant: '<S1>/Constant'
   *  Inport: '<Root>/Fuel Level'
   *  RelationalOperator: '<S1>/Compare'
   */
  FuelSensor_DW->Lighton_DSTATE = (FuelSensor_U->FuelLevel <=
    FuelSensor_P->LowFuelSensor_const);
}

/* Model initialize function */
void FuelSensor_initialize(RT_MODEL_FuelSensor_T *const FuelSensor_M,
  ExtU_FuelSensor_T *FuelSensor_U, ExtY_FuelSensor_T *FuelSensor_Y)
{
  P_FuelSensor_T *FuelSensor_P = ((P_FuelSensor_T *)
    FuelSensor_M->ModelData.defaultParam);
  DW_FuelSensor_T *FuelSensor_DW = ((DW_FuelSensor_T *)
    FuelSensor_M->ModelData.dwork);

  /* Registration code */

  /* states (dwork) */
  (void) memset((void *)FuelSensor_DW, 0,
                sizeof(DW_FuelSensor_T));

  /* external inputs */
  FuelSensor_U->FuelLevel = 0.0F;

  /* external outputs */
  (void) memset((void *)FuelSensor_Y, 0,
                sizeof(ExtY_FuelSensor_T));

  /* InitializeConditions for UnitDelay: '<Root>/Light on  ' */
  FuelSensor_DW->Lighton_DSTATE = FuelSensor_P->Lighton_InitialCondition;
}

/* Model terminate function */
void FuelSensor_terminate(RT_MODEL_FuelSensor_T *const FuelSensor_M)
{
  /* (no terminate code required) */
  UNUSED_PARAMETER(FuelSensor_M);
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
