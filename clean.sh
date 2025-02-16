#!/bin/bash

clear

rm -f build/bin/*.o
rm -f build/*.bin
rm -f iso/boot/*.bin
rm -f iso/boot/grub/*.img
rm -f iso/*.iso

> debug-log.txt