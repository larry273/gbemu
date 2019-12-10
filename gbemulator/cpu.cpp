#include "cpu.h"



cpu::cpu()
{
    cpu_loop();
}

//read file as bytes
void cpu::readFiletoBytes(struct gbData &gbdata){
    std::cout << "starting file read \n";

    std::ifstream file(gbdata.filename, std::ios::binary);
    if( !file.is_open() ) {
        std::cout << "file failed to open \n";
        return;
    }

    //read content of file into buffer
    std::vector<unsigned char> content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    int count = 0;
    for(unsigned char& c : content) {
        if (count % 16 == 0){
            std::cout << "\n" << std::setw(6) << std::setfill('0') << count/16 << ": ";
        }
        std::cout << HEX(c) << ' ';
        count++;
    }

    gbdata.buffer = content;

    std::cout << "\nfinishing file read \n";
}


//8 bit register loads
void cpu::load(uint8_t &regA, uint8_t &regB){
    regA = regB;
}
void cpu::load(uint8_t &regA, uint8_t data){
    regA = data;
}
void cpu::load(uint8_t &regA, uint8_t &regB, uint8_t data){
    regA = 0x00;
    regB = data;

}
void cpu::load(uint8_t &regA, uint8_t &regB, uint8_t &regC, int ldType){
    //8 into 16 bit
    if (ldType == 1){
        regA = 0x00;
        regB = regC;
    }
    else{     //16 into 8 bit
        regA = regC;
    }

}
//TODO write memory class for cpu
//TODO write to memory address
void cpu::load(uint16_t data, uint8_t &regA){
}


//cpu loop
void cpu::cpu_loop(){

    opcode opcode;
    bool isCpuRunning = false;


    struct gbData gbdata; //gb data file
    gbdata.filename = "cpu_instrs.gb";

    //in loop
    //call read from file byte with pos
    this->readFiletoBytes(gbdata);
    opcode.decodeByte(gbdata, this->pc);

    //main loop
    while(isCpuRunning){

        std::cout << "run \n";

    }

}


