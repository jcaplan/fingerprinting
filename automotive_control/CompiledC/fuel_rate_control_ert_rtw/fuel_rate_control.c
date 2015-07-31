/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: fuel_rate_control.c
 *
 * Code generated for Simulink model 'fuel_rate_control'.
 *
 * Model version                  : 1.540
 * Simulink Coder version         : 8.8 (R2015a) 09-Feb-2015
 * C/C++ source code generated on : Fri Jul 31 10:23:26 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Specified
 * Code generation objectives:
 *    1. MISRA-C:2004 guidelines
 *    2. Safety precaution
 * Validation result: Not run
 */

#include "fuel_rate_control.h"

/* Named constants for Chart: '<S1>/control_logic' */
#define CALL_EVENT                     (-1)
#define IN_A                           ((uint8_T)1U)
#define IN_Four                        ((uint8_T)1U)
#define IN_Fuel_Disabled               ((uint8_T)1U)
#define IN_Low_Emissions               ((uint8_T)1U)
#define IN_Multi                       ((uint8_T)1U)
#define IN_NO_ACTIVE_CHILD             ((uint8_T)0U)
#define IN_None                        ((uint8_T)2U)
#define IN_Normal                      ((uint8_T)1U)
#define IN_O2_failure                  ((uint8_T)1U)
#define IN_O2_normal                   ((uint8_T)2U)
#define IN_O2_warmup                   ((uint8_T)3U)
#define IN_One                         ((uint8_T)3U)
#define IN_Overspeed                   ((uint8_T)1U)
#define IN_Rich_Mixture                ((uint8_T)2U)
#define IN_Running                     ((uint8_T)2U)
#define IN_Shutdown                    ((uint8_T)2U)
#define IN_Single_Failure              ((uint8_T)1U)
#define IN_Three                       ((uint8_T)2U)
#define IN_Two                         ((uint8_T)3U)
#define IN_Warmup                      ((uint8_T)2U)
#define IN_fail                        ((uint8_T)1U)
#define IN_normal                      ((uint8_T)2U)
#define entry_to_Multi                 (23)
#define event_DEC                      (0)
#define event_INC                      (1)
#define exit_from_Multi                (24)

extern real32_T look2_iflf_linlca(real32_T u0, real32_T u1, const real32_T bp0[],
  const real32_T bp1[], const real32_T table[], const uint32_T maxIndex[],
  uint32_T stride);

/* Forward declaration for local functions */
static void fuel_rate_control_Fueling_Mode(const int32_T *sfEvent,
  ExtU_fuel_rate_control_T *fuel_rate_control_U, DW_fuel_rate_control_T
  *fuel_rate_control_DW);
static void fuel_rate_control_Fail(int32_T *sfEvent, ExtU_fuel_rate_control_T
  *fuel_rate_control_U, DW_fuel_rate_control_T *fuel_rate_control_DW);
real32_T look2_iflf_linlca(real32_T u0, real32_T u1, const real32_T bp0[], const
  real32_T bp1[], const real32_T table[], const uint32_T maxIndex[], uint32_T
  stride)
{
  real32_T y;
  real32_T frac;
  uint32_T bpIndices[2];
  real32_T fractions[2];
  real32_T yR_1d;
  uint32_T offset_1d;
  uint32_T bpIdx;

  /* Lookup 2-D
     Search method: 'linear'
     Use previous index: 'off'
     Interpolation method: 'Linear'
     Extrapolation method: 'Clip'
     Use last breakpoint for index at or above upper limit: 'on'
     Remove protection against out-of-range input in generated code: 'off'
   */
  /* Prelookup - Index and Fraction
     Index Search method: 'linear'
     Extrapolation method: 'Clip'
     Use previous index: 'off'
     Use last breakpoint for index at or above upper limit: 'on'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u0 <= bp0[0U]) {
    bpIdx = 0U;
    frac = 0.0F;
  } else if (u0 < bp0[maxIndex[0U]]) {
    /* Linear Search */
    for (bpIdx = maxIndex[0U] >> 1U; u0 < bp0[bpIdx]; bpIdx--) {
    }

    while (u0 >= bp0[bpIdx + 1U]) {
      bpIdx++;
    }

    frac = (u0 - bp0[bpIdx]) / (bp0[bpIdx + 1U] - bp0[bpIdx]);
  } else {
    bpIdx = maxIndex[0U];
    frac = 0.0F;
  }

  fractions[0U] = frac;
  bpIndices[0U] = bpIdx;

  /* Prelookup - Index and Fraction
     Index Search method: 'linear'
     Extrapolation method: 'Clip'
     Use previous index: 'off'
     Use last breakpoint for index at or above upper limit: 'on'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u1 <= bp1[0U]) {
    bpIdx = 0U;
    frac = 0.0F;
  } else if (u1 < bp1[maxIndex[1U]]) {
    /* Linear Search */
    for (bpIdx = maxIndex[1U] >> 1U; u1 < bp1[bpIdx]; bpIdx--) {
    }

    while (u1 >= bp1[bpIdx + 1U]) {
      bpIdx++;
    }

    frac = (u1 - bp1[bpIdx]) / (bp1[bpIdx + 1U] - bp1[bpIdx]);
  } else {
    bpIdx = maxIndex[1U];
    frac = 0.0F;
  }

  /* Interpolation 2-D
     Interpolation method: 'Linear'
     Use last breakpoint for index at or above upper limit: 'on'
     Overflow mode: 'wrapping'
   */
  offset_1d = bpIdx * stride + bpIndices[0U];
  if (bpIndices[0U] == maxIndex[0U]) {
    y = table[offset_1d];
  } else {
    y = (table[offset_1d + 1U] - table[offset_1d]) * fractions[0U] +
      table[offset_1d];
  }

  if (bpIdx == maxIndex[1U]) {
  } else {
    bpIdx = offset_1d + stride;
    if (bpIndices[0U] == maxIndex[0U]) {
      yR_1d = table[bpIdx];
    } else {
      yR_1d = (table[bpIdx + 1U] - table[bpIdx]) * fractions[0U] + table[bpIdx];
    }

    y += (yR_1d - y) * frac;
  }

  return y;
}

