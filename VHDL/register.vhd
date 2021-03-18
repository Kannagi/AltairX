library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity registers is
	port(
		Clk				: in std_logic;
		s_decode			: in std_logic;
		
		i_imm1			: in  std_logic_vector(19 downto 0);
		i_imm2			: in  std_logic_vector(19 downto 0);
		i_imm3			: in  std_logic_vector(19 downto 0);
		i_imm4			: in  std_logic_vector(19 downto 0);
		
		i_imm1f			: in  std_logic_vector(2 downto 0);
		i_imm2f			: in  std_logic_vector(2 downto 0);
		i_imm3f			: in  std_logic_vector(2 downto 0);
		i_imm4f			: in  std_logic_vector(2 downto 0);
		
		i_fct1			: in  std_logic_vector(3 downto 0);
		i_fct2			: in  std_logic_vector(3 downto 0);
		i_fct3			: in  std_logic_vector(3 downto 0);
		i_fct4			: in  std_logic_vector(3 downto 0);
		
		i_unit1			: in  std_logic_vector(3 downto 0);
		i_unit2			: in  std_logic_vector(3 downto 0);
		i_unit3			: in  std_logic_vector(3 downto 0);
		i_unit4			: in  std_logic_vector(3 downto 0);
		
		o_unit1			: out  std_logic_vector(3 downto 0);
		o_unit2			: out  std_logic_vector(3 downto 0);
		o_unit3			: out  std_logic_vector(3 downto 0);
		o_unit4			: out  std_logic_vector(3 downto 0);
		
		i_registerid1_w		: in  std_logic_vector(6 downto 0);
		i_registerid2_w		: in  std_logic_vector(6 downto 0);
		i_registerid3_w		: in  std_logic_vector(6 downto 0);
		i_registerid4_w		: in  std_logic_vector(6 downto 0);
		
		i_registerid1_r1		: in  std_logic_vector(6 downto 0);
		i_registerid2_r1		: in  std_logic_vector(6 downto 0);
		i_registerid3_r1		: in  std_logic_vector(6 downto 0);
		i_registerid4_r1		: in  std_logic_vector(6 downto 0);
		
		i_registerid1_r2		: in  std_logic_vector(6 downto 0);
		i_registerid2_r2		: in  std_logic_vector(6 downto 0);
		i_registerid3_r2		: in  std_logic_vector(6 downto 0);
		i_registerid4_r2		: in  std_logic_vector(6 downto 0);
		
		
		i_register1_w		: in  std_logic_vector(63 downto 0);
		i_register2_w		: in  std_logic_vector(63 downto 0);
		i_register3_w		: in  std_logic_vector(63 downto 0);
		i_register4_w		: in  std_logic_vector(63 downto 0);
		
		i_register1x_w		: in  std_logic_vector(63 downto 0);
		i_register2x_w		: in  std_logic_vector(63 downto 0);
		
		o_register1_r1		: out  std_logic_vector(63 downto 0);
		o_register2_r1		: out  std_logic_vector(63 downto 0);
		o_register3_r1		: out  std_logic_vector(63 downto 0);
		o_register4_r1		: out  std_logic_vector(63 downto 0);
		
		o_register1_r2		: out  std_logic_vector(63 downto 0);
		o_register2_r2		: out  std_logic_vector(63 downto 0);
		o_register3_r2		: out  std_logic_vector(63 downto 0);
		o_register4_r2		: out  std_logic_vector(63 downto 0);
		
		-----------
		i_pregister1_w		: in  std_logic_vector(127 downto 0);
		i_pregister2_w		: in  std_logic_vector(127 downto 0);

		o_pregister1_r1		: out  std_logic_vector(127 downto 0);
		o_pregister2_r1		: out  std_logic_vector(127 downto 0);

		o_pregister1_r2		: out  std_logic_vector(127 downto 0);
		o_pregister2_r2		: out  std_logic_vector(127 downto 0);

		
		i_flagrw			: in  std_logic_vector(127 downto 0)
		);
end entity;


architecture behaviour of registers is
begin
	process (s_decode)
	variable    v_opcode : std_logic_vector(63 downto 0);
	
	
	variable    v_flagrw : std_logic;
	variable    id : integer;
	type t_Register is array (0 to 63) of std_logic_vector(63 downto 0); 
	variable r_Reg : t_Register;
	
	type t_Register2 is array (0 to 63) of std_logic_vector(125 downto 0);
	variable r_Reg2 : t_Register;
	
	begin
		id := to_integer(unsigned(i_registerid1_r1));
		o_register1_r1  <= r_Reg(id);
		
		id := to_integer(unsigned(i_registerid1_r2));
		o_register1_r2  <= r_Reg(id);
		
		
		--ADD/SUB
		if i_unit1="0001" then 
			o_unit1 <= i_unit1;
		--ALU (OR/XOR/MOVE etc etc)
		elsif i_unit1="0010" then 
			o_unit1 <= i_unit1;
			
		--MUL
		--elsif i_unit1="0011" then 
			
		--DIV
		--elsif i_unit1="0100" then 
		
		--LSU
		--elsif i_unit1="0101" then 
		
		--VPU MULADD PS/FS/FD
		--elsif i_unit1="0110" then 
		
		--VPU MOVE
		--elsif i_unit1="0111" then 
		
		--VDIV(VSQRT)
		--elsif i_unit1="1000" then 
		
		--CMP
		--elsif i_unit1="1110" then 
		
		--MMU -> DMA
		--elsif i_unit1="1111" then 

		else
			o_unit1 <= i_unit1;
		end if; 
		
		--imm10
		if i_imm1f="001" then 
			o_register1_r2(9 downto 0) <= i_imm1(9 downto 0);
		--imm16
		elsif i_imm1f="010" then 
			o_register1_r2(15 downto 0) <= i_imm1(15 downto 0);
		--imm20
		elsif i_imm1f="011" then 
			o_register1_r2(15 downto 0) <= i_imm1(15 downto 0);
			o_register1_r2(19 downto 16) <= i_imm1(19 downto 16);
		--imm15
		elsif i_imm1f="100" then 
			o_register1_r2(14 downto 0) <= i_imm1(14 downto 0);
		--imm8/8
		elsif i_imm1f="101" then 
			o_register1_r1(7 downto 0) <= i_imm1(7 downto 0);
			o_register1_r2(7 downto 0) <= i_imm1(15 downto 8);
		--no imm
		else

		end if; 
		

	end process;

end architecture;
