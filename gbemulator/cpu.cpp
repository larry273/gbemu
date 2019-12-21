#include "cpu.h"



cpu::cpu()
{
    reg.PC = 0;
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
}
void cpu::load(uint8_t &regA, uint16_t memLoc){
    uint8_t data = mem.read(memLoc);
    regA = data;
}
void cpu::load(uint16_t memLoc, uint8_t &regA){
    mem.write(memLoc, regA);
}
void cpu::load(uint16_t memLoc, uint16_t &regA){
    mem.write(memLoc, regA);
}


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
void cpu::logAnd(uint8_t &regA, uint8_t regB){
    regA = regA & regB;

    this->setFlag(FLAGZERO, regA == 0);
    this->setFlag(FLAGSUB, false);
    this->setFlag(FLAGHALFCARRY, true); //set if carry to bit 4
    this->setFlag(FLAGCARRY, false);
}

void cpu::logOr(uint8_t &regA, uint8_t regB){
    regA = regA ^ regB;

    this->setFlag(FLAGZERO, regA == 0);
    this->setFlag(FLAGSUB, false);
    this->setFlag(FLAGHALFCARRY, false); //set if carry to bit 4
    this->setFlag(FLAGCARRY, false);
}

void cpu::logXor(uint8_t &regA, uint8_t regB){
    regA = regA ^ regB;

    this->setFlag(FLAGZERO, regA == 0);
    this->setFlag(FLAGSUB, false);
    this->setFlag(FLAGHALFCARRY, false); //set if carry to bit 4
    this->setFlag(FLAGCARRY, false);
}

void cpu::add(uint8_t &regA, uint8_t data){
    uint16_t a = uint16_t(regA);
    uint16_t d = uint16_t(data);
    uint16_t r = regA + data;

    this->setFlag(FLAGZERO, (r & 0xFF) == 0);
    this->setFlag(FLAGSUB, false);
    this->setFlag(FLAGHALFCARRY, ((a & 0x0F) + (d & 0x0F)) > 0x0F); //set if carry to bit 4
    this->setFlag(FLAGCARRY, r > 0xFF);
    regA = (r & 0xFF);
}
void cpu::sub(uint8_t &regA, uint8_t data){
    int16_t a = int16_t(regA);
    int16_t d = int16_t(data);

    int16_t r = a - d;

    this->setFlag(FLAGZERO, (r & 0xFF) == 0);
    this->setFlag(FLAGSUB, true);
    this->setFlag(FLAGHALFCARRY, ((a & 0xF) - (d & 0xF)) < 0); //set if carry to bit 4
    this->setFlag(FLAGCARRY, r < 0);
    regA = (r & 0xFF);
}
void cpu::sbc(uint8_t &regA, uint8_t data){
    uint16_t c = (reg.F & FLAGCARRY) ? 1 : 0 ;
    int16_t a = int16_t(regA);
    int16_t d = int16_t(data);

    int16_t r = a - d - c;

    this->setFlag(FLAGZERO, (r & 0xFF) == 0);
    this->setFlag(FLAGSUB, true);
    this->setFlag(FLAGHALFCARRY, ((a & 0x0F) - (d & 0x0F) - c) < 0); //set if carry to bit 4
    this->setFlag(FLAGCARRY, r < 0);
    regA = (r & 0xFF);
}
void cpu::adc(uint8_t &regA, uint8_t data){
    uint16_t c = (reg.F & FLAGCARRY) ? 1 : 0 ;
    uint16_t a = uint16_t(regA);
    uint16_t d = uint16_t(data);

    int16_t r = a + d + c;

    this->setFlag(FLAGZERO, (r & 0xFF) == 0);
    this->setFlag(FLAGSUB, false);
    this->setFlag(FLAGHALFCARRY, ((a & 0x0F) + (d & 0x0F) + c) > 0x0F); //set if carry to bit 4
    this->setFlag(FLAGCARRY, r > 0xFF);
    regA = (r & 0xFF);

}

void cpu::cp(uint8_t &regA, uint8_t data){
    int16_t a = int16_t(regA);
    int16_t d = int16_t(data);

    this->setFlag(FLAGZERO, (regA == data));
    this->setFlag(FLAGSUB, true);
    this->setFlag(FLAGHALFCARRY, ((a & 0xF) - (d & 0xF)) < 0); //set if carry to bit 4
    this->setFlag(FLAGCARRY, regA > data);
}

//stack operations
void cpu::push(uint8_t &regA, uint8_t &regB){
    reg.SP--;
    mem.write(reg.SP, regA);
    reg.SP--;
    mem.write(reg.SP, regB);
}
void cpu::pop(uint8_t &regA, uint8_t &regB){
    regA = mem.read(reg.SP);
    reg.SP++;
    regB = mem.read(reg.SP);
    reg.SP++;
}
void cpu::decSP(){ reg.SP--;}
void cpu::incSP(){ reg.SP++;}

//set register flags
void cpu::setFlag(const int flag, bool result){
    if (result){
        this->reg.F |= flag;
    }
    else {
        this->reg.F &= ~flag;
    }
}

//pair register get memory location
uint16_t cpu::pairReg(uint8_t &regA, uint8_t &regB){
    return (regA << 8) + regB;
}
uint8_t cpu::pairData(uint8_t &regA, uint8_t &regB){
    return mem.read(pairReg(regA, regB));
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
    this->decodeByte(gbdata, reg.PC);

    //main loop
    while(isCpuRunning){

        std::cout << "run \n";

    }

}


