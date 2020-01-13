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
    Q_PROPERTY(QString regSet1 READ regSet1 NOTIFY regValsChanged)
    Q_PROPERTY(QString regSet2 READ regSet2 NOTIFY regValsChanged)
    Q_PROPERTY(QString regVid READ regVid NOTIFY regValsChanged)

    Q_PROPERTY(QString memory READ memory NOTIFY memoryChanged)
    Q_PROPERTY(QString flags READ flags NOTIFY flagsChanged)
    Q_PROPERTY(QString vidMemory READ vidMemory NOTIFY vidMemChanged)
    Q_PROPERTY(QString opcode READ opcode NOTIFY opcodeChanged)


    //debugging tools
    Q_PROPERTY(bool debug READ debug WRITE enterDebug NOTIFY debugChanged)
    Q_PROPERTY(bool isWait READ isWait WRITE nextOpcode NOTIFY waitChanged)
    Q_PROPERTY(QString pcInput READ pcInput WRITE pcInputWrite NOTIFY pcInputChanged)


public:
    explicit Interface(QObject *parent = nullptr);

    QString regVals();
    QString regSet1();
    QString regSet2();
    QString regVid();
    QString flags();

    QString opcode();

    QString memory();
    QString vidMemory();

    //debugging tools
    bool debug();
    void enterDebug(const bool &);
    bool isWait();
    void nextOpcode(const bool &);
    QString pcInput();
    void pcInputWrite(const QString &input);

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
    void pcInputChanged();

private:
    CPU *cpu;
    graphics *gpu;

    QString s_opcode = "";
    QString s_pcInput = "0";

    QString s_memory = "0000: 00";
    QString s_videoMem = "0000: 00";
    QString s_flags = "s_flags";

    QString s_regSet1;
    QString s_regSet2;
    QString s_regVid;

    QImage image;


};

#endif // INTERFACE_H
