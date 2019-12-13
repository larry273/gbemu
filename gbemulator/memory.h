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
    //write value to main memory
    void write(uint8_t data);
    //read value from main memory
    void read(uint16_t location);


private:
    uint8_t mem[8196];      //C000-E000
    uint8_t echoMem[8196];  //E000-FE00
    uint8_t videoMem[8196]; //8000-A000
    uint8_t smallMem[128];  //FF80-FFFF
    uint8_t spriteMem[160]; //FE00-FEA0
};



#endif // MEMORY_H
