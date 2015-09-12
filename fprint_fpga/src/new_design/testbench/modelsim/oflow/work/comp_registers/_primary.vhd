library verilog;
use verilog.vl_types.all;
entity comp_registers is
    generic(
        idle            : integer := 0;
        st_set_pointers : integer := 1;
        st_ack_pointers : integer := 2;
        st_increment_hp_ack: integer := 3;
        st_reset_fprint_ready: integer := 4;
        st_reset_task   : integer := 5
    );
    port(
        clk             : in     vl_logic;
        reset           : in     vl_logic;
        head_tail_data  : in     vl_logic_vector(9 downto 0);
        head_tail_offset: in     vl_logic_vector(3 downto 0);
        set_head_tail   : in     vl_logic;
        head_tail_ack   : out    vl_logic;
        logical_core_id : in     vl_logic;
        fprint_task_id  : in     vl_logic_vector(3 downto 0);
        fprint_head_pointer: out    vl_logic_vector(9 downto 0);
        increment_head_pointer: in     vl_logic;
        increment_hp_ack: out    vl_logic;
        start_pointer_ex: in     vl_logic_vector(9 downto 0);
        end_pointer_ex  : in     vl_logic_vector(9 downto 0);
        start_pointer_comp: in     vl_logic_vector(9 downto 0);
        end_pointer_comp: in     vl_logic_vector(9 downto 0);
        comp_task       : in     vl_logic_vector(3 downto 0);
        fprints_ready_out: out    vl_logic_vector(15 downto 0);
        head0_matches_head1: out    vl_logic;
        tail0_matches_head0: out    vl_logic;
        tail1_matches_head1: out    vl_logic;
        comp_tail_pointer0: out    vl_logic_vector(9 downto 0);
        comp_tail_pointer1: out    vl_logic_vector(9 downto 0);
        comp_increment_tail_pointer: in     vl_logic;
        comp_reset_fprint_ready: in     vl_logic;
        reset_fprint_ack: out    vl_logic;
        comp_mismatch_detected: in     vl_logic;
        comp_reset_task : in     vl_logic;
        reset_task      : out    vl_logic
    );
    attribute mti_svvh_generic_type : integer;
    attribute mti_svvh_generic_type of idle : constant is 1;
    attribute mti_svvh_generic_type of st_set_pointers : constant is 1;
    attribute mti_svvh_generic_type of st_ack_pointers : constant is 1;
    attribute mti_svvh_generic_type of st_increment_hp_ack : constant is 1;
    attribute mti_svvh_generic_type of st_reset_fprint_ready : constant is 1;
    attribute mti_svvh_generic_type of st_reset_task : constant is 1;
end comp_registers;
