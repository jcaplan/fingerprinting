/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: RadarTracker.c
 *
 * Code generated for Simulink model 'RadarTracker'.
 *
 * Model version                  : 1.106
 * Simulink Coder version         : 8.8 (R2015a) 09-Feb-2015
 * C/C++ source code generated on : Fri Jul 31 11:31:30 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Specified
 * Code generation objectives:
 *    1. MISRA-C:2004 guidelines
 *    2. Safety precaution
 * Validation result: Not run
 */

#include "RadarTracker.h"
#include "RadarTracker_private.h"

real32_T rt_atan2f_snf(real32_T u0, real32_T u1)
{
  real32_T y;
  int32_T u0_0;
  int32_T u1_0;
  if (rtIsNaNF(u0) || rtIsNaNF(u1)) {
    y = (rtNaNF);
  } else if (rtIsInfF(u0) && rtIsInfF(u1)) {
    if (u0 > 0.0F) {
      u0_0 = 1;
    } else {
      u0_0 = -1;
    }

    if (u1 > 0.0F) {
      u1_0 = 1;
    } else {
      u1_0 = -1;
    }

    y = (real32_T)atan2f((real32_T)u0_0, (real32_T)u1_0);
  } else if (u1 == 0.0F) {
    if (u0 > 0.0F) {
      y = RT_PIF / 2.0F;
    } else if (u0 < 0.0F) {
      y = -(RT_PIF / 2.0F);
    } else {
      y = 0.0F;
    }
  } else {
    y = (real32_T)atan2f(u0, u1);
  }

  return y;
}

