#include "cpu.h"
#include <QDir>

CPU::CPU(){
    //reg.PC = 0;
    //cpu_loop();
}

void CPU::run(){
    cpu_loop();
}

//cpu loop
void CPU::cpu_loop(){

    bool isCpuRunning = true;

    //struct gbData gbdata; //gb data file
    //gbdata.filename = "cpu_instrs.gb";
    QDir::setCurrent("/home/larry/Documents/gbemu/gbemulator");
    gbdata.filename = "cpu_instrs.gb";
    //gbdata.filename = "boot.bin";

    this->readFiletoBytes(gbdata);

    //main loop
    //measure timing
    clock_t t;
    t = clock();
    int instru = 0;

    while(isCpuRunning){

        decodeByte(gbdata, reg.PC);
        instru++;

        if (instru > 200){
            break;
        }
        emit regValChanged();
        emit memoryChanged();
    }


    t = clock() - t;
    std::cout << "instructions: " << std::dec << instru << "\n";
    std::cout << "time elapsed: " << std::dec << t*1.0/CLOCKS_PER_SEC  << " seconds. done decoding\n";

}

//read file as bytes
void CPU::readFiletoBytes(struct gbData &gbdata){
    std::cout << "starting file read \n";

    std::ifstream file(gbdata.filename, std::ios::binary);
    if( !file.is_open() ) {
        std::cout << "file failed to open \n";
        return;
    }

    //read content of file into buffer

    std::vector<uint8_t> content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    int count = 0;
    for(unsigned char& c : content) {
        if (count % 16 == 0){
            std::cout << "\n" << std::setw(6) << std::setfill('0') << count/16 << ": ";
        }
        std::cout << HEX(c) << ' ';
        count++;
    }


    gbdata.buffer = content;
    mem.gameRom.insert(mem.gameRom.begin(), content.begin(), content.begin() + 0x3FFF);


    std::cout << "\nfinishing file read \n";
}

std::vector<int> CPU::getRegValues(){

    int af = joinByte(reg.A, reg.F);
    int bc = joinByte(reg.B, reg.C);
    int de = joinByte(reg.D, reg.E);
    int hl = joinByte(reg.H, reg.L);

    std::vector<int> values = {af, bc, de, hl, reg.PC, reg.SP};
    return values;
}

QString CPU::getMemory(){

    int count = 0x4000;
    QString output;
    for(unsigned char& c : mem.mainMem) {
        if (count % 16 == 0){
            //std::cout << "\n" << std::setw(6) << std::setfill('0') << count/16 << ": ";
            output += ("\n" + QString::number(count, 16).rightJustified(6, '0') + ": ");
        }
        output += QString::number(c, 16).rightJustified(2, '0') + ' ';
        //std::cout << HEX(c) << ' ';
        count++;
    }
    return output;
}

//8 bit register loads
void CPU::load(uint8_t &regA, uint8_t &regB){
    regA = regB;
}
void CPU::load(uint8_t &regA, uint8_t *data){
    regA = *data;
}
void CPU::load(uint8_t &regA, uint16_t memLoc){
    uint8_t data = mem.read(memLoc);
    regA = data;
}
void CPU::load(uint16_t memLoc, uint8_t &regA){
    mem.write(memLoc, regA);
}
void CPU::load(uint16_t memLoc, uint16_t &regA){
    mem.write(memLoc, regA);
}
void CPU::ldh_E0(uint8_t data){
    uint16_t addr = 0xFF00 + data;
    mem.write(addr, reg.A);
}
void CPU::ldh_F0(uint8_t data){
    uint16_t addr = 0xFF00 + data;
    reg.A = mem.read(addr);
}

//16 bit register load nn
void CPU::load(uint8_t &regA, uint8_t &regB, uint8_t data1, uint8_t data2){
    regA = data1;
    regB = data2;
}

void CPU::loadSP(uint8_t data1, uint8_t data2){
    uint16_t data = joinByte(data1, data2);//(data1 << 8) + data2;
    reg.SP = data;
}

