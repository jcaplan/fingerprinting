

#define DATA_REGION 1
#define INST_REGION 0

/* Tests the MPU by configuring it and running a simple hello_world program. */

#include "includes.h"
#include "sys/alt_stdio.h"
#include "sys/alt_exceptions.h"
#include "priv/alt_exception_handler_registry.h"
#include "io.h"
#include "unistd.h"
#include "stdlib.h"
#include "stddef.h"
#include "string.h"
#include "float.h"
#include "mpu_utils.h"

void nios2_mpu_data_init()
{
        Nios2MPURegion region[NIOS2_MPU_NUM_DATA_REGIONS];
        //jtag_uart.
        region[0].index = 0x0;
        region[0].base = 0x0; // Byte Address 0x100
        region[0].mask = 0x100;
        region[0].c = 0;
        region[0].perm = MPU_DATA_PERM_SUPER_NONE_USER_NONE;

        region[1].index = 0x1;
        region[1].base = 0x1000; // Byte Address 0x100
        region[1].mask = 0x1001;
        region[1].c = 0;
        region[1].perm = MPU_DATA_PERM_SUPER_NONE_USER_NONE;

        region[2].index = 0x2;
		region[2].base = 0x1040; // Byte Address 0x100
		region[2].mask = 0x1180;
		region[2].c = 0;
		region[2].perm = MPU_DATA_PERM_SUPER_NONE_USER_NONE;


        unsigned int num_of_region = NIOS2_MPU_NUM_DATA_REGIONS;

        int index;
    	for (index = 3; index < num_of_region; index++) {
    			region[index].base = 0x0;
    			region[index].index = index;
    			region[index].mask = 0x2000000;
    			region[index].c = 0;
    			region[index].perm = MPU_DATA_PERM_SUPER_RW_USER_RW; //No access for user and supervisor
    		}

        nios2_mpu_load_region(region, num_of_region, DATA_REGION);
}

void nios2_mpu_inst_init()
{
        unsigned int mask;
        Nios2MPURegion region[NIOS2_MPU_NUM_INST_REGIONS];

        //Main instruction region.
        region[0].index = 0;
        region[0].base = 0x400; // Byte Address 0x10000

        region[0].mask = 0x500; // Byte Address 0x14000

        region[0].c = 1;
        region[0].perm = MPU_INST_PERM_SUPER_EXEC_USER_EXEC;
        //Instruction region for break address.

        mask = 0x2000000;

        unsigned int num_of_region = NIOS2_MPU_NUM_INST_REGIONS;
        unsigned int index;
        for (index = 1; index < num_of_region; index++){
                region[index].base = 0x0;
                region[index].index = index;
                region[index].mask = mask;
                region[index].c = 0;
                region[index].perm = MPU_INST_PERM_SUPER_EXEC_USER_EXEC; //No access for user and supervisor
        }

        nios2_mpu_load_region(region, num_of_region, INST_REGION);
}

extern void exception_exit();

// End of transmission character... terminates terminal session.
#define EOT 0x4

// Helper functions to determine success/failure.
void fail( void )
{
  alt_printf("FAIL\n");
  exit(1);
}

void assert_equals( int expected, int actual )
{
  if(actual != expected) {
    fail();
  }
}

/* Global data structure to hold exception information. */
typedef struct {
  int exceptions_serviced_with_handler;
  int mpu_data_exception_count;
  int mpu_inst_exception_count;
  int reissue_count;
  alt_u32 last_valid_badaddr;
  alt_u32 last_exception_pc;
  alt_exception_cause last_exception_cause;
  alt_exception_result desired_exception_result;
} exception_struct;

exception_struct g_exc_info;

alt_exception_result exc_handler(
    alt_exception_cause cause,
    alt_u32 exception_pc,
    alt_u32 badaddr)
{
  exception_struct *exc_info = &g_exc_info;
  exc_info->exceptions_serviced_with_handler++;
  exc_info->last_exception_cause = cause;

  exc_info->last_exception_pc = exception_pc;
  if (exc_info->last_exception_cause == NIOS2_EXCEPTION_MPU_DATA_REGION_VIOLATION) {
    exc_info->mpu_data_exception_count++;
  }
  if (exc_info->last_exception_cause == NIOS2_EXCEPTION_MPU_INST_REGION_VIOLATION) {
    exc_info->mpu_data_exception_count++;
  }


  if( alt_exception_cause_generated_bad_addr(cause) ) {
    exc_info->last_valid_badaddr = badaddr;
  }

  if( exc_info->reissue_count ) {
    exc_info->reissue_count--;
    return NIOS2_EXCEPTION_RETURN_REISSUE_INST;
  }
  else {
    return exc_info->desired_exception_result;
  }
}

void set_exception_defaults(exception_struct *exc_info)
{
  exc_info->exceptions_serviced_with_handler = 0;
  exc_info->mpu_data_exception_count         = 0;
  exc_info->mpu_data_exception_count         = 0;
  exc_info->reissue_count                    = 0;
  exc_info->last_exception_pc                = -1;
  exc_info->last_exception_cause             = -1;
  exc_info->last_valid_badaddr               = -1;
  exc_info->desired_exception_result         = NIOS2_EXCEPTION_RETURN_SKIP_INST;
}


void test_null_data( exception_struct *exc_info )
{
  alt_printf( "MPU NULL data pointer test.\n" );

  set_exception_defaults( exc_info );

  // Issue an instruction that should cause a NULL pointer access.
  IORD_8DIRECT( 0x0, 0 );

  // Assert that all exception structure values are correct.
  assert_equals( 1, exc_info->exceptions_serviced_with_handler );
  assert_equals( 1, exc_info->mpu_data_exception_count );
  assert_equals( NIOS2_EXCEPTION_MPU_DATA_REGION_VIOLATION, exc_info->last_exception_cause );

  alt_printf( "MPU NULL data pointer test passed!\n" );
}