/* Function for Chart: '<S1>/control_logic' */
static void fuel_rate_control_Fueling_Mode(const int32_T *sfEvent,
  ExtU_fuel_rate_control_T *fuel_rate_control_U, DW_fuel_rate_control_T
  *fuel_rate_control_DW)
{
  /* During 'Fueling_Mode': '<S3>:21' */
  /* This state interprets the other states in the chart to directly control the fueling mode. */
  switch (fuel_rate_control_DW->bitsForTID0.is_Fueling_Mode) {
   case IN_Fuel_Disabled:
    /* During 'Fuel_Disabled': '<S3>:22' */
    /* The fuel is completely shut off while in this state. */
    switch (fuel_rate_control_DW->bitsForTID0.is_Fuel_Disabled) {
     case IN_Overspeed:
      /* Inport: '<Root>/sensors' */
      /* During 'Overspeed': '<S3>:24' */
      /* The speed is dangerously high, so shut off the fuel. */
      if ((fuel_rate_control_DW->bitsForTID0.is_Speed == IN_normal) &&
          (fuel_rate_control_U->sensors.speed < 603.0F)) {
        /* Transition: '<S3>:54' */
        if (!(fuel_rate_control_DW->bitsForTID0.is_Fail == IN_Multi)) {
          /* Transition: '<S3>:55' */
          fuel_rate_control_DW->bitsForTID0.is_Fuel_Disabled =
            IN_NO_ACTIVE_CHILD;
          fuel_rate_control_DW->bitsForTID0.is_Fueling_Mode = IN_Running;

          /* Entry Internal 'Running': '<S3>:23' */
          switch (fuel_rate_control_DW->bitsForTID0.was_Running) {
           case IN_Low_Emissions:
            if (fuel_rate_control_DW->bitsForTID0.is_Running != IN_Low_Emissions)
            {
              fuel_rate_control_DW->bitsForTID0.is_Running = IN_Low_Emissions;
              fuel_rate_control_DW->bitsForTID0.was_Running = IN_Low_Emissions;

              /* Entry 'Low_Emissions': '<S3>:25' */
              fuel_rate_control_DW->fuel_mode = LOW;
            }

            /* Entry Internal 'Low_Emissions': '<S3>:25' */
            switch (fuel_rate_control_DW->bitsForTID0.was_Low_Emissions) {
             case IN_Normal:
              fuel_rate_control_DW->bitsForTID0.is_Low_Emissions = IN_Normal;
              fuel_rate_control_DW->bitsForTID0.was_Low_Emissions = IN_Normal;
              break;

             case IN_Warmup:
              fuel_rate_control_DW->bitsForTID0.is_Low_Emissions = IN_Warmup;
              fuel_rate_control_DW->bitsForTID0.was_Low_Emissions = IN_Warmup;
              break;

             default:
              fuel_rate_control_DW->bitsForTID0.is_Low_Emissions =
                IN_NO_ACTIVE_CHILD;
              break;
            }
            break;

           case IN_Rich_Mixture:
            if (fuel_rate_control_DW->bitsForTID0.is_Running != IN_Rich_Mixture)
            {
              fuel_rate_control_DW->bitsForTID0.is_Running = IN_Rich_Mixture;
              fuel_rate_control_DW->bitsForTID0.was_Running = IN_Rich_Mixture;

              /* Entry 'Rich_Mixture': '<S3>:26' */
              fuel_rate_control_DW->fuel_mode = RICH;
            }

            /* Entry Internal 'Rich_Mixture': '<S3>:26' */
            fuel_rate_control_DW->bitsForTID0.is_Rich_Mixture =
              IN_Single_Failure;
            break;

           default:
            fuel_rate_control_DW->bitsForTID0.is_Running = IN_NO_ACTIVE_CHILD;
            break;
          }
        } else {
          /* Transition: '<S3>:60' */
          fuel_rate_control_DW->bitsForTID0.is_Fuel_Disabled = IN_Shutdown;
        }
      }

      /* End of Inport: '<Root>/sensors' */
      break;

     case IN_Shutdown:
      /* During 'Shutdown': '<S3>:29' */
      /* Cut off system operation due to multiple sensor failures. */
      if (*sfEvent == exit_from_Multi) {
        /* Transition: '<S3>:63' */
        fuel_rate_control_DW->bitsForTID0.is_Fuel_Disabled = IN_NO_ACTIVE_CHILD;
        fuel_rate_control_DW->bitsForTID0.is_Fueling_Mode = IN_Running;
        if (fuel_rate_control_DW->bitsForTID0.is_Running != IN_Rich_Mixture) {
          fuel_rate_control_DW->bitsForTID0.is_Running = IN_Rich_Mixture;
          fuel_rate_control_DW->bitsForTID0.was_Running = IN_Rich_Mixture;

          /* Entry 'Rich_Mixture': '<S3>:26' */
          fuel_rate_control_DW->fuel_mode = RICH;
        }

        /* Entry Internal 'Rich_Mixture': '<S3>:26' */
        fuel_rate_control_DW->bitsForTID0.is_Rich_Mixture = IN_Single_Failure;
      }
      break;

     default:
      /* Unreachable state, for coverage only */
      fuel_rate_control_DW->bitsForTID0.is_Fuel_Disabled = IN_NO_ACTIVE_CHILD;
      break;
    }
    break;

   case IN_Running:
    /* During 'Running': '<S3>:23' */
    /* The fuel is actively controlled while in this state. */
    if (*sfEvent == entry_to_Multi) {
      /* Transition: '<S3>:61' */
      /* Exit Internal 'Running': '<S3>:23' */
      /* Exit Internal 'Low_Emissions': '<S3>:25' */
      fuel_rate_control_DW->bitsForTID0.is_Low_Emissions = IN_NO_ACTIVE_CHILD;
      fuel_rate_control_DW->bitsForTID0.is_Running = IN_NO_ACTIVE_CHILD;

      /* Exit Internal 'Rich_Mixture': '<S3>:26' */
      fuel_rate_control_DW->bitsForTID0.is_Rich_Mixture = IN_NO_ACTIVE_CHILD;
      fuel_rate_control_DW->bitsForTID0.is_Fueling_Mode = IN_Fuel_Disabled;

      /* Entry 'Fuel_Disabled': '<S3>:22' */
      fuel_rate_control_DW->fuel_mode = DISABLED;
      fuel_rate_control_DW->bitsForTID0.is_Fuel_Disabled = IN_Shutdown;
    } else if (fuel_rate_control_U->sensors.speed > 628.0F) {
      /* Transition: '<S3>:53' */
      /* Exit Internal 'Running': '<S3>:23' */
      /* Exit Internal 'Low_Emissions': '<S3>:25' */
      fuel_rate_control_DW->bitsForTID0.is_Low_Emissions = IN_NO_ACTIVE_CHILD;
      fuel_rate_control_DW->bitsForTID0.is_Running = IN_NO_ACTIVE_CHILD;

      /* Exit Internal 'Rich_Mixture': '<S3>:26' */
      fuel_rate_control_DW->bitsForTID0.is_Rich_Mixture = IN_NO_ACTIVE_CHILD;
      fuel_rate_control_DW->bitsForTID0.is_Fueling_Mode = IN_Fuel_Disabled;

      /* Entry 'Fuel_Disabled': '<S3>:22' */
      fuel_rate_control_DW->fuel_mode = DISABLED;
      fuel_rate_control_DW->bitsForTID0.is_Fuel_Disabled = IN_Overspeed;
    } else {
      switch (fuel_rate_control_DW->bitsForTID0.is_Running) {
       case IN_Low_Emissions:
        /* During 'Low_Emissions': '<S3>:25' */
        switch (fuel_rate_control_DW->bitsForTID0.is_Low_Emissions) {
         case IN_Normal:
          /* During 'Normal': '<S3>:28' */
          /* All sensors are in correct operating modes, so effective closed-loop mixture control can be used. */
          if (fuel_rate_control_DW->bitsForTID0.is_Fail == IN_One) {
            /* Transition: '<S3>:56' */
            fuel_rate_control_DW->bitsForTID0.is_Low_Emissions =
              IN_NO_ACTIVE_CHILD;
            fuel_rate_control_DW->bitsForTID0.is_Running = IN_Rich_Mixture;
            fuel_rate_control_DW->bitsForTID0.was_Running = IN_Rich_Mixture;

            /* Entry 'Rich_Mixture': '<S3>:26' */
            fuel_rate_control_DW->fuel_mode = RICH;
            fuel_rate_control_DW->bitsForTID0.is_Rich_Mixture =
              IN_Single_Failure;
          }
          break;

         case IN_Warmup:
          /* During 'Warmup': '<S3>:30' */
          /* The sensors are all operational, but the oxygen (EGO) sensor is warming up.  We'll target a stoichiometric ratio, nonetheless, but won't close the loop around exhaust gas oxygen. */
          if (fuel_rate_control_DW->bitsForTID0.is_A == IN_O2_normal) {
            /* Transition: '<S3>:62' */
            if (fuel_rate_control_DW->bitsForTID0.is_Fail == IN_One) {
              /* Transition: '<S3>:59' */
              fuel_rate_control_DW->bitsForTID0.is_Low_Emissions =
                IN_NO_ACTIVE_CHILD;
              fuel_rate_control_DW->bitsForTID0.is_Running = IN_Rich_Mixture;
              fuel_rate_control_DW->bitsForTID0.was_Running = IN_Rich_Mixture;

              /* Entry 'Rich_Mixture': '<S3>:26' */
              fuel_rate_control_DW->fuel_mode = RICH;
              fuel_rate_control_DW->bitsForTID0.is_Rich_Mixture =
                IN_Single_Failure;
            } else {
              /* Transition: '<S3>:58' */
              fuel_rate_control_DW->bitsForTID0.is_Low_Emissions = IN_Normal;
              fuel_rate_control_DW->bitsForTID0.was_Low_Emissions = IN_Normal;
            }
          }
          break;

         default:
          /* Unreachable state, for coverage only */
          fuel_rate_control_DW->bitsForTID0.is_Low_Emissions =
            IN_NO_ACTIVE_CHILD;
          break;
        }
        break;

       case IN_Rich_Mixture:
        /* During 'Rich_Mixture': '<S3>:26' */
        /* This mode enriches the mixture, lowering the air/fuel ratio. */
        if (!((fuel_rate_control_DW->bitsForTID0.is_Rich_Mixture !=
               IN_Single_Failure) ||
              (!(fuel_rate_control_DW->bitsForTID0.is_Fail == IN_None)))) {
          /* During 'Single_Failure': '<S3>:27' */
          /* Continue uninterrupted engine operation, but with a richer mixture. */
          /* Transition: '<S3>:57' */
          fuel_rate_control_DW->bitsForTID0.is_Rich_Mixture = IN_NO_ACTIVE_CHILD;
          fuel_rate_control_DW->bitsForTID0.is_Running = IN_Low_Emissions;
          fuel_rate_control_DW->bitsForTID0.was_Running = IN_Low_Emissions;

          /* Entry 'Low_Emissions': '<S3>:25' */
          fuel_rate_control_DW->fuel_mode = LOW;
          fuel_rate_control_DW->bitsForTID0.is_Low_Emissions = IN_Normal;
          fuel_rate_control_DW->bitsForTID0.was_Low_Emissions = IN_Normal;
        }
        break;

       default:
        /* Unreachable state, for coverage only */
        fuel_rate_control_DW->bitsForTID0.is_Running = IN_NO_ACTIVE_CHILD;
        break;
      }
    }
    break;

   default:
    /* Unreachable state, for coverage only */
    fuel_rate_control_DW->bitsForTID0.is_Fueling_Mode = IN_NO_ACTIVE_CHILD;
    break;
  }
}

