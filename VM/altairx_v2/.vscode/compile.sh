#!/bin/sh

g++  -O3 -Wall -Wextra -pedantic -Wno-unused-parameter -fomit-frame-pointer -flto -Iinclude -c src/main.cpp -o obj/src/main.o
g++  -O3 -Wall -Wextra -pedantic -Wno-unused-parameter -fomit-frame-pointer -flto -Iinclude -c src/altairx.cpp -o obj/src/altairx.o
g++  -O3 -Wall -Wextra -pedantic -Wno-unused-parameter -fomit-frame-pointer -flto -Iinclude -c src/cpu_execute.cpp -o obj/src/cpu_execute.o
g++  -O3 -Wall -Wextra -pedantic -Wno-unused-parameter -fomit-frame-pointer -flto -Iinclude -c src/debug.cpp -o obj/src/debug.o
g++  -O3 -Wall -Wextra -pedantic -Wno-unused-parameter -fomit-frame-pointer -flto -Iinclude -c src/io.cpp -o obj/src/io.o
g++  -O3 -Wall -Wextra -pedantic -Wno-unused-parameter -fomit-frame-pointer -flto -Iinclude -c src/ax_execute.cpp -o obj/src/ax_execute.o

g++ -s   obj/src/main.o obj/src/altairx.o obj/src/cpu_execute.o obj/src/debug.o obj/src/io.o obj/src/ax_execute.o  -o bin/vm_altairx 


