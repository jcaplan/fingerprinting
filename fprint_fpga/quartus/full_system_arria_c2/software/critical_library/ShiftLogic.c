/*
 * File: ShiftLogic.c
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

/* Named constants for Chart: '<Root>/ShiftLogic' */
#define ShiftLogic_CALL_EVENT          (-1)
#define ShiftLogic_IN_downshifting     ((uint8_T)1U)
#define ShiftLogic_IN_first            ((uint8_T)1U)
#define ShiftLogic_IN_fourth           ((uint8_T)2U)
#define ShiftLogic_IN_second           ((uint8_T)3U)
#define ShiftLogic_IN_steady_state     ((uint8_T)2U)
#define ShiftLogic_IN_third            ((uint8_T)4U)
#define ShiftLogic_IN_upshifting       ((uint8_T)3U)
#define ShiftLogic_event_DOWN          (0)
#define ShiftLogic_event_UP            (1)

/* Block signals (auto storage) */
B_ShiftLogic_T ShiftLogic_B;

/* Block states (auto storage) */
DW_ShiftLogic_T ShiftLogic_DW;

/* External inputs (root inport signals with auto storage) */
ExtU_ShiftLogic_T ShiftLogic_U;

/* External outputs (root outports fed by signals with auto storage) */
ExtY_ShiftLogic_T ShiftLogic_Y;

/* Real-time model */
RT_MODEL_ShiftLogic_T ShiftLogic_M_;
RT_MODEL_ShiftLogic_T *const ShiftLogic_M = &ShiftLogic_M_;

/* Forward declaration for local functions */
static void ShiftLogic_gear_state(void);
real_T look2_binlxpw(real_T u0, real_T u1, const real_T bp0[], const real_T bp1[],
                     const real_T table[], const uint32_T maxIndex[], uint32_T
                     stride)
{
  real_T frac;
  uint32_T bpIndices[2];
  real_T fractions[2];
  real_T yL_1d;
  uint32_T iRght;
  uint32_T bpIdx;
  uint32_T iLeft;

  /* Lookup 2-D
     Search method: 'binary'
     Use previous index: 'off'
     Interpolation method: 'Linear'
     Extrapolation method: 'Linear'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  /* Prelookup - Index and Fraction
     Index Search method: 'binary'
     Extrapolation method: 'Linear'
     Use previous index: 'off'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u0 <= bp0[0U]) {
    iLeft = 0U;
    frac = (u0 - bp0[0U]) / (bp0[1U] - bp0[0U]);
  } else if (u0 < bp0[maxIndex[0U]]) {
    /* Binary Search */
    bpIdx = maxIndex[0U] >> 1U;
    iLeft = 0U;
    iRght = maxIndex[0U];
    while (iRght - iLeft > 1U) {
      if (u0 < bp0[bpIdx]) {
        iRght = bpIdx;
      } else {
        iLeft = bpIdx;
      }

      bpIdx = (iRght + iLeft) >> 1U;
    }

    frac = (u0 - bp0[iLeft]) / (bp0[iLeft + 1U] - bp0[iLeft]);
  } else {
    iLeft = maxIndex[0U] - 1U;
    frac = (u0 - bp0[maxIndex[0U] - 1U]) / (bp0[maxIndex[0U]] - bp0[maxIndex[0U]
      - 1U]);
  }

  fractions[0U] = frac;
  bpIndices[0U] = iLeft;

  /* Prelookup - Index and Fraction
     Index Search method: 'binary'
     Extrapolation method: 'Linear'
     Use previous index: 'off'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u1 <= bp1[0U]) {
    iLeft = 0U;
    frac = (u1 - bp1[0U]) / (bp1[1U] - bp1[0U]);
  } else if (u1 < bp1[maxIndex[1U]]) {
    /* Binary Search */
    bpIdx = maxIndex[1U] >> 1U;
    iLeft = 0U;
    iRght = maxIndex[1U];
    while (iRght - iLeft > 1U) {
      if (u1 < bp1[bpIdx]) {
        iRght = bpIdx;
      } else {
        iLeft = bpIdx;
      }

      bpIdx = (iRght + iLeft) >> 1U;
    }

    frac = (u1 - bp1[iLeft]) / (bp1[iLeft + 1U] - bp1[iLeft]);
  } else {
    iLeft = maxIndex[1U] - 1U;
    frac = (u1 - bp1[maxIndex[1U] - 1U]) / (bp1[maxIndex[1U]] - bp1[maxIndex[1U]
      - 1U]);
  }

  /* Interpolation 2-D
     Interpolation method: 'Linear'
     Use last breakpoint for index at or above upper limit: 'off'
     Overflow mode: 'portable wrapping'
   */
  bpIdx = iLeft * stride + bpIndices[0U];
  yL_1d = (table[bpIdx + 1U] - table[bpIdx]) * fractions[0U] + table[bpIdx];
  bpIdx += stride;
  return (((table[bpIdx + 1U] - table[bpIdx]) * fractions[0U] + table[bpIdx]) -
          yL_1d) * frac + yL_1d;
}

