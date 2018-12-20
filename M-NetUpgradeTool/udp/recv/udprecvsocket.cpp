/*
 * UdpRecvSocket.cpp
 * UDP 接收套接字
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "udprecvsocket.h"


/**
 * @brief 构造函数
 * @param parent
 */
UdpRecvSocket::UdpRecvSocket(QObject *parent) : QThread(parent)
{    
    isRun=false;
    mUdpSocket = NULL;
    mRecLock = new QReadWriteLock();
    mUdpQueueData = new QQueue<UdpBaseData*>();
}

UdpRecvSocket::~UdpRecvSocket()
{
    isRun=false;
    wait();

    delete mUdpSocket;
    delete mRecLock;
    delete mUdpQueueData;
}

/**
 * @brief 初始化套接字
 * @return 端口号
 */
bool UdpRecvSocket::initSocket(int port)
{
    mUdpSocket = new QUdpSocket(this);
    bool ret = mUdpSocket->bind(port  /*, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint*/); //此处的bind是个重载函数，连接本机的port端口，采用ShareAddress模式(即允许其它的服务连接到相同的地址和端口，特别是 用在多客户端监听同一个服务器端口等时特别有效)，和ReuseAddressHint模式(重新连接服务器)
    if(ret) {
        // connect(g_udpSocket,SIGNAL(readyRead()),this,SLOT(dataReceivedSlot())); // 数据接收
        start(); // 启动接收线程
    }
    else
        qDebug() << "udp socket create err" << port;

    return ret;
}



/**
 * @brief 获取UDP数据包
 * @param data
 * @return
 */
UdpBaseData *UdpRecvSocket::getData(void)
{
    UdpBaseData *data = NULL;

    QReadLocker locker(mRecLock); /*获取线程状态*/
    int ret = mUdpQueueData->size();
    if( ret > 0)
        data = mUdpQueueData->dequeue();

    return data;
}




/**
 * @brief 读取socket数据
 * @return
 */
bool UdpRecvSocket::dataReceived(void)
{   
    if(mUdpSocket->hasPendingDatagrams()) // 是否有数据可读
    {
        UdpBaseData *data = new UdpBaseData();
        if(data) {
            data->datagram.resize(mUdpSocket->pendingDatagramSize()); //让datagram 的大小为等待处理的数据报的大小，这样才能接收到完整的数据

            int rtn = mUdpSocket->readDatagram(data->datagram.data(),
                                               data->datagram.size(),
                                               &data->addr,
                                               &data->port);
            if(rtn > 0)
            {
                if(data->datagram.size() < 1024) /*数据最长不超过1024*/
                {
                    QWriteLocker locker(mRecLock);
                    mUdpQueueData->enqueue(data);
                }
            }
        } else {
            qDebug() << "new UdpBaseData err";
        }
    }

    return mUdpSocket->hasPendingDatagrams();
}


void UdpRecvSocket::run(void)
{
    bool ret = true;

    isRun = true;
    while(isRun)
    {
        ret = dataReceived();
        if(ret == false)
            usleep(1);
    }
}