//8 bit arithmetic
void CPU::inc(uint8_t &regA, bool isMem){
    if (isMem){
        regA = mem.read(pairReg(reg.H, reg.L));
    }

    uint8_t r = regA + 1;

    this->setFlag(FLAGZERO, r == 0);
    this->setFlag(FLAGSUB, false);
    this->setFlag(FLAGHALFCARRY, ((regA & 0xF) + (1 & 0xF)) > 0xF); //set if carry to bit 4
    regA = r;

    if (isMem){
        mem.write(pairReg(reg.H, reg.L), regA);
    }
}
void CPU::dec(uint8_t &regA, bool isMem){
    if (isMem){
        regA = mem.read(pairReg(reg.H, reg.L));
    }

    uint8_t r = regA - 1;

    this->setFlag(FLAGZERO, r == 0);
    this->setFlag(FLAGSUB, true);
    this->setFlag(FLAGHALFCARRY, ((regA & 0xF) - (1 & 0xF)) < 0); //set if no borrow from bit 4
    regA = r;

    if (isMem){
        mem.write(pairReg(reg.H, reg.L), regA);
    }
}
void CPU::logAnd(uint8_t &regA, uint8_t regB){
    regA = regA & regB;

    this->setFlag(FLAGZERO, regA == 0);
    this->setFlag(FLAGSUB, false);
    this->setFlag(FLAGHALFCARRY, true); //set if carry to bit 4
    this->setFlag(FLAGCARRY, false);
}

void CPU::logOr(uint8_t &regA, uint8_t regB){
    regA = regA ^ regB;

    this->setFlag(FLAGZERO, regA == 0);
    this->setFlag(FLAGSUB, false);
    this->setFlag(FLAGHALFCARRY, false); //set if carry to bit 4
    this->setFlag(FLAGCARRY, false);
}

void CPU::logXor(uint8_t &regA, uint8_t regB){
    regA = regA ^ regB;

    this->setFlag(FLAGZERO, regA == 0);
    this->setFlag(FLAGSUB, false);
    this->setFlag(FLAGHALFCARRY, false); //set if carry to bit 4
    this->setFlag(FLAGCARRY, false);
}

