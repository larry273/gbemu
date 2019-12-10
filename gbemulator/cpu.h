#ifndef CPU_H
#define CPU_H
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string>
#include "opcode.h"

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
    uint8_t F;
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
    int pc = 0;
    //cpu registers
    struct reg reg;

    void readFiletoBytes(struct gbData &gbdata);
    //pair two 8 bit registers for one 16 bit reg
    uint16_t pairRegister(uint8_t &regA, uint8_t &regB);
    //8 bit register loads
    void load(uint8_t &regA, uint8_t &regB);
    void load(uint8_t &regA, uint8_t data);
    void load(uint8_t &regA, uint8_t &regB, uint8_t data);
    void load(uint8_t &regA, uint8_t &regB, uint8_t &regC, int ldType);
    void load(uint16_t data, uint8_t &regA);

    //16 bit register loads
    //TODO

    //int decodeByte(struct gbData &gbdata, int &pc);


};



#endif // CPU_H
