#include "cpu.h"


#define HEX(x) std::setw(2) << std::setfill('0') << std::hex << (int)( x )

cpu::cpu()
{
    cpu_loop();

}

//read sample file
void cpu::readFiletoBytes(const char* filename){
    std::cout << "starting file read \n";

    std::ifstream file(filename, std::ios::binary);
    int count = 0;
    char c;
    while(file){
        //read count
        if(count % 16 == 0){
            file.read(&c, 3);
            std::cout << "\n";
            std::cout << std::setw(6) << std::setfill('0') << std::hex << (int)(c) << ": ";
            count = 3;
        }
        else {
            file.read(&c, 1);
            std::cout << HEX(c) << ' ';
            count++;
        }

    }
    std::cout << "finishing file read \n";

}

//translate byte code to instructions
int cpu::decodeByte(unsigned char* code, int pc){
    pc++;

    return 1;

}


//loop
void cpu::cpu_loop(){

    bool isCpuRunning = false;

    const char filename[] = "tetris.gb";
    this->readFiletoBytes(filename);

    //main loop
    while(isCpuRunning){

        std::cout << "run \n";

    }

}