/* Function for Chart: '<S1>/control_logic' */
static void fuel_rate_control_Fail(int32_T *sfEvent, ExtU_fuel_rate_control_T
  *fuel_rate_control_U, DW_fuel_rate_control_T *fuel_rate_control_DW)
{
  int32_T b_previousEvent;

  /* During 'Fail': '<S3>:14' */
  /* This state maintains a count of the number of sensors in failure mode. */
  switch (fuel_rate_control_DW->bitsForTID0.is_Fail) {
   case IN_Multi:
    /* During 'Multi': '<S3>:15' */
    /* This state represents any of the conditions in which more than one sensor is in failure mode. */
    switch (fuel_rate_control_DW->bitsForTID0.is_Multi) {
     case IN_Four:
      /* During 'Four': '<S3>:17' */
      /* All four of the engine sensors have failed. */
      if (*sfEvent == event_DEC) {
        /* Transition: '<S3>:49' */
        fuel_rate_control_DW->bitsForTID0.is_Multi = IN_Three;
      }
      break;

     case IN_Three:
      /* During 'Three': '<S3>:16' */
      /* Three of the engine sensors have failed. */
      if (*sfEvent == event_INC) {
        /* Transition: '<S3>:45' */
        fuel_rate_control_DW->bitsForTID0.is_Multi = IN_Four;
      } else {
        if (*sfEvent == event_DEC) {
          /* Transition: '<S3>:50' */
          fuel_rate_control_DW->bitsForTID0.is_Multi = IN_Two;
        }
      }
      break;

     case IN_Two:
      /* During 'Two': '<S3>:18' */
      /* Two of the engine sensors have failed. */
      if (*sfEvent == event_DEC) {
        /* Transition: '<S3>:51' */
        fuel_rate_control_DW->bitsForTID0.is_Multi = IN_NO_ACTIVE_CHILD;
        if (fuel_rate_control_DW->bitsForTID0.is_Fail == IN_Multi) {
          fuel_rate_control_DW->bitsForTID0.is_Fail = IN_NO_ACTIVE_CHILD;
          b_previousEvent = *sfEvent;
          *sfEvent = exit_from_Multi;
          if (fuel_rate_control_DW->bitsForTID0.is_active_Fueling_Mode != 0U) {
            fuel_rate_control_Fueling_Mode(sfEvent, fuel_rate_control_U,
              fuel_rate_control_DW);
          }

          *sfEvent = b_previousEvent;
        }

        fuel_rate_control_DW->bitsForTID0.is_Fail = IN_One;
      } else {
        if (*sfEvent == event_INC) {
          /* Transition: '<S3>:47' */
          fuel_rate_control_DW->bitsForTID0.is_Multi = IN_Three;
        }
      }
      break;

     default:
      /* Unreachable state, for coverage only */
      fuel_rate_control_DW->bitsForTID0.is_Multi = IN_NO_ACTIVE_CHILD;
      break;
    }
    break;

   case IN_None:
    /* During 'None': '<S3>:20' */
    /* Zero failures; all sensors are operational. */
    if (*sfEvent == event_INC) {
      /* Transition: '<S3>:46' */
      fuel_rate_control_DW->bitsForTID0.is_Fail = IN_One;
    }
    break;

   case IN_One:
    /* During 'One': '<S3>:19' */
    /* One of the engine sensors has failed. */
    if (*sfEvent == event_INC) {
      /* Transition: '<S3>:48' */
      fuel_rate_control_DW->bitsForTID0.is_Fail = IN_Multi;
      b_previousEvent = *sfEvent;
      *sfEvent = entry_to_Multi;
      if (fuel_rate_control_DW->bitsForTID0.is_active_Fueling_Mode != 0U) {
        fuel_rate_control_Fueling_Mode(sfEvent, fuel_rate_control_U,
          fuel_rate_control_DW);
      }

      *sfEvent = b_previousEvent;
      fuel_rate_control_DW->bitsForTID0.is_Multi = IN_Two;
    } else {
      if (*sfEvent == event_DEC) {
        /* Transition: '<S3>:52' */
        fuel_rate_control_DW->bitsForTID0.is_Fail = IN_None;
      }
    }
    break;

   default:
    /* Unreachable state, for coverage only */
    fuel_rate_control_DW->bitsForTID0.is_Fail = IN_NO_ACTIVE_CHILD;
    break;
  }
}

