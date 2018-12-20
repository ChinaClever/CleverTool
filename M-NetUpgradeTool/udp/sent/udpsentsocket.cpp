/*
 * udpsentsocket.cpp
 * UDP 发送套接字
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "udpsentsocket.h"




UdpSentSocket::UdpSentSocket(QObject *parent) : QObject(parent)
{
    mUdpSocket = NULL;
    sentLock = new QReadWriteLock();
    mUdpSocket = new QUdpSocket(this);
}


UdpSentSocket::~UdpSentSocket()
{
    delete mUdpSocket;
    delete sentLock;
}



bool UdpSentSocket::sentData(QHostAddress &addr,uchar *buf, ushort len)
{
    QByteArray byte;

    for(int i=0; i<len; ++i)
        byte.append(buf[i]);

    return sentData(addr,byte);
}

/**
 * @brief 发送点对点数据
 * @param msg 数据内容
 * @return true
 */
bool UdpSentSocket::sentData(QHostAddress &addr,QByteArray &msg)
{
    bool ret = true;

    QWriteLocker locker(sentLock);
    int len = mUdpSocket->writeDatagram(msg.data(),msg.size(),addr,UDP_SENT_PORT); // UDP发送端口18726
    if(len != msg.length())
    {
        ret = false;
        qDebug() << "UdpServer SentData Err" << msg;
    }

    return ret;
}

/**
 * @brief 发送广播数据
 * @param msg 数据内容
 * @return true
 */
bool UdpSentSocket::sentBroadcastData(uchar *buf, ushort len, quint16 port)
{
    QByteArray byte;

    for(int i=0; i<len; ++i)
        byte.append(buf[i]);

    return sentBroadcastData(byte,port);
}



/**
 * @brief 发送广播数据
 * @param msg 数据内容
 * @return true
 */
bool UdpSentSocket::sentBroadcastData(QByteArray &msg, quint16 port)
{
    bool ret = true;

    QWriteLocker locker(sentLock);
    int len = mUdpSocket->writeDatagram(msg.data(),msg.size(),QHostAddress::Broadcast,port);
    if(len != msg.length())
    {
        ret = false;
        qDebug() << "UdpServer SentData Err" << msg;
    }

    return ret;
}


void udp_sent_data(const QString &ip, uchar *buf, ushort len)
{
    UdpSentSocket udp;
    QHostAddress addr(ip);
    udp.sentData(addr, buf, len);
}
