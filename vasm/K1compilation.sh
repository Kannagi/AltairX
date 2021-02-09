#!/bin/bash
clear
rm vasmAltairK1_mot
make CPU=AltairK1 SYNTAX=mot
./vasmAltairK1_mot -Fbin test.asm -o test
#ghex test

