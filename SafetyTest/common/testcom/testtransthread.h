#ifndef TESTTRANSTHREAD_H
#define TESTTRANSTHREAD_H

#include "testresultwid.h"

class TestTransThread : public QObject
{
    Q_OBJECT
public:
    explicit TestTransThread(QObject *parent = nullptr);

    int sentStep(int i);
signals:

protected slots:
    void initFunSLot();

private:
    SerialPort *mSerial;
};

#endif // TESTTRANSTHREAD_H
