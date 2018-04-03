#include "tcpthread.h"

TcpThread::TcpThread()
{
    is_run = false;
    server = new TcpServer;

    QTimer *timer = new QTimer;
    connect(timer,SIGNAL(timeout()),this,SLOT(timeoutdone()));
    timer->start(8*1000);
}

void TcpThread::run()
{
    is_run = true;
    if(server->isConnecting())
    {
        data = server->getData();  //读取数据改变阈值

        if(!data.isEmpty())
            get_data_packet(data);

//        qDebug()<<"线程中的数据："<< data.size();
        data.clear();
//        qDebug() <<"清理数据";

    }
    is_run = false;

}


void TcpThread::timeoutdone()
{
    if(!is_run)
    {
        start();

    }
}

