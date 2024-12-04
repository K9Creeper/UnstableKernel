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
        # Extract the relative path from the file (excluding the extension)
        relative_path=$(dirname "$file")
        filename=$(basename "$file" .s)
        output_file="build/bin/AS_${relative_path//\//_}_$filename.o"

        echo -e "ASSEMBLY: Compiling \033[1m$file\033[0m to $output_file"
        $TARGET-as "$file" -o "$output_file"
        if ! [ -f "$output_file" ]; then
            export COMP_ERROR="1"
        fi
    done
}

# Compile C++ files
cppCompile(){
    for file in "$@"; do
        # Extract the relative path from the file (excluding the extension)
        relative_path=$(dirname "$file")
        filename=$(basename "$file" .cpp)
        output_file="build/bin/G++_${relative_path//\//_}_$filename.o"

        echo -e "G++: Compiling \033[1m$file\033[0m to $output_file"
        $TARGET-g++ -c "$file" -o "$output_file" -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -fpermissive
        if ! [ -f "$output_file" ]; then
            export COMP_ERROR="1"
        fi
    done
}

# Compile C files
cCompile(){
    for file in "$@"; do
        # Extract the relative path from the file (excluding the extension)
        relative_path=$(dirname "$file")
        filename=$(basename "$file" .c)
        output_file="build/bin/GCC_${relative_path//\//_}_$filename.o"

        echo -e "GCC: Compiling \033[1m$file\033[0m to $output_file"
        $TARGET-gcc -c "$file" -o "$output_file" -std=gnu99 -ffreestanding -O2 -Wall -Wextra
        if ! [ -f "$output_file" ]; then
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
$TARGET-gcc -T linker.ld -o build/build.bin -ffreestanding -O2 -nostdlib build/bin/*.o -lgcc
echo -e "\n"

echo -e "-------------------------------"
echo -e "Compilation Ended"
echo -e "-------------------------------\n"

if [ -f "build/build.bin" ] && [ $COMP_ERROR == "0" ]; then
    # Check whether if the file has a valid Multiboot2 header.
if grub-file --is-x86-multiboot2 build/build.bin; then
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