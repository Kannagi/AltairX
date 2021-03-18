#!/bin/bash
ghdl -s mul.vhd fetch.vhd register.vhd execute_ALU1.vhd decode.vhd func_tb.vhd
ghdl -a mul.vhd fetch.vhd register.vhd execute_ALU1.vhd decode.vhd func_tb.vhd
ghdl -e t_func
ghdl -r t_func --vcd=prg.vcd



