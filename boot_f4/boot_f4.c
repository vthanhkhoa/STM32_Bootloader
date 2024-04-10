void flash_unlock()
{
    FLASH->KEYR = 0x45670123;
    FLASH->KEYR = 0xCDEF89AB;
}

void flash_lock()
{
    FLASH->OPTCR |= FLASH_OPTCR_OPTLOCK;
}

void flash_erase(volatile uint8_t sector_num)
{
    while (FLASH->SR & FLASH_SR_BSY)
        ;
    if ((FLASH->CR & FLASH_CR_LOCK) == FLASH_CR_LOCK)
        flash_unlock();
    FLASH->CR |= FLASH_CR_SER;
    FLASH->CR |= sector_num << 3;
    FLASH->CR |= FLASH_CR_STRT;
    while (FLASH->SR & FLASH_SR_BSY)
        ;
    FLASH->CR &= ~FLASH_CR_SER;
}

void flash_write(volatile uint32_t addreg, uint32_t datawrite)
{
    while (FLASH->SR & FLASH_SR_BSY)
        ;
    //	if ((FLASH->CR & FLASH_CR_LOCK) == FLASH_CR_LOCK) flash_unlock();
    FLASH->CR |= FLASH_CR_PG;
    FLASH->CR |= 1 << 9;
    *(uint32_t *)(addreg) = datawrite;
    while (FLASH->SR & FLASH_SR_BSY)
        ;
}

void flash_read(volatile uint32_t addreg, uint8_t *dataread, uint32_t datalen)
{
    for (int count = 0; count < datalen; count++)
    {
        *(uint32_t *)(dataread + count) = *(uint32_t *)(addreg + count);
    }
}
void bootloader(volatile uint32_t addr_new)
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
