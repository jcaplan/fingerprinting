/*
 * File: Compare.h
 *
 * Code generated for Simulink model 'Compare'.
 *
 * Model version                  : 1.2
 * Simulink Coder version         : 8.6 (R2014a) 27-Dec-2013
 * C/C++ source code generated on : Sun Mar 22 14:33:06 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Generic->32-bit Embedded Processor
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_Compare_h_
#define RTW_HEADER_Compare_h_
#include "rtwtypes.h"
#include <stddef.h>
#ifndef Compare_COMMON_INCLUDES_
# define Compare_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* Compare_COMMON_INCLUDES_ */

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Forward declaration for rtModel */
typedef struct tag_RTM_Compare_T RT_MODEL_Compare_T;

/* Parameters (auto storage) */
struct P_Compare_T_ {
  real_T CompareToConstant_const;      /* Mask Parameter: CompareToConstant_const
                                        * Referenced by: '<S2>/Constant'
                                        */
};

/* Parameters (auto storage) */
typedef struct P_Compare_T_ P_Compare_T;

/* Real-time Model Data Structure */
struct tag_RTM_Compare_T {
  const char_T * volatile errorStatus;
};

/* Imported (extern) block signals */
extern boolean_T compare_out;          /* '<S2>/Compare' */
extern real_T compare_in;              /* '<Root>/In1' */

/* Block parameters (auto storage) */
extern P_Compare_T Compare_P;

/* Model entry point functions */
extern void Compare_initialize(void);
extern void Compare_step(void);
extern void Compare_terminate(void);

/* Real-time Model object */
extern RT_MODEL_Compare_T *const Compare_M;

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
 * '<Root>' : 'Compare'
 * '<S1>'   : 'Compare/Compare'
 * '<S2>'   : 'Compare/Compare/Compare To Constant'
 */
#endif                                 /* RTW_HEADER_Compare_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
