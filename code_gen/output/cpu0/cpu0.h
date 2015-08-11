#include "shared_mem.h"
/*****************************************************************************
 * Defines
 *****************************************************************************/
#define CORE_ID 			(ALT_CPU_CPU_ID_VALUE - 1)
/*****************************************************************************
 * Stack sizes
 *
 * The stack size is composed of three elements A + B + C:
 * A: Amount of room that function code requires to execute on stack
 *
 * B: empirically determined value for amount of stack used with function
 * body consisting of empty while loop
 *
 * C: Safety margin
 *
 * Note that OS_STK allocates 32 bit words so these numbers represent words
 *****************************************************************************/
#define STACKSIZE_MINOFFSET                       316
#define STACKSIZE_MARGINERROR                     150
#define AirbagModel_STACKSIZE                     (   8 + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR )
#define CruiseControlSystem_STACKSIZE             (   6 + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR )
#define FuelSensor_STACKSIZE                      (   5 + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR )
#define TransmissionControl_STACKSIZE             (   4 + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR )
#define RadarTracker_STACKSIZE                    ( 329 + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR )
#define for_loop_STACKSIZE                        (  20 + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR )
/*****************************************************************************
 * Task Priorities
 *****************************************************************************/
#define AirbagModel_PRIORITY                      1
#define CruiseControlSystem_PRIORITY              2
#define FuelSensor_PRIORITY                       6
#define TransmissionControl_PRIORITY              7
#define RadarTracker_PRIORITY                     8
#define for_loop_PRIORITY                         10
/*****************************************************************************
 * Task Table indices
 *****************************************************************************/
#define AIRBAGMODEL_TABLE_INDEX                   0
#define CRUISECONTROLSYSTEM_TABLE_INDEX           1
#define RADARTRACKER_TABLE_INDEX                  2
/*****************************************************************************
 * Task Periods
 *****************************************************************************/
#define AIRBAGMODEL_PERIOD                        50
#define CRUISECONTROLSYSTEM_PERIOD                20
#define RADARTRACKER_PERIOD                       200
