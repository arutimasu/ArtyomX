#include "disk.h"
#include <stdint.h>
#include <stddef.h>

#include "descriptor_tables.h"
#include "monitor.h"


//#include "timer.h"
#include "paging.h"
#include "multiboot.h"
#include "fs.h"
#include "initrd.h"

//#include "fs.h"
//#include "keyboard_map.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64


#define ENTER_KEY_CODE 0x1C
struct multiboot;
//char *buffer;

//#define BUFFER_SIZE 128 // Определите размер вашего буфера
//char buffer[BUFFER_SIZE];
//int buffer_cursor = 0;

//char keycode;

int is_entered = 1;
//int argc = 0;
void init_timer() {
    /* If we wanted to set a frequency. */
    /*
    int frequency = 1;
    u32int divisor = 1193180 * frequency;
    u8int l = (u8int)(divisor & 0xFF);
    u8int h = (u8int)( (divisor>>8) & 0xFF );
    */

    /* But we just want the maximum divisor instead. */
    u8int l = 0xFF;
    u8int h = 0xFF;

    /* Command byte. */
    outb(0x43, 0x36);

    // Send the frequency divisor.
    outb(0x40, l);
    outb(0x40, h);
}


void write(int fd, char* buf, int nbytes) {
	// unsigned int i = 0;
	// switch(fd){
		// case 1:
		// /* this loop writes the string to video memory */
		// for(int j=0; j<nbytes; ++j){
			// /* the character's ascii */
			// vidptr[i] = str[j];
			// /* attribute-byte: give character black bg and light grey fg */
			// vidptr[i+1] = 0x07;
			//++j;
			// i = i + 2;
		// }
		// break;
	// }
	switch(fd){
		case 1:
		for(int i = 0; i < strlen(buf); i++){
			monitor_put(buf[i]);
		}
		break;
	}
}



void read(int fd, char *buf, int nbytes) {
	int i = 0;
    switch (fd) {
        case 0:
          	while (i < nbytes) {
			//for(int i = 0; i < nbytes; i++){
				char c = keyboard_read();
				if (c != -1) {
					
					buf[i++] = c;
					// Обработка полученного символа
					// Например, вывод на экран, интерпретация команд и т.д.
					monitor_put(c);
					//vidptr[current_loc++] = 0x07;  // Атрибут цвета
				}
			}
			monitor_write("\n");
            break;
    }
}


void process_fat_bootsector(uint8_t* buffer) {
    // Проверяем "идентификатор" файловой системы
    if (buffer[0x1FE] != 0x55 || buffer[0x1FF] != 0xAA) {
        monitor_write("Invalid boot sector\n");
        return;
    }

    // Получаем размер сектора (обычно 512)
    uint16_t bytes_per_sector = *((uint16_t*)&buffer[0x0B]);
    
    // Получаем число секторов в кластере
    uint8_t sectors_per_cluster = buffer[0x0D];

    // Получаем общее число резервных секторов
    uint16_t reserved_sectors = *((uint16_t*)&buffer[0x0E]);

    // Информация о количестве файлов в директориях, кластерах и т.д. может быть получена аналогично

    // Вывод информации
    monitor_write("FAT Boot Sector Details:\n");
      monitor_write("Bytes per Sector: ");
      monitor_write_dec(bytes_per_sector); // Преобразуйте в строку, если у вас есть функция itoa
      monitor_write("\nSectors per Cluster: ");
      monitor_write_dec(sectors_per_cluster);
      monitor_put('\n');
}


void find_hello_world(const u8int *buffer) {
    const char *search_string = "Hello, world!";
    if (strstr((const char *)buffer, search_string) != NULL) {
         monitor_write("Found 'Hello, world!' in the disk image.\n");
    } else {
         monitor_write("'Hello, world!' not found.\n");
    }
}

