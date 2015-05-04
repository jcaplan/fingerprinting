/*
 * File: Gain.h
 *
 * Code generated for Simulink model 'Gain'.
 *
 * Model version                  : 1.1
 * Simulink Coder version         : 8.6 (R2014a) 27-Dec-2013
 * C/C++ source code generated on : Sun Mar 22 13:58:47 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Generic->32-bit Embedded Processor
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_Gain_h_
#define RTW_HEADER_Gain_h_
#include "rtwtypes.h"
#include <stddef.h>
#ifndef Gain_COMMON_INCLUDES_
# define Gain_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* Gain_COMMON_INCLUDES_ */

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Forward declaration for rtModel */
typedef struct tag_RTM_Gain_T RT_MODEL_Gain_T;

/* Parameters (auto storage) */
struct P_Gain_T_ {
  real32_T Gain_Gain;                  /* Computed Parameter: Gain_Gain
                                        * Referenced by: '<S1>/Gain'
                                        */
};

/* Parameters (auto storage) */
typedef struct P_Gain_T_ P_Gain_T;

/* Real-time Model Data Structure */
struct tag_RTM_Gain_T {
  const char_T * volatile errorStatus;
};

/* Imported (extern) block signals */
extern real32_T gain_out;              /* '<S1>/Gain' */
extern real32_T gain_in;               /* '<Root>/In1' */

/* Block parameters (auto storage) */
extern P_Gain_T Gain_P;

/* Model entry point functions */
extern void Gain_initialize(void);
extern void Gain_step(void);
extern void Gain_terminate(void);

/* Real-time Model object */
extern RT_MODEL_Gain_T *const Gain_M;

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'Gain'
 * '<S1>'   : 'Gain/Gain'
 */
#endif                                 /* RTW_HEADER_Gain_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
