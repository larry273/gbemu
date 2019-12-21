#include "cpu.h"

//translate byte code to instructions
int cpu::decodeByte(struct gbData &gbdata, uint16_t &pc){

    int opbytes; //bytes for instr op

    //measure timing
    clock_t t;
    t = clock();
    int instru = 0;

    //read all opcodes
    while(pc < 1000){
        unsigned char *code = &gbdata.buffer[pc];
        opbytes = 1; //default 1 byte opcode

        std::cout << HEXCOUNT(pc);

        //opcode decoding
        switch (*code) {
            case 0x00: std::cout << " NOP \n";  break;
            case 0x01:
                std::cout << " LD BC nn | NOT IMPLEMENTED  " << HEX(gbdata.buffer[pc+1]) << ' ' << HEX(gbdata.buffer[pc+2]) << "\n"; opbytes = 3; break;
            case 0x02: std::cout << " LD (BC) A \n";  load(pairReg(reg.B, reg.C), reg.A); break;
            case 0x03: std::cout << " INC BC | NOT IMPLEMENTED \n";  break;
            case 0x04: std::cout << " INC B \n";  inc(reg.B); break;
            case 0x05: std::cout << " DEC B \n";  dec(reg.B); break;
            case 0x06:{
                std::cout << " LD B n " << HEX(gbdata.buffer[pc+1]) << "\n"; opbytes = 2;
                load(reg.B, gbdata.buffer[pc+1]);break;
            }
            case 0x07: std::cout << " RLCA | NOT IMPLEMENTED \n";  break;
            case 0x08:{
                uint8_t val1 = gbdata.buffer[pc+1]; uint8_t val2 = gbdata.buffer[pc+2];
                load((val1 >> 8) + val2, reg.SP);
                std::cout << " LD (nn) SP " << HEX(val1) << ' ' << HEX(val2) << "\n"; opbytes = 3; break;
            }
            case 0x09: std::cout << " ADD HL BC | NOT IMPLEMENTED \n";  break;
            case 0x0A: std::cout << " LD A (BC) \n";  load(reg.A, pairReg(reg.B, reg.C)); break;
            case 0x0B: std::cout << " DEC BC | NOT IMPLEMENTED \n";  break;
            case 0x0C: std::cout << " INC C \n";  inc(reg.C); break;
            case 0x0D: std::cout << " DEC C \n";  dec(reg.C); break;
            case 0x0E:{
                std::cout << " LD C n " << HEX(gbdata.buffer[pc+1]) << "\n"; opbytes = 2;
                load(reg.C, gbdata.buffer[pc+1]);break;
            }
            case 0x0F: std::cout << " RRCA | NOT IMPLEMENTED \n";  break;
            case 0x10: std::cout << " STOP | NOT IMPLEMENTED \n";  break;
            case 0x11:
                std::cout << " LD DE nn | NOT IMPLEMENTED  " << HEX(gbdata.buffer[pc+1]) << ' ' << HEX(gbdata.buffer[pc+2]) << "\n"; opbytes = 3; break;
            case 0x12: std::cout << " LD (DE) A \n";  load(pairReg(reg.D, reg.E), reg.A); break;
            case 0x13: std::cout << " INC DE | NOT IMPLEMENTED \n";  break;
            case 0x14: std::cout << " INC D \n";  inc(reg.D); break;
            case 0x15: std::cout << " DEC D \n";  dec(reg.D); break;
            case 0x16:{
                std::cout << " LD D n " << HEX(gbdata.buffer[pc+1]) << "\n"; opbytes = 2;
                load(reg.D, gbdata.buffer[pc+1]);break;
            }
            case 0x17: std::cout << " RLA | NOT IMPLEMENTED \n";  break;
            case 0x18: std::cout << " JR e | NOT IMPLEMENTED  " << HEX(gbdata.buffer[pc+1]) << "\n"; opbytes = 2; break;
            case 0x19: std::cout << " ADD HL DE | NOT IMPLEMENTED \n";  break;
            case 0x1A: std::cout << " LD A (DE) \n";  load(reg.A, pairReg(reg.D, reg.E)); break;
            case 0x1B: std::cout << " DEC DE | NOT IMPLEMENTED \n";  break;
            case 0x1C: std::cout << " INC E \n";  inc(reg.E); break;
            case 0x1D: std::cout << " DEC E \n";  dec(reg.E); break;
            case 0x1E:{
                std::cout << " LD E n " << HEX(gbdata.buffer[pc+1]) << "\n"; opbytes = 2;
                load(reg.E, gbdata.buffer[pc+1]);break;
            }
            case 0x1F: std::cout << " RRA | NOT IMPLEMENTED \n";  break;
            case 0x20: std::cout << " JR NZ e | NOT IMPLEMENTED  " << HEX(gbdata.buffer[pc+1]) << "\n"; opbytes = 2; break;
            case 0x21:
                std::cout << " LD HL nn | NOT IMPLEMENTED  " << HEX(gbdata.buffer[pc+1]) << ' ' << HEX(gbdata.buffer[pc+2]) << "\n"; opbytes = 3; break;
            case 0x22: std::cout << " LD (HL+) A \n";  break;
            case 0x23: std::cout << " INC HL | NOT IMPLEMENTED \n";  break;
            case 0x24: std::cout << " INC H \n";  inc(reg.H); break;
            case 0x25: std::cout << " DEC H \n";  dec(reg.H); break;
            case 0x26:{
                std::cout << " LD H n " << HEX(gbdata.buffer[pc+1]) << "\n"; opbytes = 2;
                load(reg.H, gbdata.buffer[pc+1]);break;
            }
            case 0x27: std::cout << " DAA | NOT IMPLEMENTED \n";  break;
            case 0x28: std::cout << " JR Z e | NOT IMPLEMENTED  " << HEX(gbdata.buffer[pc+1]) << "\n"; opbytes = 2; break;
            case 0x29: std::cout << " ADD HL HL | NOT IMPLEMENTED \n";  break;
            case 0x2A: std::cout << " LD A (HL+) \n";  break;
            case 0x2B: std::cout << " DEC HL | NOT IMPLEMENTED \n";  break;
            case 0x2C: std::cout << " INC L \n";  inc(reg.L); break;
            case 0x2D: std::cout << " DEC L \n";  dec(reg.L); break;
            case 0x2E:{
                std::cout << " LD L n " << HEX(gbdata.buffer[pc+1]) << "\n"; opbytes = 2;
                load(reg.L, gbdata.buffer[pc+1]);break;
            }
            case 0x2F: std::cout << " CPL | NOT IMPLEMENTED \n";  break;
            case 0x30: std::cout << " JR NC e | NOT IMPLEMENTED  " << HEX(gbdata.buffer[pc+1]) << "\n"; opbytes = 2; break;
            case 0x31:
                std::cout << " LD SP nn | NOT IMPLEMENTED  " << HEX(gbdata.buffer[pc+1]) << ' ' << HEX(gbdata.buffer[pc+2]) << "\n"; opbytes = 3; break;
            case 0x32: std::cout << " LD (HL-) A \n";  break;
            case 0x33: std::cout << " INC SP \n";  incSP(); break;
            case 0x34: std::cout << " INC (HL) | NOT IMPLEMENTED \n";  break;
            case 0x35: std::cout << " DEC (HL) | NOT IMPLEMENTED \n";  break;
            case 0x36: std::cout << " LD (HL) n | NOT IMPLEMENTED  " << HEX(gbdata.buffer[pc+1]) << "\n"; opbytes = 2; break;
            case 0x37: std::cout << " SCF | NOT IMPLEMENTED \n";  break;
            case 0x38: std::cout << " JR C e | NOT IMPLEMENTED  " << HEX(gbdata.buffer[pc+1]) << "\n"; opbytes = 2; break;
            case 0x39: std::cout << " ADD HL SP | NOT IMPLEMENTED \n";  break;
            case 0x3A: std::cout << " LD A (HL-) \n";  break;
            case 0x3B: std::cout << " DEC SP \n";  decSP(); break;
            case 0x3C: std::cout << " INC A \n";  inc(reg.A); break;
            case 0x3D: std::cout << " DEC A \n";  dec(reg.A); break;
            case 0x3E:{
                std::cout << " LD A n " << HEX(gbdata.buffer[pc+1]) << "\n"; opbytes = 2;
                load(reg.A, gbdata.buffer[pc+1]);break;
            }
            case 0x3F: std::cout << " CCF | NOT IMPLEMENTED \n";  break;
            case 0x40: std::cout << " LD B B \n";  load(reg.B, reg.B); break;
            case 0x41: std::cout << " LD B C \n";  load(reg.B, reg.C); break;
            case 0x42: std::cout << " LD B D \n";  load(reg.B, reg.D); break;
            case 0x43: std::cout << " LD B E \n";  load(reg.B, reg.E); break;
            case 0x44: std::cout << " LD B H \n";  load(reg.B, reg.H); break;
            case 0x45: std::cout << " LD B L \n";  load(reg.B, reg.L); break;
            case 0x46: std::cout << " LD B (HL) \n";  load(reg.B, pairReg(reg.H, reg.L)); break;
            case 0x47: std::cout << " LD B A \n";  load(reg.B, reg.A); break;
            case 0x48: std::cout << " LD C B \n";  load(reg.C, reg.B); break;
            case 0x49: std::cout << " LD C C \n";  load(reg.C, reg.C); break;
            case 0x4A: std::cout << " LD C D \n";  load(reg.C, reg.D); break;
            case 0x4B: std::cout << " LD C E \n";  load(reg.C, reg.E); break;
            case 0x4C: std::cout << " LD C H \n";  load(reg.C, reg.H); break;
            case 0x4D: std::cout << " LD C L \n";  load(reg.C, reg.L); break;
            case 0x4E: std::cout << " LD C (HL) \n";  load(reg.C, pairReg(reg.H, reg.L)); break;
            case 0x4F: std::cout << " LD C A \n";  load(reg.C, reg.A); break;
            case 0x50: std::cout << " LD D B \n";  load(reg.D, reg.B); break;
            case 0x51: std::cout << " LD D C \n";  load(reg.D, reg.C); break;
            case 0x52: std::cout << " LD D D \n";  load(reg.D, reg.D); break;
            case 0x53: std::cout << " LD D E \n";  load(reg.D, reg.E); break;
            case 0x54: std::cout << " LD D H \n";  load(reg.D, reg.H); break;
            case 0x55: std::cout << " LD D L \n";  load(reg.D, reg.L); break;
            case 0x56: std::cout << " LD D (HL) \n";  load(reg.D, pairReg(reg.H, reg.L)); break;
            case 0x57: std::cout << " LD D A \n";  load(reg.D, reg.A); break;
            case 0x58: std::cout << " LD E B \n";  load(reg.E, reg.B); break;
            case 0x59: std::cout << " LD E C \n";  load(reg.E, reg.C); break;
            case 0x5A: std::cout << " LD E D \n";  load(reg.E, reg.D); break;
            case 0x5B: std::cout << " LD E E \n";  load(reg.E, reg.E); break;
            case 0x5C: std::cout << " LD E H \n";  load(reg.E, reg.H); break;
            case 0x5D: std::cout << " LD E L \n";  load(reg.E, reg.L); break;
            case 0x5E: std::cout << " LD E (HL) \n";  load(reg.E, pairReg(reg.H, reg.L)); break;
            case 0x5F: std::cout << " LD E A \n";  load(reg.E, reg.A); break;
            case 0x60: std::cout << " LD H B \n";  load(reg.H, reg.B); break;
            case 0x61: std::cout << " LD H C \n";  load(reg.H, reg.C); break;
            case 0x62: std::cout << " LD H D \n";  load(reg.H, reg.D); break;
            case 0x63: std::cout << " LD H E \n";  load(reg.H, reg.E); break;
            case 0x64: std::cout << " LD H H \n";  load(reg.H, reg.H); break;
            case 0x65: std::cout << " LD H L \n";  load(reg.H, reg.L); break;
            case 0x66: std::cout << " LD H (HL) \n";  load(reg.H, pairReg(reg.H, reg.L)); break;
            case 0x67: std::cout << " LD H A \n";  load(reg.H, reg.A); break;
            case 0x68: std::cout << " LD L B \n";  load(reg.L, reg.B); break;
            case 0x69: std::cout << " LD L C \n";  load(reg.L, reg.C); break;
            case 0x6A: std::cout << " LD L D \n";  load(reg.L, reg.D); break;
            case 0x6B: std::cout << " LD L E \n";  load(reg.L, reg.E); break;
            case 0x6C: std::cout << " LD L H \n";  load(reg.L, reg.H); break;
            case 0x6D: std::cout << " LD L L \n";  load(reg.L, reg.L); break;
            case 0x6E: std::cout << " LD L (HL) \n";  load(reg.L, pairReg(reg.H, reg.L)); break;
            case 0x6F: std::cout << " LD L A \n";  load(reg.L, reg.A); break;
            case 0x70: std::cout << " LD (HL) B \n";  load(pairReg(reg.H, reg.L), reg.B); break;
            case 0x71: std::cout << " LD (HL) C \n";  load(pairReg(reg.H, reg.L), reg.C); break;
            case 0x72: std::cout << " LD (HL) D \n";  load(pairReg(reg.H, reg.L), reg.D); break;
            case 0x73: std::cout << " LD (HL) E \n";  load(pairReg(reg.H, reg.L), reg.E); break;
            case 0x74: std::cout << " LD (HL) H \n";  load(pairReg(reg.H, reg.L), reg.H); break;
            case 0x75: std::cout << " LD (HL) L \n";  load(pairReg(reg.H, reg.L), reg.L); break;
            case 0x76: std::cout << " HALT \n";  break;
            case 0x77: std::cout << " LD (HL) A \n";  load(pairReg(reg.H, reg.L), reg.A); break;
            case 0x78: std::cout << " LD A B \n";  load(reg.A, reg.B); break;
            case 0x79: std::cout << " LD A C \n";  load(reg.A, reg.C); break;
            case 0x7A: std::cout << " LD A D \n";  load(reg.A, reg.D); break;
            case 0x7B: std::cout << " LD A E \n";  load(reg.A, reg.E); break;
            case 0x7C: std::cout << " LD A H \n";  load(reg.A, reg.H); break;
            case 0x7D: std::cout << " LD A L \n";  load(reg.A, reg.L); break;
            case 0x7E: std::cout << " LD A (HL) \n";  load(reg.A, pairReg(reg.H, reg.L)); break;
            case 0x7F: std::cout << " LD A A \n";  load(reg.A, reg.A); break;
            case 0x80: std::cout << " ADD B \n";  add(reg.A, reg.B); break;
            case 0x81: std::cout << " ADD C \n";  add(reg.A, reg.C); break;
            case 0x82: std::cout << " ADD D \n";  add(reg.A, reg.D); break;
            case 0x83: std::cout << " ADD E \n";  add(reg.A, reg.E); break;
            case 0x84: std::cout << " ADD H \n";  add(reg.A, reg.H); break;
            case 0x85: std::cout << " ADD L \n";  add(reg.A, reg.L); break;
            case 0x86: std::cout << " ADD (HL) \n";  add(reg.A, pairData(reg.H, reg.L)); break;
            case 0x87: std::cout << " ADD A \n";  add(reg.A, reg.A); break;
            case 0x88: std::cout << " ADC B \n";  adc(reg.A, reg.B); break;
            case 0x89: std::cout << " ADC C \n";  adc(reg.A, reg.C); break;
            case 0x8A: std::cout << " ADC D \n";  adc(reg.A, reg.D); break;
            case 0x8B: std::cout << " ADC E \n";  adc(reg.A, reg.E); break;
            case 0x8C: std::cout << " ADC H \n";  adc(reg.A, reg.H); break;
            case 0x8D: std::cout << " ADC L \n";  adc(reg.A, reg.L); break;
            case 0x8E: std::cout << " ADC (HL) \n";  adc(reg.A, pairData(reg.H, reg.L)); break;
            case 0x8F: std::cout << " ADC A \n";  adc(reg.A, reg.A); break;
            case 0x90: std::cout << " SUB B \n";  sub(reg.A, reg.B); break;
            case 0x91: std::cout << " SUB C \n";  sub(reg.A, reg.C); break;
            case 0x92: std::cout << " SUB D \n";  sub(reg.A, reg.D); break;
            case 0x93: std::cout << " SUB E \n";  sub(reg.A, reg.E); break;
            case 0x94: std::cout << " SUB H \n";  sub(reg.A, reg.H); break;
            case 0x95: std::cout << " SUB L \n";  sub(reg.A, reg.L); break;
            case 0x96: std::cout << " SUB (HL) \n";  sub(reg.A, pairData(reg.H, reg.L)); break;
            case 0x97: std::cout << " SUB A \n";  sub(reg.A, reg.A); break;
            case 0x98: std::cout << " SBC B \n";  sbc(reg.A, reg.B); break;
            case 0x99: std::cout << " SBC C \n";  sbc(reg.A, reg.C); break;
            case 0x9A: std::cout << " SBC D \n";  sbc(reg.A, reg.D); break;
            case 0x9B: std::cout << " SBC E \n";  sbc(reg.A, reg.E); break;
            case 0x9C: std::cout << " SBC H \n";  sbc(reg.A, reg.H); break;
            case 0x9D: std::cout << " SBC L \n";  sbc(reg.A, reg.L); break;
            case 0x9E: std::cout << " SBC (HL) \n";  sbc(reg.A, pairData(reg.H, reg.L)); break;
            case 0x9F: std::cout << " SBC A \n";  sbc(reg.A, reg.A); break;
            case 0xA0: std::cout << " AND B \n";  logAnd(reg.A, reg.B); break;
            case 0xA1: std::cout << " AND C \n";  logAnd(reg.A, reg.C); break;
            case 0xA2: std::cout << " AND D \n";  logAnd(reg.A, reg.D); break;
            case 0xA3: std::cout << " AND E \n";  logAnd(reg.A, reg.E); break;
            case 0xA4: std::cout << " AND H \n";  logAnd(reg.A, reg.H); break;
            case 0xA5: std::cout << " AND L \n";  logAnd(reg.A, reg.L); break;
            case 0xA6: std::cout << " AND (HL) \n";  logAnd(reg.A, pairData(reg.H, reg.L)); break;
            case 0xA7: std::cout << " AND A \n";  logAnd(reg.A, reg.A); break;
            case 0xA8: std::cout << " XOR B \n";  logXor(reg.A, reg.B); break;
            case 0xA9: std::cout << " XOR C \n";  logXor(reg.A, reg.C); break;
            case 0xAA: std::cout << " XOR D \n";  logXor(reg.A, reg.D); break;
            case 0xAB: std::cout << " XOR E \n";  logXor(reg.A, reg.E); break;
            case 0xAC: std::cout << " XOR H \n";  logXor(reg.A, reg.H); break;
            case 0xAD: std::cout << " XOR L \n";  logXor(reg.A, reg.L); break;
            case 0xAE: std::cout << " XOR (HL) \n";  logXor(reg.A, pairData(reg.H, reg.L)); break;
            case 0xAF: std::cout << " XOR A \n";  logXor(reg.A, reg.A); break;
            case 0xB0: std::cout << " OR B \n";  logOr(reg.A, reg.B); break;
            case 0xB1: std::cout << " OR C \n";  logOr(reg.A, reg.C); break;
            case 0xB2: std::cout << " OR D \n";  logOr(reg.A, reg.D); break;
            case 0xB3: std::cout << " OR E \n";  logOr(reg.A, reg.E); break;
            case 0xB4: std::cout << " OR H \n";  logOr(reg.A, reg.H); break;
            case 0xB5: std::cout << " OR L \n";  logOr(reg.A, reg.L); break;
            case 0xB6: std::cout << " OR (HL) \n";  logOr(reg.A, pairData(reg.H, reg.L)); break;
            case 0xB7: std::cout << " OR A \n";  logOr(reg.A, reg.A); break;
            case 0xB8: std::cout << " CP B \n";  cp(reg.A, reg.B); break;
            case 0xB9: std::cout << " CP C \n";  cp(reg.A, reg.C); break;
            case 0xBA: std::cout << " CP D \n";  cp(reg.A, reg.D); break;
            case 0xBB: std::cout << " CP E \n";  cp(reg.A, reg.E); break;
            case 0xBC: std::cout << " CP H \n";  cp(reg.A, reg.H); break;
            case 0xBD: std::cout << " CP L \n";  cp(reg.A, reg.L); break;
            case 0xBE: std::cout << " CP (HL) \n";  cp(reg.A, pairData(reg.H, reg.L)); break;
            case 0xBF: std::cout << " CP A \n";  cp(reg.A, reg.A); break;
            case 0xC0: std::cout << " RET NZ | NOT IMPLEMENTED \n";  break;
            case 0xC1: std::cout << " POP BC \n";  pop(reg.B, reg.C); break;
            case 0xC2:
                std::cout << " JP NZ nn | NOT IMPLEMENTED  " << HEX(gbdata.buffer[pc+1]) << ' ' << HEX(gbdata.buffer[pc+2]) << "\n"; opbytes = 3; break;
            case 0xC3:
                std::cout << " JP nn | NOT IMPLEMENTED  " << HEX(gbdata.buffer[pc+1]) << ' ' << HEX(gbdata.buffer[pc+2]) << "\n"; opbytes = 3; break;
            case 0xC4:
                std::cout << " CALL NZ nn | NOT IMPLEMENTED  " << HEX(gbdata.buffer[pc+1]) << ' ' << HEX(gbdata.buffer[pc+2]) << "\n"; opbytes = 3; break;
            case 0xC5: std::cout << " PUSH BC \n";  push(reg.B, reg.C); break;
            case 0xC6:{
                std::cout << " ADD n " << HEX(gbdata.buffer[pc+1]) << "\n"; opbytes = 2;
                add(reg.A, gbdata.buffer[pc+1]);break;
            }
            case 0xC7: std::cout << " RST 0x00 | NOT IMPLEMENTED \n";  break;
            case 0xC8: std::cout << " RET Z | NOT IMPLEMENTED \n";  break;
            case 0xC9: std::cout << " RET | NOT IMPLEMENTED \n";  break;
            case 0xCA:
                std::cout << " JP Z nn | NOT IMPLEMENTED  " << HEX(gbdata.buffer[pc+1]) << ' ' << HEX(gbdata.buffer[pc+2]) << "\n"; opbytes = 3; break;
            case 0xCB: std::cout << " CB op | NOT IMPLEMENTED \n";  break;
            case 0xCC:
                std::cout << " CALL Z nn | NOT IMPLEMENTED  " << HEX(gbdata.buffer[pc+1]) << ' ' << HEX(gbdata.buffer[pc+2]) << "\n"; opbytes = 3; break;
            case 0xCD:
                std::cout << " CALL nn | NOT IMPLEMENTED  " << HEX(gbdata.buffer[pc+1]) << ' ' << HEX(gbdata.buffer[pc+2]) << "\n"; opbytes = 3; break;
            case 0xCE:{
                std::cout << " ADC n " << HEX(gbdata.buffer[pc+1]) << "\n"; opbytes = 2;
                adc(reg.A, gbdata.buffer[pc+1]);break;
            }
            case 0xCF: std::cout << " RST 0x08 | NOT IMPLEMENTED \n";  break;
            case 0xD0: std::cout << " RET NC | NOT IMPLEMENTED \n";  break;
            case 0xD1: std::cout << " POP DE \n";  pop(reg.D, reg.E); break;
            case 0xD2:
                std::cout << " JP NC nn | NOT IMPLEMENTED  " << HEX(gbdata.buffer[pc+1]) << ' ' << HEX(gbdata.buffer[pc+2]) << "\n"; opbytes = 3; break;
            case 0xD4:
                std::cout << " CALL NC nn | NOT IMPLEMENTED  " << HEX(gbdata.buffer[pc+1]) << ' ' << HEX(gbdata.buffer[pc+2]) << "\n"; opbytes = 3; break;
            case 0xD5: std::cout << " PUSH DE \n";  push(reg.D, reg.E); break;
            case 0xD6:{
                std::cout << " SUB n " << HEX(gbdata.buffer[pc+1]) << "\n"; opbytes = 2;
                sub(reg.A, gbdata.buffer[pc+1]);break;
            }
            case 0xD7: std::cout << " RST 0x10 | NOT IMPLEMENTED \n";  break;
            case 0xD8: std::cout << " RET C | NOT IMPLEMENTED \n";  break;
            case 0xD9: std::cout << " RETI | NOT IMPLEMENTED \n";  break;
            case 0xDA:
                std::cout << " JP C nn | NOT IMPLEMENTED  " << HEX(gbdata.buffer[pc+1]) << ' ' << HEX(gbdata.buffer[pc+2]) << "\n"; opbytes = 3; break;
            case 0xDC:
                std::cout << " CALL C nn | NOT IMPLEMENTED  " << HEX(gbdata.buffer[pc+1]) << ' ' << HEX(gbdata.buffer[pc+2]) << "\n"; opbytes = 3; break;
            case 0xDE:{
                std::cout << " SBC n " << HEX(gbdata.buffer[pc+1]) << "\n"; opbytes = 2;
                sbc(reg.A, gbdata.buffer[pc+1]);break;
            }
            case 0xDF: std::cout << " RST 0x18 | NOT IMPLEMENTED \n";  break;
            case 0xE0: std::cout << " LDH (n) A | NOT IMPLEMENTED  " << HEX(gbdata.buffer[pc+1]) << "\n"; opbytes = 2; break;
            case 0xE1: std::cout << " POP HL \n";  pop(reg.H, reg.L); break;
            case 0xE2: std::cout << " LD (C) A \n";  break;
            case 0xE5: std::cout << " PUSH HL \n";  push(reg.H, reg.L); break;
            case 0xE6:{
                std::cout << " AND n " << HEX(gbdata.buffer[pc+1]) << "\n"; opbytes = 2;
                logAnd(reg.A, gbdata.buffer[pc+1]);break;
            }
            case 0xE7: std::cout << " RST 0x20 | NOT IMPLEMENTED \n";  break;
            case 0xE8: std::cout << " ADD SP e | NOT IMPLEMENTED  " << HEX(gbdata.buffer[pc+1]) << "\n"; opbytes = 2; break;
            case 0xE9: std::cout << " JP HL | NOT IMPLEMENTED \n";  break;
            case 0xEA:{
                uint8_t val1 = gbdata.buffer[pc+1]; uint8_t val2 = gbdata.buffer[pc+2];
                load((val1 >> 8) + val2, reg.A);
                std::cout << " LD (nn) A " << HEX(val1) << ' ' << HEX(val2) << "\n"; opbytes = 3; break;
            }
            case 0xEE:{
                std::cout << " XOR n " << HEX(gbdata.buffer[pc+1]) << "\n"; opbytes = 2;
                logXor(reg.A, gbdata.buffer[pc+1]);break;
            }
            case 0xEF: std::cout << " RST 0x28 | NOT IMPLEMENTED \n";  break;
            case 0xF0: std::cout << " LDH A (n) | NOT IMPLEMENTED  " << HEX(gbdata.buffer[pc+1]) << "\n"; opbytes = 2; break;
            case 0xF1: std::cout << " POP AF \n";  pop(reg.A, reg.F); break;
            case 0xF2: std::cout << " LD A (C) \n";  break;
            case 0xF3: std::cout << " DI | NOT IMPLEMENTED \n";  break;
            case 0xF5: std::cout << " PUSH AF \n";  push(reg.A, reg.F); break;
            case 0xF6:{
                std::cout << " OR n " << HEX(gbdata.buffer[pc+1]) << "\n"; opbytes = 2;
                logOr(reg.A, gbdata.buffer[pc+1]);break;
            }
            case 0xF7: std::cout << " RST 0x30 | NOT IMPLEMENTED \n";  break;
            case 0xF8: std::cout << " LD HL SP+e | NOT IMPLEMENTED  " << HEX(gbdata.buffer[pc+1]) << "\n"; opbytes = 2; break;
            case 0xF9: std::cout << " LD SP HL \n";  break;
            case 0xFA:{
                uint8_t val1 = gbdata.buffer[pc+1]; uint8_t val2 = gbdata.buffer[pc+2];
                load(reg.A, (val1 >> 8) + val2);
                std::cout << " LD A (nn) " << HEX(val1) << ' ' << HEX(val2) << "\n"; opbytes = 3; break;
            }
            case 0xFB: std::cout << " EI | NOT IMPLEMENTED \n";  break;
            case 0xFE:{
                std::cout << " CP n " << HEX(gbdata.buffer[pc+1]) << "\n"; opbytes = 2;
                cp(reg.A, gbdata.buffer[pc+1]);break;
            }
            case 0xFF: std::cout << " RST 0x38 | NOT IMPLEMENTED \n";  break;
            default: std::cout << "***Instruction not recognized*** " << HEX(*code) << "\n"; break;
        }
        pc = pc + opbytes;
        instru++;
    }
    t = clock() - t;
    std::cout << "instructions: " << std::dec << instru << "\n";
    std::cout << "time elapsed: " << std::dec << t*1.0/CLOCKS_PER_SEC  << " seconds. done decoding\n";

    return opbytes;
}
