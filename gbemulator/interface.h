#ifndef INTERFACE_H
#define INTERFACE_H

#include <QString>
#include <QObject>
#include <qdebug.h>

#include "cpu.h"

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


public:
    explicit Interface(QObject *parent = nullptr);

    QString regVals();

    QString regAF();
    QString regBC();
    QString regDE();
    QString regHL();
    QString regSP();
    QString regPC();

    QString memory();

    QString currentInstru;

    Q_INVOKABLE void test();
    void updateRegValues();
    void updateMemory();

private:
    CPU *cpu;

    QString s_regVals = "ducks";
    QString s_memory = "0000: 12 34";

    int flagZero;
    int flagCarry;
    int flagSub;
    int flagHalf;

    QString s_regAF = "0x0000";
    QString s_regBC = "0x0000";
    QString s_regDE = "0x0000";
    QString s_regHL = "0x0000";
    QString s_regSP = "0x0000";
    QString s_regPC = "0x0000";

Q_SIGNALS:
    void regValsChanged();
    void memoryChanged();

};

#endif // INTERFACE_H
