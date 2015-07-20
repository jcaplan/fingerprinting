library verilog;
use verilog.vl_types.all;
entity comparator is
    generic(
        init            : integer := 0;
        set_task        : integer := 1;
        load_pointer    : integer := 2;
        load_fprint     : integer := 3;
        check_task_status: integer := 4;
        task_complete   : integer := 5;
        compare_fprints : integer := 6;
        mismatch_detected: integer := 7;
        task_verified   : integer := 8;
        increment_tail_pointer: integer := 9;
        check_if_done   : integer := 10;
        reset_fprint_ready: integer := 11;
        st_reset_task   : integer := 12;
        write_status_reg: integer := 13
    );
    port(
        clk             : in     vl_logic;
        reset           : in     vl_logic;
        comp_task       : out    vl_logic_vector(3 downto 0);
        head0_matches_head1: in     vl_logic;
        tail0_matches_head0: in     vl_logic;
        tail1_matches_head1: in     vl_logic;
        fprints_ready   : in     vl_logic_vector(15 downto 0);
        checkin         : in     vl_logic_vector(15 downto 0);
        fprint0         : in     vl_logic_vector(31 downto 0);
        fprint1         : in     vl_logic_vector(31 downto 0);
        comp_increment_tail_pointer: out    vl_logic;
        comp_reset_fprint_ready: out    vl_logic;
        reset_fprint_ack: in     vl_logic;
        comp_task_verified: out    vl_logic;
        fprint_reg_ack  : in     vl_logic;
        comp_status_write: out    vl_logic;
        comp_status_ack : in     vl_logic;
        comp_mismatch_detected: out    vl_logic;
        comp_reset_task : out    vl_logic;
        reset_task_ack  : in     vl_logic
    );
    attribute mti_svvh_generic_type : integer;
    attribute mti_svvh_generic_type of init : constant is 1;
    attribute mti_svvh_generic_type of set_task : constant is 1;
    attribute mti_svvh_generic_type of load_pointer : constant is 1;
    attribute mti_svvh_generic_type of load_fprint : constant is 1;
    attribute mti_svvh_generic_type of check_task_status : constant is 1;
    attribute mti_svvh_generic_type of task_complete : constant is 1;
    attribute mti_svvh_generic_type of compare_fprints : constant is 1;
    attribute mti_svvh_generic_type of mismatch_detected : constant is 1;
    attribute mti_svvh_generic_type of task_verified : constant is 1;
    attribute mti_svvh_generic_type of increment_tail_pointer : constant is 1;
    attribute mti_svvh_generic_type of check_if_done : constant is 1;
    attribute mti_svvh_generic_type of reset_fprint_ready : constant is 1;
    attribute mti_svvh_generic_type of st_reset_task : constant is 1;
    attribute mti_svvh_generic_type of write_status_reg : constant is 1;
end comparator;
