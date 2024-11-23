#!/bin/bash

asmCompile(){ for file in "$@"; do
        i686-elf-as "$file" -o "build/bin/AS_$(basename "$file" .s).o"
        echo "ASSEMBLY...   Compiled $file to build/bin/AS_$(basename "$file" .s).o"
    done
}

cCompile(){
    for file in "$@"; do
        i686-elf-gcc -c "$file" -o "build/bin/GCC_$(basename "$file" .c).o" -std=gnu99 -ffreestanding -O2 -Wall -Wextra
        echo "GCC...    Compiled $file to build/bin/GCC_$(basename "$file" .c).o"
    done
}

cleanPrevBuilds(){
    rm -f build/bin/*.o
    rm -f build/*.bin
    rm -f usb/boot/*.bin
}

clear

cleanPrevBuilds

echo "------------------"
echo "Compilation Begin"
echo ""

echo ""
echo "ASSEMBLY Compilation"
echo "------------------"
echo ""
asmCompile $(find source/ -name "*.s")

echo ""
echo ""
echo "GCC Compilation"
echo "------------------"
echo ""

cCompile $(find source/ -name "*.c")

echo ""
echo ""
echo "Linker Compilation"
echo "------------------"
echo ""

i686-elf-gcc -T linker.ld -o build/build.bin -ffreestanding -O2 -nostdlib build/bin/GCC_kernel.o build/bin/AS_boot.o -lgcc

echo ""
echo ""
echo "Compilation End"
echo "------------------"

echo ""
echo ""
echo "Copying build/build.bin to usb/boot/build.bin"

cp build/build.bin usb/boot/build.bin

cleanPrevBuilds

echo ""
echo ""
echo "------------------"
echo ""
echo "Read usb/readme.md"
echo ""
echo "------------------"