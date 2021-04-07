#include "cpu.h"
#include <QDir>

#include <QDebug>

CPU::CPU(){
    gpu = new graphics(&mem);
}

void CPU::run(){
    cpu_loop();
    std::cout << "If you can read this send help \n";
}

//cpu loop
void CPU::cpu_loop(){
    bool isCpuRunning = true;
    QElapsedTimer timer;

    QDir::setCurrent("C:\\Files\\PROGRAMMING\\C++\\gbemu\\gbemu");

    gbdata.filename = "blue.gb";
    //gbdata.filename = "boot.bin";

    //reset registers to default no boot values
    reset();

    this->readFiletoBytes(gbdata, true);
    this->readFiletoBytes(gbdata);

    int currentCycle = 0;
    float cyclesPerFrame = CLOCK_SPEED / 60;
    float frameTime = 1000 / 60;
    //debugging
    debug = true;
    printOpCode = true;

    emit gpu->frameCompleted();

    //1 machine cycles = 4 clock cycles
    while(isCpuRunning){
        //one loop equals one frame
        timer.restart();
        cycles = 0;
        currentCycle = 0;

        //opcodes per frame loop
        while (currentCycle < cyclesPerFrame){
            decodeByte(reg.PC);
            //gpu->updateScanline(cycles);
            //performInterrupts(reg.PC);
            currentCycle += cycles;
        }

        //delay timings
        float delayTime = frameTime - timer.elapsed();
        if (timer.elapsed() < frameTime){
            //std::cout << "DELAYTIME: " << delayTime << "\n";
            QThread::msleep(delayTime);
        }
    }
}

//read file as bytes
void CPU::readFiletoBytes(struct gbData &gbdata, bool isBoot){
    qDebug() << "starting file read \n";

    std::string loadFile = gbdata.filename;
    if(isBoot){
        loadFile = "boot.bin";
    }

    std::ifstream file(loadFile, std::ios::binary);
    if( !file.is_open() ) {
        qDebug() << "file failed to open \n";
        return;
    }

    //read content of file into buffer
    std::vector<uint8_t> content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    /*
    int count = 0;
    for(unsigned char& c : content) {
        if (count % 16 == 0){
            std::cout << "\n" << std::setw(6) << std::setfill('0') << count/16 << ": ";
        }
        std::cout << HEX(c) << ' ';
        count++;
    }
    */

    if(!isBoot){
        gbdata.buffer = content;
        //add game rom to memory location
        mem.gameRom.insert(mem.gameRom.begin() + 0x0100, content.begin() + 0x100, content.begin() + (0x3FFF - 0x0100) );
        mem.gameRomSwitchable.insert(mem.gameRomSwitchable.begin(), content.begin() + (0x3FFF - 0x0100), content.begin() + (0x7FFF - 0x0100));
    }
    else{
        gbdata.boot = content;
        mem.gameRom.insert(mem.gameRom.begin(), content.begin(), content.begin() + 0x3FFF);
    }


    std::cout << "\nfinish file read \n";
}

std::vector<int> CPU::getRegValues(){
    int af = joinByte(reg.A, reg.F);
    int bc = joinByte(reg.B, reg.C);
    int de = joinByte(reg.D, reg.E);
    int hl = joinByte(reg.H, reg.L);

    int lcdc = mem.read(0xFF40);
    int stat = mem.read(0xFF41);
    int ly = mem.read(0xFF44);

    std::vector<int> values = {af, bc, de, hl, reg.SP, reg.PC, lcdc, stat, ly};
    return values;
}

QString CPU::getMemory(){
    QString output;
    //output += formatMemory(mem.gameRom, 0);
    //output += formatMemory(mem.mainMem, 0x4000);
    output += formatMemory(mem.smallMem, 0xFF00);
    output += formatMemory(mem.videoMem, 0x8000);
    return output;
}
QString CPU::getVidMemory(){
    QString output;
    output += formatMemory(mem.videoMem, 0x8000);
    output += formatMemory(mem.spriteMem, 0xFE00);
    return output;
}
QString CPU::getFlags(){
    int Z = reg.F >> 7;
    int N = (reg.F >> 6) & 0x1;
    int H = (reg.F >> 5) & 0x1;
    int C = (reg.F >> 4) & 0x1;

    QString output = QString("Z: %1 N: %2 H: %3 C: %4").arg(Z).arg(N).arg(H).arg(C);
    return output;
}
QString CPU::getOpcode(){
    return currentOpCode + " \n";
}

