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
#define FOR_LOOP_50000_50000_STACKSIZE            (  20 + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR )
#define FOR_LOOP_100000_0_STACKSIZE               (  20 + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR )
/*****************************************************************************
 * Task Priorities
 *****************************************************************************/
#define FOR_LOOP_50000_50000_PRIORITY             1
#define FOR_LOOP_100000_0_PRIORITY                2
/*****************************************************************************
 * Task Table indices
 *****************************************************************************/
#define FOR_LOOP_100000_0_TABLE_INDEX             0
/*****************************************************************************
 * Task Periods
 *****************************************************************************/
#define NUM_TASKS                                 2
#define FOR_LOOP_50000_50000_PERIOD               30
#define FOR_LOOP_100000_0_PERIOD                  30
/*****************************************************************************
 * Runtime monitor table index
 *****************************************************************************/
#define FOR_LOOP_50000_50000_RT_PRIO              0
#define FOR_LOOP_100000_0_RT_PRIO                 1
/*****************************************************************************
 * Execution time lower bounds
 *****************************************************************************/
#define FOR_LOOP_50000_50000_WCET_LOWERBOUND      1600035
#define FOR_LOOP_100000_0_WCET_LOWERBOUND         1600004
