/*
 * tcpclient.cpp
 * UDP广播发送数据
 *      1、增加要发送的数据 udp_queue_append()
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "udpbdsent.h"

static QReadWriteLock  *gLock = new QReadWriteLock();
static QQueue<QByteArray> *gArrayQue = new QQueue<QByteArray>; // 需要发送的数据都会保存至此队列中

/**
 * @brief 往列队中增加数据
 * @param data
 */
void udp_queue_append(QByteArray &data)
{
    QWriteLocker locker(gLock);
    gArrayQue->enqueue(data);
}

/**
 * @brief 增加数据
 * @param buf 缓冲区
 * @param len 数据长度
 */
void udp_queue_append(uchar *buf, int len)
{
    QByteArray data;
    data.append((char *)buf, len);
    udp_queue_append(data);
}


UdpBDSent::UdpBDSent(QObject *parent) : QThread(parent)
{
    mLock = gLock;
    mArrayQue = gArrayQue;
    mSocket = new UdpSentSocket(this);

    timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    timer->start(500);
}


void UdpBDSent::timeoutDone(void)
{
//    start(); // 在定时器中启动线程
    run();
}


/**
 * @brief 发送数据
 */
void UdpBDSent::sentData(void)
{
    if(mArrayQue->size() > 0)
    {
        QWriteLocker locker(mLock);
        QByteArray data = mArrayQue->dequeue();

        mSocket->sentBroadcastData(data, UDP_SENT_PORT);
    }
}


void UdpBDSent::run(void)
{
    if(isRun != true) // 在运行就不执行
    {
        isRun = true;
        sentData();

        isRun = false;
    }
}
