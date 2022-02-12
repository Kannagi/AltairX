`timescale 1ns/10ps // For simulation purpose
/*
 * True dual port, dual clock, vendor independant block RAM.
 * Note: This code is meant for FPGA but can be ported to ASIC.
 *
 * Tested on FPGAs: Xilinx XST, Xilinx Vivado, Altera Quartus, Yosys ECP5
 */

module bram_dual_port (
    a_clk, // port a clock, read/write occurs on the rising edge
    a_wr, // port a write/read, 1'b1 to write
    a_addr, // port b address, undefined behavior if both are accessing the memory at the same time
    a_din, // port a data in
    a_dout, // port a data out

    b_clk,
    b_wr,
    b_addr,
    b_din,
    b_dout
);

parameter DATASIZE = 32;
parameter ADDRSIZE = 10;

input wire a_clk;
input wire a_wr;
input wire [ADDRSIZE-1:0] a_addr;
input wire [DATASIZE-1:0] a_din;
output reg [DATASIZE-1:0] a_dout;

input wire b_clk;
input wire b_wr;
input wire [ADDRSIZE-1:0] b_addr;
input wire [DATASIZE-1:0] b_din;
output reg [DATASIZE-1:0] b_dout;

reg [DATASIZE-1:0] mem [(2**ADDRSIZE)-1:0];

always @ (posedge a_clk)
begin
    if (a_wr)
    begin
        a_dout <= a_din;
        mem[a_addr] <= a_din;
    end
    else
        a_dout <= mem[a_addr];
end

always @ (posedge b_clk)
begin
    if (b_wr)
    begin
        b_dout <= b_din;
        mem[b_addr] <= b_din;
    end else
        b_dout <= mem[b_addr];
end

// For simulation only
always @ (posedge b_clk)
    // If at least one of them is writing while the other
    // is reading/writing the same address we have a problem
	if (a_addr == b_addr && (b_wr || a_wr))
		$display("BRAM_DUAL_PORT: Error simultanous memory access");

endmodule