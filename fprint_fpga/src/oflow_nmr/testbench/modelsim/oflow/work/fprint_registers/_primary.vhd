library verilog;
use verilog.vl_types.all;
entity fprint_registers is
    generic(
        idle            : integer := 0;
        st_waitrequest  : integer := 1;
        st_read_fifo    : integer := 1;
        st_decode       : integer := 2;
        st_reg_store    : integer := 3;
        st_crc_store_0  : integer := 4;
        st_crc_store_1  : integer := 5;
        st_increment_head_pointer: integer := 6;
        st_task_verified: integer := 7
    );
    port(
        clk             : in     vl_logic;
        reset           : in     vl_logic;
        fprint_address  : in     vl_logic_vector(9 downto 0);
        fprint_write    : in     vl_logic;
        fprint_writedata: in     vl_logic_vector(31 downto 0);
        fprint_waitrequest: out    vl_logic;
        physical_core_id: out    vl_logic_vector(3 downto 0);
        fprint_task_id  : out    vl_logic_vector(3 downto 0);
        logical_core_id : in     vl_logic;
        fprint_head_pointer: in     vl_logic_vector(9 downto 0);
        increment_head_pointer: out    vl_logic;
        increment_hp_ack: in     vl_logic;
        checkin_reg_out : out    vl_logic_vector(15 downto 0);
        comp_tail_pointer0: in     vl_logic_vector(9 downto 0);
        comp_tail_pointer1: in     vl_logic_vector(9 downto 0);
        fprint0         : out    vl_logic_vector(31 downto 0);
        fprint1         : out    vl_logic_vector(31 downto 0);
        comp_task_verified: in     vl_logic;
        fprint_reg_ack  : out    vl_logic;
        comp_task       : in     vl_logic_vector(3 downto 0)
    );
    attribute mti_svvh_generic_type : integer;
    attribute mti_svvh_generic_type of idle : constant is 1;
    attribute mti_svvh_generic_type of st_waitrequest : constant is 1;
    attribute mti_svvh_generic_type of st_read_fifo : constant is 1;
    attribute mti_svvh_generic_type of st_decode : constant is 1;
    attribute mti_svvh_generic_type of st_reg_store : constant is 1;
    attribute mti_svvh_generic_type of st_crc_store_0 : constant is 1;
    attribute mti_svvh_generic_type of st_crc_store_1 : constant is 1;
    attribute mti_svvh_generic_type of st_increment_head_pointer : constant is 1;
    attribute mti_svvh_generic_type of st_task_verified : constant is 1;
end fprint_registers;