/* Function for Chart: '<Root>/ShiftLogic' */
static void ShiftLogic_gear_state(void)
{
  /* During 'gear_state': '<S1>:2' */
  switch (ShiftLogic_DW.is_gear_state) {
   case ShiftLogic_IN_first:
    /* During 'first': '<S1>:6' */
    if (ShiftLogic_DW.sfEvent == ShiftLogic_event_UP) {
      /* Transition: '<S1>:12' */
      ShiftLogic_DW.is_gear_state = ShiftLogic_IN_second;

      /* Entry 'second': '<S1>:4' */
      ShiftLogic_Y.Gear = 2.0;
    }
    break;

   case ShiftLogic_IN_fourth:
    /* During 'fourth': '<S1>:3' */
    if (ShiftLogic_DW.sfEvent == ShiftLogic_event_DOWN) {
      /* Transition: '<S1>:14' */
      ShiftLogic_DW.is_gear_state = ShiftLogic_IN_third;

      /* Entry 'third': '<S1>:5' */
      ShiftLogic_Y.Gear = 3.0;
    }
    break;

   case ShiftLogic_IN_second:
    /* During 'second': '<S1>:4' */
    switch (ShiftLogic_DW.sfEvent) {
     case ShiftLogic_event_UP:
      /* Transition: '<S1>:11' */
      ShiftLogic_DW.is_gear_state = ShiftLogic_IN_third;

      /* Entry 'third': '<S1>:5' */
      ShiftLogic_Y.Gear = 3.0;
      break;

     case ShiftLogic_event_DOWN:
      /* Transition: '<S1>:16' */
      ShiftLogic_DW.is_gear_state = ShiftLogic_IN_first;

      /* Entry 'first': '<S1>:6' */
      ShiftLogic_Y.Gear = 1.0;
      break;
    }
    break;

   case ShiftLogic_IN_third:
    /* During 'third': '<S1>:5' */
    switch (ShiftLogic_DW.sfEvent) {
     case ShiftLogic_event_UP:
      /* Transition: '<S1>:10' */
      ShiftLogic_DW.is_gear_state = ShiftLogic_IN_fourth;

      /* Entry 'fourth': '<S1>:3' */
      ShiftLogic_Y.Gear = 4.0;
      break;

     case ShiftLogic_event_DOWN:
      /* Transition: '<S1>:15' */
      ShiftLogic_DW.is_gear_state = ShiftLogic_IN_second;

      /* Entry 'second': '<S1>:4' */
      ShiftLogic_Y.Gear = 2.0;
      break;
    }
    break;
  }
}

