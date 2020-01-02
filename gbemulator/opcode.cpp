#include "cpu.h"

//translate byte code to instructions
void CPU::decodeByte(struct gbData &gbdata, uint16_t &pc){

    int opbytes; //bytes for instr op

    uint8_t code = gbdata.buffer[pc];
    opbytes = 1; //default 1 byte opcode
    bool isJump = false;

    std::cout << HEXCOUNT(pc);

    //opcode decoding
    switch (code) {
        case 0x00: std::cout << " NOP \n";  break;
        case 0x01:{
            uint8_t val1 = gbdata.buffer[pc+1]; uint8_t val2 = gbdata.buffer[pc+2];
            load(reg.B, reg.C, val1, val2);
            std::cout << " LD BC nn " << HEX(val1) << ' ' << HEX(val2) << "\n"; opbytes = 3; break;
        }
        case 0x02: std::cout << " LD (BC) A \n";  load(pairReg(reg.B, reg.C), reg.A); break;
        case 0x03: std::cout << " INC BC \n";  inc(reg.B, reg.C); break;
        case 0x04: std::cout << " INC B \n";  inc(reg.B); break;
        case 0x05: std::cout << " DEC B \n";  dec(reg.B); break;
        case 0x06:{
            std::cout << " LD B n " << HEX(gbdata.buffer[pc+1]) << "\n"; opbytes = 2;
            load(reg.B, gbdata.buffer[pc+1]);break;
        }
        case 0x07: std::cout << " RLCA \n";  rlca(); break;
        case 0x08:{
            uint8_t val1 = gbdata.buffer[pc+1]; uint8_t val2 = gbdata.buffer[pc+2];
            load( joinByte(val1, val2), reg.SP);
            std::cout << " LD (nn) SP " << HEX(val1) << ' ' << HEX(val2) << "\n"; opbytes = 3; break;
        }
        case 0x09: std::cout << " ADD HL BC \n";  add(reg.H, reg.L, reg.B, reg.C); break;
        case 0x0A: std::cout << " LD A (BC) \n";  load(reg.A, pairReg(reg.B, reg.C)); break;
        case 0x0B: std::cout << " DEC BC \n";  dec(reg.B, reg.C); break;
        case 0x0C: std::cout << " INC C \n";  inc(reg.C); break;
        case 0x0D: std::cout << " DEC C \n";  dec(reg.C); break;
        case 0x0E:{
            std::cout << " LD C n " << HEX(gbdata.buffer[pc+1]) << "\n"; opbytes = 2;
            load(reg.C, gbdata.buffer[pc+1]);break;
        }
        case 0x0F: std::cout << " RRCA \n";  rrca(); break;
        case 0x10: std::cout << " STOP | NOT IMPLEMENTED \n";  break;
        case 0x11:{
            uint8_t val1 = gbdata.buffer[pc+1]; uint8_t val2 = gbdata.buffer[pc+2];
            load(reg.D, reg.E, val1, val2);
            std::cout << " LD DE nn " << HEX(val1) << ' ' << HEX(val2) << "\n"; opbytes = 3; break;
        }
        case 0x12: std::cout << " LD (DE) A \n";  load(pairReg(reg.D, reg.E), reg.A); break;
        case 0x13: std::cout << " INC DE \n";  inc(reg.D, reg.E); break;
        case 0x14: std::cout << " INC D \n";  inc(reg.D); break;
        case 0x15: std::cout << " DEC D \n";  dec(reg.D); break;
        case 0x16:{
            std::cout << " LD D n " << HEX(gbdata.buffer[pc+1]) << "\n"; opbytes = 2;
            load(reg.D, gbdata.buffer[pc+1]);break;
        }
        case 0x17: std::cout << " RLA \n";  rla(); break;
        case 0x18:{
            int8_t val1 = gbdata.buffer[pc+1];
            std::cout << " JR e " << HEX(val1) << "\n"; opbytes = 2;
            jr(val1);break;
        }
        case 0x19: std::cout << " ADD HL DE \n";  add(reg.H, reg.L, reg.D, reg.E); break;
        case 0x1A: std::cout << " LD A (DE) \n";  load(reg.A, pairReg(reg.D, reg.E)); break;
        case 0x1B: std::cout << " DEC DE \n";  dec(reg.D, reg.E); break;
        case 0x1C: std::cout << " INC E \n";  inc(reg.E); break;
        case 0x1D: std::cout << " DEC E \n";  dec(reg.E); break;
        case 0x1E:{
            std::cout << " LD E n " << HEX(gbdata.buffer[pc+1]) << "\n"; opbytes = 2;
            load(reg.E, gbdata.buffer[pc+1]);break;
        }
        case 0x1F: std::cout << " RRA \n";  rra(); break;
        case 0x20:{
            int8_t val1 = gbdata.buffer[pc+1];
            std::cout << " JR NZ e " << HEX(val1) << "\n"; opbytes = 2;
            jr(val1, ~FLAGZERO);break;
        }
        case 0x21:{
            uint8_t val1 = gbdata.buffer[pc+1]; uint8_t val2 = gbdata.buffer[pc+2];
            load(reg.H, reg.L, val1, val2);
            std::cout << " LD HL nn " << HEX(val1) << ' ' << HEX(val2) << "\n"; opbytes = 3; break;
        }
        case 0x22: std::cout << " LD (HL+) A \n";  load(pairReg(reg.H, reg.L), reg.A); inc(reg.A, true); break;
        case 0x23: std::cout << " INC HL \n";  inc(reg.H, reg.L); break;
        case 0x24: std::cout << " INC H \n";  inc(reg.H); break;
        case 0x25: std::cout << " DEC H \n";  dec(reg.H); break;
        case 0x26:{
            std::cout << " LD H n " << HEX(gbdata.buffer[pc+1]) << "\n"; opbytes = 2;
            load(reg.H, gbdata.buffer[pc+1]);break;
        }
        case 0x27: std::cout << " DAA \n";  daa(); break;
        case 0x28:{
            int8_t val1 = gbdata.buffer[pc+1];
            std::cout << " JR Z e " << HEX(val1) << "\n"; opbytes = 2;
            jr(val1, FLAGZERO);break;
        }
        case 0x29: std::cout << " ADD HL HL \n";  add(reg.H, reg.L, reg.H, reg.L); break;
        case 0x2A: std::cout << " LD A (HL+) \n";  load(reg.A, pairReg(reg.H, reg.L)); inc(reg.A, true); break;
        case 0x2B: std::cout << " DEC HL \n";  dec(reg.H, reg.L); break;
        case 0x2C: std::cout << " INC L \n";  inc(reg.L); break;
        case 0x2D: std::cout << " DEC L \n";  dec(reg.L); break;
        case 0x2E:{
            std::cout << " LD L n " << HEX(gbdata.buffer[pc+1]) << "\n"; opbytes = 2;
            load(reg.L, gbdata.buffer[pc+1]);break;
        }
        case 0x2F: std::cout << " CPL \n";  cpl(); break;
        case 0x30:{
            int8_t val1 = gbdata.buffer[pc+1];
            std::cout << " JR NC e " << HEX(val1) << "\n"; opbytes = 2;
            jr(val1, ~FLAGCARRY);break;
        }
        case 0x31:{
            uint8_t val1 = gbdata.buffer[pc+1]; uint8_t val2 = gbdata.buffer[pc+2];
            std::cout << " LD SP nn \n"; loadSP(val1, val2); opbytes = 3; break;
        }
        case 0x32: std::cout << " LD (HL-) A \n";  load(pairReg(reg.H, reg.L), reg.A); dec(reg.A, true); break;
        case 0x33: std::cout << " INC SP \n";  incSP(); break;
        case 0x34: std::cout << " INC (HL) \n";  inc(reg.A, true); break;
        case 0x35: std::cout << " DEC (HL) \n";  dec(reg.A, true); break;
        case 0x36:{
            std::cout << " LD (HL) n " << HEX(gbdata.buffer[pc+1]) << "\n"; opbytes = 2;
            load( pairReg(reg.H, reg.L ), gbdata.buffer[pc+1]);break;
        }
        case 0x37: std::cout << " SCF \n";  scf(); break;
        case 0x38:{
            int8_t val1 = gbdata.buffer[pc+1];
            std::cout << " JR C e " << HEX(val1) << "\n"; opbytes = 2;
            jr(val1, FLAGCARRY);break;
        }
        case 0x39: std::cout << " ADD HL SP \n"; addSP2Reg(reg.H, reg.L); break;
        case 0x3A: std::cout << " LD A (HL-) \n";  load(reg.A, pairReg(reg.H, reg.L) ); dec(reg.A, true); break;
        case 0x3B: std::cout << " DEC SP \n";  decSP(); break;
        case 0x3C: std::cout << " INC A \n";  inc(reg.A); break;
        case 0x3D: std::cout << " DEC A \n";  dec(reg.A); break;
        case 0x3E:{
            std::cout << " LD A n " << HEX(gbdata.buffer[pc+1]) << "\n"; opbytes = 2;
            load(reg.A, gbdata.buffer[pc+1]);break;
        }
        case 0x3F: std::cout << " CCF \n";  ccf(); break;
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
        case 0x86: std::cout << " ADD (HL) \n";  add(reg.A, pairRegData(reg.H, reg.L)); break;
        case 0x87: std::cout << " ADD A \n";  add(reg.A, reg.A); break;
        case 0x88: std::cout << " ADC B \n";  adc(reg.A, reg.B); break;
        case 0x89: std::cout << " ADC C \n";  adc(reg.A, reg.C); break;
        case 0x8A: std::cout << " ADC D \n";  adc(reg.A, reg.D); break;
        case 0x8B: std::cout << " ADC E \n";  adc(reg.A, reg.E); break;
        case 0x8C: std::cout << " ADC H \n";  adc(reg.A, reg.H); break;
        case 0x8D: std::cout << " ADC L \n";  adc(reg.A, reg.L); break;
        case 0x8E: std::cout << " ADC (HL) \n";  adc(reg.A, pairRegData(reg.H, reg.L)); break;
        case 0x8F: std::cout << " ADC A \n";  adc(reg.A, reg.A); break;
        case 0x90: std::cout << " SUB B \n";  sub(reg.A, reg.B); break;
        case 0x91: std::cout << " SUB C \n";  sub(reg.A, reg.C); break;
        case 0x92: std::cout << " SUB D \n";  sub(reg.A, reg.D); break;
        case 0x93: std::cout << " SUB E \n";  sub(reg.A, reg.E); break;
        case 0x94: std::cout << " SUB H \n";  sub(reg.A, reg.H); break;
        case 0x95: std::cout << " SUB L \n";  sub(reg.A, reg.L); break;
        case 0x96: std::cout << " SUB (HL) \n";  sub(reg.A, pairRegData(reg.H, reg.L)); break;
        case 0x97: std::cout << " SUB A \n";  sub(reg.A, reg.A); break;
        case 0x98: std::cout << " SBC B \n";  sbc(reg.A, reg.B); break;
        case 0x99: std::cout << " SBC C \n";  sbc(reg.A, reg.C); break;
        case 0x9A: std::cout << " SBC D \n";  sbc(reg.A, reg.D); break;
        case 0x9B: std::cout << " SBC E \n";  sbc(reg.A, reg.E); break;
        case 0x9C: std::cout << " SBC H \n";  sbc(reg.A, reg.H); break;
        case 0x9D: std::cout << " SBC L \n";  sbc(reg.A, reg.L); break;
        case 0x9E: std::cout << " SBC (HL) \n";  sbc(reg.A, pairRegData(reg.H, reg.L)); break;
        case 0x9F: std::cout << " SBC A \n";  sbc(reg.A, reg.A); break;
        case 0xA0: std::cout << " AND B \n";  logAnd(reg.A, reg.B); break;
        case 0xA1: std::cout << " AND C \n";  logAnd(reg.A, reg.C); break;
        case 0xA2: std::cout << " AND D \n";  logAnd(reg.A, reg.D); break;
        case 0xA3: std::cout << " AND E \n";  logAnd(reg.A, reg.E); break;
        case 0xA4: std::cout << " AND H \n";  logAnd(reg.A, reg.H); break;
        case 0xA5: std::cout << " AND L \n";  logAnd(reg.A, reg.L); break;
        case 0xA6: std::cout << " AND (HL) \n";  logAnd(reg.A, pairRegData(reg.H, reg.L)); break;
        case 0xA7: std::cout << " AND A \n";  logAnd(reg.A, reg.A); break;
        case 0xA8: std::cout << " XOR B \n";  logXor(reg.A, reg.B); break;
        case 0xA9: std::cout << " XOR C \n";  logXor(reg.A, reg.C); break;
        case 0xAA: std::cout << " XOR D \n";  logXor(reg.A, reg.D); break;
        case 0xAB: std::cout << " XOR E \n";  logXor(reg.A, reg.E); break;
        case 0xAC: std::cout << " XOR H \n";  logXor(reg.A, reg.H); break;
        case 0xAD: std::cout << " XOR L \n";  logXor(reg.A, reg.L); break;
        case 0xAE: std::cout << " XOR (HL) \n";  logXor(reg.A, pairRegData(reg.H, reg.L)); break;
        case 0xAF: std::cout << " XOR A \n";  logXor(reg.A, reg.A); break;
        case 0xB0: std::cout << " OR B \n";  logOr(reg.A, reg.B); break;
        case 0xB1: std::cout << " OR C \n";  logOr(reg.A, reg.C); break;
        case 0xB2: std::cout << " OR D \n";  logOr(reg.A, reg.D); break;
        case 0xB3: std::cout << " OR E \n";  logOr(reg.A, reg.E); break;
        case 0xB4: std::cout << " OR H \n";  logOr(reg.A, reg.H); break;
        case 0xB5: std::cout << " OR L \n";  logOr(reg.A, reg.L); break;
        case 0xB6: std::cout << " OR (HL) \n";  logOr(reg.A, pairRegData(reg.H, reg.L)); break;
        case 0xB7: std::cout << " OR A \n";  logOr(reg.A, reg.A); break;
        case 0xB8: std::cout << " CP B \n";  cp(reg.A, reg.B); break;
        case 0xB9: std::cout << " CP C \n";  cp(reg.A, reg.C); break;
        case 0xBA: std::cout << " CP D \n";  cp(reg.A, reg.D); break;
        case 0xBB: std::cout << " CP E \n";  cp(reg.A, reg.E); break;
        case 0xBC: std::cout << " CP H \n";  cp(reg.A, reg.H); break;
        case 0xBD: std::cout << " CP L \n";  cp(reg.A, reg.L); break;
        case 0xBE: std::cout << " CP (HL) \n";  cp(reg.A, pairRegData(reg.H, reg.L)); break;
        case 0xBF: std::cout << " CP A \n";  cp(reg.A, reg.A); break;
        case 0xC0:{
            bool jump = ret( ~FLAGZERO);
            std::cout << " RET NZ \n"; if (jump) {isJump = true ;} break;
        }
        case 0xC1: std::cout << " POP BC \n";  pop(reg.B, reg.C); break;
        case 0xC2:{
            uint8_t val1 = gbdata.buffer[pc+1]; uint8_t val2 = gbdata.buffer[pc+2];
            bool jump = jp( joinByte(val1,val2), ~FLAGZERO);
            std::cout << " JP NZ nn " << HEX(val1) << ' ' << HEX(val2) << "\n"; opbytes = 3; if (jump) {isJump = true ;} break;
        }
        case 0xC3:{
            uint8_t val1 = gbdata.buffer[pc+1]; uint8_t val2 = gbdata.buffer[pc+2];
            jp( joinByte(val1,val2) );
            std::cout << " JP nn " << HEX(val1) << ' ' << HEX(val2) << "\n"; opbytes = 3; isJump = true ; break;
        }
        case 0xC4:{
            uint8_t val1 = gbdata.buffer[pc+1]; uint8_t val2 = gbdata.buffer[pc+2];
            bool jump = call(val1, val2, ~FLAGZERO);
            std::cout << " CALL NZ nn " << HEX(val1) << ' ' << HEX(val2) << "\n"; opbytes = 3; if (jump) {isJump = true ;} break;
        }
        case 0xC5: std::cout << " PUSH BC \n";  push(reg.B, reg.C); break;
        case 0xC6:{
            std::cout << " ADD n " << HEX(gbdata.buffer[pc+1]) << "\n"; opbytes = 2;
            add(reg.A, gbdata.buffer[pc+1]);break;
        }
        case 0xC7: std::cout << " RST 0x00 \n";  rst(0x00); break;
        case 0xC8:{
            bool jump = ret( FLAGZERO);
            std::cout << " RET Z \n"; if (jump) {isJump = true ;} break;
        }
        case 0xC9: std::cout << " RET \n";  ret(); isJump = true ; break;
        case 0xCA:{
            uint8_t val1 = gbdata.buffer[pc+1]; uint8_t val2 = gbdata.buffer[pc+2];
            bool jump = jp( joinByte(val1,val2), FLAGZERO);
            std::cout << " JP Z nn " << HEX(val1) << ' ' << HEX(val2) << "\n"; opbytes = 3; if (jump) {isJump = true ;} break;
        }
        case 0xCB: std::cout << " CB op \n";  decodePreByte(gbdata.buffer[pc+1]); opbytes = 2; break;
        case 0xCC:{
            uint8_t val1 = gbdata.buffer[pc+1]; uint8_t val2 = gbdata.buffer[pc+2];
            bool jump = call(val1, val2, FLAGZERO);
            std::cout << " CALL Z nn " << HEX(val1) << ' ' << HEX(val2) << "\n"; opbytes = 3; if (jump) {isJump = true ;} break;
        }
        case 0xCD:{
            uint8_t val1 = gbdata.buffer[pc+1]; uint8_t val2 = gbdata.buffer[pc+2];
            call(val1, val2);
            std::cout << " CALL nn " << HEX(val1) << ' ' << HEX(val2) << "\n"; opbytes = 3; break;
        }
        case 0xCE:{
            std::cout << " ADC n " << HEX(gbdata.buffer[pc+1]) << "\n"; opbytes = 2;
            adc(reg.A, gbdata.buffer[pc+1]);break;
        }
        case 0xCF: std::cout << " RST 0x08 \n";  rst(0x08); break;
        case 0xD0:{
            bool jump = ret( ~FLAGCARRY);
            std::cout << " RET NC \n"; if (jump) {isJump = true ;} break;
        }
        case 0xD1: std::cout << " POP DE \n";  pop(reg.D, reg.E); break;
        case 0xD2:{
            uint8_t val1 = gbdata.buffer[pc+1]; uint8_t val2 = gbdata.buffer[pc+2];
            bool jump = jp( joinByte(val1,val2), ~FLAGCARRY);
            std::cout << " JP NC nn " << HEX(val1) << ' ' << HEX(val2) << "\n"; opbytes = 3; if (jump) {isJump = true ;} break;
        }
        case 0xD4:{
            uint8_t val1 = gbdata.buffer[pc+1]; uint8_t val2 = gbdata.buffer[pc+2];
            bool jump = call(val1, val2, ~FLAGCARRY);
            std::cout << " CALL NC nn " << HEX(val1) << ' ' << HEX(val2) << "\n"; opbytes = 3; if (jump) {isJump = true ;} break;
        }
        case 0xD5: std::cout << " PUSH DE \n";  push(reg.D, reg.E); break;
        case 0xD6:{
            std::cout << " SUB n " << HEX(gbdata.buffer[pc+1]) << "\n"; opbytes = 2;
            sub(reg.A, gbdata.buffer[pc+1]);break;
        }
        case 0xD7: std::cout << " RST 0x10 \n";  rst(0x10); break;
        case 0xD8:{
            bool jump = ret( FLAGCARRY);
            std::cout << " RET C \n"; if (jump) {isJump = true ;} break;
        }
        case 0xD9: std::cout << " RETI \n";  reti(); break;
        case 0xDA:{
            uint8_t val1 = gbdata.buffer[pc+1]; uint8_t val2 = gbdata.buffer[pc+2];
            bool jump = jp( joinByte(val1,val2), FLAGCARRY);
            std::cout << " JP C nn " << HEX(val1) << ' ' << HEX(val2) << "\n"; opbytes = 3; if (jump) {isJump = true ;} break;
        }
        case 0xDC:{
            uint8_t val1 = gbdata.buffer[pc+1]; uint8_t val2 = gbdata.buffer[pc+2];
            bool jump = call(val1, val2, FLAGCARRY);
            std::cout << " CALL C nn " << HEX(val1) << ' ' << HEX(val2) << "\n"; opbytes = 3; if (jump) {isJump = true ;} break;
        }
        case 0xDE:{
            std::cout << " SBC n " << HEX(gbdata.buffer[pc+1]) << "\n"; opbytes = 2;
            sbc(reg.A, gbdata.buffer[pc+1]);break;
        }
        case 0xDF: std::cout << " RST 0x18 \n";  rst(0x18); break;
        case 0xE0:{
            std::cout << " LDH (n) A " << HEX(gbdata.buffer[pc+1]) << "\n"; opbytes = 2;
             ldh_E0(gbdata.buffer[pc+1]);break;
        }
        case 0xE1: std::cout << " POP HL \n";  pop(reg.H, reg.L); break;
        case 0xE2: std::cout << " LD (C) A \n";  load( joinByte(0x00, reg.C), reg.A ); break;
        case 0xE5: std::cout << " PUSH HL \n";  push(reg.H, reg.L); break;
        case 0xE6:{
            std::cout << " AND n " << HEX(gbdata.buffer[pc+1]) << "\n"; opbytes = 2;
            logAnd(reg.A, gbdata.buffer[pc+1]);break;
        }
        case 0xE7: std::cout << " RST 0x20 \n";  rst(0x20); break;
        case 0xE8:{
            int8_t val1 = gbdata.buffer[pc+1];
            std::cout << " ADD SP e " << HEX(val1) << "\n"; opbytes = 2;
             addByte2SP(val1);break;
        }
        case 0xE9: std::cout << " JP HL \n";  jp( pairReg(reg.H, reg.L) ); break;
        case 0xEA:{
            uint8_t val1 = gbdata.buffer[pc+1]; uint8_t val2 = gbdata.buffer[pc+2];
            load( joinByte(val1, val2), reg.A);
            std::cout << " LD (nn) A " << HEX(val1) << ' ' << HEX(val2) << "\n"; opbytes = 3; break;
        }
        case 0xEE:{
            std::cout << " XOR n " << HEX(gbdata.buffer[pc+1]) << "\n"; opbytes = 2;
            logXor(reg.A, gbdata.buffer[pc+1]);break;
        }
        case 0xEF: std::cout << " RST 0x28 \n";  rst(0x28); break;
        case 0xF0:{
            std::cout << " LDH A (n) " << HEX(gbdata.buffer[pc+1]) << "\n"; opbytes = 2;
             ldh_F0(gbdata.buffer[pc+1]);break;
        }
        case 0xF1: std::cout << " POP AF \n";  pop(reg.A, reg.F); break;
        case 0xF2: std::cout << " LD A (C) \n";  load( reg.A, mem.read(joinByte(0x00, reg.C)) );  break;
        case 0xF3: std::cout << " DI \n";  di(); break;
        case 0xF5: std::cout << " PUSH AF \n";  push(reg.A, reg.F); break;
        case 0xF6:{
            std::cout << " OR n " << HEX(gbdata.buffer[pc+1]) << "\n"; opbytes = 2;
            logOr(reg.A, gbdata.buffer[pc+1]);break;
        }
        case 0xF7: std::cout << " RST 0x30 \n";  rst(0x30); break;
        case 0xF8:{
            std::cout << " LD HL SP+e " << HEX(gbdata.buffer[pc+1]) << "\n"; opbytes = 2;
            loadSP2Reg( reg.H, reg.L); addByte2SP(gbdata.buffer[pc+1]); break;
        }
        case 0xF9: std::cout << " LD SP HL \n";  loadSP( reg.H, reg.L); break;
        case 0xFA:{
            uint8_t val1 = gbdata.buffer[pc+1]; uint8_t val2 = gbdata.buffer[pc+2];
            load(reg.A, joinByte(val1,val2) );
            std::cout << " LD A (nn) " << HEX(val1) << ' ' << HEX(val2) << "\n"; opbytes = 3; break;
        }
        case 0xFB: std::cout << " EI \n";  ei(); break;
        case 0xFE:{
            std::cout << " CP n " << HEX(gbdata.buffer[pc+1]) << "\n"; opbytes = 2;
            cp(reg.A, gbdata.buffer[pc+1]);break;
        }
        case 0xFF: std::cout << " RST 0x38 \n";  rst(0x38); break;
        default: std::cout << "***Instruction not recognized*** " << HEX(code) << "\n"; break;
    }

    if (!isJump){
        pc = pc + opbytes;
    }
    //isJump = false;

}


