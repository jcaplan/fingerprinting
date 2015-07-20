library verilog;
use verilog.vl_types.all;
entity csr_registers is
    generic(
        idle            : integer := 0;
        st_csr_write    : integer := 1;
        st_csr_read     : integer := 2;
        st_waitrequest  : integer := 3;
        st_comp_write   : integer := 4;
        st_comp_ack     : integer := 5;
        st_set_head_tail: integer := 6
    );
    port(
        clk             : in     vl_logic;
        reset           : in     vl_logic;
        csr_address     : in     vl_logic_vector(9 downto 0);
        csr_read        : in     vl_logic;
        csr_readdata    : out    vl_logic_vector(31 downto 0);
        csr_write       : in     vl_logic;
        csr_writedata   : in     vl_logic_vector(31 downto 0);
        csr_waitrequest : out    vl_logic;
        comp_status_write: in     vl_logic;
        comp_status_ack : out    vl_logic;
        comp_task       : in     vl_logic_vector(3 downto 0);
        comp_collision_detected: in     vl_logic;
        physical_core_id: in     vl_logic_vector(3 downto 0);
        fprint_task_id  : in     vl_logic_vector(3 downto 0);
        logical_core_id : out    vl_logic;
        csr_maxcount_write: out    vl_logic;
        csr_maxcount_writedata: out    vl_logic_vector(31 downto 0);
        head_tail_data  : out    vl_logic_vector(9 downto 0);
        head_tail_offset: out    vl_logic_vector(3 downto 0);
        set_head_tail   : out    vl_logic;
        head_tail_ack   : in     vl_logic;
        start_pointer_ex: out    vl_logic_vector(9 downto 0);
        end_pointer_ex  : out    vl_logic_vector(9 downto 0);
        start_pointer_comp: out    vl_logic_vector(9 downto 0);
        end_pointer_comp: out    vl_logic_vector(9 downto 0);
        irq             : out    vl_logic
    );
    attribute mti_svvh_generic_type : integer;
    attribute mti_svvh_generic_type of idle : constant is 1;
    attribute mti_svvh_generic_type of st_csr_write : constant is 1;
    attribute mti_svvh_generic_type of st_csr_read : constant is 1;
    attribute mti_svvh_generic_type of st_waitrequest : constant is 1;
    attribute mti_svvh_generic_type of st_comp_write : constant is 1;
    attribute mti_svvh_generic_type of st_comp_ack : constant is 1;
    attribute mti_svvh_generic_type of st_set_head_tail : constant is 1;
end csr_registers;
