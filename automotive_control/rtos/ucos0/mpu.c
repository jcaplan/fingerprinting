#include "mpu_utils.h"
#include "sys/alt_stdio.h"
#include "sys/alt_exceptions.h"
#include "priv/alt_exception_handler_registry.h"
#include "io.h"
#include "unistd.h"
#include "stdlib.h"
#include "stddef.h"
#include "string.h"

#define DATA_REGION 1
#define INST_REGION 0


void nios2_mpu_data_init() {
	Nios2MPURegion region[NIOS2_MPU_NUM_DATA_REGIONS];

	//1kB region size

	region[0].index = 0x0;
	region[0].base = 0x0;
	region[0].mask = 0x0;
	region[0].c = 0;
	region[0].perm = MPU_DATA_PERM_SUPER_NONE_USER_NONE;
	unsigned int num_of_region = NIOS2_MPU_NUM_DATA_REGIONS;
	nios2_mpu_load_region(region, num_of_region, DATA_REGION);
}

void nios2_mpu_inst_init() {
	unsigned int mask = 0;
	Nios2MPURegion region[NIOS2_MPU_NUM_INST_REGIONS];

	unsigned int num_of_region = NIOS2_MPU_NUM_INST_REGIONS;
	unsigned int index;
	for (index = 0; index < num_of_region; index++) {
		region[index].base = 0x0;
		region[index].index = index;
		region[index].mask = mask;
		region[index].c = 0;
		region[index].perm = MPU_INST_PERM_SUPER_NONE_USER_NONE; //No access for user and supervisor
	}

	nios2_mpu_load_region(region, num_of_region, INST_REGION);
}

/* Tests the MPU by configuring it and running a simple hello_world program. */


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
  memcpy( buf, 0x10000, 1024 );

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
