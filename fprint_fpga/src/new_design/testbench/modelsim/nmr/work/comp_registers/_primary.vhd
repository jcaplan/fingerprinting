library verilog;
use verilog.vl_types.all;
entity comp_registers is
    generic(
        idle            : integer := 0;
        st_pointer_start_write: integer := 1;
        st_pointer_end_write: integer := 2;
        st_comp_pointer_ack: integer := 3;
        st_inc_head_pointer: integer := 4;
        st_comp_inc_head_pointer_ack: integer := 5;
        st_inc_tail_pointer: integer := 6;
        st_comp_inc_tail_pointer_ack: integer := 7;
        st_reset_task   : integer := 8;
        st_comp_reset_task_ack: integer := 9
    );
    port(
        clk             : in     vl_logic;
        reset           : in     vl_logic;
        csr_logical_core_id: in     vl_logic_vector(1 downto 0);
        csr_task_id     : in     vl_logic_vector(3 downto 0);
        csr_pointer_start_write: in     vl_logic;
        csr_pointer_end_write: in     vl_logic;
        csr_pointer_data: in     vl_logic_vector(9 downto 0);
        comp_pointer_ack: out    vl_logic;
        fprint_logical_core_id: in     vl_logic_vector(1 downto 0);
        fprint_task_id  : in     vl_logic_vector(3 downto 0);
        fprint_inc_head_pointer: in     vl_logic;
        comp_inc_head_pointer_ack: out    vl_logic;
        fprint_head_pointer: out    vl_logic_vector(9 downto 0);
        comp_tail_pointer_0: out    vl_logic_vector(9 downto 0);
        comp_tail_pointer_1: out    vl_logic_vector(9 downto 0);
        comp_tail_pointer_2: out    vl_logic_vector(9 downto 0);
        comparator_task_id: in     vl_logic_vector(3 downto 0);
        comparator_inc_tail_pointer: in     vl_logic;
        comp_inc_tail_pointer_ack: out    vl_logic;
        comp_reset_task : in     vl_logic;
        comp_reset_task_ack: out    vl_logic
    );
    attribute mti_svvh_generic_type : integer;
    attribute mti_svvh_generic_type of idle : constant is 1;
    attribute mti_svvh_generic_type of st_pointer_start_write : constant is 1;
    attribute mti_svvh_generic_type of st_pointer_end_write : constant is 1;
    attribute mti_svvh_generic_type of st_comp_pointer_ack : constant is 1;
    attribute mti_svvh_generic_type of st_inc_head_pointer : constant is 1;
    attribute mti_svvh_generic_type of st_comp_inc_head_pointer_ack : constant is 1;
    attribute mti_svvh_generic_type of st_inc_tail_pointer : constant is 1;
    attribute mti_svvh_generic_type of st_comp_inc_tail_pointer_ack : constant is 1;
    attribute mti_svvh_generic_type of st_reset_task : constant is 1;
    attribute mti_svvh_generic_type of st_comp_reset_task_ack : constant is 1;
end comp_registers;
