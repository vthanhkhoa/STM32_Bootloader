#ifndef __BOOT_F1__
#define __BOOT_F1__

#include "main.h"

#define BASE_ADDRESS_FLASH 0x08010000

void flash_erase(volatile uint32_t addreg);
void flash_write(volatile uint32_t addreg, uint8_t *datawrite, uint32_t datalen);
void flash_read(volatile uint32_t addreg, uint8_t *dataread, uint32_t datalen);
void bootloader(volatile uint32_t addr_new);
void boot_to_app(void);
void boot2(void);
#endif
