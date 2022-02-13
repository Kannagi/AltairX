# Aldebaran GPU

Altair for GPU shares many points in common with the CPU version, it is a simplified version oriented parallel computing and rendering.

So Aldebaran G1 is a VLIW in order processor.  
128 KiB L1 data Scratchpad memory.  
128 KiB L1 instruction Scratchpad memory.  
32 KiB  L1 data cache (Set-associative 4 ways).  

1 MiB L2 Texture cache (Set-associative 4 ways).  
2 MiB L2 cache Scratchpad memory.  
1 MiB L2 Buffer cache (Set-associative 4 ways).  

The calculation unit can only execute 2 instructions / cycle.  
And 16 VFPU (Vector Float Point Unit), 32 FDIV (Float Division), 32 EFU (Elementary Function Unit).  
AND 16 ALU (32 bits , no division), 32 LSU.

The EFU will have all the necessary instructions to speed up calculations and 3D rendering.  
(sin, cos, arctan, dot, rasterization, Min / Max, Clip, Culling , Sorting etc etc).  

L2 SPM (2 MiB) stores the vertex / textcoord / normal / index / color / other + code of the shaders.  
If this is insufficient, it is stored on the L2 cache.  

The Pixel Process Unit is used to manage the Frame buffer / Zbuffer and Blending.  
The compute unit does the various necessary calculations and indicates a pixel line to write.  
The PPU takes care of this, internally it will have a buffer and a queue to manage the different line draw commands of the different Compute Unit.  

For the textures, here is what will have to manage:  
- RGB/RGBA 32 bits/16 bits
- brightness/gray 8 bits
- DXT1/DXT3/DXT7
- VQ Compressed
- Palette 8 bpp and 4 bpp


## Pipeline GPU
<img src="AldebaranG1_diagram.png?raw=true" alt="Pipeline">

## Pipeline Compute Unit
<img src="Pipeline_Graphics_Diagram.png?raw=true" alt="Compute Unit">


