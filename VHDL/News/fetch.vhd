library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity fetch is
	port(
		Clk	: in std_logic;
				
		i_PC				: in  std_logic_vector(15 downto 0);
		
		o_opcode1		: out  std_logic_vector(31 downto 0);
		o_opcode2		: out  std_logic_vector(31 downto 0);
		o_opcode3		: out  std_logic_vector(31 downto 0);
		o_opcode4		: out  std_logic_vector(31 downto 0);
		
		o_s_fetch	: out std_logic;
		
		io_opcode1		: inout  std_logic_vector(31 downto 0);
		io_opcode2		: inout  std_logic_vector(31 downto 0);
		io_opcode3		: inout  std_logic_vector(31 downto 0);
		io_opcode4		: inout  std_logic_vector(31 downto 0);
		
		var_PC			: inout  std_logic_vector(15 downto 0);
		io_pairing			: inout  std_logic_vector(3 downto 0);

		o_PC				: out  std_logic_vector(15 downto 0)
		);
end entity;


architecture behaviour of fetch is
begin
	process (Clk)
	
	--0 to 512
	type t_SRAM is array (0 to 63) of std_logic_vector(31 downto 0);
	variable r_SRAM : t_SRAM;
	
	begin
		--io_opcode1 <= r_SRAM(i_PC+0);
		--io_opcode2 <= r_SRAM(i_PC+1);
		--io_opcode3 <= r_SRAM(i_PC+2);
		--io_opcode4 <= r_SRAM(i_PC+3);
		
		io_pairing(0) <= io_opcode1(0);
		io_pairing(1) <= io_opcode2(0);
		io_pairing(2) <= io_opcode3(0);
		io_pairing(3) <= io_opcode4(0);
		
		if io_pairing(0)='0' then 
			var_PC <= x"0001";
			io_opcode2 <= x"00000000";
			io_opcode3 <= x"00000000";
			io_opcode4 <= x"00000000";
			
		elsif  io_pairing(1 downto 0)="01" then 
			var_PC <= x"0002";
			io_opcode3 <= x"00000000";
			io_opcode4 <= x"00000000";
			
		elsif   io_pairing(2 downto 0)="011" then 
			var_PC <= x"0003";
			io_opcode4 <= x"00000000";
		
		elsif   io_pairing(3 downto 0)="0111" then 
			var_PC <= x"0004";
		else --error
			
		end if; 
		
		o_PC <= var_PC;
		
	end process;

end architecture;
