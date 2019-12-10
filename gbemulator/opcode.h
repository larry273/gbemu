#ifndef OPCODE_H
#define OPCODE_H

#include <time.h>
#include "cpu.h"


class opcode
{
public:
    opcode();
    int decodeByte(struct gbData &gbdata, int &pc);

};

#endif // OPCODE_H
