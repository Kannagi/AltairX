#!/bin/bash
clear
rm obj/K1_mot_cpu.o
make CPU=K1 SYNTAX=mot
./vasmK1_mot -Fbin test.asm -o test
#ghex test

