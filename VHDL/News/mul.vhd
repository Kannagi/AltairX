library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity mul_loop is
	port(
		mul1	: in  std_logic_vector(31 downto 0);
		bits		: inout  std_logic_vector(31 downto 0);
		mul2	: inout std_logic_vector(31 downto 0);
		
		mul	: inout std_logic_vector(31 downto 0);
		mult	: inout std_logic_vector(31 downto 0);
		
		I1	:  in  std_logic
		);
end entity;


architecture behaviour of mul_loop is
begin
	process (I1)
	begin
	
	mult <= bits AND mul1;
	
	if mult=bits then 
		--mul <= mul + mul2;
	end if; 
	
	mul2(31 downto 1) <= mul2(30 downto 0);
	bits(31 downto 1) <= bits(30 downto 0);
	
	end process;
	
end architecture;

