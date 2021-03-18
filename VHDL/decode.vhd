library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

--in,out,inout,buffer
--std_logic  std_logic_vector
entity decode is
	port(
		Clk				: in std_logic;
		s_fetch			: in std_logic;
		
		i_PC				: in  std_logic_vector(13 downto 0);
		
		i_opcode1			: in  std_logic_vector(31 downto 0);
		i_opcode2			: in  std_logic_vector(31 downto 0);
		i_opcode3			: in  std_logic_vector(31 downto 0);
		i_opcode4			: in  std_logic_vector(31 downto 0);
		
		o_SWT			: out  std_logic_vector(1 downto 0);
		
		o_opcode1i		: out  std_logic_vector(9 downto 0);
		o_opcode2i		: out  std_logic_vector(9 downto 0);
		o_opcode3i		: out  std_logic_vector(9 downto 0);
		o_opcode4i		: out  std_logic_vector(9 downto 0);
		
		o_size1			: out  std_logic_vector(3 downto 0);
		o_size2			: out  std_logic_vector(3 downto 0);
		o_size3			: out  std_logic_vector(3 downto 0);
		o_size4			: out  std_logic_vector(3 downto 0);
		
		o_imm1f			: out  std_logic_vector(2 downto 0);
		o_imm2f			: out  std_logic_vector(2 downto 0);
		o_imm3f			: out  std_logic_vector(2 downto 0);
		o_imm4f			: out  std_logic_vector(2 downto 0);
		
		o_imm1			: out  std_logic_vector(19 downto 0);
		o_imm2			: out  std_logic_vector(19 downto 0);
		o_imm3			: out  std_logic_vector(19 downto 0);
		o_imm4			: out  std_logic_vector(19 downto 0);
		
		o_register1_w		: out  std_logic_vector(6 downto 0);
		o_register2_w		: out  std_logic_vector(6 downto 0);
		o_register3_w		: out  std_logic_vector(6 downto 0);
		o_register4_w		: out  std_logic_vector(6 downto 0);
		
		o_register1_r1		: out  std_logic_vector(6 downto 0);
		o_register2_r1		: out  std_logic_vector(6 downto 0);
		o_register3_r1		: out  std_logic_vector(6 downto 0);
		o_register4_r1		: out  std_logic_vector(6 downto 0);
		
		o_register1_r2		: out  std_logic_vector(6 downto 0);
		o_register2_r2		: out  std_logic_vector(6 downto 0);
		o_register3_r2		: out  std_logic_vector(6 downto 0);
		o_register4_r2		: out  std_logic_vector(6 downto 0);
		
		o_PC				: out  std_logic_vector(13 downto 0);
		cycle			: inout  integer
		
	);
end entity;




architecture behaviour of decode is
	
