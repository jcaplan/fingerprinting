/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: fuel_rate_control.h
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

#ifndef RTW_HEADER_fuel_rate_control_h_
#define RTW_HEADER_fuel_rate_control_h_
#include "rtwtypes.h"
#ifndef fuel_rate_control_COMMON_INCLUDES_
# define fuel_rate_control_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* fuel_rate_control_COMMON_INCLUDES_ */

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Forward declaration for rtModel */
typedef struct tag_RTM_fuel_rate_control_T RT_MODEL_fuel_rate_control_T;

#ifndef _DEFINED_TYPEDEF_FOR_sld_FuelModes_
#define _DEFINED_TYPEDEF_FOR_sld_FuelModes_

typedef enum {
  LOW = 1,                             /* Default value */
  RICH,
  DISABLED
} sld_FuelModes;

#endif

#ifndef _DEFINED_TYPEDEF_FOR_EngSensors_
#define _DEFINED_TYPEDEF_FOR_EngSensors_

typedef struct {
  real32_T throttle;
  real32_T speed;
  real32_T ego;
  real32_T map;
} EngSensors;

#endif

/* Block signals and states (auto storage) for system '<Root>' */
typedef struct {
  EngSensors es_o;                     /* '<S1>/control_logic' */
  real32_T DiscreteIntegrator_DSTATE;  /* '<S2>/Discrete Integrator' */
  real32_T ThrottleTransient_states;   /* '<S2>/Throttle Transient' */
  real32_T DiscreteFilter_states;      /* '<S14>/Discrete Filter' */
  real32_T DiscreteFilter_states_i;    /* '<S13>/Discrete Filter' */
  sld_FuelModes fuel_mode;             /* '<S1>/control_logic' */
  struct {
    uint_T is_A:2;                     /* '<S1>/control_logic' */
    uint_T is_Pressure:2;              /* '<S1>/control_logic' */
    uint_T is_Throttle:2;              /* '<S1>/control_logic' */
    uint_T is_Speed:2;                 /* '<S1>/control_logic' */
    uint_T is_Fail:2;                  /* '<S1>/control_logic' */
    uint_T is_Multi:2;                 /* '<S1>/control_logic' */
    uint_T is_Fueling_Mode:2;          /* '<S1>/control_logic' */
    uint_T is_Fuel_Disabled:2;         /* '<S1>/control_logic' */
    uint_T is_Running:2;               /* '<S1>/control_logic' */
    uint_T was_Running:2;              /* '<S1>/control_logic' */
    uint_T is_Low_Emissions:2;         /* '<S1>/control_logic' */
    uint_T was_Low_Emissions:2;        /* '<S1>/control_logic' */
    uint_T is_active_c1_fuel_rate_control:1;/* '<S1>/control_logic' */
    uint_T is_O2:1;                    /* '<S1>/control_logic' */
    uint_T is_active_O2:1;             /* '<S1>/control_logic' */
    uint_T is_active_Pressure:1;       /* '<S1>/control_logic' */
    uint_T is_active_Throttle:1;       /* '<S1>/control_logic' */
    uint_T is_active_Speed:1;          /* '<S1>/control_logic' */
    uint_T is_active_Fail:1;           /* '<S1>/control_logic' */
    uint_T is_active_Fueling_Mode:1;   /* '<S1>/control_logic' */
    uint_T is_Rich_Mixture:1;          /* '<S1>/control_logic' */
    uint_T O2_normal:1;                /* '<S1>/control_logic' */
  } bitsForTID0;

  uint16_T temporalCounter_i1;         /* '<S1>/control_logic' */
} DW_fuel_rate_control_T;

