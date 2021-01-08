#!/bin/sh
read message
echo \"$message\"
#git init
git add git.sh
git add ISA/
git add OS/
git add CPU.png
git add Pipeline2.png
git add Pipeline3.png
git add Pipeline4.png
git add Pipeline.txt
git add MemoryMap.txt
git add VLIW_Unit.txt
git add ISA.txt
git add die.png
git add README.md
git add vasm

git add GPU/
git add GPU.png
git add dieGPU.png
git add dieGPU2.png
git add dieGPU3.png
git add PipelineGPU.png

git commit -m "$message"
#git remote add origin https://github.com/Kannagi/Altair.git/ 
#git push -u origin master
git push