/* Model step function */
void ShiftLogic_step(void)
{
  int32_T b_previousEvent;

  /* Chart: '<Root>/ShiftLogic' incorporates:
   *  Inport: '<Root>/VehicleSpeed'
   */
  /* Gateway: ShiftLogic */
  ShiftLogic_DW.sfEvent = ShiftLogic_CALL_EVENT;
  if (ShiftLogic_DW.temporalCounter_i1 < MAX_uint32_T) {
    ShiftLogic_DW.temporalCounter_i1++;
  }

  /* During: ShiftLogic */
  if (ShiftLogic_DW.is_active_c1_ShiftLogic == 0U) {
    /* Entry: ShiftLogic */
    ShiftLogic_DW.is_active_c1_ShiftLogic = 1U;

    /* Entry Internal: ShiftLogic */
    ShiftLogic_DW.is_active_gear_state = 1U;

    /* Entry Internal 'gear_state': '<S1>:2' */
    /* Transition: '<S1>:13' */
    if (ShiftLogic_DW.is_gear_state == ShiftLogic_IN_first) {
    } else {
      ShiftLogic_DW.is_gear_state = ShiftLogic_IN_first;

      /* Outport: '<Root>/Gear' */
      /* Entry 'first': '<S1>:6' */
      ShiftLogic_Y.Gear = 1.0;
    }

    ShiftLogic_DW.is_active_selection_state = 1U;

    /* Entry Internal 'selection_state': '<S1>:7' */
    /* Transition: '<S1>:17' */
    ShiftLogic_DW.is_selection_state = ShiftLogic_IN_steady_state;
  } else {
    if (ShiftLogic_DW.is_active_gear_state == 0U) {
    } else {
      ShiftLogic_gear_state();
    }

    if (ShiftLogic_DW.is_active_selection_state == 0U) {
    } else {
      /* Outputs for Function Call SubSystem: '<S1>/ComputeThreshold' */
      /* Lookup_n-D: '<S2>/InterpDown' incorporates:
       *  Inport: '<Root>/Throttle'
       */
      /* During 'selection_state': '<S1>:7' */
      /* Simulink Function 'ComputeThreshold': '<S1>:33' */
      ShiftLogic_B.InterpDown = look2_binlxpw(ShiftLogic_U.Throttle,
        ShiftLogic_Y.Gear, ShiftLogic_P.InterpDown_bp01Data,
        ShiftLogic_P.InterpDown_bp02Data, ShiftLogic_P.InterpDown_tableData,
        ShiftLogic_P.InterpDown_maxIndex, 6U);

      /* Lookup_n-D: '<S2>/InterpUp' incorporates:
       *  Inport: '<Root>/Throttle'
       */
      ShiftLogic_B.InterpUp = look2_binlxpw(ShiftLogic_U.Throttle,
        ShiftLogic_Y.Gear, ShiftLogic_P.InterpUp_bp01Data,
        ShiftLogic_P.InterpUp_bp02Data, ShiftLogic_P.InterpUp_tableData,
        ShiftLogic_P.InterpUp_maxIndex, 6U);

      /* End of Outputs for SubSystem: '<S1>/ComputeThreshold' */
      switch (ShiftLogic_DW.is_selection_state) {
       case ShiftLogic_IN_downshifting:
        /* During 'downshifting': '<S1>:1' */
        if ((ShiftLogic_DW.sfEvent == ShiftLogic_CALL_EVENT) &&
            (ShiftLogic_DW.temporalCounter_i1 >= (uint32_T)
             ShiftLogic_P.ShiftLogic_TWAIT) && (ShiftLogic_U.VehicleSpeed <=
             ShiftLogic_B.InterpDown)) {
          /* Transition: '<S1>:22' */
          /* Event: '<S1>:30' */
          b_previousEvent = ShiftLogic_DW.sfEvent;
          ShiftLogic_DW.sfEvent = ShiftLogic_event_DOWN;
          if (ShiftLogic_DW.is_active_gear_state == 0U) {
          } else {
            ShiftLogic_gear_state();
          }

          ShiftLogic_DW.sfEvent = b_previousEvent;
          ShiftLogic_DW.is_selection_state = ShiftLogic_IN_steady_state;
        } else {
          if (ShiftLogic_U.VehicleSpeed > ShiftLogic_B.InterpDown) {
            /* Transition: '<S1>:21' */
            ShiftLogic_DW.is_selection_state = ShiftLogic_IN_steady_state;
          }
        }
        break;

       case ShiftLogic_IN_steady_state:
        /* During 'steady_state': '<S1>:9' */
        if (ShiftLogic_U.VehicleSpeed > ShiftLogic_B.InterpUp) {
          /* Transition: '<S1>:18' */
          ShiftLogic_DW.is_selection_state = ShiftLogic_IN_upshifting;
          ShiftLogic_DW.temporalCounter_i1 = 0U;
        } else {
          if (ShiftLogic_U.VehicleSpeed < ShiftLogic_B.InterpDown) {
            /* Transition: '<S1>:19' */
            ShiftLogic_DW.is_selection_state = ShiftLogic_IN_downshifting;
            ShiftLogic_DW.temporalCounter_i1 = 0U;
          }
        }
        break;

       case ShiftLogic_IN_upshifting:
        /* During 'upshifting': '<S1>:8' */
        if ((ShiftLogic_DW.sfEvent == ShiftLogic_CALL_EVENT) &&
            (ShiftLogic_DW.temporalCounter_i1 >= (uint32_T)
             ShiftLogic_P.ShiftLogic_TWAIT) && (ShiftLogic_U.VehicleSpeed >=
             ShiftLogic_B.InterpUp)) {
          /* Transition: '<S1>:23' */
          /* Event: '<S1>:31' */
          b_previousEvent = ShiftLogic_DW.sfEvent;
          ShiftLogic_DW.sfEvent = ShiftLogic_event_UP;
          if (ShiftLogic_DW.is_active_gear_state == 0U) {
          } else {
            ShiftLogic_gear_state();
          }

          ShiftLogic_DW.sfEvent = b_previousEvent;
          ShiftLogic_DW.is_selection_state = ShiftLogic_IN_steady_state;
        } else {
          if (ShiftLogic_U.VehicleSpeed < ShiftLogic_B.InterpUp) {
            /* Transition: '<S1>:20' */
            ShiftLogic_DW.is_selection_state = ShiftLogic_IN_steady_state;
          }
        }
        break;
      }
    }
  }

  /* End of Chart: '<Root>/ShiftLogic' */
}

/* Model initialize function */
void ShiftLogic_initialize(void)
{
  /* Start for Chart: '<Root>/ShiftLogic' incorporates:
   *  Start for SubSystem: '<S1>/ComputeThreshold'
   */
  /* VirtualOutportStart for Outport: '<S2>/down_th' */
  ShiftLogic_B.InterpDown = ShiftLogic_P.down_th_Y0;

  /* VirtualOutportStart for Outport: '<S2>/up_th' */
  ShiftLogic_B.InterpUp = ShiftLogic_P.up_th_Y0;

  /* InitializeConditions for Chart: '<Root>/ShiftLogic' */
  ShiftLogic_DW.sfEvent = ShiftLogic_CALL_EVENT;
}

/* Model terminate function */
void ShiftLogic_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
