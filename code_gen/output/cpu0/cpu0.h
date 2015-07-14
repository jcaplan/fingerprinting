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
#define STACKSIZE_MINOFFSET                       314
#define STACKSIZE_MARGINERROR                     20
#define AirbagModel_STACKSIZE                     ( 132 + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR )
#define Derivative_STACKSIZE                      ( 124 + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR )
#define CollisionAvoidance_STACKSIZE              ( 136 + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR )
#define TransmissionControl_STACKSIZE             (  24 + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR )
/*****************************************************************************
 * Task Priorities
 *****************************************************************************/
#define AirbagModel_PRIORITY                      1
#define Derivative_PRIORITY                       3
#define CollisionAvoidance_PRIORITY               5
#define TransmissionControl_PRIORITY              7
