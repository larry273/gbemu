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
        case 0x01: std::cout << "LD BC,nn\n"; break;
        case 0x02: std::cout << "LD BC,A\n"; break;
        case 0x03: std::cout << "INC BC\n"; break;
        case 0x04: std::cout << "INC B\n"; break;
        case 0x05: std::cout << "DEC B\n"; break;
        case 0x06: std::cout << "LD B,n\n"; break;
        case 0x07: std::cout << "RLCA\n"; break;
        case 0x08: std::cout << "LD (nn),SP\n"; break;
        case 0x09: std::cout << "ADD HL,BC\n"; break;
        case 0x0a: std::cout << "LD A,(BC)\n"; break;
        case 0x0b: std::cout << "DEC BC\n"; break;
        case 0x0c: std::cout << "INC C\n"; break;
        case 0x0d: std::cout << "DEC C\n"; break;
        case 0x0e: std::cout << "LD C,n\n"; break;
        case 0x0f: std::cout << "RRCA\n"; break;


        default: break;
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


