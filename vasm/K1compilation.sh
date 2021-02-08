#!/bin/bash
clear
rm vasmK1_mot
make CPU=K1 SYNTAX=mot
./vasmK1_mot -Fbin test.asm -o test
#ghex test

