/*
 * File: TransmissionControl.h
 *
 * Code generated for Simulink model 'TransmissionControl'.
 *
 * Model version                  : 1.4
 * Simulink Coder version         : 8.6 (R2014a) 27-Dec-2013
 * C/C++ source code generated on : Sun Mar 22 13:54:15 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ASIC/FPGA->ASIC/FPGA
 * Emulation hardware selection:
 *    Differs from embedded hardware (Generic->32-bit Embedded Processor)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_TransmissionControl_h_
#define RTW_HEADER_TransmissionControl_h_
#include "rtwtypes.h"
#include "multiword_types.h"
#include <stddef.h>
#ifndef TransmissionControl_COMMON_INCLUDES_
# define TransmissionControl_COMMON_INCLUDES_
#endif                                 /* TransmissionControl_COMMON_INCLUDES_ */

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
typedef struct tag_RTM_TransmissionControl_T RT_MODEL_TransmissionControl_T;

/* Parameters (auto storage) */
struct P_TransmissionControl_T_ {
  real32_T LowFuelSensor_const;        /* Mask Parameter: LowFuelSensor_const
                                        * Referenced by: '<S1>/Constant'
                                        */
};

/* Parameters (auto storage) */
typedef struct P_TransmissionControl_T_ P_TransmissionControl_T;

/* Real-time Model Data Structure */
struct tag_RTM_TransmissionControl_T {
  const char_T * volatile errorStatus;
};

/* Imported (extern) block signals */
extern boolean_T warning_light;        /* '<S1>/Compare' */
extern real32_T fluid_sensor;          /* '<Root>/Fluid Sensor' */

/* Block parameters (auto storage) */
extern P_TransmissionControl_T TransmissionControl_P;

/* Model entry point functions */
extern void TransmissionControl_initialize(void);
extern void TransmissionControl_step(void);
extern void TransmissionControl_terminate(void);

/* Real-time Model object */
extern RT_MODEL_TransmissionControl_T *const TransmissionControl_M;

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
 * '<Root>' : 'TransmissionControl'
 * '<S1>'   : 'TransmissionControl/Low Fuel Sensor'
 */
#endif                                 /* RTW_HEADER_TransmissionControl_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
