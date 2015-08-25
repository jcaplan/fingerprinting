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
#define AIRBAGMODEL_STACKSIZE                     (   8 + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR )
#define CRUISECONTROLSYSTEM_STACKSIZE             (   6 + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR )
#define FUELSENSOR_STACKSIZE                      (   5 + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR )
#define TRANSMISSIONCONTROL_STACKSIZE             (   4 + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR )
#define RADARTRACKER_STACKSIZE                    ( 329 + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR )
#define FOR_LOOP_50000_50000_STACKSIZE            (  20 + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR )
/*****************************************************************************
 * Task Priorities
 *****************************************************************************/
#define AIRBAGMODEL_PRIORITY                      1
#define CRUISECONTROLSYSTEM_PRIORITY              2
#define FUELSENSOR_PRIORITY                       6
#define TRANSMISSIONCONTROL_PRIORITY              7
#define RADARTRACKER_PRIORITY                     8
#define FOR_LOOP_50000_50000_PRIORITY             10
/*****************************************************************************
 * Task Table indices
 *****************************************************************************/
#define AIRBAGMODEL_TABLE_INDEX                   0
#define CRUISECONTROLSYSTEM_TABLE_INDEX           1
#define RADARTRACKER_TABLE_INDEX                  2
/*****************************************************************************
 * Task Periods
 *****************************************************************************/
#define NUM_TASKS                                 6
#define AIRBAGMODEL_PERIOD                        50
#define CRUISECONTROLSYSTEM_PERIOD                20
#define FUELSENSOR_PERIOD                         75
#define TRANSMISSIONCONTROL_PERIOD                15
#define RADARTRACKER_PERIOD                       300
#define FOR_LOOP_50000_50000_PERIOD               13
/*****************************************************************************
 * Runtime monitor table index
 *****************************************************************************/
#define AIRBAGMODEL_RT_PRIO                       0
#define CRUISECONTROLSYSTEM_RT_PRIO               1
#define FUELSENSOR_RT_PRIO                        2
#define TRANSMISSIONCONTROL_RT_PRIO               3
#define RADARTRACKER_RT_PRIO                      4
#define FOR_LOOP_50000_50000_RT_PRIO              5
