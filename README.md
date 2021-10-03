# Altair
New computer and new CPU PoC

The main processor of the Altair K1 is a VLIW In Order CPU.  
It has 3 internal memory:  
128 KiB L1 data Scratchpad memory.  
128 KiB L1 instruction Scratchpad memory.  
32 KiB  L1 data cache (Set-associative 2/4 ways).  
16 KiB  L1 data instruction (Set-associative 2 ways).  

1 MiB L2 cache (Set-associative 4/8 ways).  
1 MiB L2 Scratchpad memory.  

The processor has no branch prediction, it will be based on the delay slot (1 cycle for Fetch) and 1 decode cycle + Jump (Delay)

The number of instructions is done via a "Pairing" bit, when it is equal to 1, there is another instruction to be executed in parallel, 0 indicates the end of the bundle.  

<img src="Altair_instruction.png?raw=true" alt="arch">   

The goal of this processor is to reach the minimum of latency, and to solve the problem of latency of the RAM.  
For this, the compiler will have to do two things:  
- resolve pipeline conflicts  
- preload the data in advance with a DMA

This is a technique used on consoles like the Playstation 2 and 3, we have to make a double buffer, and therefore execute and read our data in buffer 1, while we preload our data in buffer 2.  
Then we execute the buffer 2 and we preload the buffer 1 and so on.  

<img src="Single-and-Double-Buffering.png?raw=true" alt="arch">  

To resolve pipeline conflicts, it has an accumulator internal to the ALU and to the VFPU which is register 61.  
To avoid multiple writes to registers due to unsynchronized pipeline, there are two special registers P and Q (Product and Quotient) which are registers 62 and 63, to handle mul / div / sqrt etc etc.  

It also has the uncached accelerated to speed up reads only (cache miss lasts half the time).

For floating point numbers in Altair , it will not be 100% compatible with the standard with IEEE 754  
-Non-normalized numbers are not handled (they are equal to zero).  
-Infinite numbers cannot be handled (they are worth the max value).  
-NaN values are not manage (they are worth the max value).  
-Rounding is always towards 0  
-Exceptions are not handled   

For the calculation unit it has:  
2ALU+2ALU(32 bits) 2+2LSU 1VFPU/FDIV 1DIV/MUL BRU/CMP  

For the 32-bits version:  
-there will only be 2 ALUs  
-no Double  
-no EFU   


The advantage of this processor is that it has a simple design, and requires little transistor for "high performance" and therefore consume / cost less than CISC/RISC Out Of Order processors.

### Todo list
- Finish the assembler program
- Make documentation (pdf / html) (ISA and hardware)
- Translate the IR code (Clang) for Altair
- Make the virtual machine

## Target configuration
Main core : Altair K1 2.5 GHz  
Sub  core : Altair K1 2.5 GHz , 6 cores   
LPDDR4 3200 MHz , 8GB in a unified memory  
GPU Aldebaran G1 1 GHz , 4 CU , 512 GFlops  

## Link
Altair K1 ISA : https://docs.google.com/spreadsheets/d/1AmdMslRcXIX9pKGBSRJJcx2IvRyzBLjA61SzxmlEYf8/edit?usp=sharing   

ALTAIR K1 Pipeline : https://docs.google.com/spreadsheets/d/1u-XBjAyq8LOzAFcWMXsdAChMMzbmTIuZtzWQ7XDTRdk/edit?usp=sharing  

ALTAIR K1 Memory Map : https://docs.google.com/spreadsheets/d/1UQ15KpRRWncc_Ouzhas0W1uWuSIfjAODw8KD-2-AoDA/edit?usp=sharing  

ALTAIR IR ISA : https://docs.google.com/spreadsheets/d/19nOBbH_4KWaXxDSNA4JuZjaBble0VRrBxcVlEjTZ3iI/edit?usp=sharing

ALTAIR Executable Header : https://docs.google.com/spreadsheets/d/1g7mEhaBIVBJ75-5gJ_TrYiVJVTZHEJQnqN0XXUBX57g/edit?usp=sharing

Aldebaran G1 ISA : https://docs.google.com/spreadsheets/d/1LiSZbdd6wCpa-sZZ9uLg5eAyGxdpMl363waUP927xS4/edit?usp=sharing  

GPU todo list : https://docs.google.com/spreadsheets/d/1eRX1vLHEJdrAsx2u1OiycSSz82G3cboVMcu8gBYkgGA/edit?usp=sharing  


## Pipeline
<img src="AltairE0_diagram.png?raw=true" alt="Pipeline">

## Architecture intern 
<img src="Altair_arch_Intern.png?raw=true" alt="arch">

## Architecture Bus 
<img src="Altair_Bus2.png?raw=true" alt="arch">

## APU Die (700-900 MT) 40-50 mm² , 28 nm FD-SOI
<img src="APU.png?raw=true" alt="APU">


