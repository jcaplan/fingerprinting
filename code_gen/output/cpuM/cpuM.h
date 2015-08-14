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
#define DERIVATIVE_STACKSIZE                      (  20 + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR )
#define TRACTIONCONTROL_STACKSIZE                 (  19 + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR )
#define DMA_STACKSIZE                             (1024 + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR )
#define AIRBAGMODEL_STACKSIZE                     (   8 + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR )
#define CRUISECONTROLSYSTEM_STACKSIZE             (   6 + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR )
#define RADARTRACKER_STACKSIZE                    ( 329 + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR )
/*****************************************************************************
 * Task Priorities
 *****************************************************************************/
#define DERIVATIVE_PRIORITY                       3
#define TRACTIONCONTROL_PRIORITY                  4
#define DMA_PRIORITY                              0
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
#define DERIVATIVE_PERIOD                         40
#define TRACTIONCONTROL_PERIOD                    30
#define DMA_PERIOD                                0
#define AIRBAGMODEL_PERIOD                        50
#define CRUISECONTROLSYSTEM_PERIOD                20
#define RADARTRACKER_PERIOD                       300
/*****************************************************************************
 * Runtime monitor table index
 *****************************************************************************/
#define DERIVATIVE_RT_PRIO                        0
#define TRACTIONCONTROL_RT_PRIO                   1
#define DMA_RT_PRIO                               2
#define AIRBAGMODEL_RT_PRIO                       3
#define CRUISECONTROLSYSTEM_RT_PRIO               4
#define RADARTRACKER_RT_PRIO                      5
