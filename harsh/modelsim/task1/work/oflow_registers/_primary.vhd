library verilog;
use verilog.vl_types.all;
entity oflow_registers is
    generic(
        idle            : integer := 0;
        st_overflow_fifo_read: integer := 1;
        st_underflow_fifo_read: integer := 2;
        st_overflow_output: integer := 3;
        st_underflow_output: integer := 4;
        st_output_ready : integer := 5
    );
    port(
        clk             : in     vl_logic;
        reset           : in     vl_logic;
        oflow_write     : out    vl_logic;
        oflow_writedata : out    vl_logic_vector(31 downto 0);
        oflow_address   : out    vl_logic_vector(26 downto 0);
        oflow_waitrequest: in     vl_logic;
        csr_maxcount_write: in     vl_logic;
        csr_maxcount_writedata: in     vl_logic_vector(31 downto 0);
        checkin_in      : in     vl_logic_vector(15 downto 0);
        checkin_out     : out    vl_logic_vector(15 downto 0);
        count_inc       : in     vl_logic;
        count_dec       : in     vl_logic;
        count_inc_physical_core_id: in     vl_logic_vector(3 downto 0);
        count_inc_task_id: in     vl_logic_vector(3 downto 0);
        count_dec_task_id: in     vl_logic_vector(3 downto 0);
        count_inc_logical_core_id: in     vl_logic
    );
    attribute mti_svvh_generic_type : integer;
    attribute mti_svvh_generic_type of idle : constant is 1;
    attribute mti_svvh_generic_type of st_overflow_fifo_read : constant is 1;
    attribute mti_svvh_generic_type of st_underflow_fifo_read : constant is 1;
    attribute mti_svvh_generic_type of st_overflow_output : constant is 1;
    attribute mti_svvh_generic_type of st_underflow_output : constant is 1;
    attribute mti_svvh_generic_type of st_output_ready : constant is 1;
end oflow_registers;
