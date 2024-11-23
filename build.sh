#!/bin/bash

nasmCompile(){
    for file in "$@"; do
        C:/cygwin64/bin/nasm.exe -f elf32 "$file" -o "build/bin/NASM_$(basename "$file" .asm).o"
        echo "NASM  | Compiled $file to build/bin/NASM_$(basename "$file" .asm).o"
    done
}

cCompile(){
    for file in "$@"; do
        C:/cygwin64/bin/clang-8.exe -c --target=i686-none-elf -o "build/bin/CLANG_$(basename "$file" .c).o" -ffreestanding -w -mno-sse -Wall "$file"
        echo "CLANG | Compiled $file to build/bin/CLANG_$(basename "$file" .c).o"
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
cCompile $(find source/ -name "*.c")

C:/cygwin64/bin/ld.exe -m i386pe -T linker.ld -o build/build.bin -nostdlib -static build/bin/*.o

C:/cygwin64/bin/objcopy.exe -I elf32-i386 build/build.bin build/build


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
