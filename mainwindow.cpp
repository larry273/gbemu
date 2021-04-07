#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    cpu = new CPU;
    cpu->start();

    //gpu = cpu->gpu;

    image = QImage(160,144,QImage::Format_RGB32);
    image.fill(QColor("black"));

    //connect cpu value changes
    connect(cpu, &CPU::regValChanged, this, &MainWindow::updateRegValues);
    connect(cpu, &CPU::memoryChanged, this, &MainWindow::updateMemory);
    connect(cpu, &CPU::vidMemChanged, this, &MainWindow::updateVideoMemory);
    connect(cpu, &CPU::flagsChanged, this, &MainWindow::updateFlags);
    connect(cpu, &CPU::opcodeChanged, this, &MainWindow::updateOpcode);

    //connect(gpu, &graphics::frameCompleted, this, &MainWindow::updateFrame);
}

void MainWindow::updateRegValues(){
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
    //emit regValsChanged();
}

void MainWindow::updateMemory(){
    s_memory = cpu->getMemory();
    //emit memoryChanged();
}
void MainWindow::updateVideoMemory(){
    s_videoMem = cpu->getVidMemory();
    //emit vidMemChanged();
}
void MainWindow::updateFlags(){
    s_flags = cpu->getFlags();
    //emit flagsChanged();
}

void MainWindow::updateFrame(){
    image = gpu->sendFrame();
    //emit imageChanged(image);
}
void MainWindow::updateOpcode(){
    s_opcode = cpu->getOpcode() + s_opcode;
    //emit opcodeChanged();
}

//debugging tools
bool MainWindow::debug(){
    return cpu->debug;
}
bool MainWindow::isWait(){
    return cpu->nextOpCode;
}
void MainWindow::enterDebug(const bool &){
    cpu->debug = !cpu->debug;

    if (!cpu->debug){
        cpu->nextOpCode = false;
    }
    else{
        cpu->nextOpCode = true;
    }
    emit debugChanged();
}
QString MainWindow::pcInput(){
    return s_pcInput;
}
void MainWindow::pcInputWrite(const QString &input){
    s_pcInput = input;
    bool ok;
    cpu->pcStop = input.toInt(&ok, 16);
    //emit pcInputChanged();
}

void MainWindow::nextOpcode(const bool &){
    cpu->nextOpCode = !cpu->nextOpCode;
    //emit waitChanged();
}

QString MainWindow::opcode(){
    return s_opcode;
}

QString MainWindow::flags(){
    return s_flags;
}
QString MainWindow::memory(){
    return s_memory;
}
QString MainWindow::vidMemory(){
    return s_videoMem;
}
QString MainWindow::regSet1(){
    return s_regSet1;
}
QString MainWindow::regSet2(){
    return s_regSet2;
}
QString MainWindow::regVid(){
    return s_regVid;
}

MainWindow::~MainWindow()
{
    delete ui;
}