/* Model step function */
void RadarTracker_step(RT_MODEL_RadarTracker_T *const RadarTracker_M,
  ExtU_RadarTracker_T *RadarTracker_U, ExtY_RadarTracker_T *RadarTracker_Y)
{
  DW_RadarTracker_T *RadarTracker_DW = ((DW_RadarTracker_T *)
    RadarTracker_M->ModelData.dwork);
  int8_T Phi[16];
  real_T Q[16];
  real32_T Rangehat;
  real32_T Bearinghat;
  real32_T M[8];
  real32_T W[8];
  real32_T x[4];
  static const real32_T b[4] = { 90000.0F, 0.0F, 0.0F, 1.0E-6F };

  real32_T Phi_0[16];
  int32_T i;
  real32_T Phi_1[4];
  real32_T M_0[8];
  real32_T Q_0[16];
  real32_T I[16];
  int32_T i_0;
  real32_T rtb_residual_idx_0;
  real32_T rtb_residual_idx_1;
  real32_T b_y_idx_0;
  real32_T b_y_idx_1;
  real32_T b_y_idx_2;

  /* MATLAB Function: '<Root>/RadarTracker' incorporates:
   *  Inport: '<Root>/meas'
   */
  /* MATLAB Function 'RadarTracker': '<S1>:1' */
  /*  AERO_EXTKALMAN Radar Data Processing Tracker Using an Extended Kalman */
  /*  Filter.  Radar update time deltat is inherited from workspace. */
  /*  Initialization */
  /*  1. Compute Phi, Q, and R  */
  /* '<S1>:1:14' */
  Phi[0] = 1;
  Phi[4] = 1;
  Phi[8] = 0;
  Phi[12] = 0;
  Phi[1] = 0;
  Phi[5] = 1;
  Phi[9] = 0;
  Phi[13] = 0;
  Phi[2] = 0;
  Phi[6] = 0;
  Phi[10] = 1;
  Phi[14] = 1;
  Phi[3] = 0;
  Phi[7] = 0;
  Phi[11] = 0;
  Phi[15] = 1;

  /* '<S1>:1:15' */
  memset(&Q[0], 0, sizeof(real_T) << 4U);
  Q[0] = 0.0;
  Q[5] = 0.005;
  Q[10] = 0.0;
  Q[15] = 0.005;

  /* '<S1>:1:16' */
  /*  2. Propagate the covariance matrix: */
  /* '<S1>:1:19' */
  for (i = 0; i < 4; i++) {
    for (i_0 = 0; i_0 < 4; i_0++) {
      Phi_0[i + (i_0 << 2)] = 0.0F;
      Phi_0[i + (i_0 << 2)] += RadarTracker_DW->P[i_0 << 2] * (real32_T)Phi[i];
      Phi_0[i + (i_0 << 2)] += RadarTracker_DW->P[(i_0 << 2) + 1] * (real32_T)
        Phi[i + 4];
      Phi_0[i + (i_0 << 2)] += RadarTracker_DW->P[(i_0 << 2) + 2] * (real32_T)
        Phi[i + 8];
      Phi_0[i + (i_0 << 2)] += RadarTracker_DW->P[(i_0 << 2) + 3] * (real32_T)
        Phi[i + 12];
    }
  }

  for (i = 0; i < 4; i++) {
    for (i_0 = 0; i_0 < 4; i_0++) {
      RadarTracker_DW->P[i + (i_0 << 2)] = (((Phi_0[i + 4] * (real32_T)Phi[i_0 +
        4] + Phi_0[i] * (real32_T)Phi[i_0]) + Phi_0[i + 8] * (real32_T)Phi[i_0 +
        8]) + Phi_0[i + 12] * (real32_T)Phi[i_0 + 12]) + (real32_T)Q[(i_0 << 2)
        + i];
    }
  }

  /*  3. Propagate the track estimate:: */
  /* '<S1>:1:22' */
  for (i = 0; i < 4; i++) {
    Rangehat = (real32_T)Phi[i + 12] * RadarTracker_DW->xhat[3] + ((real32_T)
      Phi[i + 8] * RadarTracker_DW->xhat[2] + ((real32_T)Phi[i + 4] *
      RadarTracker_DW->xhat[1] + (real32_T)Phi[i] * RadarTracker_DW->xhat[0]));
    Phi_1[i] = Rangehat;
  }

  RadarTracker_DW->xhat[0] = Phi_1[0];
  RadarTracker_DW->xhat[1] = Phi_1[1];
  RadarTracker_DW->xhat[2] = Phi_1[2];
  RadarTracker_DW->xhat[3] = Phi_1[3];

  /*  4 a). Compute observation estimates: */
  /* '<S1>:1:25' */
  Rangehat = (real32_T)sqrtf(RadarTracker_DW->xhat[0] * RadarTracker_DW->xhat[0]
    + RadarTracker_DW->xhat[2] * RadarTracker_DW->xhat[2]);

  /* '<S1>:1:26' */
  Bearinghat = rt_atan2f_snf(RadarTracker_DW->xhat[2], RadarTracker_DW->xhat[0]);

  /*  4 b). Compute observation vector y and linearized measurement matrix M  */
  /* '<S1>:1:29' */
  /* '<S1>:1:31' */
  M[0] = (real32_T)cosf(Bearinghat);
  M[2] = 0.0F;
  M[4] = (real32_T)sinf(Bearinghat);
  M[6] = 0.0F;
  M[1] = -(real32_T)sinf(Bearinghat) / Rangehat;
  M[3] = 0.0F;
  M[5] = (real32_T)cosf(Bearinghat) / Rangehat;
  M[7] = 0.0F;

  /*  4 c).  Compute residual (Estimation Error) */
  /* '<S1>:1:35' */
  rtb_residual_idx_0 = RadarTracker_U->meas[0] - Rangehat;
  rtb_residual_idx_1 = RadarTracker_U->meas[1] - Bearinghat;

  /*  5. Compute Kalman Gain: */
  /* '<S1>:1:38' */
  for (i = 0; i < 2; i++) {
    for (i_0 = 0; i_0 < 4; i_0++) {
      M_0[i + (i_0 << 1)] = 0.0F;
      M_0[i + (i_0 << 1)] += RadarTracker_DW->P[i_0 << 2] * M[i];
      M_0[i + (i_0 << 1)] += RadarTracker_DW->P[(i_0 << 2) + 1] * 0.0F;
      M_0[i + (i_0 << 1)] += RadarTracker_DW->P[(i_0 << 2) + 2] * M[i + 4];
      M_0[i + (i_0 << 1)] += RadarTracker_DW->P[(i_0 << 2) + 3] * 0.0F;
    }
  }

  for (i = 0; i < 2; i++) {
    for (i_0 = 0; i_0 < 2; i_0++) {
      x[i + (i_0 << 1)] = (((M_0[i + 2] * 0.0F + M_0[i] * M[i_0]) + M_0[i + 4] *
                            M[i_0 + 4]) + M_0[i + 6] * 0.0F) + b[(i_0 << 1) + i];
    }
  }

  if ((real32_T)fabs(x[1]) > (real32_T)fabs(x[0])) {
    Rangehat = x[0] / x[1];
    Bearinghat = 1.0F / (Rangehat * x[3] - x[2]);
    b_y_idx_0 = x[3] / x[1] * Bearinghat;
    b_y_idx_1 = -Bearinghat;
    b_y_idx_2 = -x[2] / x[1] * Bearinghat;
    Bearinghat *= Rangehat;
  } else {
    Rangehat = x[1] / x[0];
    Bearinghat = 1.0F / (x[3] - Rangehat * x[2]);
    b_y_idx_0 = x[3] / x[0] * Bearinghat;
    b_y_idx_1 = -Rangehat * Bearinghat;
    b_y_idx_2 = -x[2] / x[0] * Bearinghat;
  }

  for (i = 0; i < 4; i++) {
    for (i_0 = 0; i_0 < 2; i_0++) {
      M_0[i + (i_0 << 2)] = 0.0F;
      M_0[i + (i_0 << 2)] += RadarTracker_DW->P[i] * M[i_0];
      M_0[i + (i_0 << 2)] += RadarTracker_DW->P[i + 4] * 0.0F;
      M_0[i + (i_0 << 2)] += RadarTracker_DW->P[i + 8] * M[i_0 + 4];
      M_0[i + (i_0 << 2)] += RadarTracker_DW->P[i + 12] * 0.0F;
    }
  }

  for (i = 0; i < 4; i++) {
    W[i] = 0.0F;
    W[i] += M_0[i] * b_y_idx_0;
    W[i] += M_0[i + 4] * b_y_idx_1;
    W[i + 4] = 0.0F;
    W[i + 4] += M_0[i] * b_y_idx_2;
    W[i + 4] += M_0[i + 4] * Bearinghat;
  }

  /*  6. Update estimate */
  /* '<S1>:1:41' */
  for (i = 0; i < 4; i++) {
    RadarTracker_DW->xhat[i] += W[i + 4] * rtb_residual_idx_1 + W[i] *
      rtb_residual_idx_0;
  }

  /*  7. Update Covariance Matrix */
  /* '<S1>:1:44' */
  for (i = 0; i < 16; i++) {
    Q[i] = 0.0;
    Phi[i] = 0;
  }

  Q[0] = 1.0;
  Q[5] = 1.0;
  Q[10] = 1.0;
  Q[15] = 1.0;
  Phi[0] = 1;
  Phi[5] = 1;
  Phi[10] = 1;
  Phi[15] = 1;
  for (i = 0; i < 4; i++) {
    for (i_0 = 0; i_0 < 4; i_0++) {
      Q_0[i + (i_0 << 2)] = (real32_T)Q[(i_0 << 2) + i] - (M[(i_0 << 1) + 1] *
        W[i + 4] + M[i_0 << 1] * W[i]);
    }
  }

  for (i = 0; i < 4; i++) {
    for (i_0 = 0; i_0 < 4; i_0++) {
      Phi_0[i + (i_0 << 2)] = 0.0F;
      Phi_0[i + (i_0 << 2)] += RadarTracker_DW->P[i_0 << 2] * Q_0[i];
      Phi_0[i + (i_0 << 2)] += RadarTracker_DW->P[(i_0 << 2) + 1] * Q_0[i + 4];
      Phi_0[i + (i_0 << 2)] += RadarTracker_DW->P[(i_0 << 2) + 2] * Q_0[i + 8];
      Phi_0[i + (i_0 << 2)] += RadarTracker_DW->P[(i_0 << 2) + 3] * Q_0[i + 12];
    }
  }

  for (i = 0; i < 4; i++) {
    for (i_0 = 0; i_0 < 4; i_0++) {
      I[i + (i_0 << 2)] = (real32_T)Phi[(i << 2) + i_0] - (M[(i << 1) + 1] *
        W[i_0 + 4] + M[i << 1] * W[i_0]);
    }
  }

  for (i = 0; i < 4; i++) {
    M_0[i] = 0.0F;
    M_0[i] += W[i] * 90000.0F;
    M_0[i] += W[i + 4] * 0.0F;
    M_0[i + 4] = 0.0F;
    M_0[i + 4] += W[i] * 0.0F;
    M_0[i + 4] += W[i + 4] * 1.0E-6F;
  }

  for (i = 0; i < 4; i++) {
    for (i_0 = 0; i_0 < 4; i_0++) {
      Q_0[i + (i_0 << 2)] = 0.0F;
      Q_0[i + (i_0 << 2)] += I[i_0 << 2] * Phi_0[i];
      Q_0[i + (i_0 << 2)] += I[(i_0 << 2) + 1] * Phi_0[i + 4];
      Q_0[i + (i_0 << 2)] += I[(i_0 << 2) + 2] * Phi_0[i + 8];
      Q_0[i + (i_0 << 2)] += I[(i_0 << 2) + 3] * Phi_0[i + 12];
    }
  }

  for (i = 0; i < 4; i++) {
    for (i_0 = 0; i_0 < 4; i_0++) {
      Phi_0[i + (i_0 << 2)] = 0.0F;
      Phi_0[i + (i_0 << 2)] += M_0[i] * W[i_0];
      Phi_0[i + (i_0 << 2)] += M_0[i + 4] * W[i_0 + 4];
    }
  }

  for (i = 0; i < 4; i++) {
    RadarTracker_DW->P[i << 2] = Q_0[i << 2] + Phi_0[i << 2];
    RadarTracker_DW->P[1 + (i << 2)] = Q_0[(i << 2) + 1] + Phi_0[(i << 2) + 1];
    RadarTracker_DW->P[2 + (i << 2)] = Q_0[(i << 2) + 2] + Phi_0[(i << 2) + 2];
    RadarTracker_DW->P[3 + (i << 2)] = Q_0[(i << 2) + 3] + Phi_0[(i << 2) + 3];
  }

  /* Outport: '<Root>/residual' */
  /* '<S1>:1:46' */
  RadarTracker_Y->residual[0] = rtb_residual_idx_0;
  RadarTracker_Y->residual[1] = rtb_residual_idx_1;

  /* Outport: '<Root>/xhatOut' incorporates:
   *  MATLAB Function: '<Root>/RadarTracker'
   */
  RadarTracker_Y->xhatOut[0] = RadarTracker_DW->xhat[0];
  RadarTracker_Y->xhatOut[1] = RadarTracker_DW->xhat[1];
  RadarTracker_Y->xhatOut[2] = RadarTracker_DW->xhat[2];
  RadarTracker_Y->xhatOut[3] = RadarTracker_DW->xhat[3];
}

