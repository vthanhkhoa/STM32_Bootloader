# STM32_Bootloader
## Description
    Simple bootloader driver for STM32f103c8t6 and STM32f407vgt6
## Getting Started
### Using
`boot_f1`   and `boot_f4` are drivers loaded into your project  

In `example`, `app_prog` is your new application, which you want to run and replace current program; `base_prog` is current firmware and it has function to boot to new firmware

### Issues
- You must erase before writing new firmware to flash and with stm32f1, you will have to erase a page (1KB) each time

- Cannot program with SCB->VTOR (base memory register - after reset, this address will be first implementation)
- Don't have BOOT_ADD0: this reg regulates address initialized flash after reset (ex: with default addr - 0x0800 0000, BOOT_ADD0 is 0x2000 and with new firmware at 0x0801 0000, reg value will be 0x2004)
- Pressing reset button or using `NVIC_SystemReset()`: will be boot from old code at 0x0800 0000 in flash, don't use new one at 0x0801 0000 (perhaps it use code started from 0x0000 0000 (?)) 

## Connect
### For using this example:     
    With new firmware, you can use any sources, not mandatory for using my app_program, but you must config to transmit it via UART
`Step 1:`
    Config UART to receive new firmware     
`Step 2:`
    After transmitting perfectly, use `boot2()` or `boot_to_app()` func
