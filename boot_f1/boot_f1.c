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
    flash_lock();
}

void flash_write(volatile uint32_t addreg, uint8_t *datawrite, uint32_t datalen)
{
    flash_unlock();
    while (FLASH->SR & FLASH_SR_BSY)
        ;
    FLASH->CR |= FLASH_CR_PG;
    for (uint32_t count = 0; count < datalen / 2; count++)
    {
        *(uint32_t *)(addreg + count) = *(uint32_t *)(datawrite + count);
    }
    while (FLASH->SR & FLASH_SR_BSY)
        ;
    FLASH->CR |= FLASH_CR_PG;
    flash_lock();
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
    SCB->VTOR = addr_new;
    NVIC_SystemReset();
}
