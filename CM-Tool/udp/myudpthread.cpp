#include "myudpthread.h"

MyUdpThread::MyUdpThread()
{
    is_run = false;

    heartbeat = get_HeartBeat();
    souce = get_datasouce();
    qDebug()<<"MyUdpThread";

    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timeoutdone()));
    timer->start(2*1000); //每5s唤醒一次线程
}

MyUdpThread::~MyUdpThread()
{
    wait();
}

void MyUdpThread::run()
{
//    qDebug()<<"线程进入success";

    if(heartbeat ->getData() == "Clever-Manager PDU PC Server OK!")
    {
//        qDebug()<<"心跳包正确！";
//        qDebug()<<"IP:"<<heartbeat->getAddr().toString();

        souce->renewalSouceData();//刷新随机数据
        souceData_To_protcldata();
        heartbeat->clearData();
    }

    is_run = false;
}

void MyUdpThread::timeoutdone()
{
    if(!is_run)
    {
        is_run = true;
        start();
    }
}