void CPU::performInterrupts(uint16_t &pc){
    //interrupt enable register FFFF

}

void CPU::reset(){
    reg.A =  0;
    reg.B =  0;
    reg.C =  0;
    reg.D =  0;
    reg.E =  0;
    reg.F =  0;
    reg.H =  0;
    reg.L =  0;
    reg.SP = 0;
    reg.PC = 0;
}
void CPU::resetNoBoot(){
    reg.A = 0x01;
    reg.B = 0x00;
    reg.C = 0x13;
    reg.D = 0x00;
    reg.E = 0xD8;
    reg.F = 0xB0;
    reg.H = 0x01;
    reg.L = 0x4D;
    reg.SP = 0xFFFE;
    reg.PC = 0x100;
}


//8 bit register loads
void CPU::load(uint8_t &regA, uint8_t regB){
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
        //regA = mem.read(pairReg(reg.H, reg.L));
        inc(reg.H, reg.L);
        return;
    }

    uint8_t r = regA + 1;

    setFlag(FLAGZERO, r == 0);
    setFlag(FLAGSUB, false);
    setFlag(FLAGHALFCARRY, ((regA & 0xF) + (1 & 0xF)) > 0xF); //set if carry to bit 4
    regA = r;

    //if (isMem){
    //    mem.write(pairReg(reg.H, reg.L), regA);
    //}
}
void CPU::dec(uint8_t &regA, bool isMem){
    if (isMem){
        //regA = mem.read(pairReg(reg.H, reg.L));
        dec(reg.H, reg.L);
        return;
    }

    uint8_t r = regA - 1;

    setFlag(FLAGZERO, r == 0);
    setFlag(FLAGSUB, true);
    setFlag(FLAGHALFCARRY, ((regA & 0xF) - (1 & 0xF)) < 0); //set if no borrow from bit 4
    regA = r;

    //if (isMem){
    //    mem.write(pairReg(reg.H, reg.L), regA);
    //}
}
void CPU::logAnd(uint8_t &regA, uint8_t regB){
    regA = regA & regB;

    setFlag(FLAGZERO, regA == 0);
    setFlag(FLAGSUB, false);
    setFlag(FLAGHALFCARRY, true); //set if carry to bit 4
    setFlag(FLAGCARRY, false);
}

void CPU::logOr(uint8_t &regA, uint8_t regB){
    regA = regA ^ regB;

    setFlag(FLAGZERO, regA == 0);
    setFlag(FLAGSUB, false);
    setFlag(FLAGHALFCARRY, false); //set if carry to bit 4
    setFlag(FLAGCARRY, false);
}

void CPU::logXor(uint8_t &regA, uint8_t regB){
    regA = regA ^ regB;

    setFlag(FLAGZERO, regA == 0);
    setFlag(FLAGSUB, false);
    setFlag(FLAGHALFCARRY, false); //set if carry to bit 4
    setFlag(FLAGCARRY, false);
}

