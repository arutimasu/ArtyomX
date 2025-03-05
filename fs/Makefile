# Makefile для руководств по сборке ядра - автор James Molloy.
# Правила обработки C и C++ указаны как используемые по умолчанию.
# Единственное, что нужно изменить, это указать правила обработки 
# ассемблера, поскольку мы используем nasm вместо GNU as.

.POSIX:
.PHONY: clean 

all: fs.o ird.o ldr.o


fs.o: fs.c
	gcc -c -m32 -std=gnu99 -ffreestanding -fno-builtin -Os -o '$@' -Wall -Wextra '$<'
	
ird.o: initrd.c
	gcc -c -m32 -std=gnu99 -ffreestanding -fno-builtin -Os -o '$@' -Wall -Wextra '$<'
	
ldr.o: loader.c
	gcc -c -m32 -std=gnu99 -ffreestanding -fno-builtin -Os -o '$@' -Wall -Wextra '$<'
	
clean:
	rm -f  *.o 

