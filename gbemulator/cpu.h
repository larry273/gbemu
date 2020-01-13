#ifndef CPU_H
#define CPU_H
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

#include <QObject>
#include <QThread>
#include <QString>
#include <QElapsedTimer>
#include <QDebug>


#include "memory.h"
#include "graphics.h"

#define HEX(x) std::setw(2) << std::setfill('0') << std::hex << int(x)
#define HEXCOUNT(x) std::setw(6) << std::setfill('0') << std::hex << int(x)

#define HEXPRINT(x) QString::number(x, 16).rightJustified(2, '0')
#define HEXPRINT_6(x) QString::number(x, 16).rightJustified(6, '0')


struct gbData {
    std::string filename;
    std::vector<uint8_t> boot;
    std::vector<uint8_t> buffer;
};

struct reg {
    //8 bit registers
    uint8_t A = 0x01;
    uint8_t B = 0x00;
    uint8_t C = 0x13;
    uint8_t D = 0x00;
    uint8_t E = 0xD8;
    uint8_t F = 0xB0; //flag register 7=Zero 6=Subtract(N) 5=Half Carry 4=Carry 3-0 nada
    uint8_t H = 0x01;
    uint8_t L = 0x4D;
    //16 bit
    uint16_t SP = 0xFFFE;
    uint16_t PC = 0x100;
};

class CPU : public QThread
{
    Q_OBJECT

public:
    CPU();
    void cpu_loop();
    void run();

    int cycles;
    memory mem;
    graphics *gpu;

    bool debug = false;
    bool nextOpCode = true;
    bool printOpCode = true;
    int pcStop = 0;

    void reset();
    void resetNoBoot();

    //interface methods
    //return values of all registers into vector
    std::vector<int> getRegValues();
    //get memory values
    QString getMemory();
    QString getVidMemory();
    QString getFlags();
    QString getOpcode();

signals:
    void regValChanged();
    void memoryChanged();
    void vidMemChanged();
    void frameChanged();
    void flagsChanged();
    void opcodeChanged();

private:
    //FLAGS
    const int FLAGZERO = 0b10000000;
    const int FLAGSUB = 0b01000000;
    const int FLAGHALFCARRY = 0b00100000;
    const int FLAGCARRY = 0b0001000;

    const int BITZERO =     0b00000001;
    const int BITONE =      0b00000010;
    const int BITTWO =      0b00000100;
    const int BITTHREE =    0b00001000;
    const int BITFOUR =     0b00010000;
    const int BITFIVE =     0b00100000;
    const int BITSIX =      0b01000000;
    const int BITSEVEN =    0b10000000;

    float CLOCK_SPEED = 4.194304 * 1e6;
    //cpu registers
    struct reg reg;

    bool interruptsEnabled;
    struct gbData gbdata;
    QString currentOpCode;
    int lastOpByte;

    void setFlag(const int flag, bool result);
    //pair two 8 bit registers for one 16 bit reg
    uint16_t pairReg(uint8_t &regA, uint8_t &regB);
    //join two bytes together
    uint16_t joinByte(uint8_t byte1, uint8_t byte2);
    uint8_t pairRegData(uint8_t &regA, uint8_t &regB);
    void pairRegSet(uint8_t &regA, uint8_t &regB, uint16_t data);




    void performInterrupts(uint16_t &pc);

    //8 bit register loads
    void load(uint8_t &regA, uint8_t regB);
    void load(uint8_t &regA, uint8_t *data);

    //16 bit register load
    void load(uint8_t &regA, uint8_t &regB, uint8_t data1, uint8_t data2);

    //8 bit loads from/to memory locations
    void load(uint8_t &regA, uint16_t memLoc);
    void load(uint16_t memLoc, uint8_t &regA);
    void load(uint16_t memLoc, uint16_t &regA);

    //LDH
    void ldh_E0(uint8_t data);
    void ldh_F0(uint8_t data);

    //8 bit arithmetic
    void add(uint8_t &regA, uint8_t data);
    void sub(uint8_t &regA, uint8_t data);
    void sbc(uint8_t &regA, uint8_t data);
    void adc(uint8_t &regA, uint8_t data);
    void logXor(uint8_t &regA, uint8_t regB);
    void logAnd(uint8_t &regA, uint8_t regB);
    void logOr(uint8_t &regA, uint8_t regB);
    void inc(uint8_t &regA, bool isMem = false);
    void dec(uint8_t &regA, bool isMem = false);
    void cp(uint8_t &regA, uint8_t data);

    //16 bit arithmetic
    void inc(uint8_t &regA, uint8_t &regB);
    void dec(uint8_t &regA, uint8_t &regB);
    void add(uint8_t &regA, uint8_t &regB, uint8_t &regC, uint8_t &regD);

    //stack operations
    void decSP();
    void incSP();
    void push(uint8_t &regA, uint8_t &regB);
    void pop(uint8_t &regA, uint8_t &regB);
    void call(uint8_t byte1, uint8_t byte2);
    bool call(uint8_t byte1, uint8_t byte2, int flag);
    void addSP2Reg(uint8_t &regA, uint8_t &regB);
    void loadSP2Reg(uint8_t &regA, uint8_t &regB);
    void loadSP(uint8_t data1, uint8_t data2);
    void addByte2SP(int8_t data);

    void swap(uint8_t &regA, bool isMem = false);

    //interrupts enable/disable
    void ei();
    void di();

    //adjust a to obtain BCD representation of decimal
    void daa();

    //rotates
    void rlca();    //rotate a left, 7 bit into carry
    void rla();     //rotate a left through carry
    void rrca();    //rotate a right, 0 bit to carry
    void rra();     //rotate a right through carry
    void rlc(uint8_t &data, bool isMem = false);    //rotate n left, 7 carry
    void rl(uint8_t &data, bool isMem = false);
    void rrc(uint8_t &data, bool isMem = false);
    void rr(uint8_t &data, bool isMem = false);

    //shifts
    void sla(uint8_t &data, bool isMem = false);    //shift left, 7 bit carry, 0 bit zero
    void sra(uint8_t &data, bool isMem = false);    //shift right, MSB stay the same
    void srl(uint8_t &data, bool isMem = false);    //shift right, MSB 0

    //bit operations
    void bit(int bit, uint8_t &regA, bool isMem = false);
    void set(int bit, uint8_t &regA,  bool isMem = false);
    void res(int bit, uint8_t &regA,  bool isMem = false);

    void cpl();
    void ccf();
    void scf();
    void nop();
    void halt();
    void stop();

    //jump operations
    void jp(uint16_t addr);
    bool jp(uint16_t addr, int flag);

    //jump to relative address
    void jr(int8_t data);
    bool jr(int8_t data, int flag);

    //returns
    void ret(); //pop 2 from stack and jump to addr
    bool ret(int flag); //NZ,Z,NC,C
    void reti();

    void rst(uint8_t byte);

    void decodeByte(uint16_t &pc);
    void decodePreByte(uint8_t bitCode); //0xCB prefixed opcode
    void readFiletoBytes(struct gbData &gbdata, bool isBoot=false);
    QString formatMemory(std::vector<uint8_t> data, int count);
    QString formatHex(uint8_t data, int fill = 2);

};



#endif // CPU_H
