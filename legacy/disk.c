#include "common.h"

#include "monitor.h"
//#include "descriptor_tables.h"
#define HD_DATA		0x1f0		/* регистр данных */
#define HD_ERROR	0x1f1		/* регистр ошибок */
#define HD_NSECTOR	0x1f2		/* регистр счетчика секторов */
#define HD_SECTOR	0x1f3		/* регистр стартового сектора */
#define HD_LCYL		0x1f4		/* регистр младшего байта номера цилиндра */
#define HD_HCY		0x1f5		/* регистр старшего байта номера цилиндра */
#define HD_CURRENT	0x1f6		/* 101dhhhh , d=устройство, hhhh=головка */
#define HD_STATUS	0x1f7		/* регистр состояния/команд */


#define ATA_PRIMARY 0x1F0
#define ATA_SECONDARY 0x170
#define ATA_CMD_READ 0x20
#define ATA_CMD_WRITE 0x30
#define ATA_STATUS_REG (ATA_PRIMARY + 7)
#define ATA_DATA_REG (ATA_PRIMARY)
#define ATA_SECTOR_COUNT (ATA_PRIMARY + 2)
#define ATA_LBA_LOW (ATA_PRIMARY + 3)
#define ATA_LBA_MID (ATA_PRIMARY + 4)
#define ATA_LBA_HIGH (ATA_PRIMARY + 5)
#define ATA_DRIVE (ATA_PRIMARY + 6)
#define ATA_CMD_REG (ATA_PRIMARY + 7)
#define ATA_CTL_REG (ATA_PRIMARY + 6)

void ata_init() {
    // Убедитесь, что диск готов к работе
    outb(ATA_CMD_REG, 0x02); // Reset command
    // Здесь могут быть добавлены другие команды инициализации, если нужно
}


void ata_read_sector(u32int lba, u8int *buffer) {
    // Установка параметров для передачи команды чтения
    outb(ATA_LBA_LOW, (lba & 0xFF));        // Либо 0-255
    outb(ATA_LBA_MID, (lba >> 8) & 0xFF);  // Либо 256-511
    outb(ATA_LBA_HIGH, (lba >> 16) & 0xFF); // Либо 512-767
    outb(ATA_SECTOR_COUNT, 1);              // Счётчик секторов

    outb(ATA_DRIVE, 0xE0); // Активируем первый диск (0xE0 для MBR)

    // Запуск команды чтения
    outb(ATA_CMD_REG, ATA_CMD_READ);

    // Ожидание завершения чтения
    while (!(inb(ATA_STATUS_REG) & 0x08)); // Дожидаемся готовности

    // Чтение данных
    for (int i = 0; i < 256 * 2; i++) { // 256 = 512 байт
        ((u16int *)buffer)[i] = ((u16int *)buffer)[i]; // Копируем данные
    }
}
void ata_read_sectors(u32int start_lba, u8int *buffer, int sector_count) {
    for (int i = 0; i < sector_count; i++) {
        // Считываем каждый сектор по очереди
        ata_read_sector(start_lba + i, buffer + (i * 512));
    }
}
