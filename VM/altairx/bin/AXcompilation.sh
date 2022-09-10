#!/bin/bash
./vasmAltairX_mot -Fbin test.asm -o prog.bin
./vm_altairx prog.bin
#ghex test

