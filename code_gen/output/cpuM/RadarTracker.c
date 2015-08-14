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
 * C/C++ source code generated on : Wed Aug  5 10:20:43 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Specified
 * Code generation objectives:
 *    1. MISRA-C:2004 guidelines
 *    2. Safety precaution
 * Validation result: All passed
 */

#include "RadarTracker.h"
#include "RadarTracker_private.h"

/* Model step function */
void RadarTracker_step(RT_MODEL_RadarTracker_T *const RadarTracker_M,
  ExtU_RadarTracker_T *RadarTracker_U, ExtY_RadarTracker_T *RadarTracker_Y)
{
  DW_RadarTracker_T *RadarTracker_DW = ((DW_RadarTracker_T *)
    RadarTracker_M->ModelData.dwork);
  real32_T Phi[16];
  real_T Q[16];
  real32_T Rangehat;
  real32_T Bearinghat;
  real32_T M[8];
  real32_T W[8];
  real32_T x[4];
  int8_T I[16];
  static const real32_T b[4] = { 90000.0F, 0.0F, 0.0F, 1.0E-6F };

  real32_T Phi_0[16];
  int32_T i;
  real32_T Phi_1[4];
  real32_T M_0[8];
  real32_T Q_0[16];
  int32_T i_0;
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
  Phi[0] = 1.0F;
  Phi[4] = 1.0F;
  Phi[8] = 0.0F;
  Phi[12] = 0.0F;
  Phi[1] = 0.0F;
  Phi[5] = 1.0F;
  Phi[9] = 0.0F;
  Phi[13] = 0.0F;
  Phi[2] = 0.0F;
  Phi[6] = 0.0F;
  Phi[10] = 1.0F;
  Phi[14] = 1.0F;
  Phi[3] = 0.0F;
  Phi[7] = 0.0F;
  Phi[11] = 0.0F;
  Phi[15] = 1.0F;

  /* '<S1>:1:15' */
  memset(&Q[0], 0, (sizeof(real_T)) << 4U);
  Q[0] = 0.0;
  Q[5] = 0.005;
  Q[10] = 0.0;
  Q[15] = 0.005;

  /* '<S1>:1:16' */
  /*  2. Propagate the covariance matrix: */
  /* '<S1>:1:19' */
  for (i = 0; i < 4; i++) {
    for (i_0 = 0; i_0 < 4; i_0++) {
      Phi_0[i + (i_0 * 4)] = 0.0F;
      Phi_0[i + (i_0 * 4)] = Phi_0[(i_0 * 4) + i] + (RadarTracker_DW->P[i_0 * 4]
        * Phi[i]);
      Phi_0[i + (i_0 * 4)] = (RadarTracker_DW->P[(i_0 * 4) + 1] * Phi[i + 4]) +
        Phi_0[(i_0 * 4) + i];
      Phi_0[i + (i_0 * 4)] = (RadarTracker_DW->P[(i_0 * 4) + 2] * Phi[i + 8]) +
        Phi_0[(i_0 * 4) + i];
      Phi_0[i + (i_0 * 4)] = (RadarTracker_DW->P[(i_0 * 4) + 3] * Phi[i + 12]) +
        Phi_0[(i_0 * 4) + i];
    }
  }

  for (i = 0; i < 4; i++) {
    for (i_0 = 0; i_0 < 4; i_0++) {
      RadarTracker_DW->P[i + (i_0 * 4)] = ((((Phi_0[i + 4] * Phi[i_0 + 4]) +
        (Phi_0[i] * Phi[i_0])) + (Phi_0[i + 8] * Phi[i_0 + 8])) + (Phi_0[i + 12]
        * Phi[i_0 + 12])) + ((real32_T)Q[(i_0 * 4) + i]);
    }
  }

  /*  3. Propagate the track estimate:: */
  /* '<S1>:1:22' */
  for (i = 0; i < 4; i++) {
    Rangehat = (Phi[i + 12] * RadarTracker_DW->xhat[3]) + ((Phi[i + 8] *
      RadarTracker_DW->xhat[2]) + ((Phi[i + 4] * RadarTracker_DW->xhat[1]) +
      (Phi[i] * RadarTracker_DW->xhat[0])));
    Phi_1[i] = Rangehat;
  }

  RadarTracker_DW->xhat[0] = Phi_1[0];
  RadarTracker_DW->xhat[1] = Phi_1[1];
  RadarTracker_DW->xhat[2] = Phi_1[2];
  RadarTracker_DW->xhat[3] = Phi_1[3];

  /*  4 a). Compute observation estimates: */
  /* '<S1>:1:25' */
  Rangehat = (real32_T)sqrt((RadarTracker_DW->xhat[0] * RadarTracker_DW->xhat[0])
    + (RadarTracker_DW->xhat[2] * RadarTracker_DW->xhat[2]));

  /* '<S1>:1:26' */
  Bearinghat = (real32_T)atan2(RadarTracker_DW->xhat[2], RadarTracker_DW->xhat[0]);

  /*  4 b). Compute observation vector y and linearized measurement matrix M  */
  /* '<S1>:1:29' */
  /* '<S1>:1:31' */
  M[0] = (real32_T)cos(Bearinghat);
  M[2] = 0.0F;
  M[4] = (real32_T)sin(Bearinghat);
  M[6] = 0.0F;
  M[1] = (-((real32_T)sin(Bearinghat))) / Rangehat;
  M[3] = 0.0F;
  M[5] = ((real32_T)cos(Bearinghat)) / Rangehat;
  M[7] = 0.0F;

  /*  4 c).  Compute residual (Estimation Error) */
  /* '<S1>:1:35' */
  RadarTracker_Y->residual[0] = RadarTracker_U->meas[0] - Rangehat;
  RadarTracker_Y->residual[1] = RadarTracker_U->meas[1] - Bearinghat;

  /*  5. Compute Kalman Gain: */
  /* '<S1>:1:38' */
  for (i = 0; i < 2; i++) {
    for (i_0 = 0; i_0 < 4; i_0++) {
      M_0[i + (i_0 * 2)] = 0.0F;
      M_0[i + (i_0 * 2)] = M_0[(i_0 * 2) + i] + (RadarTracker_DW->P[i_0 * 4] *
        M[i]);
      M_0[i + (i_0 * 2)] = (RadarTracker_DW->P[(i_0 * 4) + 2] * M[i + 4]) + M_0
        [(i_0 * 2) + i];
    }
  }

  for (i = 0; i < 2; i++) {
    for (i_0 = 0; i_0 < 2; i_0++) {
      x[i + (i_0 * 2)] = ((M_0[i + 4] * M[i_0 + 4]) + (M_0[i] * M[i_0])) + b
        [(i_0 * 2) + i];
    }
  }

  if (((real32_T)fabs(x[1])) > ((real32_T)fabs(x[0]))) {
    Rangehat = x[0] / x[1];
    Bearinghat = 1.0F / ((Rangehat * x[3]) - x[2]);
    b_y_idx_0 = (x[3] / x[1]) * Bearinghat;
    b_y_idx_1 = -Bearinghat;
    b_y_idx_2 = ((-x[2]) / x[1]) * Bearinghat;
    Bearinghat *= Rangehat;
  } else {
    Rangehat = x[1] / x[0];
    Bearinghat = 1.0F / (x[3] - (Rangehat * x[2]));
    b_y_idx_0 = (x[3] / x[0]) * Bearinghat;
    b_y_idx_1 = (-Rangehat) * Bearinghat;
    b_y_idx_2 = ((-x[2]) / x[0]) * Bearinghat;
  }

  for (i = 0; i < 4; i++) {
    for (i_0 = 0; i_0 < 2; i_0++) {
      M_0[i + (i_0 * 4)] = 0.0F;
      M_0[i + (i_0 * 4)] = M_0[(i_0 * 4) + i] + (RadarTracker_DW->P[i] * M[i_0]);
      M_0[i + (i_0 * 4)] = M_0[(i_0 * 4) + i] + (RadarTracker_DW->P[i + 8] *
        M[i_0 + 4]);
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
    RadarTracker_DW->xhat[i] += (W[i + 4] * RadarTracker_Y->residual[1]) + (W[i]
      * RadarTracker_Y->residual[0]);
  }

  /*  7. Update Covariance Matrix */
  /* '<S1>:1:44' */
  for (i = 0; i < 16; i++) {
    Q[i] = 0.0;
    I[i] = 0;
  }

  Q[0] = 1.0;
  Q[5] = 1.0;
  Q[10] = 1.0;
  Q[15] = 1.0;
  I[0] = 1;
  I[5] = 1;
  I[10] = 1;
  I[15] = 1;
  for (i = 0; i < 4; i++) {
    for (i_0 = 0; i_0 < 4; i_0++) {
      Q_0[i + (i_0 * 4)] = ((real32_T)Q[(i_0 * 4) + i]) - ((M[(i_0 * 2) + 1] *
        W[i + 4]) + (M[i_0 * 2] * W[i]));
    }
  }

  for (i = 0; i < 4; i++) {
    for (i_0 = 0; i_0 < 4; i_0++) {
      Phi_0[i + (i_0 * 4)] = 0.0F;
      Phi_0[i + (i_0 * 4)] = Phi_0[(i_0 * 4) + i] + (RadarTracker_DW->P[i_0 * 4]
        * Q_0[i]);
      Phi_0[i + (i_0 * 4)] = (RadarTracker_DW->P[(i_0 * 4) + 1] * Q_0[i + 4]) +
        Phi_0[(i_0 * 4) + i];
      Phi_0[i + (i_0 * 4)] = (RadarTracker_DW->P[(i_0 * 4) + 2] * Q_0[i + 8]) +
        Phi_0[(i_0 * 4) + i];
      Phi_0[i + (i_0 * 4)] = (RadarTracker_DW->P[(i_0 * 4) + 3] * Q_0[i + 12]) +
        Phi_0[(i_0 * 4) + i];
    }
  }

  for (i = 0; i < 4; i++) {
    for (i_0 = 0; i_0 < 4; i_0++) {
      Phi[i + (i_0 * 4)] = ((real32_T)I[(i * 4) + i_0]) - ((M[(i * 2) + 1] *
        W[i_0 + 4]) + (M[i * 2] * W[i_0]));
    }
  }

  for (i = 0; i < 4; i++) {
    M_0[i] = 0.0F;
    M_0[i] += W[i] * 90000.0F;
    M_0[i + 4] = 0.0F;
    M_0[i + 4] += W[i + 4] * 1.0E-6F;
  }

  for (i = 0; i < 4; i++) {
    for (i_0 = 0; i_0 < 4; i_0++) {
      Q_0[i + (i_0 * 4)] = 0.0F;
      Q_0[i + (i_0 * 4)] = Q_0[(i_0 * 4) + i] + (Phi[i_0 * 4] * Phi_0[i]);
      Q_0[i + (i_0 * 4)] = (Phi[(i_0 * 4) + 1] * Phi_0[i + 4]) + Q_0[(i_0 * 4) +
        i];
      Q_0[i + (i_0 * 4)] = (Phi[(i_0 * 4) + 2] * Phi_0[i + 8]) + Q_0[(i_0 * 4) +
        i];
      Q_0[i + (i_0 * 4)] = (Phi[(i_0 * 4) + 3] * Phi_0[i + 12]) + Q_0[(i_0 * 4)
        + i];
    }
  }

  for (i = 0; i < 4; i++) {
    for (i_0 = 0; i_0 < 4; i_0++) {
      Phi[i + (i_0 * 4)] = 0.0F;
      Phi[i + (i_0 * 4)] = Phi[(i_0 * 4) + i] + (M_0[i] * W[i_0]);
      Phi[i + (i_0 * 4)] = Phi[(i_0 * 4) + i] + (M_0[i + 4] * W[i_0 + 4]);
    }
  }

  for (i = 0; i < 4; i++) {
    RadarTracker_DW->P[i * 4] = Q_0[i * 4] + Phi[i * 4];
    RadarTracker_DW->P[1 + (i * 4)] = Q_0[(i * 4) + 1] + Phi[(i * 4) + 1];
    RadarTracker_DW->P[2 + (i * 4)] = Q_0[(i * 4) + 2] + Phi[(i * 4) + 2];
    RadarTracker_DW->P[3 + (i * 4)] = Q_0[(i * 4) + 3] + Phi[(i * 4) + 3];
  }

  /* Outport: '<Root>/xhatOut' incorporates:
   *  MATLAB Function: '<Root>/RadarTracker'
   */
  /* '<S1>:1:46' */
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
  memset(&RadarTracker_DW->P[0], 0, (sizeof(real32_T)) << 4U);
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
