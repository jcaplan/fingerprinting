library verilog;
use verilog.vl_types.all;
entity cfpu is
    port(
        clk             : in     vl_logic;
        reset           : in     vl_logic;
        csr_address     : in     vl_logic_vector(9 downto 0);
        csr_read        : in     vl_logic;
        csr_readdata    : out    vl_logic_vector(31 downto 0);
        csr_write       : in     vl_logic;
        csr_writedata   : in     vl_logic_vector(31 downto 0);
        csr_waitrequest : out    vl_logic;
        fprint_address  : in     vl_logic_vector(9 downto 0);
        fprint_write    : in     vl_logic;
        fprint_writedata: in     vl_logic_vector(31 downto 0);
        fprint_waitrequest: out    vl_logic;
        oflow_write     : out    vl_logic;
        oflow_writedata : out    vl_logic_vector(31 downto 0);
        oflow_address   : out    vl_logic_vector(26 downto 0);
        oflow_waitrequest: in     vl_logic;
        irq             : out    vl_logic
    );
end cfpu;
