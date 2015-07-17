/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: TransmissionControl.c
 *
 * Code generated for Simulink model 'TransmissionControl'.
 *
 * Model version                  : 1.5
 * Simulink Coder version         : 8.8 (R2015a) 09-Feb-2015
 * C/C++ source code generated on : Mon May  4 13:55:47 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ASIC/FPGA->ASIC/FPGA
 * Emulation hardware selection:
 *    Differs from embedded hardware (Generic->32-bit Embedded Processor)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "TransmissionControl.h"

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
void TransmissionControl_step(RT_MODEL_TransmissionControl_T *const
  TransmissionControl_M, ExtU_TransmissionControl_T *TransmissionControl_U,
  ExtY_TransmissionControl_T *TransmissionControl_Y)
{
  P_TransmissionControl_T *TransmissionControl_P = ((P_TransmissionControl_T *)
    TransmissionControl_M->ModelData.defaultParam);

  /* Outport: '<Root>/Warning Light' incorporates:
   *  Constant: '<S1>/Constant'
   *  Inport: '<Root>/Fluid Sensor'
   *  RelationalOperator: '<S1>/Compare'
   */
  TransmissionControl_Y->WarningLight = (TransmissionControl_U->FluidSensor <=
    TransmissionControl_P->LowFuelSensor_const);
}

/* Model initialize function */
void TransmissionControl_initialize(RT_MODEL_TransmissionControl_T *const
  TransmissionControl_M, ExtU_TransmissionControl_T *TransmissionControl_U,
  ExtY_TransmissionControl_T *TransmissionControl_Y)
{
  /* Registration code */

  /* external inputs */
  TransmissionControl_U->FluidSensor = 0.0F;

  /* external outputs */
  TransmissionControl_Y->WarningLight = false;
  UNUSED_PARAMETER(TransmissionControl_M);
}

/* Model terminate function */
void TransmissionControl_terminate(RT_MODEL_TransmissionControl_T *const
  TransmissionControl_M)
{
  /* (no terminate code required) */
  UNUSED_PARAMETER(TransmissionControl_M);
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
