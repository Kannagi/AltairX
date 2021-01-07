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


For the calculation unit it has:  
4ALU 2VFPU 2LSU FDIV DIV BRU AGU

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

## ---------------------------------------------------  

# Altair GPU

Altair for GPU shares many points in common with the CPU version, it is a simplified version oriented parallel computing and rendering.

So Altair G1 is a VLIW in order processor.  
It has the same L1 cache count.  
It has 32MB eDRAM memory for textures.  
A 128 KB SRAM for texture indexes (to manage palettes or VQ).  
L1 and L2 cache memory is intended for textures that are too large  
For example if we use two non-compressive textures of 4096x4096, it does not enter the texture buffer, and it will be managed by the cache (longer and therefore with a lot of missing cache).  
To avoid using the cache for managing images (or 2D rendering) with a large texture, there will be a “Sprite” type geometry management.  

The calculation unit can only execute 2 instructions / cycle.  
It has an ALU (no division), CMP, BRU, LSU and AGU.  
And 32 VPU (Vector Posits Unit), 32 PDIV (Posits Division), 32 EFU (Elementary Function Unit).  
The LSU will have specific instructions to load / store 32 vectors at a time.  
he has access to EDRAM which will take several cycles.  

The EFU will have all the necessary instructions to speed up calculations and 3D rendering.  
(sin, cos, arctan, dot, rasterization, Min / Max, Clip, Culling / Sorting etc etc).  


It has a L2 SRAM 4 MB to manage the code or the data of the different shader.  

The Pixel Process Unit is used to manage the Frame buffer / Zbuffer and Blending.  
The compute unit does the various necessary calculations and indicates a pixel line to write.  
The PPU takes care of this, internally it will have a buffer and a queue to manage the different line draw commands of the different Compute Unit.  

## Pipeline GPU
<img src="GPU.png?raw=true" alt="Pipeline" width="584" height="650">

## Pipeline Compute Unit
<img src="PipelineGPU.png?raw=true" alt="Compute Unit" width="640" height="840">

## GPU Die
<img src="dieGPU.png?raw=true" alt="GPU" width="640" height="1024">
