all: loader elf

loader: loader.c
	gcc -m32 -g -Wall -o loader loader.c -ldl

elf: elf.c
	gcc -m32 -pie -fPIE -o elf elf.c