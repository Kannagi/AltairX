library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity fetch is
	port(
		Clk	: in std_logic;
				
		i_PC				: in  std_logic_vector(15 downto 0);
		
		i_DMA_DATA		: in  std_logic_vector(127 downto 0);
		i_DMA_ADR		: in  std_logic_vector(15 downto 0);
		
		o_wri	: out std_logic;
		
		o_opcode1		: out  std_logic_vector(31 downto 0);
		o_opcode2		: out  std_logic_vector(31 downto 0);
		o_opcode3		: out  std_logic_vector(31 downto 0);
		o_opcode4		: out  std_logic_vector(31 downto 0);
		
		o_s_fetch	: out std_logic;

		o_PC				: out  std_logic_vector(15 downto 0)
		);
end entity;


architecture behaviour of fetch is
begin
	process (Clk)
	variable    v_opcode : std_logic_vector(63 downto 0);
	
	variable    pc : integer;
	variable    adr : integer;
	
	variable    v_imm : std_logic_vector(15 downto 0);
	--0 to 512
	type t_DSRAM is array (0 to 63) of std_logic_vector(63 downto 0);
	variable r_SRAM : t_DSRAM;
	
	begin
		--pc := to_integer(unsigned(i_PC));
		--adr := to_integer(unsigned(i_DMA_ADR));
		o_s_fetch <= '1';
		
		--if
		--r_SRAM(adr) := i_DMA_DATA(63 downto 0);
		--r_SRAM(adr+1) := i_DMA_DATA2(127 downto 0);
		
		--v_opcode   := r_SRAM(pc);
		v_imm := "0000000000000001";

		o_opcode1 <= X"12140406";
		o_opcode2 <= X"00000000";
		o_opcode3 <= X"00000000";
		o_opcode4 <= X"00000000";
		
		o_wri <= '1';
		--o_PC <= std_logic_vector(unsigned(i_PC) + unsigned(v_imm));
		--pc := pc+4;
		
		--io_PC <=  std_logic_vector(to_unsigned(pc,io_PC'length));
		o_PC <= std_logic_vector(unsigned(i_PC) + unsigned(v_imm));
	end process;

end architecture;
