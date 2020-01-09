#ifndef INTERFACE_H
#define INTERFACE_H

#include <QString>
#include <QObject>
#include <qdebug.h>
#include <QImage>

#include "cpu.h"
#include "graphics.h"

class Interface : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString regVals READ regVals NOTIFY regValsChanged)
    Q_PROPERTY(QString regAF READ regAF NOTIFY regValsChanged)
    Q_PROPERTY(QString regBC READ regBC NOTIFY regValsChanged)
    Q_PROPERTY(QString regDE READ regDE NOTIFY regValsChanged)
    Q_PROPERTY(QString regHL READ regHL NOTIFY regValsChanged)
    Q_PROPERTY(QString regSP READ regSP NOTIFY regValsChanged)
    Q_PROPERTY(QString regPC READ regPC NOTIFY regValsChanged)
    Q_PROPERTY(QString memory READ memory NOTIFY memoryChanged)
    Q_PROPERTY(QString flags READ flags NOTIFY flagsChanged)
    Q_PROPERTY(QString vidMemory READ vidMemory NOTIFY vidMemChanged)
    Q_PROPERTY(QString opcode READ opcode NOTIFY opcodeChanged)

    //debugging
    Q_PROPERTY(bool debug READ debug WRITE enterDebug NOTIFY debugChanged)
    Q_PROPERTY(bool isWait READ isWait WRITE nextOpcode NOTIFY waitChanged)

public:
    explicit Interface(QObject *parent = nullptr);

    QString regVals();
    QString regAF();
    QString regBC();
    QString regDE();
    QString regHL();
    QString regSP();
    QString regPC();
    QString flags();

    QString opcode();

    QString memory();
    QString vidMemory();

    //debugging tools
    bool debug();
    void enterDebug(const bool &);
    bool isWait();
    void nextOpcode(const bool &);

    Q_INVOKABLE void test();
    void updateRegValues();
    void updateMemory();
    void updateVideoMemory();
    void updateFrame();
    void updateFlags();
    void updateOpcode();

Q_SIGNALS:
    void regValsChanged();
    void memoryChanged();
    void vidMemChanged();
    void flagsChanged();
    void imageChanged(const QImage &);
    void opcodeChanged();

    void debugChanged();
    void waitChanged();

private:
    CPU *cpu;
    graphics *gpu;

    QString s_opcode = "N/A";

    QString s_regVals = "ducks";
    QString s_memory = "0000: 00";
    QString s_videoMem = "0000: 00";
    QString s_flags = "s_flags";

    QString s_regAF = "0x0000";
    QString s_regBC = "0x0000";
    QString s_regDE = "0x0000";
    QString s_regHL = "0x0000";
    QString s_regSP = "0x0000";
    QString s_regPC = "0x0000";

    QImage image;


};

#endif // INTERFACE_H
