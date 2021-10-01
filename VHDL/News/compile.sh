#!/bin/bash
ghdl -s fetch.vhd decode.vhd ALU1.vhd mul.vhd div.vhd
ghdl -a fetch.vhd decode.vhd ALU1.vhd mul.vhd div.vhd