/* Model initialize function */
void RadarTracker_initialize(RT_MODEL_RadarTracker_T *const RadarTracker_M,
  ExtU_RadarTracker_T *RadarTracker_U, ExtY_RadarTracker_T *RadarTracker_Y)
{
  DW_RadarTracker_T *RadarTracker_DW = ((DW_RadarTracker_T *)
    RadarTracker_M->ModelData.dwork);

  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* states (dwork) */
  (void) memset((void *)RadarTracker_DW, 0,
                sizeof(DW_RadarTracker_T));

  /* external inputs */
  (void) memset(RadarTracker_U->meas, 0,
                2U*sizeof(real32_T));

  /* external outputs */
  (void) memset((void *)RadarTracker_Y, 0,
                sizeof(ExtY_RadarTracker_T));

  /* InitializeConditions for MATLAB Function: '<Root>/RadarTracker' */
  RadarTracker_DW->xhat[0] = 0.001F;
  RadarTracker_DW->xhat[1] = 0.01F;
  RadarTracker_DW->xhat[2] = 0.001F;
  RadarTracker_DW->xhat[3] = 400.0F;
  memset(&RadarTracker_DW->P[0], 0, sizeof(real32_T) << 4U);
}

/* Model terminate function */
void RadarTracker_terminate(RT_MODEL_RadarTracker_T *const RadarTracker_M)
{
  /* (no terminate code required) */
  UNUSED_PARAMETER(RadarTracker_M);
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
