#!/bin/bash

nasmCompile(){
    for file in "$@"; do
        C:/msys64/ucrt64/bin/nasm.exe "$file" -f elf32 -o "build/bin/NASM_$(basename "$file" .asm).o"
        echo "NASM  | Compiled $file to build/bin/NASM_$(basename "$file" .asm).o"
    done
}

clangCompile(){
    for file in "$@"; do
        C:/msys64/mingw64/bin/clang.exe -c -target i386-pe -o "build/bin/CLANG_$(basename "$file" .c).o" -ffreestanding -w -mno-sse -Wall "$file"
        echo "Clang | Compiled $file to build/bin/CLANG_$(basename "$file" .c).o"
    done
}

clean(){
    rm -f build/*.bin
    rm -f build/bin/*.o
}

mkdir -p build/bin usb/boot

rm -f usb/boot/*.bin

clean

clear

echo "-------------------------------------"
echo "| Compilation Start |"
echo ""

nasmCompile $(find source/ -name "*.asm")
clangCompile $(find source/ -name "*.c")

C:/msys64/ucrt64/bin/ld.exe -m i386pe -T linker.ld -o build/build.bin -nostdlib -static build/bin/*.o

cp build/build.bin usb/boot/build.bin

echo ""
echo "| Compilation Completed |"
echo "-------------------------------------"

echo ""
echo "-------------------"
echo ""
echo "READ: usb/readme.md"
echo ""
echo "-------------------"
