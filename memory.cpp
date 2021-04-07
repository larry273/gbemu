#include "memory.h"


memory::memory()
{
    mainMem.resize(0x4000);
    //echoMem.resize(8192);
    videoMem.resize(0x4000);
    smallMem.resize(0x00FF);
    spriteMem.resize(0x009F);
    gameRomSwitchable.resize(0x4000);
    externalMem.resize(0x4000);
    clearMemory();

}

uint8_t memory::read(uint16_t absoluteLoc){
    uint8_t val;

    //game rom unswitchable memory
    if (inRange(0x0000, 0x3FFF, absoluteLoc)){
        val = gameRom[absoluteLoc];
    }
    //game rom switchable memory
    else if (inRange(0x4000, 0x7FFF, absoluteLoc)){
        val = gameRomSwitchable[absoluteLoc - 0x4000];
    }

    //video ram
    else if (inRange(0x8000, 0x9FFF, absoluteLoc)){
        val = videoMem[absoluteLoc - 0x8000];
    }
    //external ram
    else if (inRange(0xA000, 0xBFFF, absoluteLoc)){
        val = externalMem[absoluteLoc - 0xA000];
    }

    //main ram
    else if (inRange(0xC000, 0xDFFF, absoluteLoc)){
        val = mainMem[absoluteLoc - 0xC000];
    }
    //echo ram
    else if (inRange(0xE000, 0xFDFF, absoluteLoc)){
        val = mainMem[absoluteLoc - 0xE000 - 0x2000];
    }

    //sprite memory
    else if (inRange(0xFE00, 0xFE9F, absoluteLoc)){
        val = spriteMem[absoluteLoc - 0xFE00];
    }
    //not usable memory
    else if (inRange(0xFEA0, 0xFEFF, absoluteLoc)){
        val = 0;
    }

    //small ram
    else if (inRange(0xFF00, 0xFFFF, absoluteLoc)){
        val = smallMem[absoluteLoc - 0xFF00];
    }

    else{
        std::cout << "ERROR read memory loc: " << HEX16(absoluteLoc) << " \n";
        return 0xFF;
    }

    //std::cout << "Read from memory: " << HEX16(absoluteLoc) << " :"<<  HEX(val) << " \n";
    return val;
}

void memory::write(uint16_t absoluteLoc, uint8_t data){

    //gameRom no write
    if (inRange(0x0000, 0x3FFF, absoluteLoc)){
    }
    //game rom switchable memory
    else if (inRange(0x4000, 0x7FFF, absoluteLoc)){
        gameRomSwitchable[absoluteLoc - 0x4000] = data;
    }
    //video memory
    else if (inRange(0x8000, 0x9FFF, absoluteLoc)){
        videoMem[absoluteLoc - 0x8000] = data;
    }
    //external ram
    else if (inRange(0xA000, 0xBFFF, absoluteLoc)){
       externalMem[absoluteLoc - 0xA000] = data;
    }
    //main memory
    else if (inRange(0xC000, 0xE000, absoluteLoc)){
        mainMem[absoluteLoc - 0xC000] = data;
    }
    //echo memory
    else if (inRange(0xE000, 0xFDFF, absoluteLoc)){
        //mainMem[absoluteLoc - 0xE000 - 0x2000] = data;
    }


    //sprite memory
    else if (inRange(0xFE00, 0xFE9F, absoluteLoc)){
        spriteMem[absoluteLoc - 0xFE00] = data;
    }
    //not usable memory
    else if (inRange(0xFEA0, 0xFEFF, absoluteLoc)){
    }


    //small memory
    else if (inRange(0xFF00, 0xFFFF, absoluteLoc)){
        smallMem[absoluteLoc - 0xFF00] = data;
    }
   //if access scanline GB resets to zero
   else if (absoluteLoc == 0xFF44){
           smallMem[0xFF44 - 0xFF00] = 0;
   }
    else{
        std::cout << "ERROR memory write: " << HEX16(absoluteLoc) << " :" << HEX(data) << " \n";
        return;
    }

    //memory read/write sanity check
    if (read(absoluteLoc) != data){
        std::cout << "ERROR memory write: " << HEX16(absoluteLoc) << " :" << HEX(data) << " \n";
        return;
    }
    //std::cout << "Wrote to memory: " << HEX16(absoluteLoc) << ":" << HEX(data) << " \n";
}

void memory::clearMemory(){
    std::fill(mainMem.begin(), mainMem.end(), 0);
    //std::fill(echoMem.begin(), echoMem.end(), 0);
    std::fill(videoMem.begin(), videoMem.end(), 0);
    std::fill(smallMem.begin(), smallMem.end(), 0);
    std::fill(spriteMem.begin(), spriteMem.end(), 0);
    std::fill(gameRomSwitchable.begin(), gameRomSwitchable.end(), 0);
    std::fill(externalMem.begin(), externalMem.end(), 0);

}




bool memory::inRange(uint16_t low, uint16_t high, uint16_t loc){
    return loc >= low && loc <= high;
}
