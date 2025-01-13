# Makefile для руководств по сборке ядра - автор James Molloy.
# Правила обработки C и C++ указаны как используемые по умолчанию.
# Единственное, что нужно изменить, это указать правила обработки 
# ассемблера, поскольку мы используем nasm вместо GNU as.

.POSIX:
.PHONY: all clean run run-img

all:
	$(MAKE) -C kernel
	$(MAKE) -C drivers
	$(MAKE) -C fs
	$(MAKE) -C common
	$(MAKE) main.elf
main.img: main.elf
	cp '$<' iso/boot
	grub-mkrescue -o '$@' iso

# main.elf is the the multiboot file.
main.elf: kasm.o common/com.o common/sc.o common/mem.o  drivers/mon.o drivers/kb.o fs/fs.o fs/ird.o fs/ldr.o kernel/kern.o kernel/dt.o  kernel/gdt.o  kernel/isr.o  kernel/int.o   
	ld -m elf_i386 -nostdlib -T link.ld $^ -o '$@'  
	#$^
	

kasm.o: boot/boot.asm
	nasm -f elf32 '$<' -o '$@'
	


#dsk.o: disk.c
#	gcc -c -m32 -std=gnu99 -ffreestanding -fno-builtin -Os -o '$@' -Wall -Wextra '$<'

	
clean:
	rm  -f *.elf *.o iso/boot/*.elf *.img
	
	find	./kernel -name "*.o" -type f -delete
	find ./fs -name "*.o" -type f -delete
	find ./drivers -name "*.o" -type f -delete
	find ./common -name "*.o" -type f -delete

	#rm ./kernel -f *.o 
	#rm ./drivers -f *.o 
	#rm ./fs -f *.o 
	#rm ./common -f *.o 

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
