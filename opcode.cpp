#include "cpu.h"
#include <fstream>


//translate byte code to instructions
void CPU::decodeByte(uint16_t &pc){

    int opbytes; //bytes for instr op

    uint8_t code = mem.read(pc);//gbdata.buffer[pc];
    opbytes = 1; //default 1 byte opcode
    bool isJump = false;

    QDebug debugOpCode = qDebug().noquote();
    if (printOpCode){
        debugOpCode << HEXPRINT_6(pc);
    }

    lastOpByte = pc;

    //debugging
    if (reg.PC == pcStop){ //0X005B lcd on //008e to compare logo data
        debug = true;
    }
    //debug stay in loop
    if (debug){

        emit memoryChanged();
        emit opcodeChanged();

        emit regValChanged();
        emit flagsChanged();

        while(nextOpCode){
            //blocking loop
        }

        //halt emulation until button pressed
        nextOpCode = true;
    }


    //opcode decoding
    switch (code) {
        case 0x00:  nop(); cycles=4; currentOpCode= "NOP"; break;
        case 0x01:{
            uint8_t val1 = mem.read(pc+1); uint8_t val2 = mem.read(pc+2);
            load(reg.B, reg.C, val2, val1);
            currentOpCode= "LD BC nn : " + formatHex(val2) + formatHex(val1);
            opbytes = 3; cycles=12;
            break;
        }
        case 0x02:  load(pairReg(reg.B, reg.C), reg.A); cycles=8; currentOpCode= "LD (BC) A"; break;
        case 0x03:  inc(reg.B, reg.C); cycles=8; currentOpCode= "INC BC"; break;
        case 0x04:  inc(reg.B); cycles=4; currentOpCode= "INC B"; break;
        case 0x05:  dec(reg.B); cycles=4; currentOpCode= "DEC B"; break;
        case 0x06:{
            opbytes = 2; cycles=8;
            currentOpCode= "LD B n : " + formatHex(mem.read(pc+1));
            load(reg.B, mem.read(pc+1));
            break;
        }
        case 0x07:  rlca(); cycles=4; currentOpCode= "RLCA"; break;
        case 0x08:{
            uint8_t val1 = mem.read(pc+1); uint8_t val2 = mem.read(pc+2);
            load( joinByte(val2, val1), reg.SP); currentOpCode= "LD (nn) SP : " + formatHex(val2) + formatHex(val1);
            opbytes = 3; cycles=8;
            break;
        }
        case 0x09:  add(reg.H, reg.L, reg.B, reg.C); cycles=8; currentOpCode= "ADD HL BC"; break;
        case 0x0A:  load(reg.A, pairReg(reg.B, reg.C)); cycles=8; currentOpCode= "LD A (BC)"; break;
        case 0x0B:  dec(reg.B, reg.C); cycles=8; currentOpCode= "DEC BC"; break;
        case 0x0C:  inc(reg.C); cycles=4; currentOpCode= "INC C"; break;
        case 0x0D:  dec(reg.C); cycles=4; currentOpCode= "DEC C"; break;
        case 0x0E:{
            opbytes = 2; cycles=8;
            currentOpCode= "LD C n : " + formatHex(mem.read(pc+1));
            load(reg.C, mem.read(pc+1));
            break;
        }
        case 0x0F:  rrca(); cycles=4; currentOpCode= "RRCA"; break;
        case 0x10:  break;
        case 0x11:{
            uint8_t val1 = mem.read(pc+1); uint8_t val2 = mem.read(pc+2);
            load(reg.D, reg.E, val2, val1);
            currentOpCode= "LD DE nn : " + formatHex(val2) + formatHex(val1);
            opbytes = 3; cycles=12;
            break;
        }
        case 0x12:  load(pairReg(reg.D, reg.E), reg.A); cycles=8; currentOpCode= "LD (DE) A"; break;
        case 0x13:  inc(reg.D, reg.E); cycles=8; currentOpCode= "INC DE"; break;
        case 0x14:  inc(reg.D); cycles=4; currentOpCode= "INC D"; break;
        case 0x15:  dec(reg.D); cycles=4; currentOpCode= "DEC D"; break;
        case 0x16:{
            opbytes = 2; cycles=8;
            currentOpCode= "LD D n : " + formatHex(mem.read(pc+1));
            load(reg.D, mem.read(pc+1));
            break;
        }
        case 0x17:  rla(); cycles=4; currentOpCode= "RLA"; break;
        case 0x18:{
            int8_t val1 = int8_t(mem.read(pc+1));
            opbytes = 2; isJump = true; cycles=8;
            currentOpCode= "JR e : " + formatHex(mem.read(pc+1));
            jr(val1);		break;
        }
        case 0x19:  add(reg.H, reg.L, reg.D, reg.E); cycles=8; currentOpCode= "ADD HL DE"; break;
        case 0x1A:  load(reg.A, pairReg(reg.D, reg.E)); cycles=8; currentOpCode= "LD A (DE)"; break;
        case 0x1B:  dec(reg.D, reg.E); cycles=8; currentOpCode= "DEC DE"; break;
        case 0x1C:  inc(reg.E); cycles=4; currentOpCode= "INC E"; break;
        case 0x1D:  dec(reg.E); cycles=4; currentOpCode= "DEC E"; break;
        case 0x1E:{
            opbytes = 2; cycles=8;
            currentOpCode= "LD E n : " + formatHex(mem.read(pc+1));
            load(reg.E, mem.read(pc+1));
            break;
        }
        case 0x1F:  rra(); cycles=4; currentOpCode= "RRA"; break;
        case 0x20:{
            int8_t val1 = int8_t(mem.read(pc+1));
            opbytes = 2;
            bool jump = jr(val1, ~FLAGZERO); if (jump) {isJump = true; cycles=12;} cycles=8;
            currentOpCode= "JR NZ e : " + formatHex(mem.read(pc+1));
    break;
        }
        case 0x21:{
            uint8_t val1 = mem.read(pc+1); uint8_t val2 = mem.read(pc+2);
            load(reg.H, reg.L, val2, val1);
            currentOpCode= "LD HL nn : " + formatHex(val2) + formatHex(val1);
            opbytes = 3; cycles=12;
            break;
        }
        case 0x22:  load(pairReg(reg.H, reg.L), reg.A); inc(reg.A, true); cycles=8; currentOpCode= "LD (HL+) A"; break;
        case 0x23:  inc(reg.H, reg.L); cycles=8; currentOpCode= "INC HL"; break;
        case 0x24:  inc(reg.H); cycles=4; currentOpCode= "INC H"; break;
        case 0x25:  dec(reg.H); cycles=4; currentOpCode= "DEC H"; break;
        case 0x26:{
            opbytes = 2; cycles=8;
            currentOpCode= "LD H n : " + formatHex(mem.read(pc+1));
            load(reg.H, mem.read(pc+1));
            break;
        }
        case 0x27:  daa(); cycles=4; currentOpCode= "DAA"; break;
        case 0x28:{
            int8_t val1 = int8_t(mem.read(pc+1));
            opbytes = 2;
            bool jump = jr(val1, FLAGZERO); if (jump) {isJump = true; cycles=12;} cycles=8;
            currentOpCode= "JR Z e : " + formatHex(mem.read(pc+1));
    break;
        }
        case 0x29:  add(reg.H, reg.L, reg.H, reg.L); cycles=8; currentOpCode= "ADD HL HL"; break;
        case 0x2A:  load(reg.A, pairReg(reg.H, reg.L)); inc(reg.A, true); cycles=8; currentOpCode= "LD A (HL+)"; break;
        case 0x2B:  dec(reg.H, reg.L); cycles=8; currentOpCode= "DEC HL"; break;
        case 0x2C:  inc(reg.L); cycles=4; currentOpCode= "INC L"; break;
        case 0x2D:  dec(reg.L); cycles=4; currentOpCode= "DEC L"; break;
        case 0x2E:{
            opbytes = 2; cycles=8;
            currentOpCode= "LD L n : " + formatHex(mem.read(pc+1));
            load(reg.L, mem.read(pc+1));
            break;
        }
        case 0x2F:  cpl(); cycles=4; currentOpCode= "CPL"; break;
        case 0x30:{
            int8_t val1 = int8_t(mem.read(pc+1));
            opbytes = 2;
            bool jump = jr(val1, ~FLAGCARRY); if (jump) {isJump = true; cycles=12;} cycles=8;
            currentOpCode= "JR NC e : " + formatHex(mem.read(pc+1));
    break;
        }
        case 0x31:{
            uint8_t val1 = mem.read(pc+1); uint8_t val2 = mem.read(pc+2);
            currentOpCode= "LD SP nn : " + formatHex(val2) + formatHex(val1);
            opbytes = 3; cycles=12;
            break;
        }
        case 0x32:  load(pairReg(reg.H, reg.L), reg.A); dec(reg.A, true); cycles=8; currentOpCode= "LD (HL-) A"; break;
        case 0x33:  incSP(); cycles=8; currentOpCode= "INC SP"; break;
        case 0x34:  inc(reg.A, true); cycles=12; currentOpCode= "INC (HL)"; break;
        case 0x35:  dec(reg.A, true); cycles=12; currentOpCode= "DEC (HL)"; break;
        case 0x36:{
            uint16_t val1 = mem.read(pc+1);
            opbytes = 2; cycles=12;
            currentOpCode= "LD (HL) n : " + formatHex(mem.read(pc+1));
            load( pairReg(reg.H, reg.L ), val1);		break;
        }
        case 0x37:  scf(); cycles=4; currentOpCode= "SCF"; break;
        case 0x38:{
            int8_t val1 = int8_t(mem.read(pc+1));
            opbytes = 2;
            bool jump = jr(val1, FLAGCARRY); if (jump) {isJump = true; cycles=12;} cycles=8;
            currentOpCode= "JR C e : " + formatHex(mem.read(pc+1));
    break;
        }
        case 0x39:  addSP2Reg(reg.H, reg.L); cycles=8; currentOpCode= "ADD HL SP"; break;
        case 0x3A:  load(reg.A, pairReg(reg.H, reg.L) ); dec(reg.A, true); cycles=8; currentOpCode= "LD A (HL-)"; break;
        case 0x3B:  decSP(); cycles=8; currentOpCode= "DEC SP"; break;
        case 0x3C:  inc(reg.A); cycles=4; currentOpCode= "INC A"; break;
        case 0x3D:  dec(reg.A); cycles=4; currentOpCode= "DEC A"; break;
        case 0x3E:{
            opbytes = 2; cycles=8;
            currentOpCode= "LD A n : " + formatHex(mem.read(pc+1));
            load(reg.A, mem.read(pc+1));
            break;
        }
        case 0x3F:  ccf(); cycles=4; currentOpCode= "CCF"; break;
        case 0x40:  load(reg.B, reg.B); cycles=4; currentOpCode= "LD B B"; break;
        case 0x41:  load(reg.B, reg.C); cycles=4; currentOpCode= "LD B C"; break;
        case 0x42:  load(reg.B, reg.D); cycles=4; currentOpCode= "LD B D"; break;
        case 0x43:  load(reg.B, reg.E); cycles=4; currentOpCode= "LD B E"; break;
        case 0x44:  load(reg.B, reg.H); cycles=4; currentOpCode= "LD B H"; break;
        case 0x45:  load(reg.B, reg.L); cycles=4; currentOpCode= "LD B L"; break;
        case 0x46:  load(reg.B, pairReg(reg.H, reg.L)); cycles=8; currentOpCode= "LD B (HL)"; break;
        case 0x47:  load(reg.B, reg.A); cycles=4; currentOpCode= "LD B A"; break;
        case 0x48:  load(reg.C, reg.B); cycles=4; currentOpCode= "LD C B"; break;
        case 0x49:  load(reg.C, reg.C); cycles=4; currentOpCode= "LD C C"; break;
        case 0x4A:  load(reg.C, reg.D); cycles=4; currentOpCode= "LD C D"; break;
        case 0x4B:  load(reg.C, reg.E); cycles=4; currentOpCode= "LD C E"; break;
        case 0x4C:  load(reg.C, reg.H); cycles=4; currentOpCode= "LD C H"; break;
        case 0x4D:  load(reg.C, reg.L); cycles=4; currentOpCode= "LD C L"; break;
        case 0x4E:  load(reg.C, pairReg(reg.H, reg.L)); cycles=8; currentOpCode= "LD C (HL)"; break;
        case 0x4F:  load(reg.C, reg.A); cycles=4; currentOpCode= "LD C A"; break;
        case 0x50:  load(reg.D, reg.B); cycles=4; currentOpCode= "LD D B"; break;
        case 0x51:  load(reg.D, reg.C); cycles=4; currentOpCode= "LD D C"; break;
        case 0x52:  load(reg.D, reg.D); cycles=4; currentOpCode= "LD D D"; break;
        case 0x53:  load(reg.D, reg.E); cycles=4; currentOpCode= "LD D E"; break;
        case 0x54:  load(reg.D, reg.H); cycles=4; currentOpCode= "LD D H"; break;
        case 0x55:  load(reg.D, reg.L); cycles=4; currentOpCode= "LD D L"; break;
        case 0x56:  load(reg.D, pairReg(reg.H, reg.L)); cycles=8; currentOpCode= "LD D (HL)"; break;
        case 0x57:  load(reg.D, reg.A); cycles=4; currentOpCode= "LD D A"; break;
        case 0x58:  load(reg.E, reg.B); cycles=4; currentOpCode= "LD E B"; break;
        case 0x59:  load(reg.E, reg.C); cycles=4; currentOpCode= "LD E C"; break;
        case 0x5A:  load(reg.E, reg.D); cycles=4; currentOpCode= "LD E D"; break;
        case 0x5B:  load(reg.E, reg.E); cycles=4; currentOpCode= "LD E E"; break;
        case 0x5C:  load(reg.E, reg.H); cycles=4; currentOpCode= "LD E H"; break;
        case 0x5D:  load(reg.E, reg.L); cycles=4; currentOpCode= "LD E L"; break;
        case 0x5E:  load(reg.E, pairReg(reg.H, reg.L)); cycles=8; currentOpCode= "LD E (HL)"; break;
        case 0x5F:  load(reg.E, reg.A); cycles=4; currentOpCode= "LD E A"; break;
        case 0x60:  load(reg.H, reg.B); cycles=4; currentOpCode= "LD H B"; break;
        case 0x61:  load(reg.H, reg.C); cycles=4; currentOpCode= "LD H C"; break;
        case 0x62:  load(reg.H, reg.D); cycles=4; currentOpCode= "LD H D"; break;
        case 0x63:  load(reg.H, reg.E); cycles=4; currentOpCode= "LD H E"; break;
        case 0x64:  load(reg.H, reg.H); cycles=4; currentOpCode= "LD H H"; break;
        case 0x65:  load(reg.H, reg.L); cycles=4; currentOpCode= "LD H L"; break;
        case 0x66:  load(reg.H, pairReg(reg.H, reg.L)); cycles=8; currentOpCode= "LD H (HL)"; break;
        case 0x67:  load(reg.H, reg.A); cycles=4; currentOpCode= "LD H A"; break;
        case 0x68:  load(reg.L, reg.B); cycles=4; currentOpCode= "LD L B"; break;
        case 0x69:  load(reg.L, reg.C); cycles=4; currentOpCode= "LD L C"; break;
        case 0x6A:  load(reg.L, reg.D); cycles=4; currentOpCode= "LD L D"; break;
        case 0x6B:  load(reg.L, reg.E); cycles=4; currentOpCode= "LD L E"; break;
        case 0x6C:  load(reg.L, reg.H); cycles=4; currentOpCode= "LD L H"; break;
        case 0x6D:  load(reg.L, reg.L); cycles=4; currentOpCode= "LD L L"; break;
        case 0x6E:  load(reg.L, pairReg(reg.H, reg.L)); cycles=8; currentOpCode= "LD L (HL)"; break;
        case 0x6F:  load(reg.L, reg.A); cycles=4; currentOpCode= "LD L A"; break;
        case 0x70:  load(pairReg(reg.H, reg.L), reg.B); cycles=8; currentOpCode= "LD (HL) B"; break;
        case 0x71:  load(pairReg(reg.H, reg.L), reg.C); cycles=8; currentOpCode= "LD (HL) C"; break;
        case 0x72:  load(pairReg(reg.H, reg.L), reg.D); cycles=8; currentOpCode= "LD (HL) D"; break;
        case 0x73:  load(pairReg(reg.H, reg.L), reg.E); cycles=8; currentOpCode= "LD (HL) E"; break;
        case 0x74:  load(pairReg(reg.H, reg.L), reg.H); cycles=8; currentOpCode= "LD (HL) H"; break;
        case 0x75:  load(pairReg(reg.H, reg.L), reg.L); cycles=8; currentOpCode= "LD (HL) L"; break;
        case 0x76:  halt(); currentOpCode= "HALT"; break;
        case 0x77:  load(pairReg(reg.H, reg.L), reg.A); cycles=8; currentOpCode= "LD (HL) A"; break;
        case 0x78:  load(reg.A, reg.B); cycles=4; currentOpCode= "LD A B"; break;
        case 0x79:  load(reg.A, reg.C); cycles=4; currentOpCode= "LD A C"; break;
        case 0x7A:  load(reg.A, reg.D); cycles=4; currentOpCode= "LD A D"; break;
        case 0x7B:  load(reg.A, reg.E); cycles=4; currentOpCode= "LD A E"; break;
        case 0x7C:  load(reg.A, reg.H); cycles=4; currentOpCode= "LD A H"; break;
        case 0x7D:  load(reg.A, reg.L); cycles=4; currentOpCode= "LD A L"; break;
        case 0x7E:  load(reg.A, pairReg(reg.H, reg.L)); cycles=8; currentOpCode= "LD A (HL)"; break;
        case 0x7F:  load(reg.A, reg.A); cycles=4; currentOpCode= "LD A A"; break;
        case 0x80:  add(reg.A, reg.B); cycles=4; currentOpCode= "ADD B"; break;
        case 0x81:  add(reg.A, reg.C); cycles=4; currentOpCode= "ADD C"; break;
        case 0x82:  add(reg.A, reg.D); cycles=4; currentOpCode= "ADD D"; break;
        case 0x83:  add(reg.A, reg.E); cycles=4; currentOpCode= "ADD E"; break;
        case 0x84:  add(reg.A, reg.H); cycles=4; currentOpCode= "ADD H"; break;
        case 0x85:  add(reg.A, reg.L); cycles=4; currentOpCode= "ADD L"; break;
        case 0x86:  add(reg.A, pairRegData(reg.H, reg.L)); cycles=8; currentOpCode= "ADD (HL)"; break;
        case 0x87:  add(reg.A, reg.A); cycles=4; currentOpCode= "ADD A"; break;
        case 0x88:  adc(reg.A, reg.B); cycles=4; currentOpCode= "ADC B"; break;
        case 0x89:  adc(reg.A, reg.C); cycles=4; currentOpCode= "ADC C"; break;
        case 0x8A:  adc(reg.A, reg.D); cycles=4; currentOpCode= "ADC D"; break;
        case 0x8B:  adc(reg.A, reg.E); cycles=4; currentOpCode= "ADC E"; break;
        case 0x8C:  adc(reg.A, reg.H); cycles=4; currentOpCode= "ADC H"; break;
        case 0x8D:  adc(reg.A, reg.L); cycles=4; currentOpCode= "ADC L"; break;
        case 0x8E:  adc(reg.A, pairRegData(reg.H, reg.L)); cycles=8; currentOpCode= "ADC (HL)"; break;
        case 0x8F:  adc(reg.A, reg.A); cycles=4; currentOpCode= "ADC A"; break;
        case 0x90:  sub(reg.A, reg.B); cycles=4; currentOpCode= "SUB B"; break;
        case 0x91:  sub(reg.A, reg.C); cycles=4; currentOpCode= "SUB C"; break;
        case 0x92:  sub(reg.A, reg.D); cycles=4; currentOpCode= "SUB D"; break;
        case 0x93:  sub(reg.A, reg.E); cycles=4; currentOpCode= "SUB E"; break;
        case 0x94:  sub(reg.A, reg.H); cycles=4; currentOpCode= "SUB H"; break;
        case 0x95:  sub(reg.A, reg.L); cycles=4; currentOpCode= "SUB L"; break;
        case 0x96:  sub(reg.A, pairRegData(reg.H, reg.L)); cycles=8; currentOpCode= "SUB (HL)"; break;
        case 0x97:  sub(reg.A, reg.A); cycles=4; currentOpCode= "SUB A"; break;
        case 0x98:  sbc(reg.A, reg.B); cycles=4; currentOpCode= "SBC B"; break;
        case 0x99:  sbc(reg.A, reg.C); cycles=4; currentOpCode= "SBC C"; break;
        case 0x9A:  sbc(reg.A, reg.D); cycles=4; currentOpCode= "SBC D"; break;
        case 0x9B:  sbc(reg.A, reg.E); cycles=4; currentOpCode= "SBC E"; break;
        case 0x9C:  sbc(reg.A, reg.H); cycles=4; currentOpCode= "SBC H"; break;
        case 0x9D:  sbc(reg.A, reg.L); cycles=4; currentOpCode= "SBC L"; break;
        case 0x9E:  sbc(reg.A, pairRegData(reg.H, reg.L)); cycles=8; currentOpCode= "SBC (HL)"; break;
        case 0x9F:  sbc(reg.A, reg.A); cycles=4; currentOpCode= "SBC A"; break;
        case 0xA0:  logAnd(reg.A, reg.B); cycles=4; currentOpCode= "AND B"; break;
        case 0xA1:  logAnd(reg.A, reg.C); cycles=4; currentOpCode= "AND C"; break;
        case 0xA2:  logAnd(reg.A, reg.D); cycles=4; currentOpCode= "AND D"; break;
        case 0xA3:  logAnd(reg.A, reg.E); cycles=4; currentOpCode= "AND E"; break;
        case 0xA4:  logAnd(reg.A, reg.H); cycles=4; currentOpCode= "AND H"; break;
        case 0xA5:  logAnd(reg.A, reg.L); cycles=4; currentOpCode= "AND L"; break;
        case 0xA6:  logAnd(reg.A, pairRegData(reg.H, reg.L)); cycles=8; currentOpCode= "AND (HL)"; break;
        case 0xA7:  logAnd(reg.A, reg.A); cycles=4; currentOpCode= "AND A"; break;
        case 0xA8:  logXor(reg.A, reg.B); cycles=4; currentOpCode= "XOR B"; break;
        case 0xA9:  logXor(reg.A, reg.C); cycles=4; currentOpCode= "XOR C"; break;
        case 0xAA:  logXor(reg.A, reg.D); cycles=4; currentOpCode= "XOR D"; break;
        case 0xAB:  logXor(reg.A, reg.E); cycles=4; currentOpCode= "XOR E"; break;
        case 0xAC:  logXor(reg.A, reg.H); cycles=4; currentOpCode= "XOR H"; break;
        case 0xAD:  logXor(reg.A, reg.L); cycles=4; currentOpCode= "XOR L"; break;
        case 0xAE:  logXor(reg.A, pairRegData(reg.H, reg.L)); cycles=8;  currentOpCode= "XOR (HL)"; break;
        case 0xAF:  logXor(reg.A, reg.A); cycles=4; currentOpCode= "XOR A"; break;
        case 0xB0:  logOr(reg.A, reg.B); cycles=4; currentOpCode= "OR B"; break;
        case 0xB1:  logOr(reg.A, reg.C); cycles=4; currentOpCode= "OR C"; break;
        case 0xB2:  logOr(reg.A, reg.D); cycles=4; currentOpCode= "OR D"; break;
        case 0xB3:  logOr(reg.A, reg.E); cycles=4; currentOpCode= "OR E"; break;
        case 0xB4:  logOr(reg.A, reg.H); cycles=4; currentOpCode= "OR H"; break;
        case 0xB5:  logOr(reg.A, reg.L); cycles=4; currentOpCode= "OR L"; break;
        case 0xB6:  logOr(reg.A, pairRegData(reg.H, reg.L)); cycles=8; currentOpCode= "OR (HL)"; break;
        case 0xB7:  logOr(reg.A, reg.A); cycles=4; currentOpCode= "OR A"; break;
        case 0xB8:  cp(reg.A, reg.B); cycles=4; currentOpCode= "CP B"; break;
        case 0xB9:  cp(reg.A, reg.C); cycles=4; currentOpCode= "CP C"; break;
        case 0xBA:  cp(reg.A, reg.D); cycles=4; currentOpCode= "CP D"; break;
        case 0xBB:  cp(reg.A, reg.E); cycles=4; currentOpCode= "CP E"; break;
        case 0xBC:  cp(reg.A, reg.H); cycles=4; currentOpCode= "CP H"; break;
        case 0xBD:  cp(reg.A, reg.L); cycles=4; currentOpCode= "CP L"; break;
        case 0xBE:  cp(reg.A, pairRegData(reg.H, reg.L)); cycles=8; currentOpCode= "CP (HL)"; break;
        case 0xBF:  cp(reg.A, reg.A); cycles=4; currentOpCode= "CP A"; break;
        case 0xC0:{
            bool jump = ret( ~FLAGZERO); currentOpCode= "RET NZ";
            debugOpCode << " RET NZ \n"; if (jump) {isJump = true; cycles=20;} cycles=8; break;
        }
        case 0xC1:  pop(reg.B, reg.C); cycles=12; currentOpCode= "POP BC"; break;
        case 0xC2:{
            uint8_t val1 = mem.read(pc+1); uint8_t val2 = mem.read(pc+2);
            bool jump = jp( joinByte(val2,val1), ~FLAGZERO);
            currentOpCode= "JP NZ nn : " + formatHex(val2) + formatHex(val1);
            opbytes = 3; cycles=12; if (jump) {isJump = true; cycles=16;}
            break;
        }
        case 0xC3:{
            uint8_t val1 = mem.read(pc+1); uint8_t val2 = mem.read(pc+2);
            jp( joinByte(val2,val1) );
            currentOpCode= "JP nn : " + formatHex(val2) + formatHex(val1);
            opbytes = 3; cycles=16; isJump = true ;
            break;
        }
        case 0xC4:{
            uint8_t val1 = mem.read(pc+1); uint8_t val2 = mem.read(pc+2);
            bool jump = call(val2, val1, ~FLAGZERO);
            currentOpCode= "CALL NZ nn : " + formatHex(val2) + formatHex(val1);
            opbytes = 3; if (jump) {isJump = true; cycles=24;} cycles=12;
            break;
        }
        case 0xC5:  push(reg.B, reg.C); cycles=16; currentOpCode= "PUSH BC"; break;
        case 0xC6:{
            opbytes = 2; cycles=8;
            currentOpCode= "ADD n : " + formatHex(mem.read(pc+1));
            add(reg.A, mem.read(pc+1));break;
        }
        case 0xC7:  rst(0x00); cycles=16; currentOpCode= "RST 0x00"; break;
        case 0xC8:{
            bool jump = ret( FLAGZERO); currentOpCode= "RET Z";
            debugOpCode << " RET Z \n"; if (jump) {isJump = true; cycles=20;} cycles=8; break;
        }
        case 0xC9:  ret(); isJump = true; cycles=16; currentOpCode= "RET"; break;
        case 0xCA:{
            uint8_t val1 = mem.read(pc+1); uint8_t val2 = mem.read(pc+2);
            bool jump = jp( joinByte(val2,val1), FLAGZERO);
            currentOpCode= "JP Z nn : " + formatHex(val2) + formatHex(val1);
            opbytes = 3; cycles=12; if (jump) {isJump = true; cycles=16;}
            break;
        }
        case 0xCB:  decodePreByte(mem.read(pc+1)); opbytes = 2; break;
        case 0xCC:{
            uint8_t val1 = mem.read(pc+1); uint8_t val2 = mem.read(pc+2);
            bool jump = call(val2, val1, FLAGZERO);
            currentOpCode= "CALL Z nn : " + formatHex(val2) + formatHex(val1);
            opbytes = 3; if (jump) {isJump = true; cycles=24;} cycles=12;
            break;
        }
        case 0xCD:{
            uint8_t val1 = mem.read(pc+1); uint8_t val2 = mem.read(pc+2);
            call(val2, val1);
            currentOpCode= "CALL nn : " + formatHex(val2) + formatHex(val1);
            opbytes = 3; isJump = true; cycles=24;
            break;
        }
        case 0xCE:{
            opbytes = 2; cycles=8;
            currentOpCode= "ADC n : " + formatHex(mem.read(pc+1));
            adc(reg.A, mem.read(pc+1));break;
        }
        case 0xCF:  rst(0x08); cycles=16; currentOpCode= "RST 0x08"; break;
        case 0xD0:{
            bool jump = ret( ~FLAGCARRY); currentOpCode= "RET NC";
            debugOpCode << " RET NC \n"; if (jump) {isJump = true; cycles=20;} cycles=8; break;
        }
        case 0xD1:  pop(reg.D, reg.E); cycles=12; currentOpCode= "POP DE"; break;
        case 0xD2:{
            uint8_t val1 = mem.read(pc+1); uint8_t val2 = mem.read(pc+2);
            bool jump = jp( joinByte(val2,val1), ~FLAGCARRY);
            currentOpCode= "JP NC nn : " + formatHex(val2) + formatHex(val1);
            opbytes = 3; cycles=12; if (jump) {isJump = true; cycles=16;}
            break;
        }
        case 0xD4:{
            uint8_t val1 = mem.read(pc+1); uint8_t val2 = mem.read(pc+2);
            bool jump = call(val2, val1, ~FLAGCARRY);
            currentOpCode= "CALL NC nn : " + formatHex(val2) + formatHex(val1);
            opbytes = 3; if (jump) {isJump = true; cycles=24;} cycles=12;
            break;
        }
        case 0xD5:  push(reg.D, reg.E); cycles=16; currentOpCode= "PUSH DE"; break;
        case 0xD6:{
            opbytes = 2; cycles=8;
            currentOpCode= "SUB n : " + formatHex(mem.read(pc+1));
            sub(reg.A, mem.read(pc+1));break;
        }
        case 0xD7:  rst(0x10); cycles=16; currentOpCode= "RST 0x10"; break;
        case 0xD8:{
            bool jump = ret( FLAGCARRY); currentOpCode= "RET C";
            debugOpCode << " RET C \n"; if (jump) {isJump = true; cycles=20;} cycles=8; break;
        }
        case 0xD9:  reti(); cycles=16; currentOpCode= "RETI"; break;
        case 0xDA:{
            uint8_t val1 = mem.read(pc+1); uint8_t val2 = mem.read(pc+2);
            bool jump = jp( joinByte(val2,val1), FLAGCARRY);
            currentOpCode= "JP C nn : " + formatHex(val2) + formatHex(val1);
            opbytes = 3; cycles=12; if (jump) {isJump = true; cycles=16;}
            break;
        }
        case 0xDC:{
            uint8_t val1 = mem.read(pc+1); uint8_t val2 = mem.read(pc+2);
            bool jump = call(val2, val1, FLAGCARRY);
            currentOpCode= "CALL C nn : " + formatHex(val2) + formatHex(val1);
            opbytes = 3; if (jump) {isJump = true; cycles=24;} cycles=12;
            break;
        }
        case 0xDE:{
            opbytes = 2; cycles=8;
            currentOpCode= "SBC n : " + formatHex(mem.read(pc+1));
            sbc(reg.A, mem.read(pc+1));break;
        }
        case 0xDF:  rst(0x18); cycles=16; currentOpCode= "RST 0x18"; break;
        case 0xE0:{
            opbytes = 2; cycles=12;
            currentOpCode= "LDH (n) A : " + formatHex(mem.read(pc+1));
             ldh_E0(mem.read(pc+1));
            break;
        }
        case 0xE1:  pop(reg.H, reg.L); cycles=12; currentOpCode= "POP HL"; break;
        case 0xE2:  load( joinByte(0x00, reg.C), reg.A ); cycles=8; currentOpCode= "LD (C) A"; break;
        case 0xE5:  push(reg.H, reg.L); cycles=16; currentOpCode= "PUSH HL"; break;
        case 0xE6:{
            opbytes = 2; cycles=8;
            currentOpCode= "AND n : " + formatHex(mem.read(pc+1));
            logAnd(reg.A, mem.read(pc+1));break;
        }
        case 0xE7:  rst(0x20); cycles=16; currentOpCode= "RST 0x20"; break;
        case 0xE8:{
            int8_t val1 = int8_t(mem.read(pc+1));
            opbytes = 2; cycles=16;
            currentOpCode= "ADD SP e : " + formatHex(mem.read(pc+1));
             addByte2SP(val1);		break;
        }
        case 0xE9:  jp( pairReg(reg.H, reg.L) ); cycles=4; currentOpCode= "JP HL"; break;
        case 0xEA:{
            uint8_t val1 = mem.read(pc+1); uint8_t val2 = mem.read(pc+2);
            load( joinByte(val2, val1), reg.A); currentOpCode= "LD (nn) A : " + formatHex(val2) + formatHex(val1);
            opbytes = 3; cycles=8;
            break;
        }
        case 0xEE:{
            opbytes = 2; cycles=8;
            currentOpCode= "XOR n : " + formatHex(mem.read(pc+1));
            logXor(reg.A, mem.read(pc+1));break;
        }
        case 0xEF:  rst(0x28); cycles=16; currentOpCode= "RST 0x28"; break;
        case 0xF0:{
            opbytes = 2; cycles=12;
            currentOpCode= "LDH A (n) : " + formatHex(mem.read(pc+1));
             ldh_F0(mem.read(pc+1));
            break;
        }
        case 0xF1:  pop(reg.A, reg.F); cycles=12; currentOpCode= "POP AF"; break;
        case 0xF2:  load( reg.A, mem.read(joinByte(0x00, reg.C)) ); cycles=8; currentOpCode= "LD A (C)"; break;
        case 0xF3:  di(); cycles=4; currentOpCode= "DI"; break;
        case 0xF5:  push(reg.A, reg.F); cycles=16; currentOpCode= "PUSH AF"; break;
        case 0xF6:{
            opbytes = 2; cycles=8;
            currentOpCode= "OR n : " + formatHex(mem.read(pc+1));
            logOr(reg.A, mem.read(pc+1));break;
        }
        case 0xF7:  rst(0x30); cycles=16; currentOpCode= "RST 0x30"; break;
        case 0xF8:{
            opbytes = 2; cycles=12;
            currentOpCode= "LD HL SP+e : " + formatHex(mem.read(pc+1));
            loadSP2Reg( reg.H, reg.L); addByte2SP(int8_t(mem.read(pc+1)));
            break;
        }
        case 0xF9:  loadSP( reg.H, reg.L); cycles=8; currentOpCode= "LD SP HL"; break;
        case 0xFA:{
            uint8_t val1 = mem.read(pc+1); uint8_t val2 = mem.read(pc+2);
            load(reg.A, joinByte(val2,val1) ); currentOpCode= "LD A (nn) : " + formatHex(val2) + formatHex(val1);
            opbytes = 3; cycles=8;
            break;
        }
        case 0xFB:  ei(); cycles=4; currentOpCode= "EI"; break;
        case 0xFE:{
            opbytes = 2; cycles=8;
            currentOpCode= "CP n : " + formatHex(mem.read(pc+1));
            cp(reg.A, mem.read(pc+1));break;
        }
        case 0xFF:  rst(0x38); cycles=16; currentOpCode= "RST 0x38"; break;
        default: debugOpCode << "***Instruction not recognized*** " << HEXPRINT(code) << "\n"; break;
    }

    if (printOpCode){
        debugOpCode << currentOpCode;
    }

    if (!isJump){
        pc = pc + opbytes;
    }
}


