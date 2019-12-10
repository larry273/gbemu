#ifndef MEMORY_H
#define MEMORY_H
#include <stdio.h>
#include <iomanip>

/*
 * interrupt enable reg     FFFF
 * internal ram             FF80
 * empty, unusable for IO   FF4C
 * IO                       FF00
 * empty, unusable for IO   FEA0
 * sprite attrib mem        FE00 --ram
 * echo of 8kB              E000 --ram
 * 8kB internal             C000 --ram
 * 8kB switch ram bank      A000
 * 8kB video ram            8000 ---
 * 16kB switchable rom bank 4000 cart
 * 16kB rom bank 0          0000 ---
 */

class memory
{
public:
    memory();


private:
    uint8_t ram[8196];      //C000-E000
    uint8_t echoRam[8196];  //E000-FE00
    uint8_t videoram[8196]; //8000-A000
    uint8_t smallRam[128];  //FF80-FFFF
    uint8_t spriteRam[160]; //FE00-FEA0
};



#endif // MEMORY_H
