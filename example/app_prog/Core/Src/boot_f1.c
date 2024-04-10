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

/**
 * The function `boot_to_app` sets the main stack pointer and calls the reset handler function to boot
 * into an application.
 */
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

/**
 * The `boot2` function resets the clock, initializes the application reset handler, and jumps to the
 * application code.
 */
void boot2(void)
{
    void (*app_reset_handler)(void) = (void *)(*((volatile uint32_t *)(BASE_ADDRESS_FLASH + 4U)));

    // Turn OFF the Green Led to tell the user that Bootloader is not running
    //	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET );    //Green LED OFF

    /* Reset the Clock */
    HAL_RCC_DeInit();
    HAL_DeInit();
    __set_MSP(*(volatile uint32_t *)BASE_ADDRESS_FLASH);
    SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL = 0;

    /* Jump to application */
    //	  SCB->VTOR = BASE_ADDRESS_FLASH;
    app_reset_handler(); // call the app reset handler
    //	  NVIC_SystemReset();
}
