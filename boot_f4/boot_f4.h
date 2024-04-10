#ifndef __BOOT_F4__
#define __BOOT_F4__

#define BASE_ADDRESS_FLASH 0x08010000

void flash_erase(volatile uint8_t sector_num);
void flash_write(volatile uint32_t addreg, uint32_t datawrite);
void flash_read(volatile uint32_t addreg, uint8_t *dataread, uint32_t datalen);
void bootloader(volatile uint32_t addr_new);
#endif