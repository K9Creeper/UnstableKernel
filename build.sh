#!/bin/bash

export TARGET=i686-elf

# This must be done for the cross-compiler to be recognized
export PATH=$HOME/opt/cross/bin:$PATH

export COMP_ERROR="0"

# Clean the build directory
cleanBuild(){
    echo -e "\nCleaning up previous build files..."
    rm -f build/bin/*.o
    rm -f build/*.bin
}

# Compile Assembly files
asmCompile(){
    for file in "$@"; do
        echo -e "ASSEMBLY: Compiling \033[1m$file\033[0m to build/bin/AS_$(basename "$file" .s).o"
        $TARGET-as "$file" -o "build/bin/AS_$(basename "$file" .s).o"
        if ! [ -f "build/bin/AS_$(basename "$file" .s).o" ]; then
            export COMP_ERROR="1"
        fi
    done
}

# Compile C++ files
cppCompile(){
    for file in "$@"; do
        echo -e "G++: Compiling \033[1m$file\033[0m to build/bin/G++_$(basename "$file" .cpp).o"
        $TARGET-g++ -c "$file" -o "build/bin/G++_$(basename "$file" .cpp).o" -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
        if ! [ -f "build/bin/G++_$(basename "$file" .cpp).o" ]; then
            export COMP_ERROR="1"
        fi
    done
}

# Compile C files
cCompile(){
    for file in "$@"; do
        echo -e "GCC: Compiling \033[1m$file\033[0m to build/bin/GCC_$(basename "$file" .c).o"
        $TARGET-gcc -c "$file" -o "build/bin/GCC_$(basename "$file" .c).o" -std=gnu99 -ffreestanding -O2 -Wall -Wextra
        if ! [ -f "build/bin/GCC_$(basename "$file" .c).o" ]; then
            export COMP_ERROR="1"
        fi
    done
}

clear
./clean.sh

mkdir build build/bin

echo -e "\n-------------------------------"
echo -e "Compilation Process Started"
echo -e "-------------------------------\n"

# Assembly Compilation
echo -e "\033[1mASSEMBLY Compilation\033[0m"
echo -e "---------------------"
asmCompile $(find source/ -name "*.s")
echo -e "\n"

# GCC Compilation
echo -e "\033[1mGCC Compilation\033[0m"
echo -e "---------------"
cCompile $(find source/ -name "*.c")
echo -e "\n"

# G++ Compilation
echo -e "\033[1mG++ Compilation\033[0m"
echo -e "---------------"
cppCompile $(find source/ -name "*.cpp")
echo -e "\n"

# Linker Compilation
echo -e "Linking with GCC"
echo -e "----------------"
i686-elf-gcc -T linker.ld -o build/build.bin -ffreestanding -O2 -nostdlib build/bin/*.o -lgcc
echo -e "\n"

echo -e "-------------------------------"
echo -e "Compilation Ended"
echo -e "-------------------------------\n"

if [ -f "build/build.bin" ] && [ $COMP_ERROR == "0" ]; then
    # Check whether if the file has a valid Multiboot header.
if grub-file --is-x86-multiboot build/build.bin; then
    echo -e "File has a valid Multiboot header.\n"

    # Copy the build to the USB boot directory
    echo -e "Copying build/build.bin to iso/boot/build.bin"
    cp build/build.bin iso/boot/build.bin

    cleanBuild

    rmdir build/bin
    rmdir build

    echo -e ""

    # Creating ISO with Grub
    echo -e "\033[1mGrub-Mkrescue\033[0m"
    echo -e "---------------"
    grub-mkrescue -o iso/os.iso iso
    echo -e "\n"


else
    echo -e "\033[1mFile does not have a valid Multiboot header.\033[0m"

    cleanBuild

    rmdir build/bin
    rmdir build

    echo -e ""
fi

else

echo -e "\033[1mCompilation Issue\033[0m"

cleanBuild

rmdir build/bin
rmdir build

echo -e ""

fi

export COMP_ERROR="0"