library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity ALU1 is
	port(
		i_unit			: in  std_logic_vector(4 downto 0);
		i_size			: in  std_logic_vector(1 downto 0);
		
		i_register_Q		: in  std_logic_vector(31 downto 0);
		i_register_P		: in  std_logic_vector(31 downto 0);
		
		i_register_r1		: in  std_logic_vector(31 downto 0);
		i_register_r2		: in  std_logic_vector(31 downto 0);
		
		i_register_rs		: in  std_logic_vector(1 downto 0);
		i_register_ws		: in  std_logic_vector;
		
		o_register_w		: out  std_logic_vector(31 downto 0);
		
		regr		: inout  std_logic_vector(31 downto 0);

		I1	: in  std_logic_vector(3 downto 0)
		);
end entity;


architecture behaviour of ALU1 is
begin
	process (I1)
	variable acc : std_logic_vector(31 downto 0);
	
	variable regw : std_logic_vector(31 downto 0);
	
	begin
	
		if i_register_rs="00" then 
			regr <= i_register_r1;
			
		elsif  i_register_rs="01" then 
			regr <= acc;
			
		elsif  i_register_rs="10" then 
			regr <= i_register_P;
		else --11
			regr <= i_register_Q;
		end if; 
		
		if i_unit="00000" then 
			--regw := reg1+i_register_r2;
			
		elsif i_unit="00001" then 
			--regw := reg1-i_register_r2;
			
		elsif i_unit="00010" then 
			regw := regr XOR i_register_r2;
			
		elsif i_unit="00011" then 
			regw := regr OR i_register_r2;
		
		elsif i_unit="00100" then 
			regw := regr AND i_register_r2;
			
		elsif i_unit="00101" then 
			--regw := reg1 << i_register_r2;
			
		elsif i_unit="00110" then 
			--regw := reg1 >> i_register_r2;
			
		elsif i_unit="00111" then 
			--regw := reg1 >> i_register_r2;
			
			
		elsif i_unit="10001" then --SEXT
		
			if i_size=x"00" then 
				if regr(7)='1' then
					regw(31 downto 8) := x"111111";
				end if; 
	
			elsif  i_size=x"01" then 
				if regr(15)='1' then
					regw(31 downto 16) := x"1111";
				end if; 
			
			else
				
			end if; 
						
		elsif i_unit="10010" then --BOOL
			if regr=x"00000000" then 
				regw := x"00000000";
			else
				regw := x"00000001";
			end if; 
			
			
		elsif i_unit="10100" then --SLTS
			--regw := reg1 < i_register_r2;
			
		elsif i_unit="10101" then --SLTU
			--regw := reg1 < i_register_r2;
			
		elsif i_unit="10110" then --SMOVE
			--regw := reg1 < i_register_r2;
			
		elsif i_unit="10111" then --MOVE
			regw := regr;
		else
		
		
		
		end if; 
		
		if i_register_ws="0" then 
			o_register_w <= regw;
		else
			acc := regw;
		end if; 
		
	end process;

end architecture;
