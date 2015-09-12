library verilog;
use verilog.vl_types.all;
entity fprint_registers is
    generic(
        idle            : integer := 0;
        st_fifo_wr_en   : integer := 1;
        st_waitrequest  : integer := 2;
        st_comparator_regs_write: integer := 1;
        st_fprint_reset_task_ack: integer := 2;
        st_fifo_rd_en   : integer := 3;
        st_decode       : integer := 4;
        st_fprint_regs_write: integer := 5;
        st_fprint_mem_write: integer := 6;
        st_fprint_inc_head_pointer: integer := 7;
        st_fprint_count_inc: integer := 8
    );
    port(
        clk             : in     vl_logic;
        reset           : in     vl_logic;
        fprint_address  : in     vl_logic_vector(9 downto 0);
        fprint_write    : in     vl_logic;
        fprint_writedata: in     vl_logic_vector(31 downto 0);
        fprint_waitrequest: out    vl_logic;
        fprint_task_id  : out    vl_logic_vector(3 downto 0);
        fprint_physical_core_id: out    vl_logic_vector(3 downto 0);
        fprint_logical_core_id: in     vl_logic_vector(1 downto 0);
        fprint_nmr      : in     vl_logic_vector(15 downto 0);
        fprint_count_inc: out    vl_logic;
        oflow_count_inc_ack: in     vl_logic;
        fprint_inc_head_pointer: out    vl_logic;
        comp_inc_head_pointer_ack: in     vl_logic;
        fprint_head_pointer: in     vl_logic_vector(9 downto 0);
        comp_tail_pointer_0: in     vl_logic_vector(9 downto 0);
        comp_tail_pointer_1: in     vl_logic_vector(9 downto 0);
        comp_tail_pointer_2: in     vl_logic_vector(9 downto 0);
        fprint_0        : out    vl_logic_vector(31 downto 0);
        fprint_1        : out    vl_logic_vector(31 downto 0);
        fprint_2        : out    vl_logic_vector(31 downto 0);
        comparator_task_id: in     vl_logic_vector(3 downto 0);
        fprint_reset_task: in     vl_logic;
        fprint_reset_task_ack: out    vl_logic;
        fprint_checkin  : out    vl_logic_vector(15 downto 0)
    );
    attribute mti_svvh_generic_type : integer;
    attribute mti_svvh_generic_type of idle : constant is 1;
    attribute mti_svvh_generic_type of st_fifo_wr_en : constant is 1;
    attribute mti_svvh_generic_type of st_waitrequest : constant is 1;
    attribute mti_svvh_generic_type of st_comparator_regs_write : constant is 1;
    attribute mti_svvh_generic_type of st_fprint_reset_task_ack : constant is 1;
    attribute mti_svvh_generic_type of st_fifo_rd_en : constant is 1;
    attribute mti_svvh_generic_type of st_decode : constant is 1;
    attribute mti_svvh_generic_type of st_fprint_regs_write : constant is 1;
    attribute mti_svvh_generic_type of st_fprint_mem_write : constant is 1;
    attribute mti_svvh_generic_type of st_fprint_inc_head_pointer : constant is 1;
    attribute mti_svvh_generic_type of st_fprint_count_inc : constant is 1;
end fprint_registers;
