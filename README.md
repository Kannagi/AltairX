# AltairX
New computer and new CPU.

The main processor of the AltairX is a VLIW In Order CPU.  
It has 4 internal memory:  

64 KiB L1 instruction Cache 4-way.  
32 KiB L1 data Cache RW 4-way.  
32 KiB L1 data Cache R 4-way.  
16 KiB L1 data Scratchpad memory.



Algorithms cache used is MRU based Pseudo LRU (PLRUm).  
If used 2-way Algorithms cache used is FIFO.

The processor has no branch prediction.

The number of instructions is done via a "Pairing" bit, when it is equal to 1, there is another instruction to be executed in parallel, 0 indicates the end of the bundle.  

<img src="graph/Altair_instruction.png?raw=true" alt="arch">   

The goal of this processor is to achieve the minimum CPU latency, and to solve the RAM latency problem.
To do this, the compiler will have to do two things:
- resolve pipeline conflicts
- indicate the instructions to be executed in parallel
- optimize pipeline
- prefetch instruction and data
- use SPM


The processor has 54 general registers of 64 bits, and 54 registers of 64 bits for the FPU.
It has 8 additional special registers: Zero, and 6 for bypass

It has 140 instructions distributed like this:  
ALU : 32  
LSU : 12  
BRU : 20  
FPU : 16x3  
EFU : 16  
MDU : 6 
CU : 4 


For floating point numbers in AltairX , it will not be 100% compatible with the standard with IEEE 754  
-Non-normalized numbers are not handled (they are equal to zero).  
-Infinite numbers cannot be handled (they are worth the max value).  
-NaN values are not manage (they are worth the max value).  
-Rounding is always towards 0  
-Exceptions are not handled   

The advantage of this processor is that it has a simple design, and requires little transistor for "high performance" and therefore consume / cost less than CISC/RISC Out Of Order processors.

### Todo list
- Translate the IR code (Clang) for AltairX
- Terminate virtual machine (cycle emulation and device management) 

## Target configuration
8 cores : AltairX 2.5 GHz  
DD3 1600 MHz , 8GB in a unified memory  


## Link
AltairX K1 ISA : https://docs.google.com/spreadsheets/d/1AmdMslRcXIX9pKGBSRJJcx2IvRyzBLjA61SzxmlEYf8/edit?usp=sharing   

AltairX K1 Pipeline : https://docs.google.com/spreadsheets/d/1u-XBjAyq8LOzAFcWMXsdAChMMzbmTIuZtzWQ7XDTRdk/edit?usp=sharing  

AltairX K1 Memory Map : https://docs.google.com/spreadsheets/d/1UQ15KpRRWncc_Ouzhas0W1uWuSIfjAODw8KD-2-AoDA/edit?usp=sharing  

AltairX IR ISA : https://docs.google.com/spreadsheets/d/19nOBbH_4KWaXxDSNA4JuZjaBble0VRrBxcVlEjTZ3iI/edit?usp=sharing

AltairX Executable Header : https://docs.google.com/spreadsheets/d/1g7mEhaBIVBJ75-5gJ_TrYiVJVTZHEJQnqN0XXUBX57g/edit?usp=sharing

Aldebaran G1 ISA : https://docs.google.com/spreadsheets/d/1LiSZbdd6wCpa-sZZ9uLg5eAyGxdpMl363waUP927xS4/edit?usp=sharing  

GPU todo list : https://docs.google.com/spreadsheets/d/1eRX1vLHEJdrAsx2u1OiycSSz82G3cboVMcu8gBYkgGA/edit?usp=sharing 

Compiler todo list and calling convention : https://docs.google.com/document/d/1E5NZOjEt6VJfSfHW3z5Fhfy_PPxvRx1Uxr-N-eZfBqc/edit?usp=sharing 

## AltairX Pipeline  
<img src="graph/Pipeline.png?raw=true" alt="Pipeline">

## Pipeline
<img src="graph/Pipeline_AX.png?raw=true" alt="Pipeline">

## Architecture
<img src="graph/Architecture.png" alt="Architecture">


