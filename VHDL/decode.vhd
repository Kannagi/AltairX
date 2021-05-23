library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

--in,out,inout,buffer
--std_logic  std_logic_vector
entity decode is
	port(
		Clk				: in std_logic;
		s_fetch			: in std_logic;
		
		i_PC				: in  std_logic_vector(15 downto 0);
		
		i_opcode1			: in  std_logic_vector(31 downto 0);
		i_opcode2			: in  std_logic_vector(31 downto 0);
		i_opcode3			: in  std_logic_vector(31 downto 0);
		i_opcode4			: in  std_logic_vector(31 downto 0);
				
		o_opcode1i		: out  std_logic_vector(9 downto 0);
		o_opcode2i		: out  std_logic_vector(9 downto 0);
		o_opcode3i		: out  std_logic_vector(9 downto 0);
		o_opcode4i		: out  std_logic_vector(9 downto 0);
		
		o_fct1a			: out  std_logic_vector(1 downto 0);
		o_fct2a			: out  std_logic_vector(1 downto 0);
		o_fct3a			: out  std_logic_vector(1 downto 0);
		o_fct4a			: out  std_logic_vector(1 downto 0);
		
		o_imm1			: out  std_logic_vector(63 downto 0);
		o_imm2			: out  std_logic_vector(63 downto 0);
		o_imm3			: out  std_logic_vector(63 downto 0);
		o_imm4			: out  std_logic_vector(63 downto 0);
		
		o_register1_w		: out  std_logic_vector(5 downto 0);
		o_register2_w		: out  std_logic_vector(5 downto 0);
		o_register3_w		: out  std_logic_vector(5 downto 0);
		o_register4_w		: out  std_logic_vector(5 downto 0);
		
		o_register1_r1		: out  std_logic_vector(5 downto 0);
		o_register2_r1		: out  std_logic_vector(5 downto 0);
		o_register3_r1		: out  std_logic_vector(5 downto 0);
		o_register4_r1		: out  std_logic_vector(5 downto 0);
		
		o_register1_r2		: out  std_logic_vector(5 downto 0);
		o_register2_r2		: out  std_logic_vector(5 downto 0);
		o_register3_r2		: out  std_logic_vector(5 downto 0);
		o_register4_r2		: out  std_logic_vector(5 downto 0);
		
		o_unit1			: out  std_logic_vector(3 downto 0);
		o_unit2			: out  std_logic_vector(3 downto 0);
		o_unit3			: out  std_logic_vector(3 downto 0);
		o_unit4			: out  std_logic_vector(3 downto 0);
			
		o_PC				: out  std_logic_vector(15 downto 0);
		cycle			: inout  integer
		
	);
end entity;




architecture behaviour of decode is
	
