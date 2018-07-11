#!/bin/bash

echo Now assembling and compiling
nasm -f elf32 kernel.asm -o kernel.o

gcc -m32 -fno-stack-protector -Wall -c gdt.c -o gdt.o
gcc -m32 -fno-stack-protector -Wall -c console.c -o console.o
gcc -m32 -fno-stack-protector -Wall -c idt.c -o idt.o
gcc -m32 -fno-stack-protector -Wall -c irq.c -o irq.o
gcc -m32 -fno-stack-protector -Wall -c isrs.c -o isrs.o
gcc -m32 -fno-stack-protector -Wall -c kernel.c -o kc.o
gcc -m32 -fno-stack-protector -Wall -c timer.c -o timer.o
gcc -m32 -fno-stack-protector -Wall -c serial.c -o serial.o
gcc -m32 -fno-stack-protector -Wall -c multiboot.c -o multiboot.o
gcc -m32 -fno-stack-protector -Wall -c system.c -o system.o

ld -m elf_i386 -T link.ld -o kernel kernel.o kc.o isrs.o irq.o idt.o console.o gdt.o timer.o serial.o multiboot.o system.o

qemu-system-i386 -curses -kernel kernel
