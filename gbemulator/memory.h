#ifndef MEMORY_H
#define MEMORY_H
#include <stdio.h>
#include <iomanip>
#include <iostream>
#include <vector>


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

//0000-00FF boot rom


#define HEX(x) std::setw(2) << std::setfill('0') << std::hex << int(x)
#define HEX16(x) std::setw(4) << std::setfill('0') << std::hex << int(x)


class memory
{
public:
    memory();
    void write(uint16_t absoluteLoc, uint8_t data);
    uint8_t read(uint16_t absoluteLoc);
    void clearMemory();

    std::vector<uint8_t> gameRom;
    std::vector<uint8_t> gameRomSwitchable;


    std::vector<uint8_t> mainMem;  //C000-E000
    std::vector<uint8_t> echoMem;  //E000-FE00
    std::vector<uint8_t> videoMem; //8000-A000
    std::vector<uint8_t> spriteMem; //FE00-FEFF
    std::vector<uint8_t> smallMem;  //FF00-FFFF
    std::vector<uint8_t> externalMem;


private:

    bool inRange(uint16_t low, uint16_t high, uint16_t loc);
};


#endif // MEMORY_H
