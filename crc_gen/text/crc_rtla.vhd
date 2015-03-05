--Commented lines with * are automatically excluded when data_width = crc_width, DO NOT CHANGE
--Commented lines with % are automatically excluded when data_width != crc_width, DO NOT CHANGE

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.numeric_std.all;


ENTITY crc_rtla IS
	GENERIC( data_width : INTEGER := *data_width;
				crc_width : INTEGER := *crc_width);
	PORT(d_in : IN STD_LOGIC_VECTOR(data_width-1 DOWNTO 0);
		clk,rst, data_latch,sel,en_fcs : IN STD_LOGIC;
		crc_out : OUT STD_LOGIC_VECTOR(crc_width-1 DOWNTO 0));
END crc_rtla;

ARCHITECTURE crc OF  crc_rtla IS
	SIGNAL D : STD_LOGIC_VECTOR(data_width-1 DOWNTO 0);
	SIGNAL A1,L : STD_LOGIC_VECTOR(data_width-1 DOWNTO data_width-crc_width);
	--*SIGNAL A2 : STD_LOGIC_VECTOR(data_width-crc_width-1 DOWNTO 0);
	TYPE ar1 IS ARRAY(data_width-1 DOWNTO data_width-crc_width) OF STD_LOGIC_VECTOR(crc_width-1 DOWNTO 0);
	--*TYPE ar2 IS ARRAY(data_width-crc_width-1 DOWNTO 0) OF STD_LOGIC_VECTOR(crc_width-1 DOWNTO 0);
	TYPE ar3 IS ARRAY(data_width-1 DOWNTO 0) OF STD_LOGIC_VECTOR(crc_width-1 DOWNTO 0);
	SIGNAL F1,G1 : ar1;
	--*SIGNAL F2,G2 : ar2;
	SIGNAL H : ar3;
	SIGNAL fcs, f_mux_out : STD_LOGIC_VECTOR(crc_width-1 DOWNTO 0);
	SIGNAL xor_out : STD_LOGIC_VECTOR(crc_width-1 DOWNTO 0);

	COMPONENT reg IS
	GENERIC(N : INTEGER := 4);
	PORT (clk, rst, en : IN STD_LOGIC;
			reg_in : IN STD_LOGIC_VECTOR(N-1 DOWNTO 0);
			reg_out : OUT STD_LOGIC_VECTOR(N-1 DOWNTO 0));
	END COMPONENT;


	COMPONENT mux2x1 IS
	GENERIC(data_width : INTEGER := 32);
	PORT (data1, data2 : IN STD_LOGIC_VECTOR(data_width-1 DOWNTO 0);
			sel : IN STD_LOGIC;
			q : OUT STD_LOGIC_VECTOR(data_width-1 DOWNTO 0));
	END COMPONENT;
BEGIN
		L <= (OTHERS => '0');
		
	data_in_reg : reg 	GENERIC MAP (data_width)
								PORT MAP(clk,rst,data_latch, d_in,D);
	

	
	fcs_mux : MUX2x1		GENERIC MAP (crc_width)
							PORT MAP(L,fcs,sel,f_mux_out);
								
	
	xor_out <= f_mux_out XOR D(data_width-1 DOWNTO data_width-crc_width);
						
	
		A1 <= xor_out;
		--*A2 <= D(data_width-crc_width-1 DOWNTO 0);
		
*rtla
gen1: FOR i IN data_width-crc_width TO data_width-1 GENERATE
	mux1	: mux2x1 	GENERIC MAP(crc_width)
						PORT MAP(L,F1(i),A1(i),G1(i));
	END GENERATE;
	
	--*gen2: FOR i IN 0 TO data_width-crc_width-1 GENERATE
	--*mux2  : mux2x1 	GENERIC MAP(crc_width)
	--*				PORT MAP(L,F2(i),A2(i),G2(i));

	--*END GENERATE;
	
	--*H(0) <= G2(0);
	--*gen3: FOR i IN 1 TO data_width-crc_width-1 GENERATE
	--*	H(i) <= G2(i) XOR H(i-1);
	--*	END GENERATE;
	--*gen4: FOR i IN data_width-crc_width TO data_width-1 generate
	--*		H(i) <= G1(i) XOR H(i-1);
	--*END GENERATE;
	
	--%H(0) <= G1(0);
	--%gen4: FOR i IN 1 TO data_width-1 generate
	--%H(i) <= G1(i) XOR H(i-1);
	--%END GENERATE;
	fcs_reg : reg 	GENERIC MAP(crc_width)
					PORT MAP(clk,rst,en_fcs,H(data_width-1),fcs);
						
	crc_out <= fcs;
					
END crc;

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.numeric_std.all;

ENTITY mux2x1 IS
	GENERIC(data_width : INTEGER := 32);
	PORT (data1, data2 : IN STD_LOGIC_VECTOR(data_width-1 DOWNTO 0);
			sel : IN STD_LOGIC;
			q : OUT STD_LOGIC_VECTOR(data_width-1 DOWNTO 0));
END mux2x1;

ARCHITECTURE m OF mux2x1 IS
BEGIN
	PROCESS(sel,data1,data2)
	BEGIN
		CASE sel IS
			WHEN '0' => q <= data1;
			WHEN '1' => q <= data2;
			WHEN OTHERS => q <= (OTHERS => '0');
		END CASE;
	END PROCESS;
END m;

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.numeric_std.all;



ENTITY reg IS
	GENERIC(N : INTEGER := 4);
	PORT (clk, rst, en : IN STD_LOGIC;
			reg_in : IN STD_LOGIC_VECTOR(N-1 DOWNTO 0);
			reg_out : OUT STD_LOGIC_VECTOR(N-1 DOWNTO 0));
END reg;


ARCHITECTURE reg OF reg IS
	BEGIN
	
	PROCESS(clk, rst)
		BEGIN
		IF (rst = '1') THEN
		reg_out <= (OTHERS => '0');
		ELSIF(clk'EVENT AND clk='1')THEN
			IF (en = '1') THEN
				reg_out <= reg_in;
			END IF;
		END IF;
	END PROCESS;
END reg;

	