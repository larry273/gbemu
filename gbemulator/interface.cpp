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
    s_regSet1 = "0x" + QString::number( values[0], 16).rightJustified(4, '0') + "\n"
                "0x" + QString::number( values[1], 16).rightJustified(4, '0') + "\n"
                "0x" + QString::number( values[2], 16).rightJustified(4, '0');

    s_regSet2 = "0x" + QString::number( values[3], 16).rightJustified(4, '0') + "\n"
                "0x" + QString::number( values[4], 16).rightJustified(4, '0') + "\n"
                "0x" + QString::number( values[5], 16).rightJustified(4, '0');

    //video registers
    s_regVid =  "0x" + QString::number( values[6], 16).rightJustified(2, '0') + "\n"
                "0x" + QString::number( values[7], 16).rightJustified(2, '0') + "\n"
                "0x" + QString::number( values[8], 16).rightJustified(2, '0') + "\n";
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
    s_opcode = cpu->getOpcode() + s_opcode;
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
QString Interface::pcInput(){
    return s_pcInput;
}
void Interface::pcInputWrite(const QString &input){
    s_pcInput = input;
    bool ok;
    cpu->pcStop = input.toInt(&ok, 16);
    emit pcInputChanged();
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
QString Interface::regSet1(){
    return s_regSet1;
}
QString Interface::regSet2(){
    return s_regSet2;
}
QString Interface::regVid(){
    return s_regVid;
}