begin
	process (s_fetch)
	
	variable    v_unit : std_logic_vector(1 downto 0);
	variable    v_sub1 : std_logic_vector(1 downto 0);
	variable    v_sub2 : std_logic_vector(3 downto 0);
	variable    v_sub3 : std_logic_vector(3 downto 0);
	variable    v_branch : std_logic_vector(1 downto 0);
	variable    v_imm : std_logic_vector(13 downto 0);
	
	begin
			v_branch := "00";
			v_imm := "00000000000000";
			
			v_unit(1 downto 0) := i_opcode1(1 downto 0);
			v_sub1(1 downto 0) := i_opcode1(3 downto 2);
			v_sub2(1 downto 0) := i_opcode1(5 downto 4);
			v_sub2(3 downto 2) := "00";
						
			o_size1(1 downto 0) <= i_opcode1(9 downto 8);
			o_imm1(15 downto 0) <= i_opcode1(25 downto 10);
			o_imm1(19 downto 16) <= i_opcode1(7 downto 4);
			o_register1_w(5 downto 0) <= i_opcode1(31 downto 26);
			o_register1_r1(5 downto 0) <= i_opcode1(25 downto 20);
			o_register1_r2(5 downto 0) <= i_opcode1(19 downto 14);
			o_imm1f <= "000";
			
			o_register1_r1(6) <= '0';
			o_register2_r1(6) <= '0';
			o_register3_r1(6) <= '0';
			o_register4_r1(6) <= '0';
			
			o_register1_r2(6) <= '0';
			o_register2_r2(6) <= '0';
			o_register3_r2(6) <= '0';
			o_register4_r2(6) <= '0';
			
			o_register1_w(6) <= '0';
			o_register2_w(6) <= '0';
			o_register3_w(6) <= '0';
			o_register4_w(6) <= '0';
			
			--o_PC(9 downto 0) <= i_opcode1(9 downto 0);
			
			o_opcode1i(9 downto 0) <= i_opcode1(9 downto 0);
			
			
			o_opcode3i   <= "0100010010";
			o_opcode4i   <= "0100010010";
			
			cycle <= cycle +4;
			
			--UNIT 1--
			
			--BRU/CMP
			if v_unit="00" then 
				
				v_sub2(2) := i_opcode1(6);
				v_sub2(3) := i_opcode1(7);
				--v_sub3(3 downto 0) := i_opcode1(11 downto 8);
				
				--BRU
				if v_sub1="00" then 
					v_imm(13 downto 0)  := i_opcode1(31 downto 18);
					
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
					--JUMPR
					elsif v_sub2="1100" then 
						--PC = IMM+BR
						v_branch := "11";
					--CALL
					elsif v_sub2="1101" then 
						--PC = IMM
						v_branch := "10";
					--CALLR
					elsif v_sub2="1110" then 
						--LR = PC
						--PC = IMM+BR
						v_branch := "11";
					--RET
					elsif v_sub2="1111" then 
						--PC = LR
						
					--Illegal instruction
					else
						
					end if; 
					
				--CMP/OTHER
				elsif v_sub1="01" then 
					
					--CMP
					if v_sub2="1000" then 
						o_opcode1i   <= "1000000000";
						
					--VCMP
					elsif v_sub2="1001" then 
						o_opcode1i   <= "1000000001";
						
						
					--SWT 0
					elsif v_sub2="0000" then 
						o_SWT <= "00";
					--SWT 1
					elsif v_sub2="0001" then 
						o_SWT <= "01";
					--SWT 2
					elsif v_sub2="0010" then 
						o_SWT <= "10";
						
					--ENDP
					elsif v_sub2="0011" then 
						
					--INT
					elsif v_sub2="1010" then 
						
					--WAITI
					elsif v_sub2="1011" then 
					
					--JUMPT
					elsif v_sub2="1100" then
					
					--MODE 0 (Float Single)
					elsif v_sub2="0100" then 
					
					--MODE 1 (Float Double)
					elsif v_sub2="0101" then 
					
					--MODE 2 (Posit Single ES1)
					elsif v_sub2="0110" then 
					
					--Illegal instruction
					else
						
					end if; 
				
				--CMPI
				elsif v_sub1="10" then 
					o_opcode1i   <= "1000011000";
					o_imm1f <= "011";
				--VCMPI
				else
					o_imm1f <= "011";
					o_opcode1i   <= "1000011010";
				end if; 
				
			--LSU1
			elsif v_unit="01" then 
				
				o_opcode1i(9 downto 6) <="0000";
				
				--Load/Store Register
				if v_sub1="00" then 					
					--Load Memory
					if v_sub2="0000" then 

					--Store Memory
					elsif v_sub2="0001" then 

					--Load Cache
					elsif v_sub2="0010" then 

					--Store Cache
					else

					end if; 
					
				--Load/Store Vector Register
				elsif v_sub1="01" then 
					
					--Load Memory
					if v_sub2="0000" then 
					
					--Store Memory
					elsif v_sub2="0001" then 

					--Load Cache
					elsif v_sub2="0010" then 

					--Store Cache
					else

					end if; 
				--IN/OUT
				elsif v_sub1="10" then 
					
					--IN
					if v_sub2="0000" then 
						o_imm1f <= "010";
					--OUT
					elsif v_sub2="0001" then 
						o_imm1f <= "010";
					--OUTI.B
					elsif v_sub2="0010" then 
						o_imm1f <= "010";
					--Illegal instruction
					else
					
					end if; 
					
				--Illegal instruction
				else
	
				end if; 
				
				
			--ALU1
			elsif v_unit="10" then 
				
				o_opcode1i(9 downto 8) <= "00";
				
								
				if v_sub1="00" then 
				
					v_sub2(2) := i_opcode1(6);
					v_sub2(3) := i_opcode1(7);
					
					if v_sub2/="0000" then 
						o_opcode1i(9 downto 8) <="01";
					end if;
					
					-- R,R,R
					if v_sub2="0000" then 
						o_size1(1 downto 0) <= i_opcode1(13 downto 12);
						o_opcode1i(7 downto 4) <=i_opcode1(11 downto 8);
					
					--NOP
					elsif v_sub2="0001" then 
						
					--MOVE LR,RX
					elsif v_sub2="0010" then 
						
					--MOVE RX,LR
					elsif v_sub2="0011" then 
						
					--MOVE BR,RX
					elsif v_sub2="0100" then 
					
					--MOVE RX,CYL
					elsif v_sub2="0101" then 
					
					--MOVE RX,INS
					elsif v_sub2="0110" then 
						
					--Illegal instruction
					else
						
					end if; 
				
				-- R,R,I
				elsif v_sub1="01" then 
					o_imm1f <= "001";
				-- R,I
				elsif v_sub1="10" then 
					o_imm1f <= "010";
				-- MOVEI
				else
					o_opcode1i(9 downto 4) <="000000";
					o_imm1f <= "011";
				end if; 
				
			--VPU1
			else
				if v_sub1/="11" then 
					o_size1(3 downto 0) <= i_opcode1(13 downto 10);
				end if; 
				
				o_register1_r1(6) <= '1';
				o_register1_r2(6) <= '1';
				o_register1_w(6)  <= '1';
			
				
				--VMULADD,SUB /VECTOR
				if v_sub1="00" then 
					
				--VMOVE
				elsif v_sub1="01" then 

				--VDIV/VSQRT
				elsif v_sub1="10" then 

				--VPU IMM
				else
					o_imm1f <= "010";
					o_opcode1i(9 downto 8) <= "00";
				end if; 
				
			end if; 
			
			
			
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
