/*
 * tcpclient.cpp
 * Tcp客户端 主要用于配置PDU设备
 *      1、与设备连接
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "tcpclient.h"

static bool isConnect = false;

/**
 * @brief 获取TCP连接状态
 * @return
 */
bool get_tcp_connect(void)
{
    return isConnect;
}

TcpClient::TcpClient(QObject *parent) : QObject(parent)
{
    isNew = false;
    isConnect = false;
    isOver = false;

    mTcpSocket = new QTcpSocket(this);
    connect(mTcpSocket, SIGNAL(connected()), this,SLOT(connectedSlot()));
    connect(mTcpSocket, SIGNAL(disconnected()), this,SLOT(disconnectedSlot()));
    connect(mTcpSocket,SIGNAL(readyRead()),this,SLOT(readMessageSlot()));
    connect(mTcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),
            this,SLOT(displayError(QAbstractSocket::SocketError)));

    mLock = new QReadWriteLock();
    timer = new QTimer(this);
    timer->start(10);
    connect(timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
}

TcpClient::~TcpClient()
{
    if(mTcpSocket) closeConnect();
}


/**
 * @brief 关闭连接
 */
void TcpClient::closeConnect(void)
{
    if(isConnect)
    {
        QReadLocker locker(mLock);
        mSentData.clear();
        isConnect = false;
        mTcpSocket->close(); //取消已有的连接
    }
}

void TcpClient::newConnect(const QString &host, int port)
{
    if((mServerIP != host) || (!isConnect))
    {
        isNew = true;
        mServerIP = host;
        isOver = isConnect = false;
    }
}

/**
 * @brief 建立新的连接
 * @param host 目标主机
 * @return true
 */
void TcpClient::newConnectSlot()
{
    if(!mServerIP.isEmpty() && isNew) {
        if(isConnect) closeConnect(); //先关闭
        QHostAddress host(mServerIP);
        mTcpSocket->connectToHost(host,TCP_PORT);//连接到主机，机地址和端口号
        mSentData.clear();
        isNew = false;
    }
}

/**
 * @brief 发送数据
 * @param buf 数据缓冲区
 * @param len 数据长度
 * @return
 */
bool TcpClient::sentMessage(uchar *buf,  int len)
{
    QByteArray data;
    data.append((char *)buf, len);

    return sentMessage(data);
}

/**
 * @brief 发送数据
 * @param data
 * @return true
 */
bool TcpClient::sentMessage(const QByteArray &data)
{   
    QReadLocker locker(mLock); /*获取线程状态*/
    mSentData.append(data);

    return isConnect;
}

/**
 * @brief 发送数据
 * @param data
 * @param len
 * @return -1 出错
 */
int TcpClient::writeMessage(char *buf, int len)
{
    QByteArray data;
    data.append((char *)buf, len);

    return writeMessage(data);
}

/**
 * @brief 发送数据
 * @param data
 * @return -1 出错
 */
int TcpClient::writeMessage(QByteArray &data)
{
    int rtn = -1;

    if(isConnect)
    {
        QReadLocker locker(mLock); /*获取线程状态*/
        if(mTcpSocket->isWritable())
        {
            rtn = mTcpSocket->write(data);
            if(rtn != data.size()) {
                 emit connectSig(UP_CMD_ERR);
            }
            mTcpSocket->flush();
            mTcpSocket->waitForBytesWritten();
        }
        qDebug() << "BBBBBBBB" << rtn;
    }

    return rtn;
}


void TcpClient::timeoutDone(void)
{
    if(isConnect && mSentData.size()) {
        QReadLocker locker(mLock); /*获取线程状态*/
        QByteArray data = mSentData.first();
        writeMessage(data);
        mSentData.removeFirst();
    } else {
        newConnectSlot();
    }

    if(isOver) closeConnect();
}

/**
 * @brief 已连接
 */
void TcpClient::connectedSlot()
{
    isConnect = true;
    timeoutDone();
    emit connectSig(UP_CMD_CONNECT); // 连接成功
}


/**
 * @brief 连接断开
 */
void TcpClient::disconnectedSlot()
{
    closeConnect();
    emit disconnectedSig();
    emit connectSig(UP_CMD_DIS);  // 断开连接
}

/**
 * @brief 接收到数据
 */
void TcpClient::readMessageSlot(void)
{
    int ret = 0;
    QReadLocker locker(mLock); /*获取线程状态*/
    while(mTcpSocket->bytesAvailable() > 0)
    {
        QByteArray datagram;
        datagram.resize(mTcpSocket->bytesAvailable());
        ret += mTcpSocket->read(datagram.data(), datagram.size());
        mRecvData = datagram;

        qDebug() << "readMessageSlot" << datagram.data();
    }

    if(ret)
        emit connectSig(UP_CMD_READ);
}

/**
 * @brief 出现异常
 */
void TcpClient::displayError(QAbstractSocket::SocketError)
{
    closeConnect();
    emit connectSig(UP_CMD_ERR); // 连接错误
}

/**
 * @brief 获取发送状态
 * @return  true
 */
bool TcpClient::getSentStatue(void)
{
    return mSentData.isEmpty();
}

int TcpClient::getData(QByteArray &data)
{
    data = mRecvData;
    return mRecvData.size();
}
