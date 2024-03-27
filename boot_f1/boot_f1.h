#ifndef __BOOT_F1__
#define __BOOT_F1__

void flash_erase(volatile uint32_t addreg);
void flash_write(volatile uint32_t addreg, uint8_t *datawrite, uint32_t datalen);
void flash_read(volatile uint32_t addreg, uint8_t *dataread, uint32_t datalen);
void bootloader(volatile uint32_t addr_new);
#endif