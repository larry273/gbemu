#include "interface.h"

#include <QQmlApplicationEngine>


Interface::Interface(QObject *parent) : QObject(parent){
    cpu = new CPU;
    cpu->start();

    gpu = cpu->gpu;
    //gpu = new graphics(&cpu->mem);

    image = QImage(160,144,QImage::Format_RGB32);
    image.fill(QColor("black"));

    //connect cpu value changes
    connect(cpu, &CPU::regValChanged, this, &Interface::updateRegValues);
    connect(cpu, &CPU::memoryChanged, this, &Interface::updateMemory);
    connect(cpu, &CPU::vidMemChanged, this, &Interface::updateVideoMemory);
    connect(cpu, &CPU::flagsChanged, this, &Interface::updateFlags);
    connect(cpu, &CPU::opcodeChanged, this, &Interface::updateOpcode);


    //connect(cpu, &CPU::frameChanged, this, &Interface::updateFrame);
    connect(gpu, &graphics::frameCompleted, this, &Interface::updateFrame);

}

void Interface::updateRegValues(){
    std::vector<int> values = cpu->getRegValues();
    s_regAF = "0x" + QString::number( values[0], 16).rightJustified(4, '0');
    s_regBC = "0x" + QString::number( values[1], 16).rightJustified(4, '0');
    s_regDE = "0x" + QString::number( values[2], 16).rightJustified(4, '0');
    s_regHL = "0x" + QString::number( values[3], 16).rightJustified(4, '0');
    s_regPC = "0x" + QString::number( values[4], 16).rightJustified(4, '0');
    s_regSP = "0x" + QString::number( values[5], 16).rightJustified(4, '0');
    emit regValsChanged();
}

void Interface::updateMemory(){
    s_memory = cpu->getMemory();
    emit memoryChanged();
}
void Interface::updateVideoMemory(){
    s_videoMem = cpu->getVidMemory();
    emit vidMemChanged();
}
void Interface::updateFlags(){
    s_flags = cpu->getFlags();
    emit flagsChanged();
}

void Interface::updateFrame(){
    image = gpu->sendFrame();
    emit imageChanged(image);
}
void Interface::updateOpcode(){
    s_opcode = cpu->getOpcode();
    emit opcodeChanged();
}

//debugging tools
bool Interface::debug(){
    return cpu->debug;
}
bool Interface::isWait(){
    return cpu->nextOpCode;
}
void Interface::enterDebug(const bool &){
    cpu->debug = !cpu->debug;

    if (!cpu->debug){
        cpu->nextOpCode = false;
    }
    else{
        cpu->nextOpCode = true;
    }
    emit debugChanged();
}
void Interface::nextOpcode(const bool &){
    cpu->nextOpCode = !cpu->nextOpCode;
    emit waitChanged();
}

QString Interface::opcode(){
    return s_opcode;
}

QString Interface::flags(){
    return s_flags;
}
QString Interface::memory(){
    return s_memory;
}
QString Interface::vidMemory(){
    return s_videoMem;
}
QString Interface::regVals(){
    return s_regVals;
}
QString Interface::regAF(){
    return s_regAF;
}
QString Interface::regBC(){
    return s_regBC;
}
QString Interface::regDE(){
    return s_regDE;
}
QString Interface::regHL(){
    return s_regHL;
}
QString Interface::regPC(){
    return s_regPC;
}
QString Interface::regSP(){
    return s_regSP;
}

void Interface::test(){
    s_regVals += " and more ducks";
    emit regValsChanged();
}
