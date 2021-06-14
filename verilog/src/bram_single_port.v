`timescale 1ns/10ps // For simulation purpose
/*
 * Single port, single clock, vendor independant RAM.
 * Note: This code is meant for FPGA but can be ported to ASIC.
 *
 * Tested on Altera Quartus, Yosys ECP5
 */

 module bram_single_port (
    clk, // clock, write/read on rising edge
    wr, // write/read, write it 1'b1
    addr, // address
    din, // data in
    dout // data out
 );

parameter DATASIZE = 32;
parameter ADDRSIZE = 10;

input wire clk;
input wire wr;
input wire [ADDRSIZE-1:0] addr;
input wire [DATASIZE-1:0] din;
output reg [DATASIZE-1:0] dout;

reg [DATASIZE-1:0] mem [(2**ADDRSIZE)-1:0];

always @ (posedge clk)
begin
    if (wr)
    begin
        dout <= din;
        mem[addr] <= din;
    end else
        dout <= mem[addr];
end

endmodule