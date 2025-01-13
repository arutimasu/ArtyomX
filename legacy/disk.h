#include "common.h"
#ifndef DISK_H
#define DISK_H

void ata_init();
void ata_read_sector(u32int lba, u8int *buffer);
#endif