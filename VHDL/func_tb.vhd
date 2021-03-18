library IEEE;
use IEEE.std_logic_1164.all;

entity t_func is
end t_func;

architecture t_behaviour of t_func is

	component fetch is
		port(
			Clk	: in std_logic;
			o_s_fetch	: out std_logic;
			
			i_SWT			: in  std_logic_vector(1 downto 0);
			
			i_DMA_DATA		: in  std_logic_vector(127 downto 0);
			i_DMA_ADR		: in  std_logic_vector(13 downto 0);
			
			o_opcode1		: out  std_logic_vector(31 downto 0);
			o_opcode2		: out  std_logic_vector(31 downto 0);
			o_opcode3		: out  std_logic_vector(31 downto 0);
			o_opcode4		: out  std_logic_vector(31 downto 0);
			

			
			i_PC				: in  std_logic_vector(13 downto 0);
			o_PC				: out  std_logic_vector(13 downto 0)
			);
	end component fetch;

	component decode is
		port(
			Clk	: in std_logic;
			s_fetch	: in std_logic;
			
			i_opcode1		: in  std_logic_vector(31 downto 0);
			i_opcode2		: in  std_logic_vector(31 downto 0);
			i_opcode3		: in  std_logic_vector(31 downto 0);
			i_opcode4		: in  std_logic_vector(31 downto 0);
			
			o_opcode1i	: out  std_logic_vector(9 downto 0);
			o_opcode2i	: out  std_logic_vector(9 downto 0);
			o_opcode3i	: out  std_logic_vector(9 downto 0);
			o_opcode4i	: out  std_logic_vector(9 downto 0);
			
			o_size1		: out  std_logic_vector(3 downto 0);
			o_size2		: out  std_logic_vector(3 downto 0);
			o_size3		: out  std_logic_vector(3 downto 0);
			o_size4		: out  std_logic_vector(3 downto 0);
			
			o_imm1		: out  std_logic_vector(19 downto 0);
			o_imm2		: out  std_logic_vector(19 downto 0);
			o_imm3		: out  std_logic_vector(19 downto 0);
			o_imm4		: out  std_logic_vector(19 downto 0);
			
			o_imm1f		: out  std_logic_vector(2 downto 0);
			o_imm2f		: out  std_logic_vector(2 downto 0);
			o_imm3f		: out  std_logic_vector(2 downto 0);
			o_imm4f		: out  std_logic_vector(2 downto 0);
			
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
		
			
			i_PC				: in  std_logic_vector(13 downto 0);
			o_PC				: out  std_logic_vector(13 downto 0);
			o_SWT			: out  std_logic_vector(1 downto 0);
			
			cycle			: inout  integer
			);
	end component decode;
	
	
	---------------------------------------------
	constant ClockFrequency : integer := 1000e6; -- 100 MHz
	constant ClockPeriod    : time    := 1 ps;

	signal Clk    : std_logic := '1';

	signal opcode1_t			: std_logic_vector(31 downto 0);
	signal opcode2_t			: std_logic_vector(31 downto 0);
	signal opcode3_t			: std_logic_vector(31 downto 0);
	signal opcode4_t			: std_logic_vector(31 downto 0);
	
	signal s_fetch_t				: std_logic;
	
	signal size1_t				: std_logic_vector(3 downto 0);
	signal size2_t				: std_logic_vector(3 downto 0);
	signal size3_t				: std_logic_vector(3 downto 0);
	signal size4_t				: std_logic_vector(3 downto 0);
	
	
	signal SWT_t				: std_logic_vector(1 downto 0) := "00";

	signal ISRAM_DMA_DATA_t	: std_logic_vector(127 downto 0);
	signal ISRAM_DMA_ADR_t		: std_logic_vector(13 downto 0):= "00000000000000";
	
	signal PC_t				: std_logic_vector(13 downto 0) := "00000000000000";
	signal cycle_t				:integer :=0;
	---------------------------------------------
	begin	
	
		module_fetch: fetch
		port map (
				i_SWT  => SWT_t,
				
				o_s_fetch => s_fetch_t,

				Clk    => Clk,
				o_opcode1 => opcode1_t,
				o_opcode2 => opcode2_t,
				o_opcode3 => opcode3_t,
				o_opcode4 => opcode4_t,
				
				i_DMA_DATA => ISRAM_DMA_DATA_t,
				i_DMA_ADR  =>  ISRAM_DMA_ADR_t,
				
				i_PC  => PC_t,
				o_PC  => PC_t
				);
		--
		module_decode: decode
		port map (
				
				o_SWT  => SWT_t,
				Clk    => Clk,
				cycle => cycle_t,
				s_fetch => s_fetch_t,
				
				i_opcode1 => opcode1_t,
				i_opcode2 => opcode2_t,
				i_opcode3 => opcode3_t,
				i_opcode4 => opcode4_t,
				
				o_size1 => size1_t,
				o_size2 => size2_t,
				o_size3 => size3_t,
				o_size4 => size4_t,
				
				i_PC  => PC_t,
				o_PC  => PC_t
				);
		--
		-- Process for generating the clock
		--Clk <= not Clk after ClockPeriod ;
			---------------------------------------------
			
		process
			begin
				
				-- inputs which produce '1' on the output
				
				--SWT_t <="00";
				PC_t <="00000000000000";
				Clk <= '0';	--0
				wait for 1 fs;
				Clk <= '1';
				wait for 1 fs;
				
				--wait for 200 ps;
				--wait for 200 ps;
				--wait for 200 ps;
				
				wait;
			end process;
		--
	--
--
end t_behaviour;
