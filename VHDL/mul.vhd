library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity mul is
	port(
		a	: in  std_logic;
		b	: in  std_logic;
		c	: out std_logic);
end entity;


architecture behaviour of mul is

	
	signal Result		: unsigned(1 downto 0);
	
	begin
	result(0) <= a nand b;
end architecture;
