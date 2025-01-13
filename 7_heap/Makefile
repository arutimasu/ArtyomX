# Makefile для руководств по сборке ядра - автор James Molloy.
# Правила обработки C и C++ указаны как используемые по умолчанию.
# Единственное, что нужно изменить, это указать правила обработки 
# ассемблера, поскольку мы используем nasm вместо GNU as.

.POSIX:
.PHONY: clean run run-img

main.iso: main.elf
	cp '$<' iso/boot
	grub-mkrescue -o '$@' iso

# main.elf is the the multiboot file.
main.elf: kasm.o kc.o  mon.o dt.o gdt.o isr.o int.o com.o tmr.o pg.o ord_arr.o kheap.o
#kb.o
	ld -m elf_i386 -nostdlib -T link.ld -o '$@' $^

kasm.o: boot.asm
	nasm -f elf32 '$<' -o '$@'
	
gdt.o: gdt.asm
	nasm -f elf32 '$<' -o '$@'
	
int.o: interrupt.asm
	nasm -f elf32 '$<' -o '$@'

kc.o: main.c
	gcc -c -m32 -std=gnu99 -ffreestanding -fno-builtin -Os -o '$@' -Wall -Wextra '$<'

mon.o: monitor.c
	gcc -c -m32 -std=gnu99 -ffreestanding -fno-builtin -Os -o '$@' -Wall -Wextra '$<'

tmr.o: timer.c
	gcc -c -m32 -std=gnu99 -ffreestanding -fno-builtin -Os -o '$@' -Wall -Wextra '$<'
pg.o: paging.c
	gcc -c -m32 -std=gnu99 -ffreestanding -fno-builtin -Os -o '$@' -Wall -Wextra '$<'
ord_arr.o: ordered_array.c
	gcc -c -m32 -std=gnu99 -ffreestanding -fno-builtin -Os -o '$@' -Wall -Wextra '$<'
kheap.o: kheap.c
	gcc -c -m32 -std=gnu99 -ffreestanding -fno-builtin -Os -o '$@' -Wall -Wextra '$<'
#kheap.o: kheap.c
#	gcc -c -m32 -std=gnu99 -ffreestanding -fno-builtin -Os -o '$@' -Wall -Wextra '$<'

#str.o: string.c
#	gcc -c -m32 -std=gnu99 -ffreestanding -fno-builtin -Os -o '$@' -Wall -Wextra '$<'
	
com.o: common.c
	gcc -c -m32 -std=gnu99 -ffreestanding -fno-builtin -Os -o '$@' -Wall -Wextra '$<'

#dsk.o: disk.c
#	gcc -c -m32 -std=gnu99 -ffreestanding -fno-builtin -Os -o '$@' -Wall -Wextra '$<'


dt.o: descriptor_tables.c
	gcc -c -m32 -std=gnu99 -ffreestanding -fno-builtin -Os -o '$@' -Wall -Wextra '$<'
	
isr.o: isr.c
	gcc -c -m32 -std=gnu99 -ffreestanding -fno-builtin -Os -o '$@' -Wall -Wextra '$<'
	
kb.o: keyboard.c
	gcc -c -m32 -std=gnu99 -ffreestanding -fno-builtin -Os -o '$@' -Wall -Wextra '$<'
	
clean:
	rm -f *.elf *.o iso/boot/*.elf *.img

run: main.elf
	qemu-system-i386 -kernel '$<'

run-img: main.img
	#qemu-system-x86_64 -s -S -hda '$<'
	qemu-system-i386 '$<'
bochs: main.img
	CYLINDERS="$$(($$(stat -c '%s' '$<') / 512))" && \
	bochs -qf /dev/null \
		'ata0-master: type=disk, path="$<", mode=flat, cylinders='"$$CYLINDERS"', heads=1, spt=1' \
		'boot: disk' \
		'display_library: sdl2' \
		'megs: 128'
	
	#qemu-system-x86_64 -drive format=raw,file=path/to.img
