#include "boot_f1.h"

void flash_unlock()
{
    FLASH->KEYR = 0x45670123;
    FLASH->KEYR = 0xCDEF89AB;
}

void flash_lock()
{
    FLASH->CR |= FLASH_CR_LOCK;
}

void flash_erase(volatile uint32_t addreg)
{
    flash_unlock();
    while (FLASH->SR & FLASH_SR_BSY)
        ;
    FLASH->CR |= FLASH_CR_PER;
    FLASH->AR = addreg;
    FLASH->CR |= FLASH_CR_STRT;
    while (FLASH->SR & FLASH_SR_BSY)
        ;
    FLASH->CR &= ~FLASH_CR_PER;
    FLASH->CR &= ~FLASH_CR_STRT;
    flash_lock();
}

void flash_write(volatile uint32_t addreg, uint8_t *datawrite, uint32_t datalen)
{
    flash_unlock();
    while (FLASH->SR & FLASH_SR_BSY)
        ;
    FLASH->CR |= FLASH_CR_PG;
    for (uint32_t count = 0; count < (datalen / 2); count++)
    {
        *(uint16_t *)(addreg + count * 2U) = *(uint16_t *)(datawrite + count * 2U);
    }
    while (FLASH->SR & FLASH_SR_BSY)
        ;
    FLASH->CR &= ~FLASH_CR_PG;
    flash_lock();
}

void flash_read(volatile uint32_t addreg, uint8_t *dataread, uint32_t datalen)
{
    for (int count = 0; count < datalen; count++)
    {
        *(uint8_t *)(dataread + count) = *(uint8_t *)(addreg + count);
    }
}

void bootloader(volatile uint32_t addr_new)
{
    SCB->VTOR = addr_new;
    NVIC_SystemReset();
}

void boot_to_app(void)
{
    void (*app_reset_handler)(void) = NULL;
    uint32_t app_main_stack_pointer = 0;
    uint32_t app_reset_handler_addr = 0;

    app_main_stack_pointer = *(volatile uint32_t *)(BASE_ADDRESS_FLASH);
    __set_MSP(app_main_stack_pointer);

    app_reset_handler_addr = *(volatile uint32_t *)(BASE_ADDRESS_FLASH + 0x4U);
    app_reset_handler = (void *)app_reset_handler_addr;
    app_reset_handler();
}