void CPU::add(uint8_t &regA, uint8_t data){
    uint16_t a = uint16_t(regA);
    uint16_t d = uint16_t(data);
    uint16_t r = regA + data;

    this->setFlag(FLAGZERO, (r & 0xFF) == 0);
    this->setFlag(FLAGSUB, false);
    this->setFlag(FLAGHALFCARRY, ((a & 0x0F) + (d & 0x0F)) > 0x0F); //set if carry to bit 4
    this->setFlag(FLAGCARRY, r > 0xFF);
    regA = (r & 0xFF);
}
void CPU::sub(uint8_t &regA, uint8_t data){
    int16_t a = int16_t(regA);
    int16_t d = int16_t(data);

    int16_t r = a - d;

    this->setFlag(FLAGZERO, (r & 0xFF) == 0);
    this->setFlag(FLAGSUB, true);
    this->setFlag(FLAGHALFCARRY, ((a & 0xF) - (d & 0xF)) < 0); //set if carry to bit 4
    this->setFlag(FLAGCARRY, r < 0);
    regA = (r & 0xFF);
}
void CPU::sbc(uint8_t &regA, uint8_t data){
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
void CPU::adc(uint8_t &regA, uint8_t data){
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

void CPU::cp(uint8_t &regA, uint8_t data){
    int16_t a = int16_t(regA);
    int16_t d = int16_t(data);

    this->setFlag(FLAGZERO, (regA == data));
    this->setFlag(FLAGSUB, true);
    this->setFlag(FLAGHALFCARRY, ((a & 0xF) - (d & 0xF)) < 0); //set if carry to bit 4
    this->setFlag(FLAGCARRY, regA > data);
}

//16 bit arithmetic
void CPU::inc(uint8_t &regA, uint8_t &regB){
    uint16_t pairVal = pairReg(regA, regB);
    pairVal += 1;

    pairRegSet(regA, regB, pairVal);
}
void CPU::dec(uint8_t &regA, uint8_t &regB){
    uint16_t pairVal = pairReg(regA, regB);
    pairVal -= 1;

    pairRegSet(regA, regB, pairVal);
}
void CPU::add(uint8_t &regA, uint8_t &regB, uint8_t &regC, uint8_t &regD){
    uint16_t val1 = pairReg(regA, regB);
    uint16_t val2 = pairReg(regC, regD);

    uint16_t result = val1 + val2;
    pairRegSet(regA, regB, result);
}
void CPU::addSP2Reg(uint8_t &regA, uint8_t &regB){
    uint16_t val1 = pairReg(regA, regB);

    uint16_t result = val1 + reg.SP;
    pairRegSet(regA, regB, result);
}
void CPU::loadSP2Reg(uint8_t &regA, uint8_t &regB){
    pairRegSet(regA, regB, reg.SP);
}
void CPU::addByte2SP(int8_t data){
    uint16_t r = reg.SP + data;

    this->setFlag(FLAGZERO, (r & 0xFF) == 0);
    this->setFlag(FLAGSUB, false);
    this->setFlag(FLAGHALFCARRY, ((r & 0x0F)) < (reg.SP & 0x0F)); //set if carry to bit 4
    this->setFlag(FLAGCARRY, ((r & 0xFF) < (reg.SP & 0xFF)) );
    reg.SP = (r & 0xFF);
}


//stack operations
void CPU::push(uint8_t &regA, uint8_t &regB){
    reg.SP--;
    mem.write(reg.SP, regA);
    reg.SP--;
    mem.write(reg.SP, regB);
}
void CPU::pop(uint8_t &regA, uint8_t &regB){
    regA = mem.read(reg.SP);
    reg.SP++;
    regB = mem.read(reg.SP);
    reg.SP++;
}
void CPU::call(uint8_t byte1, uint8_t byte2){
    uint16_t byte = joinByte(byte1, byte2);//(byte1 << 8) + byte2;
    uint16_t data = reg.PC;

    uint8_t data1 = data & 0x00FF;
    uint8_t data2 = (data & 0xFF00) >> 8;

    push(data1, data2);
    jp(byte);
}
bool CPU::call(uint8_t byte1, uint8_t byte2, int flag){

    bool isReset = true;
    if (flag == FLAGZERO || flag == FLAGCARRY){
        isReset = false;
    }
    else {
        flag = ~flag;
    }


    if ((reg.F & flag) == 0 && isReset){
        call(byte1, byte2);
        return true;
    }
    else if ((reg.F & flag) != 0 && !isReset) {
        call(byte1, byte2);
        return true;
    }
    else{
        return false;
    }
}

//returns
void CPU::ret(){
    uint8_t addr1 = mem.read(reg.SP++);
    uint8_t addr2 = mem.read(reg.SP++);

    jp( joinByte(addr1, addr2));
}
bool CPU::ret(int flag){
    bool isReset = true;
    if (flag == FLAGZERO || flag == FLAGCARRY){
        isReset = false;
    }
    else {
        flag = ~flag;
    }


    if ((reg.F & flag) == 0 && isReset){
        ret();
        return true;
    }
    else if ((reg.F & flag) != 0 && !isReset) {
        ret();
        return true;
    }
    else{
        return false;
    }
}
void CPU::reti(){
    ret();
    ei();
}

void CPU::decSP(){ reg.SP--;}
void CPU::incSP(){ reg.SP++;}


//interrupts, TODO implement interrupts
void CPU::ei(){ interruptsEnabled = true;}
void CPU::di(){ interruptsEnabled = false;}

void CPU::cpl(){ reg.A = ~reg.A;}
void CPU::ccf(){ this->setFlag(FLAGCARRY, !(reg.F & FLAGCARRY) );}
void CPU::scf(){ this->setFlag(FLAGCARRY, true);}

//TODO: jump operations
void CPU::jp(uint16_t addr){
    reg.PC = addr; //mem.read(addr);
}
bool CPU::jp(uint16_t addr, int flag){

    bool isReset = true;
    if (flag == FLAGZERO || flag == FLAGCARRY){
        isReset = false;
    }
    else {
        flag = ~flag;
    }


    if ((reg.F & flag) == 0 && isReset){
        jp(addr);
        return true;
    }
    else if ((reg.F & flag) != 0 && !isReset) {
        jp(addr);
        return true;
    }
    else{
        return false;
    }
}
void CPU::jr(int8_t data){
    reg.PC = reg.PC + data;
    jp(reg.PC);
}
bool CPU::jr(int8_t data, int flag){
    bool isReset = true;
    if (flag == FLAGZERO || flag == FLAGCARRY){
        isReset = false;
    }
    else {
        flag = ~flag;
    }


    if ((reg.F & flag) == 0 && isReset){
        jr(data);
        return true;
    }
    else if ((reg.F & flag) != 0 && !isReset) {
        jr(data);
        return true;
    }
    else{
        return false;
    }
}

//rotate and shift operations
void CPU::rlca(){
    int old7Bit = (0b10000000 & reg.A) != 0;
    reg.A = (reg.A << 1) | (reg.A >> 7);

    this->setFlag(FLAGZERO, (reg.A) == 0);
    this->setFlag(FLAGSUB, false);
    this->setFlag(FLAGHALFCARRY, false);
    this->setFlag(FLAGCARRY, old7Bit != 0);
}
void CPU::rla(){
    int old7Bit = (0b10000000 & reg.A) != 0;
    reg.A = (reg.A << 1) | ((reg.F & FLAGCARRY) != 0);

    this->setFlag(FLAGZERO, (reg.A) == 0);
    this->setFlag(FLAGSUB, false);
    this->setFlag(FLAGHALFCARRY, false);
    this->setFlag(FLAGCARRY, old7Bit != 0);
}
void CPU::rrca(){
    int old7Bit = (0b10000000 & reg.A) != 0;
    reg.A = (reg.A >> 1) | (reg.A << 7);

    this->setFlag(FLAGZERO, (reg.A) == 0);
    this->setFlag(FLAGSUB, false);
    this->setFlag(FLAGHALFCARRY, false);
    this->setFlag(FLAGCARRY, old7Bit != 0);
}
void CPU::rra(){
    int old7Bit = (0b10000000 & reg.A) != 0;
    reg.A = (reg.A >> 1) | (((reg.F & FLAGCARRY) != 0) << 7);

    this->setFlag(FLAGZERO, (reg.A) == 0);
    this->setFlag(FLAGSUB, false);
    this->setFlag(FLAGHALFCARRY, false);
    this->setFlag(FLAGCARRY, old7Bit != 0);
}
void CPU::rlc(uint8_t &data, bool isMem){
    if (isMem){
        data = mem.read(pairReg(reg.H, reg.L));
    }

    int old7Bit = (0b10000000 & data) != 0;
    data = (data << 1) | (data >> 7);

    this->setFlag(FLAGZERO, (data) == 0);
    this->setFlag(FLAGSUB, false);
    this->setFlag(FLAGHALFCARRY, false);
    this->setFlag(FLAGCARRY, old7Bit != 0);

    if (isMem){
        mem.write(pairReg(reg.H, reg.L), data);
    }
}
void CPU::rl(uint8_t &data, bool isMem){
    if (isMem){
        data = mem.read(pairReg(reg.H, reg.L));
    }

    int old7Bit = (0b10000000 & data) != 0;
    data = (data << 1) | ((reg.F & FLAGCARRY) != 0);

    this->setFlag(FLAGZERO, (data) == 0);
    this->setFlag(FLAGSUB, false);
    this->setFlag(FLAGHALFCARRY, false);
    this->setFlag(FLAGCARRY, old7Bit != 0);

    if (isMem){
        mem.write(pairReg(reg.H, reg.L), data);
    }

}
void CPU::rrc(uint8_t &data, bool isMem){
    if (isMem){
        data = mem.read(pairReg(reg.H, reg.L));
    }

    int old0Bit = (0b00000001 & data) != 0;
    data = (data >> 1) | (data << 7);

    this->setFlag(FLAGZERO, (data) == 0);
    this->setFlag(FLAGSUB, false);
    this->setFlag(FLAGHALFCARRY, false);
    this->setFlag(FLAGCARRY, old0Bit != 0);

    if (isMem){
        mem.write(pairReg(reg.H, reg.L), data);
    }

}
void CPU::rr(uint8_t &data, bool isMem){
    if (isMem){
        data = mem.read(pairReg(reg.H, reg.L));
    }

    int old0Bit = (0b00000001 & data) != 0;
    data = (data >> 1) | (((reg.F & FLAGCARRY) != 0) << 7);

    this->setFlag(FLAGZERO, (data) == 0);
    this->setFlag(FLAGSUB, false);
    this->setFlag(FLAGHALFCARRY, false);
    this->setFlag(FLAGCARRY, old0Bit != 0);

    if (isMem){
        mem.write(pairReg(reg.H, reg.L), data);
    }
}

void CPU::sla(uint8_t &data, bool isMem){
    if (isMem){
        data = mem.read(pairReg(reg.H, reg.L));
    }

    int old7Bit = (0b10000000 & data) != 0;
    data = (data << 1);

    this->setFlag(FLAGZERO, (data) == 0);
    this->setFlag(FLAGSUB, false);
    this->setFlag(FLAGHALFCARRY, false);
    this->setFlag(FLAGCARRY, old7Bit != 0);

    if (isMem){
        mem.write(pairReg(reg.H, reg.L), data);
    }
}

void CPU::sra(uint8_t &data, bool isMem){
    if (isMem){
        data = mem.read(pairReg(reg.H, reg.L));
    }

    int old0Bit = (0b00000001 & data) != 0;
    int old7Bit = (0b10000000 & data) != 0;

    data = (data >> 1) | (old7Bit << 7);

    this->setFlag(FLAGZERO, (data) == 0);
    this->setFlag(FLAGSUB, false);
    this->setFlag(FLAGHALFCARRY, false);
    this->setFlag(FLAGCARRY, old0Bit != 0);

    if (isMem){
        mem.write(pairReg(reg.H, reg.L), data);
    }
}

void CPU::srl(uint8_t &data, bool isMem){
    if (isMem){
        data = mem.read(pairReg(reg.H, reg.L));
    }

    int old0Bit = (0b00000001 & data) != 0;
    data = (data >> 1);

    this->setFlag(FLAGZERO, (data) == 0);
    this->setFlag(FLAGSUB, false);
    this->setFlag(FLAGHALFCARRY, false);
    this->setFlag(FLAGCARRY, old0Bit != 0);

    if (isMem){
        mem.write(pairReg(reg.H, reg.L), data);
    }
}

//bit operations
void CPU::bit(int bit, uint8_t &regA, bool isMem){
    uint16_t loc;
    if (isMem){
        loc = pairReg(reg.H, reg.L);
        regA = mem.read(loc);
    }


    int result = (bit & regA) != 0;

    if (result == 0){
        this->setFlag(FLAGZERO, true);
    }
    this->setFlag(FLAGSUB, false);
    this->setFlag(FLAGHALFCARRY, true);


    if (isMem){
        mem.write(loc, regA);
    }
}

void CPU::set(int bit, uint8_t &regA, bool isMem){
    uint16_t loc;
    if (isMem){
        loc = pairReg(reg.H, reg.L);
        regA = mem.read(loc);
    }

    regA = regA | bit;

    if (isMem){
        mem.write(loc, regA);
    }
}
void CPU::res(int bit, uint8_t &regA, bool isMem){
    uint16_t loc;
    if (isMem){
        loc = pairReg(reg.H, reg.L);
        regA = mem.read(loc);
    }

    regA = ~bit & regA;

    if (isMem){
        mem.write(loc, regA);
    }
}


void CPU::swap(uint8_t &regA, bool isMem){
    uint8_t result;
    if (isMem){
        result = mem.read(pairReg(reg.H, reg.L));
    }

    uint8_t upper = regA & 0xF0;
    uint8_t lower = regA & 0x0F;

    result =  (upper >> 4) | (lower << 4);

    this->setFlag(FLAGZERO, (result) == 0);
    this->setFlag(FLAGSUB, false);
    this->setFlag(FLAGHALFCARRY, false);
    this->setFlag(FLAGCARRY, false);

    if (isMem){
        mem.write(pairReg(reg.H, reg.L), result);
    }
}

void CPU::rst(uint8_t byte){

    //TODO: unsure why gba guide has this, explore later
    uint8_t data1 = reg.PC & 0x00FF;
    uint8_t data2 = (reg.PC & 0xFF00) >> 8;
    mem.write(--reg.SP, data2);
    mem.write(--reg.SP, data1);

    reg.PC = joinByte(0x00, byte);
}

//adjust a to obtain BCD representation of decimal
//ex: 0x202 (dec. 514) to 0x514 (dec. 1300)
void CPU::daa(){

    uint8_t binaryInput = reg.A;
    uint16_t result = 0;
    int shift = 0;

    while (binaryInput > 0) {
         result |= (binaryInput % 10) << (shift++ << 2);
         binaryInput /= 10;
      }

    this->setFlag(FLAGZERO, (result) == 0);
    this->setFlag(FLAGHALFCARRY, false);
    this->setFlag(FLAGCARRY, ((result & 0x100) != 0));

    reg.A = result;
}
void CPU::stop(){
    std::cout << "STOP\n";
}





//set register flags
void CPU::setFlag(const int flag, bool result){
    if (result){
        this->reg.F |= flag;
    }
    else {
        this->reg.F &= ~flag;
    }
}

//pair register get memory location
uint16_t CPU::pairReg(uint8_t &regA, uint8_t &regB){
    return (regA << 8) + regB;
}
uint8_t CPU::pairRegData(uint8_t &regA, uint8_t &regB){
    return mem.read(pairReg(regA, regB));
}
void CPU::pairRegSet(uint8_t &regA, uint8_t &regB, uint16_t data){
    uint8_t dataLow = data & 0x00FF;
    uint8_t dataHigh = (data & 0xFF00) >> 8;

    regA = dataHigh;
    regB = dataLow;
}

uint16_t CPU::joinByte(uint8_t byte1, uint8_t byte2){
    return (byte1 << 8) + byte2;
}

void CPU::nop(){
    std::cout << "do nothing\n";
}





