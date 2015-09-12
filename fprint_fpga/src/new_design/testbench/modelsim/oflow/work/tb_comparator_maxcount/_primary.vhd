library verilog;
use verilog.vl_types.all;
entity tb_comparator_maxcount is
    generic(
        idle            : integer := 0;
        waitforsignal   : integer := 1;
        st_isr          : integer := 2
    );
    attribute mti_svvh_generic_type : integer;
    attribute mti_svvh_generic_type of idle : constant is 1;
    attribute mti_svvh_generic_type of waitforsignal : constant is 1;
    attribute mti_svvh_generic_type of st_isr : constant is 1;
end tb_comparator_maxcount;
