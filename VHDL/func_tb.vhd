library IEEE;
use IEEE.std_logic_1164.all;

entity t_func is
end t_func;

architecture t_behaviour of t_func is

	component init_core is
		port(
			Clk	: in std_logic;
			o_wri	: out std_logic;
						
			o_DMA_ADR		: out  std_logic_vector(15 downto 0);
			o_PC				: out  std_logic_vector(15 downto 0)
			);
	end component init_core;
	
	component register_internal is
		port(
			i_wri0	: in std_logic;
			i_wri1	: in std_logic;
			i_wri2	: in std_logic;
			i_wri3	: in std_logic;
			
			io_PC		: inout  std_logic_vector(15 downto 0);
						
			i_PC0		: in  std_logic_vector(15 downto 0);
			i_PC1		: in  std_logic_vector(15 downto 0);
			i_PC2		: in  std_logic_vector(15 downto 0);
			i_LR			: in  std_logic_vector(15 downto 0);
			
			o_PC			: out  std_logic_vector(15 downto 0);
			o_LR			: out  std_logic_vector(15 downto 0)
			);
	end component register_internal;
	-------------------------------------------------
	
	component fetch is
		port(
			Clk	: in std_logic;
			o_s_fetch	: out std_logic;
			o_wri	: out std_logic;
						
			i_DMA_DATA		: in  std_logic_vector(127 downto 0);
			i_DMA_ADR		: in  std_logic_vector(15 downto 0);
			
			o_opcode1		: out  std_logic_vector(31 downto 0);
			o_opcode2		: out  std_logic_vector(31 downto 0);
			o_opcode3		: out  std_logic_vector(31 downto 0);
			o_opcode4		: out  std_logic_vector(31 downto 0);
			

			
			i_PC				: in  std_logic_vector(15 downto 0);
			o_PC				: out  std_logic_vector(15 downto 0)
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
		
			
			i_PC				: in  std_logic_vector(15 downto 0);
			o_PC				: out  std_logic_vector(15 downto 0);
			
			cycle			: inout  integer
			);
	end component decode;
	
	
	---------------------------------------------
	constant ClockFrequency : integer := 1000e6; -- 100 MHz
	constant ClockPeriod    : time    := 1 ps;

	signal Clk    : std_logic := '1';
	signal Clkinit    : std_logic := '0';
	signal wri0	: std_logic := '0';
	signal wri1	: std_logic := '0';
	signal wri2	: std_logic := '0';
	signal wri3	: std_logic := '0';

	signal opcode1_t			: std_logic_vector(31 downto 0);
	signal opcode2_t			: std_logic_vector(31 downto 0);
	signal opcode3_t			: std_logic_vector(31 downto 0);
	signal opcode4_t			: std_logic_vector(31 downto 0);
	
	signal s_fetch_t				: std_logic;
	
	signal size1_t				: std_logic_vector(3 downto 0);
	signal size2_t				: std_logic_vector(3 downto 0);
	signal size3_t				: std_logic_vector(3 downto 0);
	signal size4_t				: std_logic_vector(3 downto 0);
	
	
	signal unit1_t				: std_logic_vector(3 downto 0);
	signal unit2_t				: std_logic_vector(3 downto 0);
	signal unit3_t				: std_logic_vector(3 downto 0);
	signal unit4_t				: std_logic_vector(3 downto 0);
	
	
	signal ISRAM_DMA_DATA_t	: std_logic_vector(127 downto 0);
	signal ISRAM_DMA_ADR_t		: std_logic_vector(15 downto 0);
	
	signal r_PC_t				: std_logic_vector(15 downto 0);
	signal PC_t				: std_logic_vector(15 downto 0);
	signal PC_t0				: std_logic_vector(15 downto 0);
	signal PC_t1				: std_logic_vector(15 downto 0);
	signal PC_t2				: std_logic_vector(15 downto 0);
	signal PC_t3				: std_logic_vector(15 downto 0);
		
	signal LR_t				: std_logic_vector(15 downto 0);
	
	signal cycle_t				:integer :=0;
	---------------------------------------------
	begin	
	
		module_fetch: fetch
		port map (
				
				o_s_fetch => s_fetch_t,
				o_wri    => wri0,

				Clk    => Clk,
				o_opcode1 => opcode1_t,
				o_opcode2 => opcode2_t,
				o_opcode3 => opcode3_t,
				o_opcode4 => opcode4_t,
				
				i_DMA_DATA => ISRAM_DMA_DATA_t,
				i_DMA_ADR  =>  ISRAM_DMA_ADR_t,
				
				i_PC  => PC_t,
				o_PC  => PC_t0
				);
		--
		module_decode: decode
		port map (
				
				Clk    => Clk,
				cycle => cycle_t,
				s_fetch => s_fetch_t,
				
				i_opcode1 => opcode1_t,
				i_opcode2 => opcode2_t,
				i_opcode3 => opcode3_t,
				i_opcode4 => opcode4_t,
				
				o_unit1 => unit1_t,
				o_unit2 => unit2_t,
				o_unit3 => unit3_t,
				o_unit4 => unit4_t,

				
				i_PC  => PC_t,
				o_PC  => PC_t1
				);
		--------------------------------------------------------------------
		module_init_core: init_core
		port map (
				
				Clk    => Clkinit,
				o_wri    => wri2,
				
				o_DMA_ADR  =>  ISRAM_DMA_ADR_t,
				o_PC  => PC_t2
				);
				
		module_register_internal: register_internal
		port map (
				io_PC  => r_PC_t,
				
				i_wri0    => wri0,
				i_wri1    => wri1,
				i_wri2    => wri2,
				i_wri3    => wri3,
				
				i_LR  => LR_t,
				o_LR  => LR_t,
				
				i_PC0  => PC_t0,
				i_PC1  => PC_t1,
				i_PC2  => PC_t2,
				
				o_PC  => PC_t
				);
		--
		-- Process for generating the clock
		--Clk <= not Clk after ClockPeriod ;
			---------------------------------------------
			
		process
			begin
				
				-- inputs which produce '1' on the output
				Clkinit <= '1';	--0
				wait for 1 fs;

				Clk <= '0';	--0
				wait for 1 fs;
				Clk <= '1';
				wait for 1 fs;
				Clk <= '0';	--0
				wait for 1 fs;
				Clk <= '1';
				wait for 1 fs;
				Clkinit <= '1';	--0
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
