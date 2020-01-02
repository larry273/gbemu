#include "interface.h"

Interface::Interface(QObject *parent) : QObject(parent){
    cpu = new CPU;
    cpu->start();
    //connect cpu value changes
    connect(cpu, &CPU::regValChanged, this, &Interface::updateRegValues);
    connect(cpu, &CPU::memoryChanged, this, &Interface::updateMemory);


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

QString Interface::memory(){
    return s_memory;
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
    //qDebug() << "test function \n";
    s_regVals += " more ducks";
    emit regValsChanged();
}
