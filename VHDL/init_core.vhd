library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity init_core is
	port(
		Clk	: in std_logic;
		o_wri	: out std_logic;

		
		o_DMA_ADR		: out  std_logic_vector(15 downto 0);
		o_PC				: out  std_logic_vector(15 downto 0)
		);
end entity;


architecture behaviour of init_core is
begin
	process (Clk)
	
	
	begin
		
		o_wri <= '1';
		o_DMA_ADR <= "0000000000000000";
		o_PC <= "0000000000000000";
		
	end process;

end architecture;

--------------------------------
