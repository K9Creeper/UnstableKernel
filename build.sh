#!/bin/bash

nasmCompile(){
    C:/msys64/ucrt64/bin/nasm.exe "$@" -f elf32 -o build/bin/NASM_$(basename $@ .asm).o
    echo "NASM  | Compiled $@ to build/bin/NASM_$(basename $@ .asm).o"
}

clangCompile(){
    C:/msys64/mingw64/bin/clang.exe -c -target i686-none-elf -o build/bin/CLANG_$(basename $@ .c).o -ffreestanding -w -mno-sse -Wall "$@"
    echo "Clang | Compiled $@ to build/bin/CLANG_$(basename $@ .c).o"
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

nasmCompile $(find source/ -name "*.asm")
clangCompile $(find source/ -name "*.c")

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
