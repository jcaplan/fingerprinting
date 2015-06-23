`ifndef FINGERPRINTING_ENABLED
	`define FINGERPRINTING_ENABLED
`endif


`define NIOS_ADDRESS_WIDTH						27									//Nios Address width
`define NIOS_DATA_WIDTH							32
`define CRC_WIDTH								32									//Polynomial width
`define CRC_DATA_WIDTH							(`NIOS_ADDRESS_WIDTH + `CRC_WIDTH)  //Write address and write data

`define CRC_RAM_ADDRESS_WIDTH					10									//Width of CRC buffer in comparator
`define CRC_RAM_SIZE							(1 << (`CRC_RAM_ADDRESS_WIDTH-1))	//Size of CRC buffer in comparator (= 2^CRC_RAM_ADDRESS_WIDTH)






`define CRC_OUTPUT_BUFFER_SIZE					6
`define CRC_GP 									'h1da97
`define CRC_RST_VALUE      						1'b1
`define CRC_RST_EVENT      						posedge


`define CRC_KEY_WIDTH							4
`define CRC_KEY_SIZE							(1 << (`CRC_KEY_WIDTH))

`define CRC_DIRECTORY_START_BIT					4
`define CRC_DIRECTORY_END_BIT					5




//Comparator from FPRINT
`define COMPARATOR_ADDRESS_WIDTH				10									//Width of address for comparator slave port(s)
`define COMPARATOR_EXCEPTION_REG_WIDTH			16

`define COMPARATOR_CS_OFFSET					0
`define COMPARATOR_CRC_OFFSET					1
`define COMPARATOR_PAUSE_OFFSET					2
`define COMPARATOR_UNPAUSE_OFFSET				3
`define COMPARATOR_COREID_BITS					7:4

//FROM CPU

//Start, end, and exception register
//and
`define COMP_CSR_WIDTH							10
`define COMP_CSR_COREID_BITS					9:6
`define DIRECTORY_BITS							5:4
`define CRC_DIRECTORY_START_BASE				'h10
`define CRC_DIRECTORY_END_BASE					'h20
`define COMPARATOR_EXCEPTION_OFFSET 			'h30
`define COMPARATOR_SUCCESS_REG_OFFSET           'h31
`define COMPARATOR_FAIL_REG_OFFSET              'h32
`define COMPARATOR_MAXCOUNT_REG_OFFSET			'h33
`define COMPARATOR_OFLOW_REG_OFFSET      	 	'h35
`define COMPARATOR_CORE_ASSIGNMENT_OFFSET       'h36

`define EXCEPTION_OVERFLOW_BIT					4
`define EXCEPTION_REG_EX_BIT					5
`define EXCEPTION_REG_INTERRUPT_BIT				6
`define EXCEPTION_REG_TASK_BITS					(`CRC_KEY_WIDTH)-1:0
`define EXCEPTION_CPUID_BIT						7



`define COMPARATOR_CRC_OFFSET_4x				(`COMPARATOR_CRC_OFFSET << 2)
`define COMPARATOR_PAUSE_OFFSET_4x				(`COMPARATOR_PAUSE_OFFSET << 2)
`define COMPARATOR_UNPAUSE_OFFSET_4x			(`COMPARATOR_UNPAUSE_OFFSET << 2)

`define COUNTER_MAX_DEFAULT 					'hfff
`define COUNTER_WIDTH							16

`define PAUSE_DEPTH 							8


`define FPRINT_SPR_ADDRESS_WIDTH				8
`define FPRINT_SPR_BASE_ADDRESS					'h01100000
`define FPRINT_SPR_CURRENTSTATE 				'h0	
`define FPRINT_PAUSE_TASK_REG					'h4
`define PAUSE_STROBE_OFFSET						'h8
`define UNPAUSE_STROBE_OFFSET					'hc
`define COUNTER_MAX_OFFSET						'h10

`define STORE_BUFFER_WIDTH						3


//`define SIM

`define COMPARATOR_BASE_ADDRESS					27'h1000000


`define	COMPARATOR_CURRENT_STATE				(0 + `COMPARATOR_BASE_ADDRESS)
`define	COMPARATOR_CRC_BUFFER					(4 + `COMPARATOR_BASE_ADDRESS)

`define FPRINT_CS_KEY							`CRC_KEY_WIDTH-1:0
`define FPRINT_CS_EN							`CRC_KEY_WIDTH	

`define CRC_DIRECTORY_WIDTH						`CRC_RAM_ADDRESS_WIDTH
`define CRC_DIRECTORY_SIZE						(`CRC_KEY_SIZE)*4	//16 keys * 4 - start end head and tail pointer

`define NUM_CORES								4
`define MAX_REDUNDANCY							2

`define OFLOW_OFFSET					27'h6000000
