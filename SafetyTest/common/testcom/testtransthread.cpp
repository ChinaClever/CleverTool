#include "testtransthread.h"
TestTransThread::TestTransThread(QObject *parent) : QObject(parent)
{
     QTimer::singleShot(300,this,SLOT(initFunSLot()));
}


void TestTransThread::initFunSLot()
{
    mSerial = TestConfig::bulid()->item->serial;
}

void Delay_MSec(unsigned int msec)
{
    QTime _Timer = QTime::currentTime().addMSecs(msec);

    while( QTime::currentTime() < _Timer )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

QString TestTransThread::sentStep(int step , int i , QString & command)
{
    QByteArray recv;
    if(step == 1)
        command = sentResisCommand(i);
    else if(step == 2)
        command = sentInsulationCommand(i);
    else if(step == 3)
        command = sentWithstandCommand(i);

    for(int count = 0 ; count < 3 ; count ++)
    {
        //if((step == 1 && i == ReadResisValue )||(step == 2 && i == ReadIRValue )||(step == 3 && i == ReadDCWValue ))
        //{
        if(recv.isEmpty())
            mSerial->transmit(command.toLatin1(),recv,10);
        else
            qDebug()<<"recv"<<recv;
        if(recv == command)
            break;
        //}
        //else
        //mSerial->write((uchar*)command.toLatin1().data(),command.length());
        Delay_MSec(1000);
    }
    qDebug()<<"step:"<<step<<"index:"<<i;
    return recv;
}

QString TestTransThread::sentResisCommand(int command)
{
    QString str;
    switch(command)
    {
        case ConnectReady:
            str = "*IDN?\r\n";
            break;
        case Reset:
            str = "FB\r\n";
            break;
        case EnterMemory:
            str = "S51\r\n";
            break;
        case EnterTestStep1:
            str = "S61\r\n";
            break;
        case EnterResisTest:
            str = "FF\r\n";
            break;
        case ResisSetOpCur:
            str = "SY25\r\n";
            break;
        case ResisSetOpVol:
            str = "SZ8\r\n";
            break;
        case ResisSetTestTime:
            str = "S210\r\n";
            break;
        case ResisSetVolUp:
            str = "S0100\r\n";
            break;
        case ResisSetVolDown:
            str = "S10\r\n";
            break;
        case ResisSetHz:
            str = "FP\r\n";
            break;
        case ResisSetScanner:
            str = "S30\r\n";
            break;
        case ResisSetOffset:
            str = "S424\r\n";
            break;
        case ResisCloseMemory:
            str = "FR\r\n";
            break;
        case ResisTest:
            str = "FA\r\n";
            break;
        case ReadResisValue:
            str = "?K\r\n";
            break;
    }
    return str ;
}

QString TestTransThread::sentInsulationCommand(int command)
{
    QString str;
    switch(command)
    {
        case ConnectReady:
            str = "*IDN?\r\n";
            break;
        case Reset:
            str = "FB\r\n";
            break;
        case EnterMemory:
            str = "S51\r\n";
            break;
        case EnterTestStep2:
            str = "S62\r\n";
            break;
        case EnterIRTest:
            str = "FE\r\n";
            break;
        case IRSetVol:
            str = "SS500\r\n";
            break;
        case IRSetChargeLo:
            str = "ST0\r\n";
            break;
        case IRSetHiLimit:
            str = "SU0\r\n";
            break;
        case IRSetLoLimit:
            str = "SV2\r\n";
            break;
        case IRSetDelayTime:
            str = "SW3\r\n";
            break;
        case IRCloseMemory:
            str = "FR\r\n";
            break;
        case IRTest:
            str = "FA\r\n";
            break;
        case ReadIRValue:
            str = "?K\r\n";
            break;
    }
    return str ;
}

QString TestTransThread::sentWithstandCommand(int command)
{
    QString str;
    switch(command)
    {
        case ConnectReady:
            str = "*IDN?\r\n";
            break;
        case Reset:
            str = "FB\r\n";
            break;
        case EnterMemory:
            str = "S51\r\n";
            break;
        case EnterTestStep3:
            str = "S63\r\n";
            break;
        case EnterDCWTest:
            str = "FD\r\n";
            break;
        case DCWSetVol:
            str = "SI2121\r\n";
            break;
        case DCWSetHiLimit:
            str = "SJ5000\r\n";
            break;
        case DCWSetLoLimit:
            str = "SK0\r\n";
            break;
        case DCWSetRampTime:
            str = "SL6\r\n";
            break;
        case DCWSetDwellTime:
            str = "SM3\r\n";
            break;
        case DCWSetChargeLo:
            str = "SO0\r\n";//SO零+回车OK的O加上1234567890的0
            break;
        case DCWRampHiOn:
            str = "F8\r\n";
            break;
        case DCWSetArcSense:
            str = "SP3\r\n";
            break;
        case DCWCloseArcSense:
            str = "FN\r\n";
            break;
        case DCWSetOffset:
            str = "SR0\r\n";
            break;
        case DCWCloseMemory:
            str = "FR\r\n";
            break;
        case DCWTest:
            str = "FA\r\n";
            break;
        case ReadDCWValue:
            str = "?K\r\n";
            break;

    }
    return str ;
}
