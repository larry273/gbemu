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

void cpu::load(uint8_t &regA, uint8_t *data){
    regA = *data;
    //takes 2 cycles
}


//TODO write memory class for cpu
//TODO write to memory address

//8 bit arithmetic
void cpu::inc(uint8_t &regA){
    uint8_t r = regA + 1;

    this->setFlag(FLAGZERO, r == 0);
    this->setFlag(FLAGSUB, false);
    this->setFlag(FLAGHALFCARRY, ((regA & 0xF) + (1 & 0xF)) > 0xF); //set if carry to bit 4
    regA = r;
}
void cpu::dec(uint8_t &regA){
    uint8_t r = regA - 1;

    this->setFlag(FLAGZERO, r == 0);
    this->setFlag(FLAGSUB, true);
    this->setFlag(FLAGHALFCARRY, ((regA & 0xF) - (1 & 0xF)) < 0); //set if no borrow from bit 4
    regA = r;
}
void cpu::logAnd(uint8_t &regA, uint8_t &regB){
    regA = regA & regB;

    this->setFlag(FLAGZERO, regA == 0);
    this->setFlag(FLAGSUB, false);
    this->setFlag(FLAGHALFCARRY, true); //set if carry to bit 4
    this->setFlag(FLAGCARRY, false);
}

void cpu::logOr(uint8_t &regA, uint8_t &regB){
    regA = regA ^ regB;

    this->setFlag(FLAGZERO, regA == 0);
    this->setFlag(FLAGSUB, false);
    this->setFlag(FLAGHALFCARRY, false); //set if carry to bit 4
    this->setFlag(FLAGCARRY, false);
}

void cpu::logXor(uint8_t &regA, uint8_t &regB){
    regA = regA ^ regB;

    this->setFlag(FLAGZERO, regA == 0);
    this->setFlag(FLAGSUB, false);
    this->setFlag(FLAGHALFCARRY, false); //set if carry to bit 4
    this->setFlag(FLAGCARRY, false);
}


//set register flags
void cpu::setFlag(const int flag, bool result){
    if (result){
        this->reg.F |= flag;
    }
    else {
        this->reg.F &= ~flag;
    }
}

void cpu::nop(){
    std::cout << "do nothing\n";
}


//cpu loop
void cpu::cpu_loop(){

    bool isCpuRunning = false;


    struct gbData gbdata; //gb data file
    gbdata.filename = "cpu_instrs.gb";

    //in loop
    //call read from file byte with pos
    this->readFiletoBytes(gbdata);
    this->decodeByte(gbdata, this->pc);

    //main loop
    while(isCpuRunning){

        std::cout << "run \n";

    }

}


