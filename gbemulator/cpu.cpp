#include "cpu.h"


#define HEX(x) std::setw(2) << std::setfill('0') << std::hex << (int)( x )

cpu::cpu()
{
    cpu_loop();
}

//read file as bytes
//TODO rewrite ability to call read file from pos to pos, return struct of instruction, any bytes, and file read end position
void cpu::readFiletoBytes(struct gbData &gbdata){
    std::cout << "starting file read \n";

    std::ifstream file(gbdata.filename, std::ios::binary);
    if( !file.is_open() ) {
        std::cout << "file failed to open \n";
        return;
    }

    //read content of file into buffer
    std::vector<unsigned char> content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    int count = 0;
    for(unsigned char& c : content) {
        if (count % 16 == 0){
            std::cout << "\n" << std::setw(6) << std::setfill('0') << count/16 << ": ";
        }
        std::cout << std::setw(2) << std::setfill('0') << std::hex << int(c) << ' ';
        count++;
    }

    gbdata.buffer = content;

    std::cout << "\nfinishing file read \n";
}


//translate byte code to instructions
int cpu::decodeByte(struct gbData &gbdata, int &pc){

    int opbytes = 1; //bytes for instr op

    //for 8 bit intru
    for(int i = 0; i < 2500; i++){
        unsigned char *code = &gbdata.buffer[pc];

        //8 bit instru
        switch (*code) {
            case 0x00: std::cout << "NOP\n"; break;
            case 0x01: std::cout << "LD BC nn\n"; break;
            case 0x02: std::cout << "LD (BC) A\n"; break;
            case 0x03: std::cout << "INC BC\n"; break;
            case 0x04: std::cout << "INC B\n"; break;
            case 0x05: std::cout << "DEC B\n"; break;
            case 0x06: std::cout << "LD B n\n"; break;
            case 0x07: std::cout << "RLCA\n"; break;
            case 0x08: std::cout << "LD (nn) SP\n"; break;
            case 0x09: std::cout << "ADD HL BC\n"; break;
            case 0x0A: std::cout << "LD A (BC)\n"; break;
            case 0x0B: std::cout << "DEC BC\n"; break;
            case 0x0C: std::cout << "INC C\n"; break;
            case 0x0D: std::cout << "DEC C\n"; break;
            case 0x0E: std::cout << "LD C n\n"; break;
            case 0x0F: std::cout << "RRCA\n"; break;
            case 0x10: std::cout << "STOP\n"; break;
            case 0x11: std::cout << "LD DE nn\n"; break;
            case 0x12: std::cout << "LD (DE) A\n"; break;
            case 0x13: std::cout << "INC DE\n"; break;
            case 0x14: std::cout << "INC D\n"; break;
            case 0x15: std::cout << "DEC D\n"; break;
            case 0x16: std::cout << "LD D n\n"; break;
            case 0x17: std::cout << "RLA\n"; break;
            case 0x18: std::cout << "JR e\n"; break;
            case 0x19: std::cout << "ADD HL DE\n"; break;
            case 0x1A: std::cout << "LD A (DE)\n"; break;
            case 0x1B: std::cout << "DEC DE\n"; break;
            case 0x1C: std::cout << "INC E\n"; break;
            case 0x1D: std::cout << "DEC E\n"; break;
            case 0x1E: std::cout << "LD E n\n"; break;
            case 0x1F: std::cout << "RRA\n"; break;
            case 0x20: std::cout << "JR NZ e\n"; break;
            case 0x21: std::cout << "LD HL nn\n"; break;
            case 0x22: std::cout << "LD (HL+) A\n"; break;
            case 0x23: std::cout << "INC HL\n"; break;
            case 0x24: std::cout << "INC H\n"; break;
            case 0x25: std::cout << "DEC H\n"; break;
            case 0x26: std::cout << "LD H n\n"; break;
            case 0x27: std::cout << "DAA\n"; break;
            case 0x28: std::cout << "JR Z e\n"; break;
            case 0x29: std::cout << "ADD HL HL\n"; break;
            case 0x2A: std::cout << "LD A (HL+)\n"; break;
            case 0x2B: std::cout << "DEC HL\n"; break;
            case 0x2C: std::cout << "INC L\n"; break;
            case 0x2D: std::cout << "DEC L\n"; break;
            case 0x2E: std::cout << "LD L n\n"; break;
            case 0x2F: std::cout << "CPL\n"; break;
            case 0x30: std::cout << "JR NC e\n"; break;
            case 0x31: std::cout << "LD SP nn\n"; break;
            case 0x32: std::cout << "LD (HL-) A\n"; break;
            case 0x33: std::cout << "INC SP\n"; break;
            case 0x34: std::cout << "INC (HL)\n"; break;
            case 0x35: std::cout << "DEC (HL)\n"; break;
            case 0x36: std::cout << "LD (HL) n\n"; break;
            case 0x37: std::cout << "SCF\n"; break;
            case 0x38: std::cout << "JR C e\n"; break;
            case 0x39: std::cout << "ADD HL SP\n"; break;
            case 0x3A: std::cout << "LD A (HL-)\n"; break;
            case 0x3B: std::cout << "DEC SP\n"; break;
            case 0x3C: std::cout << "INC A\n"; break;
            case 0x3D: std::cout << "DEC A\n"; break;
            case 0x3E: std::cout << "LD A n\n"; break;
            case 0x3F: std::cout << "CCF\n"; break;
            case 0x40: std::cout << "LD B B\n"; break;
            case 0x41: std::cout << "LD B C\n"; break;
            case 0x42: std::cout << "LD B D\n"; break;
            case 0x43: std::cout << "LD B E\n"; break;
            case 0x44: std::cout << "LD B H\n"; break;
            case 0x45: std::cout << "LD B L\n"; break;
            case 0x46: std::cout << "LD B (HL)\n"; break;
            case 0x47: std::cout << "LD B A\n"; break;
            case 0x48: std::cout << "LD C B\n"; break;
            case 0x49: std::cout << "LD C C\n"; break;
            case 0x4A: std::cout << "LD C D\n"; break;
            case 0x4B: std::cout << "LD C E\n"; break;
            case 0x4C: std::cout << "LD C H\n"; break;
            case 0x4D: std::cout << "LD C L\n"; break;
            case 0x4E: std::cout << "LD C (HL)\n"; break;
            case 0x4F: std::cout << "LD C A\n"; break;
            case 0x50: std::cout << "LD D B\n"; break;
            case 0x51: std::cout << "LD D C\n"; break;
            case 0x52: std::cout << "LD D D\n"; break;
            case 0x53: std::cout << "LD D E\n"; break;
            case 0x54: std::cout << "LD D H\n"; break;
            case 0x55: std::cout << "LD D L\n"; break;
            case 0x56: std::cout << "LD D (HL)\n"; break;
            case 0x57: std::cout << "LD D A\n"; break;
            case 0x58: std::cout << "LD E B\n"; break;
            case 0x59: std::cout << "LD E C\n"; break;
            case 0x5A: std::cout << "LD E D\n"; break;
            case 0x5B: std::cout << "LD E E\n"; break;
            case 0x5C: std::cout << "LD E H\n"; break;
            case 0x5D: std::cout << "LD E L\n"; break;
            case 0x5E: std::cout << "LD E (HL)\n"; break;
            case 0x5F: std::cout << "LD E A\n"; break;
            case 0x60: std::cout << "LD H B\n"; break;
            case 0x61: std::cout << "LD H C\n"; break;
            case 0x62: std::cout << "LD H D\n"; break;
            case 0x63: std::cout << "LD H E\n"; break;
            case 0x64: std::cout << "LD H H\n"; break;
            case 0x65: std::cout << "LD H L\n"; break;
            case 0x66: std::cout << "LD H (HL)\n"; break;
            case 0x67: std::cout << "LD H A\n"; break;
            case 0x68: std::cout << "LD L B\n"; break;
            case 0x69: std::cout << "LD L C\n"; break;
            case 0x6A: std::cout << "LD L D\n"; break;
            case 0x6B: std::cout << "LD L E\n"; break;
            case 0x6C: std::cout << "LD L H\n"; break;
            case 0x6D: std::cout << "LD L L\n"; break;
            case 0x6E: std::cout << "LD L (HL)\n"; break;
            case 0x6F: std::cout << "LD L A\n"; break;
            case 0x70: std::cout << "LD (HL) B\n"; break;
            case 0x71: std::cout << "LD (HL) C\n"; break;
            case 0x72: std::cout << "LD (HL) D\n"; break;
            case 0x73: std::cout << "LD (HL) E\n"; break;
            case 0x74: std::cout << "LD (HL) H\n"; break;
            case 0x75: std::cout << "LD (HL) L\n"; break;
            case 0x76: std::cout << "HALT\n"; break;
            case 0x77: std::cout << "LD (HL) A\n"; break;
            case 0x78: std::cout << "LD A B\n"; break;
            case 0x79: std::cout << "LD A C\n"; break;
            case 0x7A: std::cout << "LD A D\n"; break;
            case 0x7B: std::cout << "LD A E\n"; break;
            case 0x7C: std::cout << "LD A H\n"; break;
            case 0x7D: std::cout << "LD A L\n"; break;
            case 0x7E: std::cout << "LD A (HL)\n"; break;
            case 0x7F: std::cout << "LD A A\n"; break;
            case 0x80: std::cout << "ADD B\n"; break;
            case 0x81: std::cout << "ADD C\n"; break;
            case 0x82: std::cout << "ADD D\n"; break;
            case 0x83: std::cout << "ADD E\n"; break;
            case 0x84: std::cout << "ADD H\n"; break;
            case 0x85: std::cout << "ADD L\n"; break;
            case 0x86: std::cout << "ADD (HL)\n"; break;
            case 0x87: std::cout << "ADD A\n"; break;
            case 0x88: std::cout << "ADC B\n"; break;
            case 0x89: std::cout << "ADC C\n"; break;
            case 0x8A: std::cout << "ADC D\n"; break;
            case 0x8B: std::cout << "ADC E\n"; break;
            case 0x8C: std::cout << "ADC H\n"; break;
            case 0x8D: std::cout << "ADC L\n"; break;
            case 0x8E: std::cout << "ADC (HL)\n"; break;
            case 0x8F: std::cout << "ADC A\n"; break;
            case 0x90: std::cout << "SUB B\n"; break;
            case 0x91: std::cout << "SUB C\n"; break;
            case 0x92: std::cout << "SUB D\n"; break;
            case 0x93: std::cout << "SUB E\n"; break;
            case 0x94: std::cout << "SUB H\n"; break;
            case 0x95: std::cout << "SUB L\n"; break;
            case 0x96: std::cout << "SUB (HL)\n"; break;
            case 0x97: std::cout << "SUB A\n"; break;
            case 0x98: std::cout << "SBC B\n"; break;
            case 0x99: std::cout << "SBC C\n"; break;
            case 0x9A: std::cout << "SBC D\n"; break;
            case 0x9B: std::cout << "SBC E\n"; break;
            case 0x9C: std::cout << "SBC H\n"; break;
            case 0x9D: std::cout << "SBC L\n"; break;
            case 0x9E: std::cout << "SBC (HL)\n"; break;
            case 0x9F: std::cout << "SBC A\n"; break;
            case 0xA0: std::cout << "AND B\n"; break;
            case 0xA1: std::cout << "AND C\n"; break;
            case 0xA2: std::cout << "AND D\n"; break;
            case 0xA3: std::cout << "AND E\n"; break;
            case 0xA4: std::cout << "AND H\n"; break;
            case 0xA5: std::cout << "AND L\n"; break;
            case 0xA6: std::cout << "AND (HL)\n"; break;
            case 0xA7: std::cout << "AND A\n"; break;
            case 0xA8: std::cout << "XOR B\n"; break;
            case 0xA9: std::cout << "XOR C\n"; break;
            case 0xAA: std::cout << "XOR D\n"; break;
            case 0xAB: std::cout << "XOR E\n"; break;
            case 0xAC: std::cout << "XOR H\n"; break;
            case 0xAD: std::cout << "XOR L\n"; break;
            case 0xAE: std::cout << "XOR (HL)\n"; break;
            case 0xAF: std::cout << "XOR A\n"; break;
            case 0xB0: std::cout << "OR B\n"; break;
            case 0xB1: std::cout << "OR C\n"; break;
            case 0xB2: std::cout << "OR D\n"; break;
            case 0xB3: std::cout << "OR E\n"; break;
            case 0xB4: std::cout << "OR H\n"; break;
            case 0xB5: std::cout << "OR L\n"; break;
            case 0xB6: std::cout << "OR (HL)\n"; break;
            case 0xB7: std::cout << "OR A\n"; break;
            case 0xB8: std::cout << "CP B\n"; break;
            case 0xB9: std::cout << "CP C\n"; break;
            case 0xBA: std::cout << "CP D\n"; break;
            case 0xBB: std::cout << "CP E\n"; break;
            case 0xBC: std::cout << "CP H\n"; break;
            case 0xBD: std::cout << "CP L\n"; break;
            case 0xBE: std::cout << "CP (HL)\n"; break;
            case 0xBF: std::cout << "CP A\n"; break;
            case 0xC0: std::cout << "RET NZ\n"; break;
            case 0xC1: std::cout << "POP BC\n"; break;
            case 0xC2: std::cout << "JP NZ nn\n"; break;
            case 0xC3: std::cout << "JP nn\n"; break;
            case 0xC4: std::cout << "CALL NZ nn\n"; break;
            case 0xC5: std::cout << "PUSH BC\n"; break;
            case 0xC6: std::cout << "ADD n\n"; break;
            case 0xC7: std::cout << "RST 0x00\n"; break;
            case 0xC8: std::cout << "RET Z\n"; break;
            case 0xC9: std::cout << "RET\n"; break;
            case 0xCA: std::cout << "JP Z nn\n"; break;
            case 0xCB: std::cout << "CB op\n"; break;
            case 0xCC: std::cout << "CALL Z nn\n"; break;
            case 0xCD: std::cout << "CALL nn\n"; break;
            case 0xCE: std::cout << "ADC n\n"; break;
            case 0xCF: std::cout << "RST 0x08\n"; break;
            case 0xD0: std::cout << "RET NC\n"; break;
            case 0xD1: std::cout << "POP DE\n"; break;
            case 0xD2: std::cout << "JP NC nn\n"; break;
            case 0xD3: std::cout << "\n"; break;
            case 0xD4: std::cout << "CALL NC nn\n"; break;
            case 0xD5: std::cout << "PUSH DE\n"; break;
            case 0xD6: std::cout << "SUB n\n"; break;
            case 0xD7: std::cout << "RST 0x10\n"; break;
            case 0xD8: std::cout << "RET C\n"; break;
            case 0xD9: std::cout << "RETI\n"; break;
            case 0xDA: std::cout << "JP C nn\n"; break;
            case 0xDB: std::cout << "\n"; break;
            case 0xDC: std::cout << "CALL C nn\n"; break;
            case 0xDD: std::cout << "\n"; break;
            case 0xDE: std::cout << "SBC n\n"; break;
            case 0xDF: std::cout << "RST 0x18\n"; break;
            case 0xE0: std::cout << "LDH (n) A\n"; break;
            case 0xE1: std::cout << "POP HL\n"; break;
            case 0xE2: std::cout << "LD (C) A\n"; break;
            case 0xE3: std::cout << "\n"; break;
            case 0xE4: std::cout << "\n"; break;
            case 0xE5: std::cout << "PUSH HL\n"; break;
            case 0xE6: std::cout << "AND n\n"; break;
            case 0xE7: std::cout << "RST 0x20\n"; break;
            case 0xE8: std::cout << "ADD SP e\n"; break;
            case 0xE9: std::cout << "JP HL\n"; break;
            case 0xEA: std::cout << "LD (nn) A\n"; break;
            case 0xEB: std::cout << "\n"; break;
            case 0xEC: std::cout << "\n"; break;
            case 0xED: std::cout << "\n"; break;
            case 0xEE: std::cout << "XOR n\n"; break;
            case 0xEF: std::cout << "RST 0x28\n"; break;
            case 0xF0: std::cout << "LDH A (n)\n"; break;
            case 0xF1: std::cout << "POP AF\n"; break;
            case 0xF2: std::cout << "LD A (C)\n"; break;
            case 0xF3: std::cout << "DI\n"; break;
            case 0xF4: std::cout << "\n"; break;
            case 0xF5: std::cout << "PUSH AF\n"; break;
            case 0xF6: std::cout << "OR n\n"; break;
            case 0xF7: std::cout << "RST 0x30\n"; break;
            case 0xF8: std::cout << "LD HL SP+e\n"; break;
            case 0xF9: std::cout << "LD SP HL\n"; break;
            case 0xFA: std::cout << "LD A (nn)\n"; break;
            case 0xFB: std::cout << "EI\n"; break;
            case 0xFC: std::cout << "\n"; break;
            case 0xFD: std::cout << "\n"; break;
            case 0xFE: std::cout << "CP n\n"; break;
            case 0xFF: std::cout << "RST 0x38\n"; break;
            default: std::cout << "***Instruction not recognized***"; break;
        }
        pc++;
    }
    std::cout << "done decoding\n";

    return opbytes;
}


//loop
void cpu::cpu_loop(){

    bool isCpuRunning = false;

    struct gbData gbdata;
    gbdata.filename = "cpu_instrs.gb";

    //in loop
    //call read from file byte with pos
    this->readFiletoBytes(gbdata);
    this->decodeByte(gbdata, this->pc);

    //main loop
    while(isCpuRunning){

        std::cout << "run \n";

    }

}


