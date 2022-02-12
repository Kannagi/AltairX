#!/bin/bash
./vasmAltairK1_mot -Fbin test.asm -o prog.bin
./vm_altairx prog.bin
#ghex test