void display_sector_contents(uint8_t* buffer) {
    //char hex_string[3];
    for (int i = 0; i < 512; i++) {
        // Преобразование одного байта в двоичное представление
        monitor_write((const char)buffer[i]);  // Форматируем как шестнадцатеричное
        //putstring(hex_string); // Выводим байт на экран

        // Печатаем новую строку каждые 16 байт для удобства
        if ((i + 1) % 16 == 0) {
            monitor_put('\n');
        }
    }
    monitor_put('\n'); // Перенос строки после вывода всего сектора
}
// Определение структуры суперблока
struct ext4_super_block {
    uint32_t s_inodes_count;
    uint32_t s_blocks_count;
    uint32_t s_r_blocks_count;
    uint32_t s_free_blocks_count;
    uint32_t s_free_inodes_count;
    uint32_t s_first_data_block;
    uint32_t s_log_block_size;
    // ... другие поля
};


void search_for_hello_world(u32int start_lba, u32int num_sectors) {
    u8int buffer[512]; // Буфер для каждого сектора
    const char *search_string = "Hello, world!";
	
    for (u32int sector = 0; sector < num_sectors; sector++) {
        ata_read_sector(start_lba + sector, buffer);
		 display_sector_contents(buffer);
		struct ext4_super_block *sb = (struct ext4_super_block *)buffer;
		monitor_write("Inodes Count: ");
		monitor_write_dec(sb->s_inodes_count);
		monitor_write("\nBlocks Count: ");
		monitor_write_dec(sb->s_blocks_count);
       
        // Проверяем, содержится ли строка
        if (strstr((char)buffer, search_string) != NULL) {
            monitor_write("Found ");
			monitor_write(search_string); 
			monitor_write(" in LBA sector ");
			monitor_write_dec(start_lba + sector);
			monitor_put('\n');
            return;
        }
    }
	
	monitor_write(search_string); 
    monitor_write(" not found in the first ");
	monitor_write_dec(num_sectors);
	monitor_write(" sectors starting from LBA ");
	monitor_write_dec(start_lba);
	
}


 
//int main(struct multiboot *mboot_ptr) {
//    init_descriptor_tables();
//    monitor_clear();

    /* Mandatory, because the PIC interrupts are maskable. */
    //asm volatile("sti");
	
	//outb(0x21, 0xFD);
	
    //read(0, str, 5);
	//write(1,str,5);
	
//	ata_init();

    //uint8_t buffer[10240]; // Буфер для хранения двух секторов (2 * 512 байт)
    //ata_read_sectors(0, buffer, 8); // Читаем два сектора (0 и 1)

    //find_hello_world(buffer); // Ищем строку в буфере
//	const u32int start_lba = 0; // Начинаем с LBA 0
//    const u32int num_sectors = 300; // Ограничиваем количество секторов для поиска

//    search_for_hello_world(start_lba, num_sectors);

	//find_hello_world(buffer); // Ищем строку в буфере

//	return 0;
//}

int main(struct multiboot *mboot_ptr)
{
    // Initialise all the ISRs and segmentation
    init_descriptor_tables();
    // Initialise the screen (by clearing it)
    monitor_clear();

    // Find the location of our initial ramdisk.
    ASSERT(mboot_ptr->mods_count > 0);
    u32int initrd_location = *((u32int*)mboot_ptr->mods_addr);
    u32int initrd_end = *(u32int*)(mboot_ptr->mods_addr+4);
    // Don't trample our module with placement accesses, please!
    placement_address = initrd_end;

    // Start paging.
    initialise_paging();

    // Initialise the initial ramdisk, and set it as the filesystem root.
    fs_root = initialise_initrd(initrd_location);

    // list the contents of /
    int i = 0;
    struct dirent *node = 0;
    while ( (node = readdir_fs(fs_root, i)) != 0)
    {
        monitor_write("Found file ");
        monitor_write(node->name);
        fs_node_t *fsnode = finddir_fs(fs_root, node->name);

        if ((fsnode->flags&0x7) == FS_DIRECTORY)
        {
            monitor_write("\n\t(directory)\n");
        }
        else
        {
            monitor_write("\n\t contents: \"");
            char buf[256];
            u32int sz = read_fs(fsnode, 0, 256, buf);
            int j;
            for (j = 0; j < sz; j++)
                monitor_put(buf[j]);
            
            monitor_write("\"\n");
        }
        i++;
    }

    return 0;
}

