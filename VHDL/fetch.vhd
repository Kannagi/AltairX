library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity fetch is
	port(
		Clk	: in std_logic;
		
		o_s_fetch	: out std_logic;
		
		i_SWT			: in  std_logic_vector(1 downto 0);
		i_PC				: in  std_logic_vector(13 downto 0);
		
		o_opcode1		: out  std_logic_vector(31 downto 0);
		o_opcode2		: out  std_logic_vector(31 downto 0);
		o_opcode3		: out  std_logic_vector(31 downto 0);
		o_opcode4		: out  std_logic_vector(31 downto 0);
		
		i_DMA_DATA		: in  std_logic_vector(127 downto 0);
		i_DMA_ADR		: in  std_logic_vector(13 downto 0);
		
		
		o_PC				: out  std_logic_vector(13 downto 0)
		);
end entity;


architecture behaviour of fetch is
begin
	process (Clk)
	variable    v_opcode : std_logic_vector(127 downto 0);
	
	variable    pc : integer;
	variable    adr : integer;
	
	variable    v_imm : std_logic_vector(13 downto 0);
	--0 to 8191
	type t_DSRAM is array (0 to 127) of std_logic_vector(127 downto 0);
	variable r_SRAM : t_DSRAM;
	
	begin
		pc := to_integer(unsigned(i_PC));
		adr := to_integer(unsigned(i_DMA_ADR));
		o_s_fetch <= '1';
		
		--if
		r_SRAM(adr) := i_DMA_DATA(127 downto 0);
		--r_SRAM(adr+1) := i_DMA_DATA2(127 downto 0);
		
		v_opcode   := r_SRAM(pc);
		
		
		if i_SWT="01" then 
			v_imm := "00000000000010";
			
		elsif i_SWT="11" then 
			v_imm := "00000000000000";
		else
			v_imm := "00000000000001";
		end if; 
		
		o_opcode1 <= X"00000076";
		o_opcode2 <= X"00000000";
		o_opcode3 <= X"00000000";
		o_opcode4 <= X"00000000";
		
		
		
		--pc := pc+4;
		--io_PC <=  std_logic_vector(to_unsigned(pc,io_PC'length));
		--o_PC <= std_logic_vector(unsigned(i_PC) + unsigned(v_imm));
	end process;

end architecture;