/* Constant parameters (auto storage) */
typedef struct {
  /* Computed Parameter: RampRateKi_tableData
   * Referenced by: '<S2>/Ramp Rate Ki'
   */
  real32_T RampRateKi_tableData[36];

  /* Computed Parameter: RampRateKi_bp01Data
   * Referenced by: '<S2>/Ramp Rate Ki'
   */
  real32_T RampRateKi_bp01Data[6];

  /* Computed Parameter: RampRateKi_bp02Data
   * Referenced by: '<S2>/Ramp Rate Ki'
   */
  real32_T RampRateKi_bp02Data[6];

  /* Computed Parameter: PressureEstimation_
   * Referenced by: '<S7>/Pressure Estimation'
   */
  real32_T PressureEstimation_[306];

  /* Pooled Parameter (Expression: SpeedVect)
   * Referenced by:
   *   '<S2>/Pumping Constant'
   *   '<S7>/Pressure Estimation'
   *   '<S9>/Throttle Estimation'
   */
  real32_T pooled1[18];

  /* Pooled Parameter (Expression: ThrotVect)
   * Referenced by:
   *   '<S7>/Pressure Estimation'
   *   '<S8>/Speed Estimation'
   */
  real32_T pooled2[17];

  /* Computed Parameter: ThrottleEstimation_
   * Referenced by: '<S9>/Throttle Estimation'
   */
  real32_T ThrottleEstimation_[342];

  /* Computed Parameter: SpeedEstimation_tab
   * Referenced by: '<S8>/Speed Estimation'
   */
  real32_T SpeedEstimation_tab[323];

  /* Pooled Parameter (Expression: PressVect)
   * Referenced by:
   *   '<S2>/Pumping Constant'
   *   '<S8>/Speed Estimation'
   *   '<S9>/Throttle Estimation'
   */
  real32_T pooled3[19];

  /* Computed Parameter: PumpingConstant_tab
   * Referenced by: '<S2>/Pumping Constant'
   */
  real32_T PumpingConstant_tab[342];

  /* Computed Parameter: RampRateKi_maxIndex
   * Referenced by: '<S2>/Ramp Rate Ki'
   */
  uint32_T RampRateKi_maxIndex[2];

  /* Computed Parameter: PressureEstimation_ma
   * Referenced by: '<S7>/Pressure Estimation'
   */
  uint32_T PressureEstimation_ma[2];

  /* Pooled Parameter (Expression: )
   * Referenced by:
   *   '<S2>/Pumping Constant'
   *   '<S9>/Throttle Estimation'
   */
  uint32_T pooled8[2];

  /* Computed Parameter: SpeedEstimation_maxIn
   * Referenced by: '<S8>/Speed Estimation'
   */
  uint32_T SpeedEstimation_maxIn[2];
} ConstP_fuel_rate_control_T;

/* External inputs (root inport signals with auto storage) */
typedef struct {
  EngSensors sensors;                  /* '<Root>/sensors' */
} ExtU_fuel_rate_control_T;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  real32_T fuel_rate;                  /* '<Root>/fuel_rate' */
} ExtY_fuel_rate_control_T;

/* Real-time Model Data Structure */
struct tag_RTM_fuel_rate_control_T {
  const char_T * volatile errorStatus;

  /*
   * ModelData:
   * The following substructure contains information regarding
   * the data used in the model.
   */
  struct {
    DW_fuel_rate_control_T *dwork;
  } ModelData;
};

/* Constant parameters (auto storage) */
extern const ConstP_fuel_rate_control_T fuel_rate_control_ConstP;

/* Model entry point functions */
extern void fuel_rate_control_initialize(RT_MODEL_fuel_rate_control_T *const
  fuel_rate_control_M);
extern void fuel_rate_control_step(RT_MODEL_fuel_rate_control_T *const
  fuel_rate_control_M, ExtU_fuel_rate_control_T *fuel_rate_control_U,
  ExtY_fuel_rate_control_T *fuel_rate_control_Y);

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Note that this particular code originates from a subsystem build,
 * and has its own system numbers different from the parent model.
 * Refer to the system hierarchy for this subsystem below, and use the
 * MATLAB hilite_system command to trace the generated code back
 * to the parent model.  For example,
 *
 * hilite_system('fuelsys/fuel_rate_control')    - opens subsystem fuelsys/fuel_rate_control
 * hilite_system('fuelsys/fuel_rate_control/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'fuelsys'
 * '<S1>'   : 'fuelsys/fuel_rate_control'
 * '<S2>'   : 'fuelsys/fuel_rate_control/airflow_calc'
 * '<S3>'   : 'fuelsys/fuel_rate_control/control_logic'
 * '<S4>'   : 'fuelsys/fuel_rate_control/fuel_calc'
 * '<S5>'   : 'fuelsys/fuel_rate_control/validate_sample_time'
 * '<S6>'   : 'fuelsys/fuel_rate_control/airflow_calc/Enumerated Constant'
 * '<S7>'   : 'fuelsys/fuel_rate_control/control_logic/Pressure.map_estimate'
 * '<S8>'   : 'fuelsys/fuel_rate_control/control_logic/Speed.speed_estimate'
 * '<S9>'   : 'fuelsys/fuel_rate_control/control_logic/Throttle.throttle_estimate'
 * '<S10>'  : 'fuelsys/fuel_rate_control/fuel_calc/feedforward_fuel_rate'
 * '<S11>'  : 'fuelsys/fuel_rate_control/fuel_calc/switchable_compensation'
 * '<S12>'  : 'fuelsys/fuel_rate_control/fuel_calc/switchable_compensation/disabled_mode'
 * '<S13>'  : 'fuelsys/fuel_rate_control/fuel_calc/switchable_compensation/low_mode'
 * '<S14>'  : 'fuelsys/fuel_rate_control/fuel_calc/switchable_compensation/rich_mode'
 * '<S15>'  : 'fuelsys/fuel_rate_control/validate_sample_time/CheckRange'
 */

/*-
 * Requirements for '<Root>': fuel_rate_control
 */
#endif                                 /* RTW_HEADER_fuel_rate_control_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
