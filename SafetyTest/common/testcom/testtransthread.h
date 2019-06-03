#ifndef TESTTRANSTHREAD_H
#define TESTTRANSTHREAD_H

#include "testresultwid.h"

enum TestResisEnum {
    ConnectReady = 1,//1
    Reset = 3,       //3
    EnterMemory = 5, //5
    EnterTestStep1 = 7, //7
    EnterTestStep2 = 7,
    EnterTestStep3 = 7,

    EnterResisTest = 8, //8
    EnterIRTest = 8, //8
    EnterDCWTest = 8,

    ResisSetOpCur = 9,      //9
    IRSetVol = 9,
    DCWSetVol = 9,

    ResisSetOpVol = 10,      //10
    IRSetChargeLo =10,
    DCWSetHiLimit = 10,

    ResisSetTestTime = 11,     //11
    IRSetHiLimit = 11,
    DCWSetLoLimit = 11,

    ResisSetVolUp = 12,       //12
    IRSetLoLimit = 12,
    DCWSetRampTime = 12,

    ResisSetVolDown = 13,     //13
    IRSetDelayTime = 13,
    DCWSetDwellTime = 13,

    ResisSetHz = 14,          //14
    IRCloseMemory = 14,

    ResisSetScanner = 15,     //15
    IRTest = 15,
    DCWSetChargeLo = 15,

    ResisSetOffset = 16,     //16
    DCWRampHiOn = 16,


    ResisCloseMemory = 17,     //17
    ReadIRValue = 17,
    DCWSetArcSense = 17,

    ResisTest = 18,     //18
    DCWCloseArcSense = 18,

    DCWSetOffset = 19,

    ReadResisValue = 20,
    DCWCloseMemory = 20, //20

    DCWTest = 21,

    ReadDCWValue = 23,
};

class TestTransThread : public QObject
{
    Q_OBJECT
public:
    explicit TestTransThread(QObject *parent = nullptr);

    QString sentStep(int step, int i ,QString & command);
    QString sentResisCommand(int command);
    QString sentInsulationCommand(int command);
    QString sentWithstandCommand(int command);

signals:

protected slots:
    void initFunSLot();

private:
    SerialPort *mSerial;
};

#endif // TESTTRANSTHREAD_H
