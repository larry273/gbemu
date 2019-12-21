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

    if (inRange(0xC000, 0xE000, absoluteLoc)){
        val = mainMem[absoluteLoc - 0xC000];
    }
    else if (inRange(0x8000, 0xA000, absoluteLoc)){
        val = videoMem[absoluteLoc - 0x8000];
    }
    else if (inRange(0xFF80, 0xFFFF, absoluteLoc)){
        val = smallMem[absoluteLoc - 0xFF80];
    }
    else if (inRange(0xFE00, 0xFEA0, absoluteLoc)){
        val = spriteMem[absoluteLoc - 0xFE00];
    }
    else{
        std::cout << "ERROR read memory loc: " << HEX16(absoluteLoc) << "\n";
        return 0;
    }

    std::cout << "Read from memory: " << HEX16(absoluteLoc) << " :"<<  HEX(val) << "\n";
    return val;
}

void memory::write(uint16_t absoluteLoc, uint8_t data){

    if (inRange(0xC000, 0xE000, absoluteLoc)){
        mainMem[absoluteLoc - 0xC000] = data;
    }
    else if (inRange(0x8000, 0xA000, absoluteLoc)){
        videoMem[absoluteLoc - 0x8000] = data;
    }
    else if (inRange(0xFF80, 0xFFFF, absoluteLoc)){
        smallMem[absoluteLoc - 0xFF80] = data;
    }
    else if (inRange(0xFE00, 0xFEA0, absoluteLoc)){
        spriteMem[absoluteLoc - 0xFE00] = data;
    }
    else{
        std::cout << "ERROR memory write: " << HEX16(absoluteLoc) << " :" << HEX(data) << "\n";
        return;
    }
    std::cout << "Wrote to memory: " << HEX16(absoluteLoc) << " :" << HEX(data) << "\n";
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