begin
	process (s_fetch)
	
	variable    v_unit : std_logic_vector(1 downto 0);
	variable    v_sub1 : std_logic_vector(1 downto 0);
	variable    v_sub2 : std_logic_vector(3 downto 0);
	
	variable    v_branch : 		std_logic_vector(1 downto 0);
	variable    v_immt1 : 			std_logic_vector(2 downto 0);
	variable    v_immt2 : 			std_logic_vector(2 downto 0);
	
	----opcode 1
	variable    v_imm1 :			 std_logic_vector(63 downto 0);
	
	variable    v_register1_a : 		std_logic_vector(5 downto 0);
	variable    v_register1_b : 		std_logic_vector(5 downto 0);
	variable    v_register1_c : 		std_logic_vector(5 downto 0);
	
	variable    v_register1_exta  :	std_logic_vector(1 downto 0);
	variable    v_register1_extb  :	std_logic_vector(1 downto 0);
	variable    v_register1_extc  :	std_logic_vector(1 downto 0);
	variable    v_execute1 :		std_logic_vector(3 downto 0);
	variable    v_execute1s :		std_logic_vector(5 downto 0);
	variable    v_fct1a :			std_logic_vector(1 downto 0);
	variable    v_fct1b :			std_logic_vector(1 downto 0);
	
	
	variable    v_register1_type :	std_logic;
	
	----opcode 2
	variable    v_imm2 : 			std_logic_vector(63 downto 0);
	
	variable    v_register2_a : 		std_logic_vector(5 downto 0);
	variable    v_register2_b : 		std_logic_vector(5 downto 0);
	variable    v_register2_c : 		std_logic_vector(5 downto 0);
	
	variable    v_register2_exta  :	std_logic_vector(1 downto 0);
	variable    v_register2_extb  :	std_logic_vector(1 downto 0);
	variable    v_register2_extc  :	std_logic_vector(1 downto 0);
	variable    v_execute2 :		std_logic_vector(3 downto 0);
	variable    v_execute2s :		std_logic_vector(5 downto 0);
	variable    v_fct2a :			std_logic_vector(1 downto 0);
	variable    v_fct2b :			std_logic_vector(1 downto 0);
	
	variable    v_register2_type :	std_logic;
	
	---opcode 3
	
	---opcode 4
	
	---
	
	begin
			v_branch := "00";
			
			v_immt1 := "000";
			
			v_imm1 := x"0000000000000000";
			v_execute1 := x"0";
			v_execute1s(5 downto 4) := "00";
			v_execute1s(3 downto 0) := i_opcode1(7 downto 4);
			
			v_unit(1 downto 0) := i_opcode1(1 downto 0);
			v_sub1(1 downto 0) := i_opcode1(3 downto 2);
			v_sub2(3 downto 0) := i_opcode1(7 downto 4);
			
			v_imm1(15 downto 0) := i_opcode1(25 downto 10);
			v_imm1(19 downto 16) := i_opcode1(7 downto 4);
						
			v_register1_a(5 downto 0) := i_opcode1(31 downto 26);
			v_register1_b(5 downto 0) := i_opcode1(25 downto 20);
			v_register1_c(5 downto 0) := i_opcode1(19 downto 14);
			
			
			
			
			--o_PC(9 downto 0) <= i_opcode1(9 downto 0);
			
			--o_opcode1i(9 downto 0) <= i_opcode1(9 downto 0);
			
			
			o_opcode3i   <= "0000001010";
			o_opcode4i   <= "0000001010";
			
			--cycle <= cycle +4;
			
			
			
			
			--UNIT 1--
			
			--BRU/CMP
			if v_unit="00" then 
				
				--BRU
				if v_sub1="00" then 					
					--BNE
					if v_sub2="0000" then 
						--PC += IMM
						v_branch := "01";
					--BEQ
					elsif v_sub2="0001" then 
						--PC += IMM
						v_branch := "01";
					--BL
					elsif v_sub2="0010" then 
						--PC += IMM
						v_branch := "01";
						
					--BLE
					elsif v_sub2="0011" then 
						--PC += IMM
						v_branch := "01";
						
					--BG
					elsif v_sub2="0100" then 
						--PC += IMM
						v_branch := "01";
						
					--BGE
					elsif v_sub2="0101" then 
						--PC += IMM
						v_branch := "01";
						
					--BLS
					elsif v_sub2="0110" then 
						--PC += IMM
						v_branch := "01";
						
					--BLES
					elsif v_sub2="0111" then 
						--PC += IMM
						v_branch := "01";
						
					--BGS
					elsif v_sub2="1000" then 
						--PC += IMM
						v_branch := "01";
						
					--BGES
					elsif v_sub2="1001" then 
						--PC += IMM
						v_branch := "01";
						
					--BRA
					elsif v_sub2="1010" then 
						--PC += IMM
						v_branch := "01";
						
					--JUMP
					elsif v_sub2="1011" then 
						--PC = IMM
						v_branch := "10";
					--JUMPBR
					elsif v_sub2="1100" then 
						--PC = IMM+BR
						v_branch := "11";
					--CALL
					elsif v_sub2="1101" then 
						--PC = IMM
						v_branch := "10";
					--CALLBR
					elsif v_sub2="1110" then 
						--LR = PC
						--PC = IMM+BR
						v_branch := "11";
					--JUMPT
					elsif v_sub2="1111" then 
						--PC2 = IMM+BR
						
					--Illegal instruction
					else
						
					end if; 
					
				--CMP
				elsif v_sub1="01" then 
					--CMP R,R
					if v_sub2="0000" then 
						v_execute1 := x"9";
					--FCMP R,R
					elsif v_sub2="0100" then
						v_execute1 := x"9";
					--DCMP R,R
					elsif v_sub2="0101" then 
						v_execute1 := x"9";
					--FCMP R,I
					elsif v_sub2="0110" then 
						v_execute1 := x"9";
						v_immt1 := "101";
					--DCMP R,I
					elsif v_sub2="0111" then 
						v_execute1 := x"9";
						v_immt1 := "110";
					--ENDP
					elsif v_sub2="1000" then 
						
					--INT
					elsif v_sub2="1001" then 
						v_execute1 := x"C";
						v_immt1 := "011";
					--WAITI
					elsif v_sub2="1010" then 
						
					--RET
					elsif v_sub2="1011" then 
						
					--Illegal instruction
					else
						
					end if; 
				--CMPI
				elsif v_sub1="10" then 
					v_execute1 := x"9";
					v_immt1 := "100";
				--Illegal instruction
				else
				
				end if; 
			
			--LSU
			elsif v_unit="01" then 
				
				--
				if v_sub1="00" then 
					v_execute1 := x"5";
					--IMM
					if v_sub2(3)='1' then 
						v_immt1 := "010";
					--REGISTER
					elsif v_sub2(3)='0' then 
					
					else
					
					end if; 
					
				-- IMM LONG
				elsif v_sub1="01" then
					v_execute1 := x"5";
					v_immt1 := "100";
				--Illegal instruction
				else
				
				end if; 
				
				--Register
				if v_sub2(2)='0' then 
				
				--Vector Register
				elsif v_sub2(2)='1' then 
				
				else
				
				end if; 
				
			
			--ALU
			elsif v_unit="10" then
			
				-- R,R,R
				if v_sub1="00" then 
					v_execute1 := x"1";
				-- R,R,I
				elsif v_sub1="01" then 
					v_execute1 := x"1";
					v_immt1 := "001";
				-- MOVE/OTHER
				elsif v_sub1="10" then 
				
					--NOP
					if v_sub2="0000" then 					
					
					--MOVE
					--elsif v_sub2="0001" then 
					
					--MOVE INFO
					elsif v_sub2="0010" then 
						v_execute1 := x"E";
					--MOVE LR,RX
					elsif v_sub2="0011" then 
						v_execute1 := x"E";
					--MOVE BR,RX
					elsif v_sub2="0100" then 
						v_execute1 := x"E";
					--MOVE RX,LR
					elsif v_sub2="0101" then 
						v_execute1 := x"E";
					--MOVE RX,INS
					elsif v_sub2="0110" then 
						v_execute1 := x"E";
					--MOVE RX,CYL
					elsif v_sub2="0111" then 
						v_execute1 := x"E";
					--Illegal instruction
					else
					
					end if; 
				
				-- MOVEI
				else
					v_immt1 := "100";
				end if; 
				
			--VU
			elsif v_unit="11" then
			
				v_execute1s(5 downto 4) := i_opcode1(3 downto 2);
				v_execute1 := x"7";
				--VFPU
				if v_sub1="00" then 
				
				--DOUBLE/MOVE/OTHER
				elsif v_sub1="01" then
				
				--DIV/SIMD
				elsif v_sub1="10" then
				
				--CONVERSION
				elsif v_sub1="11" then
				
				--Illegal instruction
				else
				
				end if; 
			
			
			else 
			
			end if;
			-------------
			--signed 10 bits
			if v_immt1="001" then 
				if v_imm1(9)='1'then
					v_imm1(63 downto 10) := "111111111111111111111111111111111111111111111111111111"; 
				else
					v_imm1(19 downto 10) := "0000000000";
				end if;  
			--unsigned 10 bits
			elsif v_immt1="010" then 
				v_imm1(19 downto 10) := "0000000000";
				
			--unsigned 16 bits
			elsif v_branch="011" then 
				v_imm1(19 downto 16) := "0000";
				
			--signed 20 bits
			elsif v_immt1="100" then 
				if v_imm1(19)='1'then
					v_imm1(63 downto 20) := "11111111111111111111111111111111111111111111";
				end if; 
				
			--half to float
			elsif v_branch="101" then 
			
			--half to double
			elsif v_immt1="110" then 
			
			else
				
			end if; 
			
			-------------
			o_imm1 <= v_imm1;
			o_register1_w  <= v_register1_a;
			o_register1_r1 <= v_register1_b;
			o_register1_r2 <= v_register1_c;
			-------------
			
			if v_branch="01" then 
				--PC = PC + IMM
				--o_PC <= std_logic_vector(unsigned(i_PC) + unsigned(v_imm));
			elsif v_branch="10" then 
				--PC = IMM
				--o_PC <= std_logic_vector(unsigned(v_imm));
			elsif v_branch="11" then 
				--PC = IMM+BR
				--o_PC <= std_logic_vector(unsigned(i_PC) + unsigned(I_BR));
			else
				--NOP
			end if; 
	end process;
end architecture;
