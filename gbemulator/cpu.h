#ifndef CPU_H
#define CPU_H
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string>




class cpu
{
public:
    cpu();
    void cpu_loop();
    int pc = 0;

    struct gbData {
        std::string filename;
        std::string instruction;
        std::vector<unsigned char> buffer;
        int data;
    };

    void readFiletoBytes(struct gbData &gbdata);
    int decodeByte(struct gbData &gbdata, int &pc);


};

#endif // CPU_H
