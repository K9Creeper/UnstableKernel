#!/bin/bash

export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

export PATH="$HOME/opt/cross/bin:$PATH"

asmCompile(){ for file in "$@"; do
        $TARGET-as "$file" -o "build/bin/AS_$(basename "$file" .s).o"
        echo "ASSEMBLY...   Compiled $file to build/bin/AS_$(basename "$file" .s).o"
    done
}

cCompile(){
    for file in "$@"; do
        $TARGET-gcc -c "$file" -o "build/bin/GCC_$(basename "$file" .c).o" -std=gnu99 -ffreestanding -O2 -Wall -Wextra
        echo "GCC...    Compiled $file to build/bin/GCC_$(basename "$file" .c).o"
    done
}

cleanBuild(){
    rm -f build/bin/*.o
    rm -f build/*.bin
}

clear

./clean.sh

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

cp build/build.bin iso/boot/build.bin

cleanBuild

echo ""
echo ""
echo "------------------"
echo "GRUB-MKIMAGE"
echo ""
echo ""

$HOME/src/grub-for-windows/grub-mkimage.exe -o iso/boot/grub/grub.img -O i386-pc -p iso/boot/grub iso9660 part_msdos normal linux

echo ""
echo ""
echo "------------------"

echo ""
echo ""
echo "------------------"
echo "Genisoimage"
echo ""
echo ""

genisoimage -o iso/os.iso -b boot/grub/grub.img -no-emul-boot -boot-load-size 4 -boot-info-table iso

echo ""
echo ""
echo "------------------"