void CPU::decodePreByte(uint8_t bitCode){

    QDebug debugOpCode = qDebug().noquote();

    //parse cb prefixed opcode
    switch (bitCode) {
        case 0x00:  rlc(reg.B); cycles=8; currentOpCode= "RLC B"; break;
        case 0x01:  rlc(reg.C); cycles=8; currentOpCode= "RLC C"; break;
        case 0x02:  rlc(reg.D); cycles=8; currentOpCode= "RLC D"; break;
        case 0x03:  rlc(reg.E); cycles=8; currentOpCode= "RLC E"; break;
        case 0x04:  rlc(reg.H); cycles=8; currentOpCode= "RLC H"; break;
        case 0x05:  rlc(reg.L); cycles=8; currentOpCode= "RLC L"; break;
        case 0x06:  rlc(reg.A, true); cycles=16; currentOpCode= "RLC (HL)"; break;
        case 0x07:  rlc(reg.A); cycles=8; currentOpCode= "RLC A"; break;
        case 0x08:  rrc(reg.B); cycles=8; currentOpCode= "RRC B"; break;
        case 0x09:  rrc(reg.C); cycles=8; currentOpCode= "RRC C"; break;
        case 0x0A:  rrc(reg.D); cycles=8; currentOpCode= "RRC D"; break;
        case 0x0B:  rrc(reg.E); cycles=8; currentOpCode= "RRC E"; break;
        case 0x0C:  rrc(reg.H); cycles=8; currentOpCode= "RRC H"; break;
        case 0x0D:  rrc(reg.L); cycles=8; currentOpCode= "RRC L"; break;
        case 0x0E:  rrc(reg.A, true); cycles=16; currentOpCode= "RRC (HL)"; break;
        case 0x0F:  rrc(reg.A); cycles=8; currentOpCode= "RRC A"; break;
        case 0x10:  rl(reg.B); cycles=8; currentOpCode= "RL B"; break;
        case 0x11:  rl(reg.C); cycles=8; currentOpCode= "RL C"; break;
        case 0x12:  rl(reg.D); cycles=8; currentOpCode= "RL D"; break;
        case 0x13:  rl(reg.E); cycles=8; currentOpCode= "RL E"; break;
        case 0x14:  rl(reg.H); cycles=8; currentOpCode= "RL H"; break;
        case 0x15:  rl(reg.L); cycles=8; currentOpCode= "RL L"; break;
        case 0x16:  rl(reg.A, true); cycles=16; currentOpCode= "RL (HL)"; break;
        case 0x17:  rl(reg.A); cycles=8; currentOpCode= "RL A"; break;
        case 0x18:  rr(reg.B); cycles=8; currentOpCode= "RR B"; break;
        case 0x19:  rr(reg.C); cycles=8; currentOpCode= "RR C"; break;
        case 0x1A:  rr(reg.D); cycles=8; currentOpCode= "RR D"; break;
        case 0x1B:  rr(reg.E); cycles=8; currentOpCode= "RR E"; break;
        case 0x1C:  rr(reg.H); cycles=8; currentOpCode= "RR H"; break;
        case 0x1D:  rr(reg.L); cycles=8; currentOpCode= "RR L"; break;
        case 0x1E:  rr(reg.A, true); cycles=16; currentOpCode= "RR (HL)"; break;
        case 0x1F:  rr(reg.A); cycles=8; currentOpCode= "RR A"; break;
        case 0x20:  sla(reg.B); cycles=8; currentOpCode= "SLA B"; break;
        case 0x21:  sla(reg.C); cycles=8; currentOpCode= "SLA C"; break;
        case 0x22:  sla(reg.D); cycles=8; currentOpCode= "SLA D"; break;
        case 0x23:  sla(reg.E); cycles=8; currentOpCode= "SLA E"; break;
        case 0x24:  sla(reg.H); cycles=8; currentOpCode= "SLA H"; break;
        case 0x25:  sla(reg.L); cycles=8; currentOpCode= "SLA L"; break;
        case 0x26:  sla(reg.A, true); cycles=16; currentOpCode= "SLA (HL)"; break;
        case 0x27:  sla(reg.A); cycles=8; currentOpCode= "SLA A"; break;
        case 0x28:  sra(reg.B); cycles=8; currentOpCode= "SRA B"; break;
        case 0x29:  sra(reg.C); cycles=8; currentOpCode= "SRA C"; break;
        case 0x2A:  sra(reg.D); cycles=8; currentOpCode= "SRA D"; break;
        case 0x2B:  sra(reg.E); cycles=8; currentOpCode= "SRA E"; break;
        case 0x2C:  sra(reg.H); cycles=8; currentOpCode= "SRA H"; break;
        case 0x2D:  sra(reg.L); cycles=8; currentOpCode= "SRA L"; break;
        case 0x2E:  sra(reg.A, true); cycles=16; currentOpCode= "SRA (HL)"; break;
        case 0x2F:  sra(reg.A); cycles=8; currentOpCode= "SRA A"; break;
        case 0x30:  swap(reg.B); cycles=8; currentOpCode= "SWAP B"; break;
        case 0x31:  swap(reg.C); cycles=8; currentOpCode= "SWAP C"; break;
        case 0x32:  swap(reg.D); cycles=8; currentOpCode= "SWAP D"; break;
        case 0x33:  swap(reg.E); cycles=8; currentOpCode= "SWAP E"; break;
        case 0x34:  swap(reg.H); cycles=8; currentOpCode= "SWAP H"; break;
        case 0x35:  swap(reg.L); cycles=8; currentOpCode= "SWAP L"; break;
        case 0x36:  swap(reg.A, true); cycles=16; currentOpCode= "SWAP (HL)"; break;
        case 0x37:  swap(reg.A); cycles=8; currentOpCode= "SWAP A"; break;
        case 0x38:  srl(reg.B); cycles=8; currentOpCode= "SRL B"; break;
        case 0x39:  srl(reg.C); cycles=8; currentOpCode= "SRL C"; break;
        case 0x3A:  srl(reg.D); cycles=8; currentOpCode= "SRL D"; break;
        case 0x3B:  srl(reg.E); cycles=8; currentOpCode= "SRL E"; break;
        case 0x3C:  srl(reg.H); cycles=8; currentOpCode= "SRL H"; break;
        case 0x3D:  srl(reg.L); cycles=8; currentOpCode= "SRL L"; break;
        case 0x3E:  srl(reg.A, true); cycles=16; currentOpCode= "SRL (HL)"; break;
        case 0x3F:  srl(reg.A); cycles=8; currentOpCode= "SRL A"; break;
        case 0x40:  bit(BITZERO, reg.B); cycles=8; currentOpCode= "BIT 0 B"; break;
        case 0x41:  bit(BITZERO, reg.C); cycles=8; currentOpCode= "BIT 0 C"; break;
        case 0x42:  bit(BITZERO, reg.D); cycles=8; currentOpCode= "BIT 0 D"; break;
        case 0x43:  bit(BITZERO, reg.E); cycles=8; currentOpCode= "BIT 0 E"; break;
        case 0x44:  bit(BITZERO, reg.H); cycles=8; currentOpCode= "BIT 0 H"; break;
        case 0x45:  bit(BITZERO, reg.L); cycles=8; currentOpCode= "BIT 0 L"; break;
        case 0x46:  bit(BITZERO, reg.A, true); cycles=12; currentOpCode= "BIT 0 (HL)"; break;
        case 0x47:  bit(BITZERO, reg.A); cycles=8; currentOpCode= "BIT 0 A"; break;
        case 0x48:  bit(BITONE, reg.B); cycles=8; currentOpCode= "BIT 1 B"; break;
        case 0x49:  bit(BITONE, reg.C); cycles=8; currentOpCode= "BIT 1 C"; break;
        case 0x4A:  bit(BITONE, reg.D); cycles=8; currentOpCode= "BIT 1 D"; break;
        case 0x4B:  bit(BITONE, reg.E); cycles=8; currentOpCode= "BIT 1 E"; break;
        case 0x4C:  bit(BITONE, reg.H); cycles=8; currentOpCode= "BIT 1 H"; break;
        case 0x4D:  bit(BITONE, reg.L); cycles=8; currentOpCode= "BIT 1 L"; break;
        case 0x4E:  bit(BITONE, reg.A, true); cycles=12; currentOpCode= "BIT 1 (HL)"; break;
        case 0x4F:  bit(BITONE, reg.A); cycles=8; currentOpCode= "BIT 1 A"; break;
        case 0x50:  bit(BITTWO, reg.B); cycles=8; currentOpCode= "BIT 2 B"; break;
        case 0x51:  bit(BITTWO, reg.C); cycles=8; currentOpCode= "BIT 2 C"; break;
        case 0x52:  bit(BITTWO, reg.D); cycles=8; currentOpCode= "BIT 2 D"; break;
        case 0x53:  bit(BITTWO, reg.E); cycles=8; currentOpCode= "BIT 2 E"; break;
        case 0x54:  bit(BITTWO, reg.H); cycles=8; currentOpCode= "BIT 2 H"; break;
        case 0x55:  bit(BITTWO, reg.L); cycles=8; currentOpCode= "BIT 2 L"; break;
        case 0x56:  bit(BITTWO, reg.A, true); cycles=12; currentOpCode= "BIT 2 (HL)"; break;
        case 0x57:  bit(BITTWO, reg.A); cycles=8; currentOpCode= "BIT 2 A"; break;
        case 0x58:  bit(BITTHREE, reg.B); cycles=8; currentOpCode= "BIT 3 B"; break;
        case 0x59:  bit(BITTHREE, reg.C); cycles=8; currentOpCode= "BIT 3 C"; break;
        case 0x5A:  bit(BITTHREE, reg.D); cycles=8; currentOpCode= "BIT 3 D"; break;
        case 0x5B:  bit(BITTHREE, reg.E); cycles=8; currentOpCode= "BIT 3 E"; break;
        case 0x5C:  bit(BITTHREE, reg.H); cycles=8; currentOpCode= "BIT 3 H"; break;
        case 0x5D:  bit(BITTHREE, reg.L); cycles=8; currentOpCode= "BIT 3 L"; break;
        case 0x5E:  bit(BITTHREE, reg.A, true); cycles=12; currentOpCode= "BIT 3 (HL)"; break;
        case 0x5F:  bit(BITTHREE, reg.A); cycles=8; currentOpCode= "BIT 3 A"; break;
        case 0x60:  bit(BITFOUR, reg.B); cycles=8; currentOpCode= "BIT 4 B"; break;
        case 0x61:  bit(BITFOUR, reg.C); cycles=8; currentOpCode= "BIT 4 C"; break;
        case 0x62:  bit(BITFOUR, reg.D); cycles=8; currentOpCode= "BIT 4 D"; break;
        case 0x63:  bit(BITFOUR, reg.E); cycles=8; currentOpCode= "BIT 4 E"; break;
        case 0x64:  bit(BITFOUR, reg.H); cycles=8; currentOpCode= "BIT 4 H"; break;
        case 0x65:  bit(BITFOUR, reg.L); cycles=8; currentOpCode= "BIT 4 L"; break;
        case 0x66:  bit(BITFOUR, reg.A, true); cycles=12; currentOpCode= "BIT 4 (HL)"; break;
        case 0x67:  bit(BITFOUR, reg.A); cycles=8; currentOpCode= "BIT 4 A"; break;
        case 0x68:  bit(BITFIVE, reg.B); cycles=8; currentOpCode= "BIT 5 B"; break;
        case 0x69:  bit(BITFIVE, reg.C); cycles=8; currentOpCode= "BIT 5 C"; break;
        case 0x6A:  bit(BITFIVE, reg.D); cycles=8; currentOpCode= "BIT 5 D"; break;
        case 0x6B:  bit(BITFIVE, reg.E); cycles=8; currentOpCode= "BIT 5 E"; break;
        case 0x6C:  bit(BITFIVE, reg.H); cycles=8; currentOpCode= "BIT 5 H"; break;
        case 0x6D:  bit(BITFIVE, reg.L); cycles=8; currentOpCode= "BIT 5 L"; break;
        case 0x6E:  bit(BITFIVE, reg.A, true); cycles=12; currentOpCode= "BIT 5 (HL)"; break;
        case 0x6F:  bit(BITFIVE, reg.A); cycles=8; currentOpCode= "BIT 5 A"; break;
        case 0x70:  bit(BITSIX, reg.B); cycles=8; currentOpCode= "BIT 6 B"; break;
        case 0x71:  bit(BITSIX, reg.C); cycles=8; currentOpCode= "BIT 6 C"; break;
        case 0x72:  bit(BITSIX, reg.D); cycles=8; currentOpCode= "BIT 6 D"; break;
        case 0x73:  bit(BITSIX, reg.E); cycles=8; currentOpCode= "BIT 6 E"; break;
        case 0x74:  bit(BITSIX, reg.H); cycles=8; currentOpCode= "BIT 6 H"; break;
        case 0x75:  bit(BITSIX, reg.L); cycles=8; currentOpCode= "BIT 6 L"; break;
        case 0x76:  bit(BITSIX, reg.A, true); cycles=12; currentOpCode= "BIT 6 (HL)"; break;
        case 0x77:  bit(BITSIX, reg.A); cycles=8; currentOpCode= "BIT 6 A"; break;
        case 0x78:  bit(BITSEVEN, reg.B); cycles=8; currentOpCode= "BIT 7 B"; break;
        case 0x79:  bit(BITSEVEN, reg.C); cycles=8; currentOpCode= "BIT 7 C"; break;
        case 0x7A:  bit(BITSEVEN, reg.D); cycles=8; currentOpCode= "BIT 7 D"; break;
        case 0x7B:  bit(BITSEVEN, reg.E); cycles=8; currentOpCode= "BIT 7 E"; break;
        case 0x7C:  bit(BITSEVEN, reg.H); cycles=8; currentOpCode= "BIT 7 H"; break;
        case 0x7D:  bit(BITSEVEN, reg.L); cycles=8; currentOpCode= "BIT 7 L"; break;
        case 0x7E:  bit(BITSEVEN, reg.A, true); cycles=12; currentOpCode= "BIT 7 (HL)"; break;
        case 0x7F:  bit(BITSEVEN, reg.A); cycles=8; currentOpCode= "BIT 7 A"; break;
        case 0x80:  res(BITZERO, reg.B); cycles=8; currentOpCode= "RES 0 B"; break;
        case 0x81:  res(BITZERO, reg.C); cycles=8; currentOpCode= "RES 0 C"; break;
        case 0x82:  res(BITZERO, reg.D); cycles=8; currentOpCode= "RES 0 D"; break;
        case 0x83:  res(BITZERO, reg.E); cycles=8; currentOpCode= "RES 0 E"; break;
        case 0x84:  res(BITZERO, reg.H); cycles=8; currentOpCode= "RES 0 H"; break;
        case 0x85:  res(BITZERO, reg.L); cycles=8; currentOpCode= "RES 0 L"; break;
        case 0x86:  res(BITZERO, reg.A, true); cycles=16; currentOpCode= "RES 0 (HL)"; break;
        case 0x87:  res(BITZERO, reg.A); cycles=8; currentOpCode= "RES 0 A"; break;
        case 0x88:  res(BITONE, reg.B); cycles=8; currentOpCode= "RES 1 B"; break;
        case 0x89:  res(BITONE, reg.C); cycles=8; currentOpCode= "RES 1 C"; break;
        case 0x8A:  res(BITONE, reg.D); cycles=8; currentOpCode= "RES 1 D"; break;
        case 0x8B:  res(BITONE, reg.E); cycles=8; currentOpCode= "RES 1 E"; break;
        case 0x8C:  res(BITONE, reg.H); cycles=8; currentOpCode= "RES 1 H"; break;
        case 0x8D:  res(BITONE, reg.L); cycles=8; currentOpCode= "RES 1 L"; break;
        case 0x8E:  res(BITONE, reg.A, true); cycles=16; currentOpCode= "RES 1 (HL)"; break;
        case 0x8F:  res(BITONE, reg.A); cycles=8; currentOpCode= "RES 1 A"; break;
        case 0x90:  res(BITTWO, reg.B); cycles=8; currentOpCode= "RES 2 B"; break;
        case 0x91:  res(BITTWO, reg.C); cycles=8; currentOpCode= "RES 2 C"; break;
        case 0x92:  res(BITTWO, reg.D); cycles=8; currentOpCode= "RES 2 D"; break;
        case 0x93:  res(BITTWO, reg.E); cycles=8; currentOpCode= "RES 2 E"; break;
        case 0x94:  res(BITTWO, reg.H); cycles=8; currentOpCode= "RES 2 H"; break;
        case 0x95:  res(BITTWO, reg.L); cycles=8; currentOpCode= "RES 2 L"; break;
        case 0x96:  res(BITTWO, reg.A, true); cycles=16; currentOpCode= "RES 2 (HL)"; break;
        case 0x97:  res(BITTWO, reg.A); cycles=8; currentOpCode= "RES 2 A"; break;
        case 0x98:  res(BITTHREE, reg.B); cycles=8; currentOpCode= "RES 3 B"; break;
        case 0x99:  res(BITTHREE, reg.C); cycles=8; currentOpCode= "RES 3 C"; break;
        case 0x9A:  res(BITTHREE, reg.D); cycles=8; currentOpCode= "RES 3 D"; break;
        case 0x9B:  res(BITTHREE, reg.E); cycles=8; currentOpCode= "RES 3 E"; break;
        case 0x9C:  res(BITTHREE, reg.H); cycles=8; currentOpCode= "RES 3 H"; break;
        case 0x9D:  res(BITTHREE, reg.L); cycles=8; currentOpCode= "RES 3 L"; break;
        case 0x9E:  res(BITTHREE, reg.A, true); cycles=16; currentOpCode= "RES 3 (HL)"; break;
        case 0x9F:  res(BITTHREE, reg.A); cycles=8; currentOpCode= "RES 3 A"; break;
        case 0xA0:  res(BITFOUR, reg.B); cycles=8; currentOpCode= "RES 4 B"; break;
        case 0xA1:  res(BITFOUR, reg.C); cycles=8; currentOpCode= "RES 4 C"; break;
        case 0xA2:  res(BITFOUR, reg.D); cycles=8; currentOpCode= "RES 4 D"; break;
        case 0xA3:  res(BITFOUR, reg.E); cycles=8; currentOpCode= "RES 4 E"; break;
        case 0xA4:  res(BITFOUR, reg.H); cycles=8; currentOpCode= "RES 4 H"; break;
        case 0xA5:  res(BITFOUR, reg.L); cycles=8; currentOpCode= "RES 4 L"; break;
        case 0xA6:  res(BITFOUR, reg.A, true); cycles=16; currentOpCode= "RES 4 (HL)"; break;
        case 0xA7:  res(BITFOUR, reg.A); cycles=8; currentOpCode= "RES 4 A"; break;
        case 0xA8:  res(BITFIVE, reg.B); cycles=8; currentOpCode= "RES 5 B"; break;
        case 0xA9:  res(BITFIVE, reg.C); cycles=8; currentOpCode= "RES 5 C"; break;
        case 0xAA:  res(BITFIVE, reg.D); cycles=8; currentOpCode= "RES 5 D"; break;
        case 0xAB:  res(BITFIVE, reg.E); cycles=8; currentOpCode= "RES 5 E"; break;
        case 0xAC:  res(BITFIVE, reg.H); cycles=8; currentOpCode= "RES 5 H"; break;
        case 0xAD:  res(BITFIVE, reg.L); cycles=8; currentOpCode= "RES 5 L"; break;
        case 0xAE:  res(BITFIVE, reg.A, true); cycles=16; currentOpCode= "RES 5 (HL)"; break;
        case 0xAF:  res(BITFIVE, reg.A); cycles=8; currentOpCode= "RES 5 A"; break;
        case 0xB0:  res(BITSIX, reg.B); cycles=8; currentOpCode= "RES 6 B"; break;
        case 0xB1:  res(BITSIX, reg.C); cycles=8; currentOpCode= "RES 6 C"; break;
        case 0xB2:  res(BITSIX, reg.D); cycles=8; currentOpCode= "RES 6 D"; break;
        case 0xB3:  res(BITSIX, reg.E); cycles=8; currentOpCode= "RES 6 E"; break;
        case 0xB4:  res(BITSIX, reg.H); cycles=8; currentOpCode= "RES 6 H"; break;
        case 0xB5:  res(BITSIX, reg.L); cycles=8; currentOpCode= "RES 6 L"; break;
        case 0xB6:  res(BITSIX, reg.A, true); cycles=16; currentOpCode= "RES 6 (HL)"; break;
        case 0xB7:  res(BITSIX, reg.A); cycles=8; currentOpCode= "RES 6 A"; break;
        case 0xB8:  res(BITSEVEN, reg.B); cycles=8; currentOpCode= "RES 7 B"; break;
        case 0xB9:  res(BITSEVEN, reg.C); cycles=8; currentOpCode= "RES 7 C"; break;
        case 0xBA:  res(BITSEVEN, reg.D); cycles=8; currentOpCode= "RES 7 D"; break;
        case 0xBB:  res(BITSEVEN, reg.E); cycles=8; currentOpCode= "RES 7 E"; break;
        case 0xBC:  res(BITSEVEN, reg.H); cycles=8; currentOpCode= "RES 7 H"; break;
        case 0xBD:  res(BITSEVEN, reg.L); cycles=8; currentOpCode= "RES 7 L"; break;
        case 0xBE:  res(BITSEVEN, reg.A, true); cycles=16; currentOpCode= "RES 7 (HL)"; break;
        case 0xBF:  res(BITSEVEN, reg.A); cycles=8; currentOpCode= "RES 7 A"; break;
        case 0xC0:  set(BITZERO, reg.B); cycles=8; currentOpCode= "SET 0 B"; break;
        case 0xC1:  set(BITZERO, reg.C); cycles=8; currentOpCode= "SET 0 C"; break;
        case 0xC2:  set(BITZERO, reg.D); cycles=8; currentOpCode= "SET 0 D"; break;
        case 0xC3:  set(BITZERO, reg.E); cycles=8; currentOpCode= "SET 0 E"; break;
        case 0xC4:  set(BITZERO, reg.H); cycles=8; currentOpCode= "SET 0 H"; break;
        case 0xC5:  set(BITZERO, reg.L); cycles=8; currentOpCode= "SET 0 L"; break;
        case 0xC6:  set(BITZERO, reg.A, true); cycles=16; currentOpCode= "SET 0 (HL)"; break;
        case 0xC7:  set(BITZERO, reg.A); cycles=8; currentOpCode= "SET 0 A"; break;
        case 0xC8:  set(BITONE, reg.B); cycles=8; currentOpCode= "SET 1 B"; break;
        case 0xC9:  set(BITONE, reg.C); cycles=8; currentOpCode= "SET 1 C"; break;
        case 0xCA:  set(BITONE, reg.D); cycles=8; currentOpCode= "SET 1 D"; break;
        case 0xCB:  set(BITONE, reg.E); cycles=8; currentOpCode= "SET 1 E"; break;
        case 0xCC:  set(BITONE, reg.H); cycles=8; currentOpCode= "SET 1 H"; break;
        case 0xCD:  set(BITONE, reg.L); cycles=8; currentOpCode= "SET 1 L"; break;
        case 0xCE:  set(BITONE, reg.A, true); cycles=16; currentOpCode= "SET 1 (HL)"; break;
        case 0xCF:  set(BITONE, reg.A); cycles=8; currentOpCode= "SET 1 A"; break;
        case 0xD0:  set(BITTWO, reg.B); cycles=8; currentOpCode= "SET 2 B"; break;
        case 0xD1:  set(BITTWO, reg.C); cycles=8; currentOpCode= "SET 2 C"; break;
        case 0xD2:  set(BITTWO, reg.D); cycles=8; currentOpCode= "SET 2 D"; break;
        case 0xD3:  set(BITTWO, reg.E); cycles=8; currentOpCode= "SET 2 E"; break;
        case 0xD4:  set(BITTWO, reg.H); cycles=8; currentOpCode= "SET 2 H"; break;
        case 0xD5:  set(BITTWO, reg.L); cycles=8; currentOpCode= "SET 2 L"; break;
        case 0xD6:  set(BITTWO, reg.A, true); cycles=16; currentOpCode= "SET 2 (HL)"; break;
        case 0xD7:  set(BITTWO, reg.A); cycles=8; currentOpCode= "SET 2 A"; break;
        case 0xD8:  set(BITTHREE, reg.B); cycles=8; currentOpCode= "SET 3 B"; break;
        case 0xD9:  set(BITTHREE, reg.C); cycles=8; currentOpCode= "SET 3 C"; break;
        case 0xDA:  set(BITTHREE, reg.D); cycles=8; currentOpCode= "SET 3 D"; break;
        case 0xDB:  set(BITTHREE, reg.E); cycles=8; currentOpCode= "SET 3 E"; break;
        case 0xDC:  set(BITTHREE, reg.H); cycles=8; currentOpCode= "SET 3 H"; break;
        case 0xDD:  set(BITTHREE, reg.L); cycles=8; currentOpCode= "SET 3 L"; break;
        case 0xDE:  set(BITTHREE, reg.A, true); cycles=16; currentOpCode= "SET 3 (HL)"; break;
        case 0xDF:  set(BITTHREE, reg.A); cycles=8; currentOpCode= "SET 3 A"; break;
        case 0xE0:  set(BITFOUR, reg.B); cycles=8; currentOpCode= "SET 4 B"; break;
        case 0xE1:  set(BITFOUR, reg.C); cycles=8; currentOpCode= "SET 4 C"; break;
        case 0xE2:  set(BITFOUR, reg.D); cycles=8; currentOpCode= "SET 4 D"; break;
        case 0xE3:  set(BITFOUR, reg.E); cycles=8; currentOpCode= "SET 4 E"; break;
        case 0xE4:  set(BITFOUR, reg.H); cycles=8; currentOpCode= "SET 4 H"; break;
        case 0xE5:  set(BITFOUR, reg.L); cycles=8; currentOpCode= "SET 4 L"; break;
        case 0xE6:  set(BITFOUR, reg.A, true); cycles=16; currentOpCode= "SET 4 (HL)"; break;
        case 0xE7:  set(BITFOUR, reg.A); cycles=8; currentOpCode= "SET 4 A"; break;
        case 0xE8:  set(BITFIVE, reg.B); cycles=8; currentOpCode= "SET 5 B"; break;
        case 0xE9:  set(BITFIVE, reg.C); cycles=8; currentOpCode= "SET 5 C"; break;
        case 0xEA:  set(BITFIVE, reg.D); cycles=8; currentOpCode= "SET 5 D"; break;
        case 0xEB:  set(BITFIVE, reg.E); cycles=8; currentOpCode= "SET 5 E"; break;
        case 0xEC:  set(BITFIVE, reg.H); cycles=8; currentOpCode= "SET 5 H"; break;
        case 0xED:  set(BITFIVE, reg.L); cycles=8; currentOpCode= "SET 5 L"; break;
        case 0xEE:  set(BITFIVE, reg.A, true); cycles=16; currentOpCode= "SET 5 (HL)"; break;
        case 0xEF:  set(BITFIVE, reg.A); cycles=8; currentOpCode= "SET 5 A"; break;
        case 0xF0:  set(BITSIX, reg.B); cycles=8; currentOpCode= "SET 6 B"; break;
        case 0xF1:  set(BITSIX, reg.C); cycles=8; currentOpCode= "SET 6 C"; break;
        case 0xF2:  set(BITSIX, reg.D); cycles=8; currentOpCode= "SET 6 D"; break;
        case 0xF3:  set(BITSIX, reg.E); cycles=8; currentOpCode= "SET 6 E"; break;
        case 0xF4:  set(BITSIX, reg.H); cycles=8; currentOpCode= "SET 6 H"; break;
        case 0xF5:  set(BITSIX, reg.L); cycles=8; currentOpCode= "SET 6 L"; break;
        case 0xF6:  set(BITSIX, reg.A, true); cycles=16; currentOpCode= "SET 6 (HL)"; break;
        case 0xF7:  set(BITSIX, reg.A); cycles=8; currentOpCode= "SET 6 A"; break;
        case 0xF8:  set(BITSEVEN, reg.B); cycles=8; currentOpCode= "SET 7 B"; break;
        case 0xF9:  set(BITSEVEN, reg.C); cycles=8; currentOpCode= "SET 7 C"; break;
        case 0xFA:  set(BITSEVEN, reg.D); cycles=8; currentOpCode= "SET 7 D"; break;
        case 0xFB:  set(BITSEVEN, reg.E); cycles=8; currentOpCode= "SET 7 E"; break;
        case 0xFC:  set(BITSEVEN, reg.H); cycles=8; currentOpCode= "SET 7 H"; break;
        case 0xFD:  set(BITSEVEN, reg.L); cycles=8; currentOpCode= "SET 7 L"; break;
        case 0xFE:  set(BITSEVEN, reg.A, true); cycles=16; currentOpCode= "SET 7 (HL)"; break;
        case 0xFF:  set(BITSEVEN, reg.A); cycles=8; currentOpCode= "SET 7 A"; break;
        default: debugOpCode << "***Instruction not recognized*** " << HEXPRINT(bitCode) << "\n"; break;
    }

    if (printOpCode){
        debugOpCode << currentOpCode;
    }
}

