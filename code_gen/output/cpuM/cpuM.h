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
#define Derivative_STACKSIZE                      (  31 + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR )
#define dma_STACKSIZE                             (2048 + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR )
#define AirbagModel_STACKSIZE                     (  33 + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR )
#define CruiseControlSystem_STACKSIZE             (  35 + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR )
#define TractionControl_STACKSIZE                 (  30 + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR )
/*****************************************************************************
 * Task Priorities
 *****************************************************************************/
#define Derivative_PRIORITY                       3
#define dma_PRIORITY                              0
/*****************************************************************************
 * Task Table indices
 *****************************************************************************/
#define AIRBAGMODEL_TABLE_INDEX                   0
#define CRUISECONTROLSYSTEM_TABLE_INDEX           1
#define TRACTIONCONTROL_TABLE_INDEX               2
/*****************************************************************************
 * Task Periods
 *****************************************************************************/
#define AIRBAGMODEL_PERIOD                        50
#define CRUISECONTROLSYSTEM_PERIOD                20
#define TRACTIONCONTROL_PERIOD                    30
