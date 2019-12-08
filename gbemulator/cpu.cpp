#include "cpu.h"


#define HEX(x) std::setw(2) << std::setfill('0') << std::hex << (int)( x )

cpu::cpu()
{
    cpu_loop();

}

//read sample file
bool cpu::readFiletoBytes(const char* filename){
    std::cout << "starting file read \n";

    std::ifstream file(filename, std::ios::binary);
    if( !file.is_open() ) {
        std::cout << "file failed to open \n";
        return false;
    }

    int count = 0;
    while(file){
        //read count
        if(count % 16 == 0){
            std::cout << "\n" << std::setw(6) << std::setfill('0') << count/16 << ": ";
        }

        int c;
        c = file.get();
        std::cout << std::setw(2) << std::setfill('0') << std::hex << +c << ' ';
        count++;

    }
    std::cout << "\nfinishing file read \n";
    return true;

}

//translate byte code to instructions
int cpu::decodeByte(unsigned char* code, int pc){
    pc++;

    return 1;

}


//loop
void cpu::cpu_loop(){

    bool isCpuRunning = false;

    const char filename[] = "cpu_instrs.gb";
    this->readFiletoBytes(filename);

    //main loop
    while(isCpuRunning){

        std::cout << "run \n";

    }

}


