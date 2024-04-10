# STM32_Bootloader
## Description
    Simple bootloader driver for STM32f103c8t6 and STM32f407vgt6
## Using 
    Step 1: make file .ko, using cross-compile from x86 host
`make`  
    
    Step 2: make app.exe
`make app`

    Step 3: copy from host to board
`scp app x.ko debian@192.168.x.x:/home/debian`

    Step 4: load driver to kernel to use
`insmod x.ko`   
`./app`

## Demo video
[Link demo](https://drive.google.com/file/d/1bBa7PgEBdsPr1ZUbgaRVO9RaOMSRz0UP/view?usp=sharing)