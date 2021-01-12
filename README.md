# Altair
New computer and new CPU PoC

The main processor of the Altair K1 is a VLIW In Order CPU.  
It has 4 internal memory:  
A SRAM (Scratchpad) memory for 128 KiB instructions.  
Another SRAM memory for 128 KiB data.  
A 32 KiB cache memory. (Direct mapped)  
256B SRAM memory for I / O.  

The processor has no branch prediction, it will be based on the delay slot (1 cycle for Fetch) and 1 decode cycle + Jump (Delay)

He can also change the number of instructions with an xchg instruction.  
Which allows you to switch to 2 or 4 instructions / cycle mode.  
all this to avoid too many "nop" and therefore not waste unnecessary space in the SRAM.

The goal of this processor is to reach the minimum of latency, and to solve the problem of latency of the RAM.  
For this, the compiler will have to do two things:  
- resolve pipeline conflicts  
- preload the data in advance with a DMA

This is a technique used on consoles like the Playstation 2 and 3, we have to make a double buffer, and therefore execute and read our data in buffer 1, while we preload our data in buffer 2.  
Then we execute the buffer 2 and we preload the buffer 1 and so on.

The L1 I/O SRAM, is not made to read / write hardware, but to control another CPU or core.  
The configuration thought is that the main processor can indicate I/O for the other cores, while keeping a security (for the OS).  
The main core also has an I/O SRAM, if you want, for example, to have 2 processors on a machine. 

For floating point numbers Altair does not use the float / double type (IEEE 754), it uses the posits instead: https://posithub.org/khub_widget  

For the calculation unit it has:  
4ALU 2VPU 2LSU PDIV DIV BRU AGU CMP

The advantage of this processor is that it has a simple design, and requires little transistor for "high performance" and therefore consume / cost less than RISC Out Of Order processors.

### Todo list
- Finish the assembler program
- Make documentation (pdf / html) (ISA and hardware)
- Translate the IR code (Clang) for Altair
- Make the virtual machine

## Pipeline
<img src="Pipeline4.png?raw=true" alt="Pipeline" width="887" height="785">

## CPU Die
<img src="CPU.png?raw=true" alt="CPU" width="500" height="400">


## CPU Core
<img src="die.png?raw=true" alt="CPU" width="256" height="256">

