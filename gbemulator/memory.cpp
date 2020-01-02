#include "memory.h"


memory::memory()
{
    mainMem.resize(8192);
    //echoMem.resize(8192);
    videoMem.resize(8192);
    smallMem.resize(128);
    spriteMem.resize(160);
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

    //main ram
    else if (inRange(0xC000, 0xE000, absoluteLoc)){
        val = mainMem[absoluteLoc - 0xC000];
    }
    //video ram
    else if (inRange(0x8000, 0xA000, absoluteLoc)){
        val = videoMem[absoluteLoc - 0x8000];
    }

    //not usable memory
    else if (inRange(0xFEA0, 0xFEFF, absoluteLoc)){
        val = 0;
    }

    //small ram
    else if (inRange(0xFF80, 0xFFFF, absoluteLoc)){
        val = smallMem[absoluteLoc - 0xFF80];
    }
    //sprite memory
    else if (inRange(0xFE00, 0xFE9F, absoluteLoc)){
        val = spriteMem[absoluteLoc - 0xFE00];
    }
    else{
        std::cout << "ERROR read memory loc: " << HEX16(absoluteLoc) << " \n";
        return 0xFF;
    }

    std::cout << "Read from memory: " << HEX16(absoluteLoc) << " :"<<  HEX(val) << " \n";
    return val;
}

void memory::write(uint16_t absoluteLoc, uint8_t data){

    //game rom switchable memory
    if (inRange(0x4000, 0x7FFF, absoluteLoc)){
        gameRomSwitchable[absoluteLoc - 0x4000] = data;
    }
    //main memory
    else if (inRange(0xC000, 0xE000, absoluteLoc)){
        mainMem[absoluteLoc - 0xC000] = data;
    }
    //video memory
    else if (inRange(0x8000, 0xA000, absoluteLoc)){
        videoMem[absoluteLoc - 0x8000] = data;
    }

    //not usable memory
    else if (inRange(0xFEA0, 0xFEFF, absoluteLoc)){
    }
    //small memory
    else if (inRange(0xFF80, 0xFFFF, absoluteLoc)){
        smallMem[absoluteLoc - 0xFF80] = data;
    }
    //sprite memory
    else if (inRange(0xFE00, 0xFE9F, absoluteLoc)){
        spriteMem[absoluteLoc - 0xFE00] = data;
    }
    else{
        std::cout << "ERROR memory write: " << HEX16(absoluteLoc) << " :" << HEX(data) << " \n";
        return;
    }
    std::cout << "Wrote to memory: " << HEX16(absoluteLoc) << " :" << HEX(data) << " \n";
}

void memory::clearMemory(){
    std::fill(mainMem.begin(), mainMem.end(), 0);
    //std::fill(echoMem.begin(), echoMem.end(), 0);
    std::fill(videoMem.begin(), videoMem.end(), 0);
    std::fill(smallMem.begin(), smallMem.end(), 0);
    std::fill(spriteMem.begin(), spriteMem.end(), 0);
}


bool memory::inRange(uint16_t low, uint16_t high, uint16_t loc){
    return loc >= low && loc < high;
}
