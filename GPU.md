# Altair GPU

Altair for GPU shares many points in common with the CPU version, it is a simplified version oriented parallel computing and rendering.

So Altair G1 is a VLIW in order processor.  
It has the same L1 cache count.  
It has 32-64MB 1T-SRAM memory for textures.  
A 128 KB SRAM for texture indexes (to manage palettes or VQ).  

L2 cache memory is intended for textures that are too large  
For example if we use two non-compressive textures of 4096x4096, it does not enter the texture buffer, and it will be managed by the cache (longer and therefore with a lot of missing cache).  
To avoid using the cache for managing images (or 2D rendering) with a large texture, there will be a “Sprite” type geometry management.  

The calculation unit can only execute 2 instructions / cycle.  
It has an ALU (no division), CMP, BRU, LSU and AGU.  
And 32 VFPU (Vector Float Point Unit), 32 FDIV (Float Division), 32 EFU (Elementary Function Unit).  
The LSU will have specific instructions to load / store 32 vectors at a time.  
it has access to EDRAM / L2 SRAM / L2 Cache which will take several cycles, read only.  

The EFU will have all the necessary instructions to speed up calculations and 3D rendering.  
(sin, cos, arctan, dot, rasterization, Min / Max, Clip, Culling , Sorting etc etc).  

L2 SRAM (4 MB) stores the vertex / textcoord / normal / index / color / other + code of the shaders.  
If this is insufficient, it is stored on the L2 cache.  

The Pixel Process Unit is used to manage the Frame buffer / Zbuffer and Blending.  
The compute unit does the various necessary calculations and indicates a pixel line to write.  
The PPU takes care of this, internally it will have a buffer and a queue to manage the different line draw commands of the different Compute Unit.  

For the textures, here is what will have to manage:  
- RGB/RGBA 32 bits/16 bits
- brightness/gray 8 bits
- DXT1 and DXT3
- VQ Compressed
- Palette 8 bpp and 4 bpp

## Pipeline GPU
<img src="GPU.png?raw=true" alt="Pipeline" width="584" height="650">

## Pipeline Compute Unit
<img src="PipelineGPU.png?raw=true" alt="Compute Unit" width="640" height="840">