void test_wild_pointer( exception_struct *exc_info )
{
  alt_printf( "MPU wild pointer test.\n" );

  set_exception_defaults( exc_info );

  // Issue an instruction that should cause a wild pointer access.
  IORD_8DIRECT( 0x40000, 0 );

  // Assert that all exception structure values are correct.
  assert_equals( 1, exc_info->exceptions_serviced_with_handler );
  assert_equals( 1, exc_info->mpu_data_exception_count );
  assert_equals( NIOS2_EXCEPTION_MPU_DATA_REGION_VIOLATION, exc_info->last_exception_cause );

  alt_printf( "MPU wild pointer test passed!\n" );
}

void recursive_function( count )
{
  char buf[1024];

  count++;
  memcpy( buf, 0x41000, 1024 );

  if( count <= 4 )
  {
    recursive_function( count );
  }
  else
  {
    return;
  }
}

void test_stack_overflow( exception_struct *exc_info )
{
  int count = 1;
  alt_printf( "MPU stack overflow test.\n" );

  // Reset exception structure default value.
  set_exception_defaults( exc_info );

  // Use enough memory to trigger a stack overflow.  In this case 4KB.
  recursive_function( count );


  // 64 exceptions should have been triggered.  The type should be mpu data exception.
  assert_equals(64, exc_info->mpu_data_exception_count);
  assert_equals(64, exc_info->mpu_data_exception_count);
  assert_equals( NIOS2_EXCEPTION_MPU_DATA_REGION_VIOLATION, exc_info->last_exception_cause );

  alt_printf( "MPU stack overflow test passed!\n" );
}

int main()
{

  int i;
  float x[100] = {4914.16277977,
		  831.558654785,
		  294.667090774,
		  4957.57204653,
		  1462.43452335,
		  4321.53049872,
		  1410.36859861,
		  6119.17282683,
		  391.125249506,
		  9001.80343695,
		  11.0041740395,
		  1379.61787365,
		  3092.47421687,
		  7997.17562766,
		  6871.01325567,
		  6983.44128489,
		  6718.03987971,
		  9702.97329191,
		  5825.070839,
		  1999.15338958,
		  3421.01553777,
		  7174.63892997,
		  3661.36402774,
		  8693.73520555,
		  205.788022935,
		  807.322054262,
		  9237.27474007,
		  819.195091584,
		  6001.20222982,
		  3890.0647095,
		  3037.068729,
		  8536.08944963,
		  6950.62670784,
		  3112.09449627,
		  6505.99446439,
		  2022.96108493,
		  3809.90090795,
		  266.136282575,
		  2740.93085022,
		  836.994399464,
		  8022.53736602,
		  5318.16055486,
		  3183.17087273,
		  3062.8915988,
		  6765.06957982,
		  2818.82139954,
		  1836.23706946,
		  9514.33077741,
		  5350.45535848,
		  8334.34500319,
		  6342.55347201,
		  5057.71930233,
		  869.262471201,
		  3085.85153083,
		  9808.71418109,
		  907.422824965,
		  6628.83530264,
		  2135.62888015,
		  8069.11074642,
		  6917.84351081,
		  7070.71677709,
		  4982.89753073,
		  1006.58559334,
		  7522.19074506,
		  4748.37038115,
		  9217.38918036,
		  4632.94018862,
		  4533.04494479,
		  7221.47254669,
		  870.89918492,
		  5693.17725443,
		  2538.87994939,
		  998.946572859,
		  6385.53278144,
		  3560.85732372,
		  9675.94169261,
		  2720.37021109,
		  9736.49135453,
		  2083.92059638,
		  4747.55295005,
		  835.811230186,
		  917.995411685,
		  7772.50407751,
		  5948.06504986,
		  4506.96542092,
		  5597.10712361,
		  9847.92276045,
		  9412.21216332,
		  6229.95536741,
		  8237.61196106,
		  7612.56442998,
		  4175.55172421,
		  7336.0819025,
		  687.482721043,
		  8736.32641405,
		  7111.50148095,
		  1732.38182036,
		  2835.45409241,
		  5577.1487586,
		  7676.36616131
  };

  for(i = 0; i < 1000; i++){
	  float result;
	  int index1 = rand() % 100;
	  int index2 = rand() % 100;
	  result = -x[index1] * x[index2];
  }


  exception_struct *exc_info = &g_exc_info;
  // Register exception handler.
  //    - This will manage everything EXCEPT interrupts and the unimplemented instruction exception.
  alt_instruction_exception_register(&exc_handler);
  // Initialize and start the MPU.
  nios2_mpu_data_init();
  nios2_mpu_inst_init();
  nios2_mpu_enable();

  alt_putstr( "Hello from a simple MPU-Enabled Nios II System!.\n" );
  alt_putstr( "\tStarting some exceptions tests.\n" );
  alt_putstr( "=====\n" );

  // Test for NULL data pointer protection.
  test_null_data( exc_info );
  // Test for wild data pointer protection.
  test_wild_pointer( exc_info );
  // Test for stack overflow...simulated.
  test_stack_overflow( exc_info );
  alt_putstr( "=====\n" );
  alt_putstr( "\tException Tests ended.\n" );
  alt_printf( "Now exiting program.\n%c", EOT );
}
