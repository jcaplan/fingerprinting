/*
 * File: ShiftLogic.h
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

#ifndef RTW_HEADER_ShiftLogic_h_
#define RTW_HEADER_ShiftLogic_h_
#ifndef ShiftLogic_COMMON_INCLUDES_
# define ShiftLogic_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* ShiftLogic_COMMON_INCLUDES_ */

#include "ShiftLogic_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Block signals (auto storage) */
typedef struct {
  real_T InterpDown;                   /* '<S2>/InterpDown' */
  real_T InterpUp;                     /* '<S2>/InterpUp' */
} B_ShiftLogic_T;

/* Block states (auto storage) for system '<Root>' */
typedef struct {
  int32_T sfEvent;                     /* '<Root>/ShiftLogic' */
  uint32_T temporalCounter_i1;         /* '<Root>/ShiftLogic' */
  uint8_T is_active_c1_ShiftLogic;     /* '<Root>/ShiftLogic' */
  uint8_T is_gear_state;               /* '<Root>/ShiftLogic' */
  uint8_T is_active_gear_state;        /* '<Root>/ShiftLogic' */
  uint8_T is_selection_state;          /* '<Root>/ShiftLogic' */
  uint8_T is_active_selection_state;   /* '<Root>/ShiftLogic' */
} DW_ShiftLogic_T;

/* External inputs (root inport signals with auto storage) */
typedef struct {
  real_T Throttle;                     /* '<Root>/Throttle' */
  real_T VehicleSpeed;                 /* '<Root>/VehicleSpeed' */
} ExtU_ShiftLogic_T;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  real_T Gear;                         /* '<Root>/Gear' */
} ExtY_ShiftLogic_T;

/* Parameters (auto storage) */
struct P_ShiftLogic_T_ {
  real_T down_th_Y0;                   /* Expression: 0
                                        * Referenced by: '<S2>/down_th'
                                        */
  real_T up_th_Y0;                     /* Expression: 0
                                        * Referenced by: '<S2>/up_th'
                                        */
  real_T InterpDown_tableData[24];     /* Expression: DOWN_TABLE
                                        * Referenced by: '<S2>/InterpDown'
                                        */
  real_T InterpDown_bp01Data[6];       /* Expression: DOWN_TH_BP
                                        * Referenced by: '<S2>/InterpDown'
                                        */
  real_T InterpDown_bp02Data[4];       /* Expression: [1:4]
                                        * Referenced by: '<S2>/InterpDown'
                                        */
  real_T InterpUp_tableData[24];       /* Expression: UP_TABLE
                                        * Referenced by: '<S2>/InterpUp'
                                        */
  real_T InterpUp_bp01Data[6];         /* Expression: UP_TH_BP
                                        * Referenced by: '<S2>/InterpUp'
                                        */
  real_T InterpUp_bp02Data[4];         /* Expression: [1:4]
                                        * Referenced by: '<S2>/InterpUp'
                                        */
  real_T ShiftLogic_TWAIT;             /* Expression: TWAIT
                                        * Referenced by: '<Root>/ShiftLogic'
                                        */
  uint32_T InterpDown_maxIndex[2];     /* Computed Parameter: InterpDown_maxIndex
                                        * Referenced by: '<S2>/InterpDown'
                                        */
  uint32_T InterpUp_maxIndex[2];       /* Computed Parameter: InterpUp_maxIndex
                                        * Referenced by: '<S2>/InterpUp'
                                        */
};

/* Real-time Model Data Structure */
struct tag_RTM_ShiftLogic_T {
  const char_T * volatile errorStatus;
};

/* Block parameters (auto storage) */
extern P_ShiftLogic_T ShiftLogic_P __attribute__ ((section (".global_data")));

/* Block signals (auto storage) */
extern B_ShiftLogic_T ShiftLogic_B __attribute__ ((section (".global_data")));

/* Block states (auto storage) */
extern DW_ShiftLogic_T ShiftLogic_DW __attribute__ ((section (".global_data")));

/* External inputs (root inport signals with auto storage) */
extern ExtU_ShiftLogic_T ShiftLogic_U __attribute__ ((section (".global_data")));

/* External outputs (root outports fed by signals with auto storage) */
extern ExtY_ShiftLogic_T ShiftLogic_Y __attribute__ ((section (".global_data")));

/* Model entry point functions */
extern void ShiftLogic_initialize(void);
extern void ShiftLogic_step(void);
extern void ShiftLogic_terminate(void);

/* Real-time Model object */
extern RT_MODEL_ShiftLogic_T *const ShiftLogic_M ;

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
 * hilite_system('sldemo_autotrans/ShiftLogic')    - opens subsystem sldemo_autotrans/ShiftLogic
 * hilite_system('sldemo_autotrans/ShiftLogic/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'sldemo_autotrans'
 * '<S1>'   : 'sldemo_autotrans/ShiftLogic'
 * '<S2>'   : 'sldemo_autotrans/ShiftLogic/ComputeThreshold'
 */
#endif                                 /* RTW_HEADER_ShiftLogic_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
