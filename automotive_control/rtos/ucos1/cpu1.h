
/*****************************************************************************
 * Defines
 *****************************************************************************/
#define CORE_ID 							(ALT_CPU_CPU_ID_VALUE - 1)


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
#define STACKSIZE_MINOFFSET   				314
#define STACKSIZE_MARGINERROR 				50
#define FuelSensor_STACKSIZE 				(156 + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR)
#define Derivative_AirbagModel_STACKSIZE	(156 + STACKSIZE_MINOFFSET + STACKSIZE_MARGINERROR)



/*****************************************************************************
 * Task Priorities
 *****************************************************************************/
#define FuelSensor_PRIORITY 				13
#define Derivative_AirbagModel_PRIORITY     12
