#!/bin/bash

nasmCompile(){
    C:/msys64/ucrt64/bin/nasm.exe $1 -f elf32 -o $2
    echo "NASM  | Compiled $1 to $2"
}

clangCompile(){
    C:/msys64/mingw64/bin/clang.exe -c -target i686-none-elf -o $2 -ffreestanding -w -mno-sse -Wall $1
    echo "Clang | Compiled $1 to $2"
}

clean(){
    rm -f build/*.bin
    rm -f build/bin/*.o
}

rm -f usb/boot/*.bin

clean

clear

echo "-------------------------------------"
echo "| Compilation Start |"
echo ""

nasmCompile source/boot/boot.asm build/bin/boot.o
clangCompile source/kernel/kernel.c build/bin/kernel.o

C:/msys64/ucrt64/bin/ld.exe -m i386pe -T linker.ld -o build/build.bin -static -nostdlib build/bin/*.o

cp build/build.bin usb/boot/build.bin

clean

echo ""
echo "| Compilation Completed |"
echo "-------------------------------------"

echo ""
echo "-------------------"
echo "READ: usb/readme.md"
echo "-------------------"
