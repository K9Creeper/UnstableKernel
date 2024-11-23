#!/bin/bash

export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

export PATH="$HOME/opt/cross/bin:$PATH"

asmCompile(){
    for file in "$@"; do
        echo "ASSEMBLY: Compiling $file to build/bin/AS_$(basename "$file" .s).o"
        $TARGET-as "$file" -o "build/bin/AS_$(basename "$file" .s).o"
    done
}

cCompile(){
    for file in "$@"; do
        echo "GCC: Compiling $file to build/bin/GCC_$(basename "$file" .c).o"
        $TARGET-gcc -c "$file" -o "build/bin/GCC_$(basename "$file" .c).o" -std=gnu99 -ffreestanding -O2 -Wall -Wextra
    done
}

cleanBuild(){
    echo "Cleaning up previous build files..."
    rm -f build/bin/*.o
    rm -f build/*.bin
}

clear
./clean.sh

echo "-------------------------------"
echo "Compilation Process Started"
echo "-------------------------------"
echo ""

# Assembly Compilation
echo "ASSEMBLY Compilation"
echo "---------------------"
asmCompile $(find source/ -name "*.s")
echo ""

# GCC Compilation
echo "GCC Compilation"
echo "---------------"
cCompile $(find source/ -name "*.c")
echo ""

# Linker Compilation
echo "Linking with GCC"
echo "----------------"
i686-elf-gcc -T linker.ld -o build/build.bin -ffreestanding -O2 -nostdlib build/bin/GCC_kernel.o build/bin/AS_boot.o -lgcc
echo ""

echo "-------------------------------"
echo "Compilation Ended"
echo "-------------------------------"
echo ""

echo "Copying build/build.bin to usb/boot/build.bin"
cp build/build.bin iso/boot/build.bin
cleanBuild
echo ""

# GRUB-MKIMAGE
echo "-------------------------------"
echo "GRUB-MKIMAGE Process"
echo "-------------------------------"
$HOME/src/grub-for-windows/grub-mkimage.exe -o iso/boot/grub/grub.img -O i386-pc -p iso/boot/grub iso9660 part_msdos normal linux
echo ""

# Genisoimage
echo "-------------------------------"
echo "Genisoimage Process"
echo "-------------------------------"
genisoimage -o iso/os.iso -b boot/grub/grub.img -no-emul-boot -boot-load-size 4 -boot-info-table iso
echo ""

echo "-------------------------------"
echo "Build Complete!"
echo "-------------------------------"