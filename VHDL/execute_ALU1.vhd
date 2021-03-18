library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity execute is
	port(
		i_imm1			: in  std_logic_vector(20 downto 0);
		i_size			: in  std_logic_vector(4 downto 0);

		i_register_r1		: in  std_logic_vector(63 downto 0);
		i_register_r2		: in  std_logic_vector(63 downto 0);
		
		i_register_w		: out  std_logic_vector(63 downto 0);

		I1	: in  std_logic_vector(3 downto 0);
		Y	: out std_logic_vector(7 downto 0)
		);
end entity;


architecture behaviour of execute is
begin
	process (I1)
	variable v_or : std_logic_vector(31 downto 0);
	variable v_and : std_logic_vector(31 downto 0);
	variable v_carry :std_logic;
	variable v_value :std_logic_vector(31 downto 0);
	
	begin
		v_or := X"00000007" or X"00000003";
		v_and := X"00000007" and X"00000003";
		v_carry := '0';
		v_value(6 downto 0) := "0000000";
		for i in 0 to 31 loop
			if v_and(i) = '1' then
				if v_carry = '1' then
					v_value(i) := '1';
				end if;
				v_carry := '1';
			else
				if v_carry = '1' then
					if v_or(i) = '0' then
						v_value(i) := '1';
						v_carry := '0';
					end if;
				else
					if v_or(i) = '1' then
						v_value(i) := '1';
					end if;
				end if;
			end if;

		end loop;
	
	end process;

end architecture;
