/*
 * File: Sum.h
 *
 * Code generated for Simulink model 'Sum'.
 *
 * Model version                  : 1.3
 * Simulink Coder version         : 8.6 (R2014a) 27-Dec-2013
 * C/C++ source code generated on : Sun Mar 22 13:52:58 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ASIC/FPGA->ASIC/FPGA
 * Emulation hardware selection:
 *    Differs from embedded hardware (Generic->32-bit Embedded Processor)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_Sum_h_
#define RTW_HEADER_Sum_h_
#include <stddef.h>
#ifndef Sum_COMMON_INCLUDES_
# define Sum_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* Sum_COMMON_INCLUDES_ */

/* Shared type includes */
#include "multiword_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Forward declaration for rtModel */
typedef struct tag_RTM_Sum_T RT_MODEL_Sum_T;

/* Real-time Model Data Structure */
struct tag_RTM_Sum_T {
  const char_T * volatile errorStatus;
};

/* Imported (extern) block signals */
extern real32_T sum_out;               /* '<S1>/Sum' */
extern real32_T sum_in1;               /* '<Root>/In1' */
extern real32_T sum_in2;               /* '<Root>/In2' */

/* Model entry point functions */
extern void Sum_initialize(void);
extern void Sum_step(void);
extern void Sum_terminate(void);

/* Real-time Model object */
extern RT_MODEL_Sum_T *const Sum_M;

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
 * '<Root>' : 'Sum'
 * '<S1>'   : 'Sum/Sum'
 */
#endif                                 /* RTW_HEADER_Sum_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