/* Model step function */
void fuel_rate_control_step(RT_MODEL_fuel_rate_control_T *const
  fuel_rate_control_M, ExtU_fuel_rate_control_T *fuel_rate_control_U,
  ExtY_fuel_rate_control_T *fuel_rate_control_Y)
{
  DW_fuel_rate_control_T *fuel_rate_control_DW = ((DW_fuel_rate_control_T *)
    fuel_rate_control_M->ModelData.dwork);
  real32_T denAccum;
  real32_T rtb_Product;
  real32_T rtb_holdintegrator;
  int32_T sfEvent;
  real32_T PressureEstimation;

  /* Outputs for Atomic SubSystem: '<Root>/fuel_rate_control' */
  /* Chart: '<S1>/control_logic' incorporates:
   *  Inport: '<Root>/sensors'
   *
   * Block description for '<S1>/control_logic':
   *  Stateflow diagram to determine control system operating mode
   */
  /* Gateway: fuel_rate_control/control_logic */
  sfEvent = CALL_EVENT;
  if (fuel_rate_control_DW->temporalCounter_i1 < 511U) {
    fuel_rate_control_DW->temporalCounter_i1++;
  }

  /* During: fuel_rate_control/control_logic */
  if (fuel_rate_control_DW->bitsForTID0.is_active_c1_fuel_rate_control == 0U) {
    /* Entry: fuel_rate_control/control_logic */
    fuel_rate_control_DW->bitsForTID0.is_active_c1_fuel_rate_control = 1U;

    /* Entry Internal: fuel_rate_control/control_logic */
    fuel_rate_control_DW->bitsForTID0.is_active_O2 = 1U;

    /* Entry Internal 'O2': '<S3>:1' */
    /* Transition: '<S3>:180' */
    fuel_rate_control_DW->bitsForTID0.is_O2 = IN_A;

    /* Entry Internal 'A': '<S3>:171' */
    /* Transition: '<S3>:35' */
    if (fuel_rate_control_DW->bitsForTID0.is_A != IN_O2_warmup) {
      fuel_rate_control_DW->bitsForTID0.is_A = IN_O2_warmup;
      fuel_rate_control_DW->temporalCounter_i1 = 0U;
    }

    if (fuel_rate_control_DW->bitsForTID0.is_active_Pressure != 1U) {
      fuel_rate_control_DW->bitsForTID0.is_active_Pressure = 1U;

      /* Entry 'Pressure': '<S3>:2' */
    }

    /* Entry Internal 'Pressure': '<S3>:2' */
    /* Transition: '<S3>:34' */
    fuel_rate_control_DW->bitsForTID0.is_Pressure = IN_normal;
    if (fuel_rate_control_DW->bitsForTID0.is_active_Throttle != 1U) {
      fuel_rate_control_DW->bitsForTID0.is_active_Throttle = 1U;

      /* Entry 'Throttle': '<S3>:8' */
    }

    /* Entry Internal 'Throttle': '<S3>:8' */
    /* Transition: '<S3>:39' */
    fuel_rate_control_DW->bitsForTID0.is_Throttle = IN_normal;
    if (fuel_rate_control_DW->bitsForTID0.is_active_Speed != 1U) {
      fuel_rate_control_DW->bitsForTID0.is_active_Speed = 1U;

      /* Entry 'Speed': '<S3>:9' */
    }

    /* Entry Internal 'Speed': '<S3>:9' */
    /* Transition: '<S3>:41' */
    fuel_rate_control_DW->bitsForTID0.is_Speed = IN_normal;
    fuel_rate_control_DW->bitsForTID0.is_active_Fail = 1U;

    /* Entry Internal 'Fail': '<S3>:14' */
    /* Transition: '<S3>:44' */
    fuel_rate_control_DW->bitsForTID0.is_Fail = IN_None;
    fuel_rate_control_DW->bitsForTID0.is_active_Fueling_Mode = 1U;

    /* Entry Internal 'Fueling_Mode': '<S3>:21' */
    /* Transition: '<S3>:64' */
    fuel_rate_control_DW->es_o = fuel_rate_control_U->sensors;

    /* Need to copy the sensors to the output on entry since the individual elements of the bus signal are only updated in the during function. */
    fuel_rate_control_DW->bitsForTID0.is_Fueling_Mode = IN_Running;
    if (fuel_rate_control_DW->bitsForTID0.is_Running != IN_Low_Emissions) {
      fuel_rate_control_DW->bitsForTID0.is_Running = IN_Low_Emissions;
      fuel_rate_control_DW->bitsForTID0.was_Running = IN_Low_Emissions;

      /* Entry 'Low_Emissions': '<S3>:25' */
      fuel_rate_control_DW->fuel_mode = LOW;
    }

    fuel_rate_control_DW->bitsForTID0.is_Low_Emissions = IN_Warmup;
    fuel_rate_control_DW->bitsForTID0.was_Low_Emissions = IN_Warmup;
  } else {
    if (!((fuel_rate_control_DW->bitsForTID0.is_active_O2 == 0U) ||
          (fuel_rate_control_DW->bitsForTID0.is_O2 != IN_A))) {
      /* During 'O2': '<S3>:1' */
      /* This state determines the validity of the exhaust gas oxygen sensor (EGO) data. */
      /* During 'A': '<S3>:171' */
      fuel_rate_control_DW->es_o.ego = fuel_rate_control_U->sensors.ego;
      switch (fuel_rate_control_DW->bitsForTID0.is_A) {
       case IN_O2_failure:
        /* During 'O2_failure': '<S3>:3' */
        /* The EGO sensor has failed. */
        if (fuel_rate_control_U->sensors.ego < 1.2F) {
          /* Transition: '<S3>:32' */
          fuel_rate_control_DW->bitsForTID0.is_A = IN_NO_ACTIVE_CHILD;

          /* Event: '<S3>:91' */
          sfEvent = event_DEC;
          if (fuel_rate_control_DW->bitsForTID0.is_active_Fail != 0U) {
            fuel_rate_control_Fail(&sfEvent, fuel_rate_control_U,
              fuel_rate_control_DW);
          }

          sfEvent = CALL_EVENT;
          fuel_rate_control_DW->bitsForTID0.is_A = IN_O2_normal;
          fuel_rate_control_DW->bitsForTID0.O2_normal = true;
        }
        break;

       case IN_O2_normal:
        /* During 'O2_normal': '<S3>:7' */
        /* Normal EGO sensor operation gives a reliable indication of excess oxygen in the exhaust gas, from which air/fuel mixture can be inferred. */
        if (fuel_rate_control_U->sensors.ego > 1.2F) {
          /* Transition: '<S3>:33' */
          /* The failure condition is indicated by excessive output voltage. */
          fuel_rate_control_DW->bitsForTID0.O2_normal = false;
          fuel_rate_control_DW->bitsForTID0.is_A = IN_NO_ACTIVE_CHILD;

          /* Event: '<S3>:92' */
          sfEvent = event_INC;
          if (fuel_rate_control_DW->bitsForTID0.is_active_Fail != 0U) {
            fuel_rate_control_Fail(&sfEvent, fuel_rate_control_U,
              fuel_rate_control_DW);
          }

          sfEvent = CALL_EVENT;
          fuel_rate_control_DW->bitsForTID0.is_A = IN_O2_failure;
        }
        break;

       case IN_O2_warmup:
        /* During 'O2_warmup': '<S3>:4' */
        /* The EGO sensor must come up to temperature before its data is reliable. */
        if (fuel_rate_control_DW->temporalCounter_i1 >= 480) {
          /* Transition: '<S3>:36' */
          /* The value for sufficient warm up to operational temperature (nominally 4.8 seconds). */
          fuel_rate_control_DW->bitsForTID0.is_A = IN_O2_normal;
          fuel_rate_control_DW->bitsForTID0.O2_normal = true;
        }
        break;

       default:
        /* Unreachable state, for coverage only */
        fuel_rate_control_DW->bitsForTID0.is_A = IN_NO_ACTIVE_CHILD;
        break;
      }
    }

    if (fuel_rate_control_DW->bitsForTID0.is_active_Pressure != 0U) {
      /* During 'Pressure': '<S3>:2' */
      /* This state assesses the validity of the manifold absolute pressure (MAP) sensor. */
      switch (fuel_rate_control_DW->bitsForTID0.is_Pressure) {
       case IN_fail:
        /* During 'fail': '<S3>:6' */
        /* The pressure sensor (manifold, or MAP) no longer produces reliable data. */
        if ((fuel_rate_control_U->sensors.map > 0.05F) &&
            (fuel_rate_control_U->sensors.map < 0.95F)) {
          /* Transition: '<S3>:37' */
          fuel_rate_control_DW->bitsForTID0.is_Pressure = IN_NO_ACTIVE_CHILD;

          /* Event: '<S3>:91' */
          sfEvent = event_DEC;
          if (fuel_rate_control_DW->bitsForTID0.is_active_Fail != 0U) {
            fuel_rate_control_Fail(&sfEvent, fuel_rate_control_U,
              fuel_rate_control_DW);
          }

          sfEvent = -1;
          fuel_rate_control_DW->bitsForTID0.is_Pressure = IN_normal;
        } else {
          /* Outputs for Function Call SubSystem: '<S3>/Pressure.map_estimate' */
          /* Lookup_n-D: '<S7>/Pressure Estimation' */
          /* Simulink Function 'map_estimate': '<S3>:112' */
          PressureEstimation = look2_iflf_linlca
            (fuel_rate_control_U->sensors.speed,
             fuel_rate_control_U->sensors.throttle,
             fuel_rate_control_ConstP.pooled1, fuel_rate_control_ConstP.pooled2,
             fuel_rate_control_ConstP.PressureEstimation_,
             fuel_rate_control_ConstP.PressureEstimation_ma, 18U);

          /* End of Outputs for SubSystem: '<S3>/Pressure.map_estimate' */
          fuel_rate_control_DW->es_o.map = PressureEstimation;
        }
        break;

       case IN_normal:
        /* During 'normal': '<S3>:5' */
        /* The manifold pressure sensor gives a reliable indication of absolute pressure (MAP). */
        if ((fuel_rate_control_U->sensors.map > 0.95F) ||
            (fuel_rate_control_U->sensors.map < 0.05F)) {
          /* Transition: '<S3>:31' */
          fuel_rate_control_DW->bitsForTID0.is_Pressure = IN_NO_ACTIVE_CHILD;

          /* Event: '<S3>:92' */
          sfEvent = event_INC;
          if (fuel_rate_control_DW->bitsForTID0.is_active_Fail != 0U) {
            fuel_rate_control_Fail(&sfEvent, fuel_rate_control_U,
              fuel_rate_control_DW);
          }

          sfEvent = -1;
          fuel_rate_control_DW->bitsForTID0.is_Pressure = IN_fail;
        } else {
          fuel_rate_control_DW->es_o.map = fuel_rate_control_U->sensors.map;
        }
        break;

       default:
        /* Unreachable state, for coverage only */
        fuel_rate_control_DW->bitsForTID0.is_Pressure = IN_NO_ACTIVE_CHILD;
        break;
      }
    }

    if (fuel_rate_control_DW->bitsForTID0.is_active_Throttle != 0U) {
      /* During 'Throttle': '<S3>:8' */
      /* This state determines the validity of the throttle sensor signal. */
      switch (fuel_rate_control_DW->bitsForTID0.is_Throttle) {
       case IN_fail:
        /* During 'fail': '<S3>:11' */
        /* Signal levels indicate that the throttle sensor data is no longer valid. */
        if ((fuel_rate_control_U->sensors.throttle > 3.0F) &&
            (fuel_rate_control_U->sensors.throttle < 90.0F)) {
          /* Transition: '<S3>:42' */
          fuel_rate_control_DW->bitsForTID0.is_Throttle = IN_NO_ACTIVE_CHILD;

          /* Event: '<S3>:91' */
          sfEvent = event_DEC;
          if (fuel_rate_control_DW->bitsForTID0.is_active_Fail != 0U) {
            fuel_rate_control_Fail(&sfEvent, fuel_rate_control_U,
              fuel_rate_control_DW);
          }

          sfEvent = -1;
          fuel_rate_control_DW->bitsForTID0.is_Throttle = IN_normal;
        } else {
          /* Outputs for Function Call SubSystem: '<S3>/Throttle.throttle_estimate' */
          /* Lookup_n-D: '<S9>/Throttle Estimation' */
          /* Simulink Function 'throttle_estimate': '<S3>:107' */
          PressureEstimation = look2_iflf_linlca
            (fuel_rate_control_U->sensors.speed,
             fuel_rate_control_U->sensors.map, fuel_rate_control_ConstP.pooled1,
             fuel_rate_control_ConstP.pooled3,
             fuel_rate_control_ConstP.ThrottleEstimation_,
             fuel_rate_control_ConstP.pooled8, 18U);

          /* End of Outputs for SubSystem: '<S3>/Throttle.throttle_estimate' */
          fuel_rate_control_DW->es_o.throttle = PressureEstimation;
        }
        break;

       case IN_normal:
        /* During 'normal': '<S3>:10' */
        /* The throttle sensor gives a reliable indication of the angle of the throttle plate. */
        if ((fuel_rate_control_U->sensors.throttle > 90.0F) ||
            (fuel_rate_control_U->sensors.throttle < 3.0F)) {
          /* Transition: '<S3>:38' */
          /* A failure is indicated by data which is outside the expected range, typically due to an open or short circuit. */
          fuel_rate_control_DW->bitsForTID0.is_Throttle = IN_NO_ACTIVE_CHILD;

          /* Event: '<S3>:92' */
          sfEvent = event_INC;
          if (fuel_rate_control_DW->bitsForTID0.is_active_Fail != 0U) {
            fuel_rate_control_Fail(&sfEvent, fuel_rate_control_U,
              fuel_rate_control_DW);
          }

          sfEvent = -1;
          fuel_rate_control_DW->bitsForTID0.is_Throttle = IN_fail;
        } else {
          fuel_rate_control_DW->es_o.throttle =
            fuel_rate_control_U->sensors.throttle;
        }
        break;

       default:
        /* Unreachable state, for coverage only */
        fuel_rate_control_DW->bitsForTID0.is_Throttle = IN_NO_ACTIVE_CHILD;
        break;
      }
    }

    if (fuel_rate_control_DW->bitsForTID0.is_active_Speed != 0U) {
      /* During 'Speed': '<S3>:9' */
      /* This state infers the validity of the speed sensor data.  A failure is indicated by the presence of manifold vacuum at zero speed. */
      switch (fuel_rate_control_DW->bitsForTID0.is_Speed) {
       case IN_fail:
        /* During 'fail': '<S3>:13' */
        /* The engine speed data is no longer available. */
        if (fuel_rate_control_U->sensors.speed > 0.0F) {
          /* Transition: '<S3>:43' */
          fuel_rate_control_DW->bitsForTID0.is_Speed = IN_NO_ACTIVE_CHILD;

          /* Event: '<S3>:91' */
          sfEvent = event_DEC;
          if (fuel_rate_control_DW->bitsForTID0.is_active_Fail != 0U) {
            fuel_rate_control_Fail(&sfEvent, fuel_rate_control_U,
              fuel_rate_control_DW);
          }

          sfEvent = -1;
          fuel_rate_control_DW->bitsForTID0.is_Speed = IN_normal;
        } else {
          /* Outputs for Function Call SubSystem: '<S3>/Speed.speed_estimate' */
          /* Lookup_n-D: '<S8>/Speed Estimation' */
          /* Simulink Function 'speed_estimate': '<S3>:119' */
          PressureEstimation = look2_iflf_linlca
            (fuel_rate_control_U->sensors.throttle,
             fuel_rate_control_U->sensors.map, fuel_rate_control_ConstP.pooled2,
             fuel_rate_control_ConstP.pooled3,
             fuel_rate_control_ConstP.SpeedEstimation_tab,
             fuel_rate_control_ConstP.SpeedEstimation_maxIn, 17U);

          /* Saturate: '<S8>/Saturation' */
          if (PressureEstimation > 628.0F) {
            fuel_rate_control_DW->es_o.speed = 628.0F;
          } else if (PressureEstimation < 0.0F) {
            fuel_rate_control_DW->es_o.speed = 0.0F;
          } else {
            fuel_rate_control_DW->es_o.speed = PressureEstimation;
          }

          /* End of Saturate: '<S8>/Saturation' */
          /* End of Outputs for SubSystem: '<S3>/Speed.speed_estimate' */
        }
        break;

       case IN_normal:
        /* During 'normal': '<S3>:12' */
        /* The speed sensor data accurately represents the engine speed. */
        if ((fuel_rate_control_U->sensors.speed == 0.0F) &&
            (fuel_rate_control_U->sensors.map < 250.0F)) {
          /* Transition: '<S3>:40' */
          /* Loss of a reliable speed sensor signal will indicate zero speed.  This is deemed invalid when the manifold vacuum indicates otherwise. */
          fuel_rate_control_DW->bitsForTID0.is_Speed = IN_NO_ACTIVE_CHILD;

          /* Event: '<S3>:92' */
          sfEvent = event_INC;
          if (fuel_rate_control_DW->bitsForTID0.is_active_Fail != 0U) {
            fuel_rate_control_Fail(&sfEvent, fuel_rate_control_U,
              fuel_rate_control_DW);
          }

          sfEvent = -1;
          fuel_rate_control_DW->bitsForTID0.is_Speed = IN_fail;
        } else {
          fuel_rate_control_DW->es_o.speed = fuel_rate_control_U->sensors.speed;
        }
        break;

       default:
        /* Unreachable state, for coverage only */
        fuel_rate_control_DW->bitsForTID0.is_Speed = IN_NO_ACTIVE_CHILD;
        break;
      }
    }

    if (fuel_rate_control_DW->bitsForTID0.is_active_Fail != 0U) {
      fuel_rate_control_Fail(&sfEvent, fuel_rate_control_U, fuel_rate_control_DW);
    }

    if (fuel_rate_control_DW->bitsForTID0.is_active_Fueling_Mode != 0U) {
      fuel_rate_control_Fueling_Mode(&sfEvent, fuel_rate_control_U,
        fuel_rate_control_DW);
    }
  }

  /* End of Chart: '<S1>/control_logic' */

  /* Lookup_n-D: '<S2>/Pumping Constant' */
  PressureEstimation = look2_iflf_linlca(fuel_rate_control_DW->es_o.speed,
    fuel_rate_control_DW->es_o.map, fuel_rate_control_ConstP.pooled1,
    fuel_rate_control_ConstP.pooled3,
    fuel_rate_control_ConstP.PumpingConstant_tab,
    fuel_rate_control_ConstP.pooled8, 18U);

  /* Product: '<S2>/Product' */
  rtb_Product = fuel_rate_control_DW->es_o.speed * PressureEstimation;

  /* DiscreteFilter: '<S2>/Throttle Transient' */
  denAccum = fuel_rate_control_DW->es_o.throttle - -0.8F *
    fuel_rate_control_DW->ThrottleTransient_states;

  /* Switch: '<S2>/hold integrator' incorporates:
   *  Constant: '<S2>/Constant'
   *  Constant: '<S2>/Constant2'
   *  Constant: '<S2>/Oxygen Sensor Switching Threshold'
   *  Constant: '<S6>/Constant'
   *  DataTypeConversion: '<S2>/Data Type  Conversion'
   *  Logic: '<S2>/Logic1'
   *  Product: '<S2>/Product1'
   *  RelationalOperator: '<S2>/Relational Operator1'
   *  RelationalOperator: '<S2>/Relational Operator3'
   *  Sum: '<S2>/Sum1'
   */
  if (fuel_rate_control_DW->bitsForTID0.O2_normal &&
      (fuel_rate_control_DW->fuel_mode == LOW)) {
    /* Lookup_n-D: '<S2>/Ramp Rate Ki' */
    PressureEstimation = look2_iflf_linlca(fuel_rate_control_DW->es_o.speed,
      fuel_rate_control_DW->es_o.map,
      fuel_rate_control_ConstP.RampRateKi_bp01Data,
      fuel_rate_control_ConstP.RampRateKi_bp02Data,
      fuel_rate_control_ConstP.RampRateKi_tableData,
      fuel_rate_control_ConstP.RampRateKi_maxIndex, 6U);
    rtb_holdintegrator = ((real32_T)(fuel_rate_control_DW->es_o.ego <= 0.5F) -
                          0.5F) * PressureEstimation;
  } else {
    rtb_holdintegrator = 0.0F;
  }

  /* End of Switch: '<S2>/hold integrator' */

  /* MultiPortSwitch: '<S10>/Multiport Switch' incorporates:
   *  Constant: '<S10>/normal'
   *  Constant: '<S10>/rich'
   *  Constant: '<S10>/shutdown'
   */
  switch (fuel_rate_control_DW->fuel_mode) {
   case LOW:
    PressureEstimation = 0.0684931502F;
    break;

   case RICH:
    PressureEstimation = 0.0856164396F;
    break;

   default:
    PressureEstimation = 0.0F;
    break;
  }

  /* End of MultiPortSwitch: '<S10>/Multiport Switch' */

  /* Product: '<S10>/Product' incorporates:
   *  DiscreteFilter: '<S2>/Throttle Transient'
   *  Product: '<S2>/Product2'
   *  Sum: '<S2>/Sum'
   */
  PressureEstimation *= (0.01F * denAccum + -0.01F *
    fuel_rate_control_DW->ThrottleTransient_states) + rtb_Product *
    fuel_rate_control_DW->es_o.map;

  /* SwitchCase: '<S11>/Switch Case' */
  switch (fuel_rate_control_DW->fuel_mode) {
   case LOW:
    /* Outputs for IfAction SubSystem: '<S11>/low_mode' incorporates:
     *  ActionPort: '<S13>/Action Port'
     */
    /* DiscreteFilter: '<S13>/Discrete Filter' incorporates:
     *  DiscreteIntegrator: '<S2>/Discrete Integrator'
     */
    rtb_Product = fuel_rate_control_DW->DiscreteIntegrator_DSTATE - -0.7408F *
      fuel_rate_control_DW->DiscreteFilter_states_i;

    /* Outport: '<Root>/fuel_rate' incorporates:
     *  DiscreteFilter: '<S13>/Discrete Filter'
     *  Sum: '<S13>/Sum3'
     */
    fuel_rate_control_Y->fuel_rate = (8.7696F * rtb_Product + -8.5104F *
      fuel_rate_control_DW->DiscreteFilter_states_i) + PressureEstimation;

    /* Update for DiscreteFilter: '<S13>/Discrete Filter' incorporates:
     *  DiscreteFilter: '<S13>/Discrete Filter'
     */
    fuel_rate_control_DW->DiscreteFilter_states_i = rtb_Product;

    /* End of Outputs for SubSystem: '<S11>/low_mode' */
    break;

   case RICH:
    /* Outputs for IfAction SubSystem: '<S11>/rich_mode' incorporates:
     *  ActionPort: '<S14>/Action Port'
     */
    /* Outport: '<Root>/fuel_rate' incorporates:
     *  DiscreteFilter: '<S14>/Discrete Filter'
     */
    fuel_rate_control_Y->fuel_rate = 0.2592F *
      fuel_rate_control_DW->DiscreteFilter_states;

    /* Update for DiscreteFilter: '<S14>/Discrete Filter' incorporates:
     *  DiscreteIntegrator: '<S2>/Discrete Integrator'
     *  Sum: '<S14>/Sum2'
     */
    fuel_rate_control_DW->DiscreteFilter_states = (PressureEstimation +
      fuel_rate_control_DW->DiscreteIntegrator_DSTATE) - -0.7408F *
      fuel_rate_control_DW->DiscreteFilter_states;

    /* End of Outputs for SubSystem: '<S11>/rich_mode' */
    break;

   default:
    /* Outputs for IfAction SubSystem: '<S11>/disabled_mode' incorporates:
     *  ActionPort: '<S12>/Action Port'
     */
    /* Outport: '<Root>/fuel_rate' incorporates:
     *  Constant: '<S12>/shutoff'
     */
    fuel_rate_control_Y->fuel_rate = 0.0F;

    /* End of Outputs for SubSystem: '<S11>/disabled_mode' */
    break;
  }

  /* End of SwitchCase: '<S11>/Switch Case' */

  /* Update for DiscreteIntegrator: '<S2>/Discrete Integrator' */
  fuel_rate_control_DW->DiscreteIntegrator_DSTATE += 0.01F * rtb_holdintegrator;

  /* Update for DiscreteFilter: '<S2>/Throttle Transient' incorporates:
   *  DiscreteFilter: '<S2>/Throttle Transient'
   */
  fuel_rate_control_DW->ThrottleTransient_states = denAccum;

  /* End of Outputs for SubSystem: '<Root>/fuel_rate_control' */
}

/* Model initialize function */
void fuel_rate_control_initialize(RT_MODEL_fuel_rate_control_T *const
  fuel_rate_control_M)
{
  DW_fuel_rate_control_T *fuel_rate_control_DW = ((DW_fuel_rate_control_T *)
    fuel_rate_control_M->ModelData.dwork);

  /* Registration code */

  /* states (dwork) */
  {
    fuel_rate_control_DW->fuel_mode = LOW;
  }

  /* InitializeConditions for Atomic SubSystem: '<Root>/fuel_rate_control' */
  /* InitializeConditions for Chart: '<S1>/control_logic'
   *
   * Block description for '<S1>/control_logic':
   *  Stateflow diagram to determine control system operating mode
   */
  fuel_rate_control_DW->es_o.throttle = 0.0F;
  fuel_rate_control_DW->es_o.speed = 0.0F;
  fuel_rate_control_DW->es_o.ego = 0.0F;
  fuel_rate_control_DW->es_o.map = 0.0F;
  fuel_rate_control_DW->fuel_mode = LOW;

  /* End of InitializeConditions for SubSystem: '<Root>/fuel_rate_control' */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
