library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity register_internal is
	port(
		io_PC		: inout  std_logic_vector(15 downto 0);
	
		i_wri0		: in std_logic;
		i_wri1		: in std_logic;
		i_wri2		: in std_logic;
		i_wri3		: in std_logic;
		
		i_PC0		: in  std_logic_vector(15 downto 0);
		i_PC1		: in  std_logic_vector(15 downto 0);
		i_PC2		: in  std_logic_vector(15 downto 0);
		i_LR			: in  std_logic_vector(15 downto 0);
		
		o_PC			: out  std_logic_vector(15 downto 0);
		o_LR			: out  std_logic_vector(15 downto 0)
		);
end entity;


architecture behaviour of register_internal is
begin

	process (i_wri0,i_wri1,i_wri2,i_wri3)
	begin
		if i_wri0='1' then 
			io_PC<= i_PC0;
			o_PC<=i_PC0;
			elsif i_wri1='1' then 
				io_PC<= i_PC1;
				o_PC<=i_PC1;
			elsif i_wri2='1' then 
				io_PC<= i_PC2;
				o_PC<=i_PC2;
			elsif i_wri3='1' then 
				o_PC<=x"4012";
				--o_PC<=i_PC0
			else
			
		end if; 

		
	end process;
	

end architecture;