void CPU::add(uint8_t &regA, uint8_t data){
    uint16_t a = uint16_t(regA);
    uint16_t d = uint16_t(data);
    uint16_t r = regA + data;

    setFlag(FLAGZERO, (r & 0xFF) == 0);
    setFlag(FLAGSUB, false);
    setFlag(FLAGHALFCARRY, ((a & 0x0F) + (d & 0x0F)) > 0x0F); //set if carry to bit 4
    setFlag(FLAGCARRY, r > 0xFF);
    regA = (r & 0xFF);
}
void CPU::sub(uint8_t &regA, uint8_t data){
    int16_t a = int16_t(regA);
    int16_t d = int16_t(data);

    int16_t r = a - d;

    setFlag(FLAGZERO, (r & 0xFF) == 0);
    setFlag(FLAGSUB, true);
    setFlag(FLAGHALFCARRY, ((a & 0xF) - (d & 0xF)) < 0); //set if carry to bit 4
    setFlag(FLAGCARRY, r < 0);
    regA = (r & 0xFF);
}
void CPU::sbc(uint8_t &regA, uint8_t data){
    uint16_t c = (reg.F & FLAGCARRY) ? 1 : 0 ;
    int16_t a = int16_t(regA);
    int16_t d = int16_t(data);

    int16_t r = a - d - c;

    setFlag(FLAGZERO, (r & 0xFF) == 0);
    setFlag(FLAGSUB, true);
    setFlag(FLAGHALFCARRY, ((a & 0x0F) - (d & 0x0F) - c) < 0); //set if carry to bit 4
    setFlag(FLAGCARRY, r < 0);
    regA = (r & 0xFF);
}
void CPU::adc(uint8_t &regA, uint8_t data){
    uint16_t c = (reg.F & FLAGCARRY) ? 1 : 0 ;
    uint16_t a = uint16_t(regA);
    uint16_t d = uint16_t(data);

    int16_t r = a + d + c;

    setFlag(FLAGZERO, (r & 0xFF) == 0);
    setFlag(FLAGSUB, false);
    setFlag(FLAGHALFCARRY, ((a & 0x0F) + (d & 0x0F) + c) > 0x0F); //set if carry to bit 4
    setFlag(FLAGCARRY, r > 0xFF);
    regA = (r & 0xFF);
}

