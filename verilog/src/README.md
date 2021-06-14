# Verilog CPU code

This directory contain all the verilog code necessary for the CPU to work

## Syntax
The verilog syntax used is meant to be understood by most of the opensource Verilog simulator / synthesis tools available in 2021.

Therefore we prefer:
```
module module_name (
    clk,
    a,
    b,
);
parameter PARAMETER = 42;
input wire clk;
input wire a;
output reg [1:0] b;
```
Over:
```
module module_name #(
    parameter PARAMETER = 42,
) (
    input   wire a_clk,
    input   wire a,
    output  reg [1:0] b,
);
```