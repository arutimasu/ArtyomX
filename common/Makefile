# Makefile для руководств по сборке ядра - автор James Molloy.
# Правила обработки C и C++ указаны как используемые по умолчанию.
# Единственное, что нужно изменить, это указать правила обработки 
# ассемблера, поскольку мы используем nasm вместо GNU as.

.POSIX:
.PHONY: clean 

all: com.o sc.o mem.o


com.o: common.c
	gcc -c -m32 -std=gnu99 -ffreestanding -fno-builtin -Os -o '$@' -Wall -Wextra '$<'
	
sc.o: syscalls.c
	gcc -c -m32 -std=gnu99 -ffreestanding -fno-builtin -Os -o '$@' -Wall -Wextra '$<'
mem.o: memory.c
	gcc -c -m32 -std=gnu99 -ffreestanding -fno-builtin -Os -o '$@' -Wall -Wextra '$<'
	
clean:
	rm -f  *.o 

