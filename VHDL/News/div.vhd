library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity div_loop is
	port(
		
		
		tdiv	: inout std_logic_vector(63 downto 0);
		tmp	: inout std_logic_vector(31 downto 0);
		
		div1	: inout std_logic_vector(31 downto 0);
		result	: inout std_logic_vector(31 downto 0);
		
		
		I1	:  in  std_logic
		);
end entity;


architecture behaviour of div_loop is
begin
	process (I1)
	begin
	
	
	if div1 >= tdiv then 
		--div1 <= div1 + tdiv;
		--result <= result + tmp;
	end if; 
	
	tdiv(62 downto 0) <= tdiv(63 downto 1);
	tmp(30 downto 0) <= tmp(31 downto 1);
	
	end process;
	
end architecture;

