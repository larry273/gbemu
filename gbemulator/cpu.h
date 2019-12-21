#ifndef CPU_H
#define CPU_H
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string>

#include "memory.h"

#define HEX(x) std::setw(2) << std::setfill('0') << std::hex << int(x)
#define HEXCOUNT(x) std::setw(6) << std::setfill('0') << std::hex << int(x)


struct gbData {
    std::string filename;
    std::string instruction;
    std::vector<unsigned char> buffer;
    int data;
};

struct reg {
    //8 bit registers
    uint8_t A;
    uint8_t B;
    uint8_t C;
    uint8_t D;
    uint8_t E;
    uint8_t F; //flag register 7=Zero 6=Subtract(N) 5=Half Carry 4=Carry 3-0 nada
    uint8_t H;
    uint8_t L;
    //16 bit
    uint16_t SP;
    uint16_t PC;
};

class cpu
{
public:
    cpu();
    void cpu_loop();

private:
    //FLAGS
    const int FLAGZERO = 0b10000000;
    const int FLAGSUB = 0b01000000;
    const int FLAGHALFCARRY = 0b00100000;
    const int FLAGCARRY = 0b0001000;

    void setFlag(const int flag, bool result);
    //pair two 8 bit registers for one 16 bit reg
    uint16_t pairReg(uint8_t &regA, uint8_t &regB);
    uint8_t pairData(uint8_t &regA, uint8_t &regB);

    //cpu registers
    struct reg reg;

    //memory
    memory mem;

    int decodeByte(struct gbData &gbdata, uint16_t &pc);
    void readFiletoBytes(struct gbData &gbdata);

    //8 bit register loads
    void load(uint8_t &regA, uint8_t &regB);
    void load(uint8_t &regA, uint8_t *data);

    //8 bit loads from/to memory locations
    void load(uint8_t &regA, uint16_t memLoc);
    void load(uint16_t memLoc, uint8_t &regA);
    void load(uint16_t memLoc, uint16_t &regA);

    //8 bit arithmetic
    void add(uint8_t &regA, uint8_t data);
    void sub(uint8_t &regA, uint8_t data);
    void sbc(uint8_t &regA, uint8_t data);
    void adc(uint8_t &regA, uint8_t data);
    void logXor(uint8_t &regA, uint8_t regB);
    void logAnd(uint8_t &regA, uint8_t regB);
    void logOr(uint8_t &regA, uint8_t regB);
    void inc(uint8_t &regA);
    void dec(uint8_t &regA);
    void cp(uint8_t &regA, uint8_t data);

    //stack operations
    void decSP();
    void incSP();
    void push(uint8_t &regA, uint8_t &regB);
    void pop(uint8_t &regA, uint8_t &regB);
    void call(uint16_t data);

    //returns
    void ret();
    void ret(std::string condition); //NZ,Z,NC,C
    void reti();

    void nop();

    //TODO16 bit register loads

};



#endif // CPU_H
