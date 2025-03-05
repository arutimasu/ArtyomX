
# Makefile для руководств по сборке ядра - автор James Molloy.
# Правила обработки C и C++ указаны как используемые по умолчанию.
# Единственное, что нужно изменить, это указать правила обработки 
# ассемблера, поскольку мы используем nasm вместо GNU as.

.POSIX:
.PHONY: clean 

all: kern.o dt.o isr.o gdt.o int.o


kern.o: main.c
	gcc -c -m32 -std=gnu99 -ffreestanding -fno-builtin -Os -o '$@' -Wall -Wextra '$<'

dt.o: descriptor_tables.c
	gcc -c -m32 -std=gnu99 -ffreestanding -fno-builtin -Os -o '$@' -Wall -Wextra '$<'
	
isr.o: isr.c
	gcc -c -m32 -std=gnu99 -ffreestanding -fno-builtin -Os -o '$@' -Wall -Wextra '$<'

gdt.o: asm/gdt.asm
	nasm -f elf32 '$<' -o '$@'
	
int.o: asm/interrupt.asm
	nasm -f elf32 '$<' -o '$@'

clean:
	rm -f  *.o 