void CPU::decodePreByte(uint8_t bitCode){

    //parse cb prefixed opcode
    switch (bitCode) {
        case 0x00: std::cout << " RLC B \n";  rlc(reg.B); break;
        case 0x01: std::cout << " RLC C \n";  rlc(reg.C); break;
        case 0x02: std::cout << " RLC D \n";  rlc(reg.D); break;
        case 0x03: std::cout << " RLC E \n";  rlc(reg.E); break;
        case 0x04: std::cout << " RLC H \n";  rlc(reg.H); break;
        case 0x05: std::cout << " RLC L \n";  rlc(reg.L); break;
        case 0x06: std::cout << " RLC (HL) \n";  rlc(reg.A, true); break;
        case 0x07: std::cout << " RLC A \n";  rlc(reg.A); break;
        case 0x08: std::cout << " RRC B \n";  rrc(reg.B); break;
        case 0x09: std::cout << " RRC C \n";  rrc(reg.C); break;
        case 0x0A: std::cout << " RRC D \n";  rrc(reg.D); break;
        case 0x0B: std::cout << " RRC E \n";  rrc(reg.E); break;
        case 0x0C: std::cout << " RRC H \n";  rrc(reg.H); break;
        case 0x0D: std::cout << " RRC L \n";  rrc(reg.L); break;
        case 0x0E: std::cout << " RRC (HL) \n";  rrc(reg.A, true); break;
        case 0x0F: std::cout << " RRC A \n";  rrc(reg.A); break;
        case 0x10: std::cout << " RL B \n";  rl(reg.B); break;
        case 0x11: std::cout << " RL C \n";  rl(reg.C); break;
        case 0x12: std::cout << " RL D \n";  rl(reg.D); break;
        case 0x13: std::cout << " RL E \n";  rl(reg.E); break;
        case 0x14: std::cout << " RL H \n";  rl(reg.H); break;
        case 0x15: std::cout << " RL L \n";  rl(reg.L); break;
        case 0x16: std::cout << " RL (HL) \n";  rl(reg.A, true); break;
        case 0x17: std::cout << " RL A \n";  rl(reg.A); break;
        case 0x18: std::cout << " RR B \n";  rr(reg.B); break;
        case 0x19: std::cout << " RR C \n";  rr(reg.C); break;
        case 0x1A: std::cout << " RR D \n";  rr(reg.D); break;
        case 0x1B: std::cout << " RR E \n";  rr(reg.E); break;
        case 0x1C: std::cout << " RR H \n";  rr(reg.H); break;
        case 0x1D: std::cout << " RR L \n";  rr(reg.L); break;
        case 0x1E: std::cout << " RR (HL) \n";  rr(reg.A, true); break;
        case 0x1F: std::cout << " RR A \n";  rr(reg.A); break;
        case 0x20: std::cout << " SLA B \n";  sla(reg.B); break;
        case 0x21: std::cout << " SLA C \n";  sla(reg.C); break;
        case 0x22: std::cout << " SLA D \n";  sla(reg.D); break;
        case 0x23: std::cout << " SLA E \n";  sla(reg.E); break;
        case 0x24: std::cout << " SLA H \n";  sla(reg.H); break;
        case 0x25: std::cout << " SLA L \n";  sla(reg.L); break;
        case 0x26: std::cout << " SLA (HL) \n";  sla(reg.A, true); break;
        case 0x27: std::cout << " SLA A \n";  sla(reg.A); break;
        case 0x28: std::cout << " SRA B \n";  sra(reg.B); break;
        case 0x29: std::cout << " SRA C \n";  sra(reg.C); break;
        case 0x2A: std::cout << " SRA D \n";  sra(reg.D); break;
        case 0x2B: std::cout << " SRA E \n";  sra(reg.E); break;
        case 0x2C: std::cout << " SRA H \n";  sra(reg.H); break;
        case 0x2D: std::cout << " SRA L \n";  sra(reg.L); break;
        case 0x2E: std::cout << " SRA (HL) \n";  sra(reg.A, true); break;
        case 0x2F: std::cout << " SRA A \n";  sra(reg.A); break;
        case 0x30: std::cout << " SWAP B \n";  swap(reg.B); break;
        case 0x31: std::cout << " SWAP C \n";  swap(reg.C); break;
        case 0x32: std::cout << " SWAP D \n";  swap(reg.D); break;
        case 0x33: std::cout << " SWAP E \n";  swap(reg.E); break;
        case 0x34: std::cout << " SWAP H \n";  swap(reg.H); break;
        case 0x35: std::cout << " SWAP L \n";  swap(reg.L); break;
        case 0x36: std::cout << " SWAP (HL) \n";  swap(reg.A, true); break;
        case 0x37: std::cout << " SWAP A \n";  swap(reg.A); break;
        case 0x38: std::cout << " SRL B \n";  srl(reg.B); break;
        case 0x39: std::cout << " SRL C \n";  srl(reg.C); break;
        case 0x3A: std::cout << " SRL D \n";  srl(reg.D); break;
        case 0x3B: std::cout << " SRL E \n";  srl(reg.E); break;
        case 0x3C: std::cout << " SRL H \n";  srl(reg.H); break;
        case 0x3D: std::cout << " SRL L \n";  srl(reg.L); break;
        case 0x3E: std::cout << " SRL (HL) \n";  srl(reg.A, true); break;
        case 0x3F: std::cout << " SRL A \n";  srl(reg.A); break;
        case 0x40: std::cout << " BIT 0 B \n";  bit(BITZERO, reg.B); break;
        case 0x41: std::cout << " BIT 0 C \n";  bit(BITZERO, reg.C); break;
        case 0x42: std::cout << " BIT 0 D \n";  bit(BITZERO, reg.D); break;
        case 0x43: std::cout << " BIT 0 E \n";  bit(BITZERO, reg.E); break;
        case 0x44: std::cout << " BIT 0 H \n";  bit(BITZERO, reg.H); break;
        case 0x45: std::cout << " BIT 0 L \n";  bit(BITZERO, reg.L); break;
        case 0x46: std::cout << " BIT 0 (HL) \n";  bit(BITZERO, reg.A, true); break;
        case 0x47: std::cout << " BIT 0 A \n";  bit(BITZERO, reg.A); break;
        case 0x48: std::cout << " BIT 1 B \n";  bit(BITONE, reg.B); break;
        case 0x49: std::cout << " BIT 1 C \n";  bit(BITONE, reg.C); break;
        case 0x4A: std::cout << " BIT 1 D \n";  bit(BITONE, reg.D); break;
        case 0x4B: std::cout << " BIT 1 E \n";  bit(BITONE, reg.E); break;
        case 0x4C: std::cout << " BIT 1 H \n";  bit(BITONE, reg.H); break;
        case 0x4D: std::cout << " BIT 1 L \n";  bit(BITONE, reg.L); break;
        case 0x4E: std::cout << " BIT 1 (HL) \n";  bit(BITONE, reg.A, true); break;
        case 0x4F: std::cout << " BIT 1 A \n";  bit(BITONE, reg.A); break;
        case 0x50: std::cout << " BIT 2 B \n";  bit(BITTWO, reg.B); break;
        case 0x51: std::cout << " BIT 2 C \n";  bit(BITTWO, reg.C); break;
        case 0x52: std::cout << " BIT 2 D \n";  bit(BITTWO, reg.D); break;
        case 0x53: std::cout << " BIT 2 E \n";  bit(BITTWO, reg.E); break;
        case 0x54: std::cout << " BIT 2 H \n";  bit(BITTWO, reg.H); break;
        case 0x55: std::cout << " BIT 2 L \n";  bit(BITTWO, reg.L); break;
        case 0x56: std::cout << " BIT 2 (HL) \n";  bit(BITTWO, reg.A, true); break;
        case 0x57: std::cout << " BIT 2 A \n";  bit(BITTWO, reg.A); break;
        case 0x58: std::cout << " BIT 3 B \n";  bit(BITTHREE, reg.B); break;
        case 0x59: std::cout << " BIT 3 C \n";  bit(BITTHREE, reg.C); break;
        case 0x5A: std::cout << " BIT 3 D \n";  bit(BITTHREE, reg.D); break;
        case 0x5B: std::cout << " BIT 3 E \n";  bit(BITTHREE, reg.E); break;
        case 0x5C: std::cout << " BIT 3 H \n";  bit(BITTHREE, reg.H); break;
        case 0x5D: std::cout << " BIT 3 L \n";  bit(BITTHREE, reg.L); break;
        case 0x5E: std::cout << " BIT 3 (HL) \n";  bit(BITTHREE, reg.A, true); break;
        case 0x5F: std::cout << " BIT 3 A \n";  bit(BITTHREE, reg.A); break;
        case 0x60: std::cout << " BIT 4 B \n";  bit(BITFOUR, reg.B); break;
        case 0x61: std::cout << " BIT 4 C \n";  bit(BITFOUR, reg.C); break;
        case 0x62: std::cout << " BIT 4 D \n";  bit(BITFOUR, reg.D); break;
        case 0x63: std::cout << " BIT 4 E \n";  bit(BITFOUR, reg.E); break;
        case 0x64: std::cout << " BIT 4 H \n";  bit(BITFOUR, reg.H); break;
        case 0x65: std::cout << " BIT 4 L \n";  bit(BITFOUR, reg.L); break;
        case 0x66: std::cout << " BIT 4 (HL) \n";  bit(BITFOUR, reg.A, true); break;
        case 0x67: std::cout << " BIT 4 A \n";  bit(BITFOUR, reg.A); break;
        case 0x68: std::cout << " BIT 5 B \n";  bit(BITFIVE, reg.B); break;
        case 0x69: std::cout << " BIT 5 C \n";  bit(BITFIVE, reg.C); break;
        case 0x6A: std::cout << " BIT 5 D \n";  bit(BITFIVE, reg.D); break;
        case 0x6B: std::cout << " BIT 5 E \n";  bit(BITFIVE, reg.E); break;
        case 0x6C: std::cout << " BIT 5 H \n";  bit(BITFIVE, reg.H); break;
        case 0x6D: std::cout << " BIT 5 L \n";  bit(BITFIVE, reg.L); break;
        case 0x6E: std::cout << " BIT 5 (HL) \n";  bit(BITFIVE, reg.A, true); break;
        case 0x6F: std::cout << " BIT 5 A \n";  bit(BITFIVE, reg.A); break;
        case 0x70: std::cout << " BIT 6 B \n";  bit(BITSIX, reg.B); break;
        case 0x71: std::cout << " BIT 6 C \n";  bit(BITSIX, reg.C); break;
        case 0x72: std::cout << " BIT 6 D \n";  bit(BITSIX, reg.D); break;
        case 0x73: std::cout << " BIT 6 E \n";  bit(BITSIX, reg.E); break;
        case 0x74: std::cout << " BIT 6 H \n";  bit(BITSIX, reg.H); break;
        case 0x75: std::cout << " BIT 6 L \n";  bit(BITSIX, reg.L); break;
        case 0x76: std::cout << " BIT 6 (HL) \n";  bit(BITSIX, reg.A, true); break;
        case 0x77: std::cout << " BIT 6 A \n";  bit(BITSIX, reg.A); break;
        case 0x78: std::cout << " BIT 7 B \n";  bit(BITSEVEN, reg.B); break;
        case 0x79: std::cout << " BIT 7 C \n";  bit(BITSEVEN, reg.C); break;
        case 0x7A: std::cout << " BIT 7 D \n";  bit(BITSEVEN, reg.D); break;
        case 0x7B: std::cout << " BIT 7 E \n";  bit(BITSEVEN, reg.E); break;
        case 0x7C: std::cout << " BIT 7 H \n";  bit(BITSEVEN, reg.H); break;
        case 0x7D: std::cout << " BIT 7 L \n";  bit(BITSEVEN, reg.L); break;
        case 0x7E: std::cout << " BIT 7 (HL) \n";  bit(BITSEVEN, reg.A, true); break;
        case 0x7F: std::cout << " BIT 7 A \n";  bit(BITSEVEN, reg.A); break;
        case 0x80: std::cout << " RES 0 B \n";  res(BITZERO, reg.B); break;
        case 0x81: std::cout << " RES 0 C \n";  res(BITZERO, reg.C); break;
        case 0x82: std::cout << " RES 0 D \n";  res(BITZERO, reg.D); break;
        case 0x83: std::cout << " RES 0 E \n";  res(BITZERO, reg.E); break;
        case 0x84: std::cout << " RES 0 H \n";  res(BITZERO, reg.H); break;
        case 0x85: std::cout << " RES 0 L \n";  res(BITZERO, reg.L); break;
        case 0x86: std::cout << " RES 0 (HL) \n";  res(BITZERO, reg.A, true); break;
        case 0x87: std::cout << " RES 0 A \n";  res(BITZERO, reg.A); break;
        case 0x88: std::cout << " RES 1 B \n";  res(BITONE, reg.B); break;
        case 0x89: std::cout << " RES 1 C \n";  res(BITONE, reg.C); break;
        case 0x8A: std::cout << " RES 1 D \n";  res(BITONE, reg.D); break;
        case 0x8B: std::cout << " RES 1 E \n";  res(BITONE, reg.E); break;
        case 0x8C: std::cout << " RES 1 H \n";  res(BITONE, reg.H); break;
        case 0x8D: std::cout << " RES 1 L \n";  res(BITONE, reg.L); break;
        case 0x8E: std::cout << " RES 1 (HL) \n";  res(BITONE, reg.A, true); break;
        case 0x8F: std::cout << " RES 1 A \n";  res(BITONE, reg.A); break;
        case 0x90: std::cout << " RES 2 B \n";  res(BITTWO, reg.B); break;
        case 0x91: std::cout << " RES 2 C \n";  res(BITTWO, reg.C); break;
        case 0x92: std::cout << " RES 2 D \n";  res(BITTWO, reg.D); break;
        case 0x93: std::cout << " RES 2 E \n";  res(BITTWO, reg.E); break;
        case 0x94: std::cout << " RES 2 H \n";  res(BITTWO, reg.H); break;
        case 0x95: std::cout << " RES 2 L \n";  res(BITTWO, reg.L); break;
        case 0x96: std::cout << " RES 2 (HL) \n";  res(BITTWO, reg.A, true); break;
        case 0x97: std::cout << " RES 2 A \n";  res(BITTWO, reg.A); break;
        case 0x98: std::cout << " RES 3 B \n";  res(BITTHREE, reg.B); break;
        case 0x99: std::cout << " RES 3 C \n";  res(BITTHREE, reg.C); break;
        case 0x9A: std::cout << " RES 3 D \n";  res(BITTHREE, reg.D); break;
        case 0x9B: std::cout << " RES 3 E \n";  res(BITTHREE, reg.E); break;
        case 0x9C: std::cout << " RES 3 H \n";  res(BITTHREE, reg.H); break;
        case 0x9D: std::cout << " RES 3 L \n";  res(BITTHREE, reg.L); break;
        case 0x9E: std::cout << " RES 3 (HL) \n";  res(BITTHREE, reg.A, true); break;
        case 0x9F: std::cout << " RES 3 A \n";  res(BITTHREE, reg.A); break;
        case 0xA0: std::cout << " RES 4 B \n";  res(BITFOUR, reg.B); break;
        case 0xA1: std::cout << " RES 4 C \n";  res(BITFOUR, reg.C); break;
        case 0xA2: std::cout << " RES 4 D \n";  res(BITFOUR, reg.D); break;
        case 0xA3: std::cout << " RES 4 E \n";  res(BITFOUR, reg.E); break;
        case 0xA4: std::cout << " RES 4 H \n";  res(BITFOUR, reg.H); break;
        case 0xA5: std::cout << " RES 4 L \n";  res(BITFOUR, reg.L); break;
        case 0xA6: std::cout << " RES 4 (HL) \n";  res(BITFOUR, reg.A, true); break;
        case 0xA7: std::cout << " RES 4 A \n";  res(BITFOUR, reg.A); break;
        case 0xA8: std::cout << " RES 5 B \n";  res(BITFIVE, reg.B); break;
        case 0xA9: std::cout << " RES 5 C \n";  res(BITFIVE, reg.C); break;
        case 0xAA: std::cout << " RES 5 D \n";  res(BITFIVE, reg.D); break;
        case 0xAB: std::cout << " RES 5 E \n";  res(BITFIVE, reg.E); break;
        case 0xAC: std::cout << " RES 5 H \n";  res(BITFIVE, reg.H); break;
        case 0xAD: std::cout << " RES 5 L \n";  res(BITFIVE, reg.L); break;
        case 0xAE: std::cout << " RES 5 (HL) \n";  res(BITFIVE, reg.A, true); break;
        case 0xAF: std::cout << " RES 5 A \n";  res(BITFIVE, reg.A); break;
        case 0xB0: std::cout << " RES 6 B \n";  res(BITSIX, reg.B); break;
        case 0xB1: std::cout << " RES 6 C \n";  res(BITSIX, reg.C); break;
        case 0xB2: std::cout << " RES 6 D \n";  res(BITSIX, reg.D); break;
        case 0xB3: std::cout << " RES 6 E \n";  res(BITSIX, reg.E); break;
        case 0xB4: std::cout << " RES 6 H \n";  res(BITSIX, reg.H); break;
        case 0xB5: std::cout << " RES 6 L \n";  res(BITSIX, reg.L); break;
        case 0xB6: std::cout << " RES 6 (HL) \n";  res(BITSIX, reg.A, true); break;
        case 0xB7: std::cout << " RES 6 A \n";  res(BITSIX, reg.A); break;
        case 0xB8: std::cout << " RES 7 B \n";  res(BITSEVEN, reg.B); break;
        case 0xB9: std::cout << " RES 7 C \n";  res(BITSEVEN, reg.C); break;
        case 0xBA: std::cout << " RES 7 D \n";  res(BITSEVEN, reg.D); break;
        case 0xBB: std::cout << " RES 7 E \n";  res(BITSEVEN, reg.E); break;
        case 0xBC: std::cout << " RES 7 H \n";  res(BITSEVEN, reg.H); break;
        case 0xBD: std::cout << " RES 7 L \n";  res(BITSEVEN, reg.L); break;
        case 0xBE: std::cout << " RES 7 (HL) \n";  res(BITSEVEN, reg.A, true); break;
        case 0xBF: std::cout << " RES 7 A \n";  res(BITSEVEN, reg.A); break;
        case 0xC0: std::cout << " SET 0 B \n";  set(BITZERO, reg.B); break;
        case 0xC1: std::cout << " SET 0 C \n";  set(BITZERO, reg.C); break;
        case 0xC2: std::cout << " SET 0 D \n";  set(BITZERO, reg.D); break;
        case 0xC3: std::cout << " SET 0 E \n";  set(BITZERO, reg.E); break;
        case 0xC4: std::cout << " SET 0 H \n";  set(BITZERO, reg.H); break;
        case 0xC5: std::cout << " SET 0 L \n";  set(BITZERO, reg.L); break;
        case 0xC6: std::cout << " SET 0 (HL) \n";  set(BITZERO, reg.A, true); break;
        case 0xC7: std::cout << " SET 0 A \n";  set(BITZERO, reg.A); break;
        case 0xC8: std::cout << " SET 1 B \n";  set(BITONE, reg.B); break;
        case 0xC9: std::cout << " SET 1 C \n";  set(BITONE, reg.C); break;
        case 0xCA: std::cout << " SET 1 D \n";  set(BITONE, reg.D); break;
        case 0xCB: std::cout << " SET 1 E \n";  set(BITONE, reg.E); break;
        case 0xCC: std::cout << " SET 1 H \n";  set(BITONE, reg.H); break;
        case 0xCD: std::cout << " SET 1 L \n";  set(BITONE, reg.L); break;
        case 0xCE: std::cout << " SET 1 (HL) \n";  set(BITONE, reg.A, true); break;
        case 0xCF: std::cout << " SET 1 A \n";  set(BITONE, reg.A); break;
        case 0xD0: std::cout << " SET 2 B \n";  set(BITTWO, reg.B); break;
        case 0xD1: std::cout << " SET 2 C \n";  set(BITTWO, reg.C); break;
        case 0xD2: std::cout << " SET 2 D \n";  set(BITTWO, reg.D); break;
        case 0xD3: std::cout << " SET 2 E \n";  set(BITTWO, reg.E); break;
        case 0xD4: std::cout << " SET 2 H \n";  set(BITTWO, reg.H); break;
        case 0xD5: std::cout << " SET 2 L \n";  set(BITTWO, reg.L); break;
        case 0xD6: std::cout << " SET 2 (HL) \n";  set(BITTWO, reg.A, true); break;
        case 0xD7: std::cout << " SET 2 A \n";  set(BITTWO, reg.A); break;
        case 0xD8: std::cout << " SET 3 B \n";  set(BITTHREE, reg.B); break;
        case 0xD9: std::cout << " SET 3 C \n";  set(BITTHREE, reg.C); break;
        case 0xDA: std::cout << " SET 3 D \n";  set(BITTHREE, reg.D); break;
        case 0xDB: std::cout << " SET 3 E \n";  set(BITTHREE, reg.E); break;
        case 0xDC: std::cout << " SET 3 H \n";  set(BITTHREE, reg.H); break;
        case 0xDD: std::cout << " SET 3 L \n";  set(BITTHREE, reg.L); break;
        case 0xDE: std::cout << " SET 3 (HL) \n";  set(BITTHREE, reg.A, true); break;
        case 0xDF: std::cout << " SET 3 A \n";  set(BITTHREE, reg.A); break;
        case 0xE0: std::cout << " SET 4 B \n";  set(BITFOUR, reg.B); break;
        case 0xE1: std::cout << " SET 4 C \n";  set(BITFOUR, reg.C); break;
        case 0xE2: std::cout << " SET 4 D \n";  set(BITFOUR, reg.D); break;
        case 0xE3: std::cout << " SET 4 E \n";  set(BITFOUR, reg.E); break;
        case 0xE4: std::cout << " SET 4 H \n";  set(BITFOUR, reg.H); break;
        case 0xE5: std::cout << " SET 4 L \n";  set(BITFOUR, reg.L); break;
        case 0xE6: std::cout << " SET 4 (HL) \n";  set(BITFOUR, reg.A, true); break;
        case 0xE7: std::cout << " SET 4 A \n";  set(BITFOUR, reg.A); break;
        case 0xE8: std::cout << " SET 5 B \n";  set(BITFIVE, reg.B); break;
        case 0xE9: std::cout << " SET 5 C \n";  set(BITFIVE, reg.C); break;
        case 0xEA: std::cout << " SET 5 D \n";  set(BITFIVE, reg.D); break;
        case 0xEB: std::cout << " SET 5 E \n";  set(BITFIVE, reg.E); break;
        case 0xEC: std::cout << " SET 5 H \n";  set(BITFIVE, reg.H); break;
        case 0xED: std::cout << " SET 5 L \n";  set(BITFIVE, reg.L); break;
        case 0xEE: std::cout << " SET 5 (HL) \n";  set(BITFIVE, reg.A, true); break;
        case 0xEF: std::cout << " SET 5 A \n";  set(BITFIVE, reg.A); break;
        case 0xF0: std::cout << " SET 6 B \n";  set(BITSIX, reg.B); break;
        case 0xF1: std::cout << " SET 6 C \n";  set(BITSIX, reg.C); break;
        case 0xF2: std::cout << " SET 6 D \n";  set(BITSIX, reg.D); break;
        case 0xF3: std::cout << " SET 6 E \n";  set(BITSIX, reg.E); break;
        case 0xF4: std::cout << " SET 6 H \n";  set(BITSIX, reg.H); break;
        case 0xF5: std::cout << " SET 6 L \n";  set(BITSIX, reg.L); break;
        case 0xF6: std::cout << " SET 6 (HL) \n";  set(BITSIX, reg.A, true); break;
        case 0xF7: std::cout << " SET 6 A \n";  set(BITSIX, reg.A); break;
        case 0xF8: std::cout << " SET 7 B \n";  set(BITSEVEN, reg.B); break;
        case 0xF9: std::cout << " SET 7 C \n";  set(BITSEVEN, reg.C); break;
        case 0xFA: std::cout << " SET 7 D \n";  set(BITSEVEN, reg.D); break;
        case 0xFB: std::cout << " SET 7 E \n";  set(BITSEVEN, reg.E); break;
        case 0xFC: std::cout << " SET 7 H \n";  set(BITSEVEN, reg.H); break;
        case 0xFD: std::cout << " SET 7 L \n";  set(BITSEVEN, reg.L); break;
        case 0xFE: std::cout << " SET 7 (HL) \n";  set(BITSEVEN, reg.A, true); break;
        case 0xFF: std::cout << " SET 7 A \n";  set(BITSEVEN, reg.A); break;
        default: std::cout << "***Instruction not recognized*** " << HEX(bitCode) << "\n"; break;
    }


}

