#include "globalreceivethread.h"

GlobalReceiveThread::GlobalReceiveThread()
{
    isRunning = false;
    mSocket = new HeartBeat(NULL,18750);
    QTimer *timer = new QTimer;
    connect(timer,SIGNAL(timeout()),this,SLOT(timeOutDone()));
    timer->start(5*1000);
}

GlobalReceiveThread::~GlobalReceiveThread()
{
    wait();
}

void GlobalReceiveThread::run()
{

    QByteArray array = mSocket->getArray();

    if(!array.isEmpty())
        get_data_packet(array);

//    if(array.size() > 0)
//    {
//        qDebug() << "接收到数据" <<array.size();
//    }
//    else
//        qDebug() <<"未接收到数据";

    isRunning = false;
}

void GlobalReceiveThread::timeOutDone()
{
    if(!isRunning)
    {
        isRunning = true;
        start();
    }
}
