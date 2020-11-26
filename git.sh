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
git add Pipeline.txt
git add MemoryMap.txt
git add VLIW_Unit.txt
git add ISA.txt


git commit -m "$message"
git remote add origin https://github.com/Kannagi/Altair.git/ 
git push -u origin master