void CPU::cp(uint8_t &regA, uint8_t data){
    int16_t a = int16_t(regA);
    int16_t d = int16_t(data);

    setFlag(FLAGZERO, (regA == data));
    setFlag(FLAGSUB, true);
    setFlag(FLAGHALFCARRY, ((a & 0xF) - (d & 0xF)) < 0); //set if carry to bit 4
    setFlag(FLAGCARRY, regA > data);
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

    setFlag(FLAGZERO, (r & 0xFF) == 0);
    setFlag(FLAGSUB, false);
    setFlag(FLAGHALFCARRY, ((r & 0x0F)) < (reg.SP & 0x0F)); //set if carry to bit 4
    setFlag(FLAGCARRY, ((r & 0xFF) < (reg.SP & 0xFF)) );
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
    regB = mem.read(reg.SP);
    reg.SP++;
    regA = mem.read(reg.SP);
    reg.SP++;
}
void CPU::call(uint8_t byte1, uint8_t byte2){

    reg.PC += 3;
    uint16_t data = reg.PC;

    uint8_t data1 = data & 0x00FF;
    uint8_t data2 = (data & 0xFF00) >> 8;

    mem.write(--reg.SP, data2);
    mem.write(--reg.SP, data1);

    //push(data2, data1);
    uint16_t byte_double = joinByte(byte1, byte2);//(byte1 << 8) + byte2;

    jp(byte_double);
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

    jp( joinByte(addr2, addr1));
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
void CPU::ccf(){ setFlag(FLAGCARRY, !(reg.F & FLAGCARRY) );}
void CPU::scf(){ setFlag(FLAGCARRY, true);}

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
    reg.PC = (reg.PC + 2 ) + data;
    jp(reg.PC);
}
bool CPU::jr(int8_t data, int flag){
    bool isNotSet = true;
    if (flag == FLAGZERO || flag == FLAGCARRY){
        isNotSet = false;
    }
    else {
        flag = ~flag;
    }


    if ((reg.F & flag) == 0 && isNotSet){
        jr(data);
        return true;
    }
    else if ((reg.F & flag) != 0 && !isNotSet) {
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

    setFlag(FLAGZERO, (reg.A) == 0);
    setFlag(FLAGSUB, false);
    setFlag(FLAGHALFCARRY, false);
    setFlag(FLAGCARRY, old7Bit != 0);
}
void CPU::rla(){
    int old7Bit = (0b10000000 & reg.A) != 0;
    reg.A = (reg.A << 1) | ((reg.F & FLAGCARRY) != 0);

    setFlag(FLAGZERO, (reg.A) == 0);
    setFlag(FLAGSUB, false);
    setFlag(FLAGHALFCARRY, false);
    setFlag(FLAGCARRY, old7Bit != 0);
}
void CPU::rrca(){
    int old7Bit = (0b10000000 & reg.A) != 0;
    reg.A = (reg.A >> 1) | (reg.A << 7);

    setFlag(FLAGZERO, (reg.A) == 0);
    setFlag(FLAGSUB, false);
    setFlag(FLAGHALFCARRY, false);
    setFlag(FLAGCARRY, old7Bit != 0);
}
void CPU::rra(){
    int old7Bit = (0b10000000 & reg.A) != 0;
    reg.A = (reg.A >> 1) | (((reg.F & FLAGCARRY) != 0) << 7);

    setFlag(FLAGZERO, (reg.A) == 0);
    setFlag(FLAGSUB, false);
    setFlag(FLAGHALFCARRY, false);
    setFlag(FLAGCARRY, old7Bit != 0);
}
void CPU::rlc(uint8_t &data, bool isMem){
    if (isMem){
        data = mem.read(pairReg(reg.H, reg.L));
    }

    int old7Bit = (0b10000000 & data) != 0;
    data = (data << 1) | (data >> 7);

    setFlag(FLAGZERO, (data) == 0);
    setFlag(FLAGSUB, false);
    setFlag(FLAGHALFCARRY, false);
    setFlag(FLAGCARRY, old7Bit != 0);

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

    setFlag(FLAGZERO, (data) == 0);
    setFlag(FLAGSUB, false);
    setFlag(FLAGHALFCARRY, false);
    setFlag(FLAGCARRY, old7Bit != 0);

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

    setFlag(FLAGZERO, (data) == 0);
    setFlag(FLAGSUB, false);
    setFlag(FLAGHALFCARRY, false);
    setFlag(FLAGCARRY, old0Bit != 0);

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

    setFlag(FLAGZERO, (data) == 0);
    setFlag(FLAGSUB, false);
    setFlag(FLAGHALFCARRY, false);
    setFlag(FLAGCARRY, old0Bit != 0);

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

    setFlag(FLAGZERO, (data) == 0);
    setFlag(FLAGSUB, false);
    setFlag(FLAGHALFCARRY, false);
    setFlag(FLAGCARRY, old7Bit != 0);

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

    setFlag(FLAGZERO, (data) == 0);
    setFlag(FLAGSUB, false);
    setFlag(FLAGHALFCARRY, false);
    setFlag(FLAGCARRY, old0Bit != 0);

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

    setFlag(FLAGZERO, (data) == 0);
    setFlag(FLAGSUB, false);
    setFlag(FLAGHALFCARRY, false);
    setFlag(FLAGCARRY, old0Bit != 0);

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


    int result = (bit & ~regA);

    setFlag(FLAGZERO, result != 0);
    setFlag(FLAGSUB, false);
    setFlag(FLAGHALFCARRY, true);


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

    setFlag(FLAGZERO, (result) == 0);
    setFlag(FLAGSUB, false);
    setFlag(FLAGHALFCARRY, false);
    setFlag(FLAGCARRY, false);

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

    setFlag(FLAGZERO, (result) == 0);
    setFlag(FLAGHALFCARRY, false);
    setFlag(FLAGCARRY, ((result & 0x100) != 0));

    reg.A = result;
}
void CPU::stop(){
    std::cout << "STOP\n";
}
void CPU::halt(){
    //halt
}
void CPU::nop(){
    //std::cout << "do nothing\n";
}



//set register flags
void CPU::setFlag(const int flag, bool result){
    if (result){
        reg.F |= flag;
    }
    else {
        reg.F &= ~flag;
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



QString CPU::formatMemory(std::vector<uint8_t> data, int count){
    QString output;
    for(uint8_t &c : data) {
        if (count % 16 == 0){
            output += ("\n" + QString::number(count, 16).rightJustified(6, '0') + ": ");
        }
        output += QString::number(c, 16).rightJustified(2, '0') + ' ';
        count++;
    }
    return output;
}

QString CPU::formatHex(uint8_t data, int fill){
    return QString::number(data, 16).rightJustified(fill, '0');
}


