#ifndef CPU_H
#define CPU_H
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>




class cpu
{
public:
    cpu();
    void cpu_loop();
    int decodeByte(unsigned char *code, int pc);
    bool readFiletoBytes(char const* filename);

};

#endif // CPU_H
