#include "testtransthread.h"

TestTransThread::TestTransThread(QObject *parent) : QObject(parent)
{
     QTimer::singleShot(300,this,SLOT(initFunSLot()));
}


void TestTransThread::initFunSLot()
{
    mSerial = TestConfig::bulid()->item->serial;
}


int TestTransThread::sentStep(int i)
{

}
