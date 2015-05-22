

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
  float x[200] = {
		  0.740620124829,
		  -0.844965821923,
		  0.140630161833,
		  -0.89195182781,
		  0.69402834277,
		  -0.110086186349,
		  0.122416199272,
		  -0.656018264863,
		  0.558900717278,
		  -0.32028577766,
		  0.00555140624874,
		  -0.793876817015,
		  0.4598235143,
		  -0.137023717233,
		  0.688880151514,
		  -0.978041408264,
		  0.0491062663682,
		  -0.957298938125,
		  0.0617210012795,
		  -0.665820608819,
		  0.99528613461,
		  -0.745458392912,
		  0.98231387873,
		  -0.0325862530567,
		  0.64375326095,
		  -0.853890438317,
		  0.0184603015405,
		  -0.966589613111,
		  0.332655964715,
		  -0.738002043631,
		  0.00145441008224,
		  -0.875614050181,
		  0.0509100917898,
		  -0.931982710006,
		  0.858171168829,
		  -0.472461355321,
		  0.149602054349,
		  -0.409101527851,
		  0.371962286039,
		  -0.464465798763,
		  0.150194819396,
		  -0.166436564991,
		  0.924732480945,
		  -0.157744953899,
		  0.807072122343,
		  -0.864898666976,
		  0.0476996297983,
		  -0.774768081312,
		  0.608105405728,
		  -0.319805940453,
		  0.0575685581655,
		  -0.460656113891,
		  0.63947426822,
		  -0.769601642166,
		  0.572975675287,
		  -0.18423012249,
		  0.440994756474,
		  -0.301137564329,
		  0.942189388762,
		  -0.863443267505,
		  0.312001480777,
		  -0.542299403591,
		  0.921426915929,
		  -0.94736251515,
		  0.584158799548,
		  -0.659832143004,
		  0.309783865036,
		  -0.764935446958,
		  0.290632250222,
		  -0.653219472333,
		  0.0618993337397,
		  -0.999830270235,
		  0.0211657989974,
		  -0.176734347152,
		  0.284425865661,
		  -0.437657918141,
		  0.00246511572891,
		  -0.6203975473,
		  0.453444228475,
		  -0.286174494805,
		  0.654086186027,
		  -0.630585021822,
		  0.802055238949,
		  -0.551358761942,
		  0.402495278587,
		  -0.879024604318,
		  0.627724901426,
		  -0.0953898746429,
		  0.502535782095,
		  -0.16604689132,
		  0.994274975111,
		  -0.878536688921,
		  0.391665154287,
		  -0.746271405628,
		  0.835944079199,
		  -0.592161592798,
		  0.0132761457264,
		  -0.951476090241,
		  0.550342402801,
		  -0.746533148336,
		  0.779007975113,
		  -0.928151744683,
		  0.607018971364,
		  -0.122004323488,
		  0.498691794975,
		  -0.789693999042,
		  0.508213617283,
		  -0.0507560098642,
		  0.563405562104,
		  -0.0234390895201,
		  0.642053540862,
		  -0.581937384568,
		  0.522010315401,
		  -0.641345405877,
		  0.0288685390256,
		  -0.919356930266,
		  0.578999802369,
		  -0.725682998641,
		  0.236635596681,
		  -0.733152412033,
		  0.765162470615,
		  -0.0916238602856,
		  0.0177109926321,
		  -0.0886705814149,
		  0.232629715452,
		  -0.412734746781,
		  0.0257341835919,
		  -0.0129173091264,
		  0.0174998974046,
		  -0.176745651342,
		  0.424485130578,
		  -0.627258801052,
		  0.0639516115994,
		  -0.799096911335,
		  0.875992897104,
		  -0.323045732081,
		  0.820188225606,
		  -0.407465975985,
		  0.188094289241,
		  -0.0290131163495,
		  0.560483033269,
		  -0.947235524475,
		  0.887873428452,
		  -0.689352332164,
		  0.194937650158,
		  -0.454042889291,
		  0.558182468694,
		  -0.409834008628,
		  0.816755913709,
		  -0.58692523902,
		  0.595750142606,
		  -0.939138485611,
		  0.493351333804,
		  -0.350048498578,
		  0.573836878671,
		  -0.758978520633,
		  0.47435537246,
		  -0.62019549755,
		  0.485258687011,
		  -0.521336704345,
		  0.876512463799,
		  -0.243434676425,
		  0.456238080538,
		  -0.266081892542,
		  0.502689435933,
		  -0.258249719872,
		  0.736487915221,
		  -0.17188997145,
		  0.181864117511,
		  -0.727208212299,
		  0.265431566051,
		  -0.663008087161,
		  0.978303042201,
		  -0.256379487847,
		  0.777978501418,
		  -0.157510860594,
		  0.552163543189,
		  -0.152825924609,
		  0.996948611312,
		  -0.884438256472,
		  0.316839211041,
		  -0.544396064829,
		  0.917994773218,
		  -0.655487580684,
		  0.603772217865,
		  -0.305480978123,
		  0.518477259921,
		  -0.132049886133,
		  0.960902196848,
		  -0.883288387553,
		  0.635281131394,
		  -0.288668018406,
		  0.987583312147,
		  -0.59802755956,
		  0.0980490342064,
		  -0.0998995757235,
		  0.409457578992,
		  -0.413190722057,
		  0.214678212138,
		  -0.596772220823,

  };

  for(i = 0; i < 1000; i++){
	  float result;
	  int index1 = rand() % 200;
	  int index2 = rand() % 200;
	  result = -x[index1] * x[index2];
	  printf("-%f * %f = %f\n",x[index1],x[index2],result);
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
