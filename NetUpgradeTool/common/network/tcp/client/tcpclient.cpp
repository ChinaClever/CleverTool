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
    isConnect = false;
    mServerIP = new QHostAddress();

    mTcpSocket = new QTcpSocket(this);
    connect(mTcpSocket, SIGNAL(connected()), this,SLOT(connectedSlot()));
    connect(mTcpSocket, SIGNAL(disconnected()), this,SLOT(disconnectedSlot()));
    connect(mTcpSocket,SIGNAL(readyRead()),this,SLOT(readMessageSlot()));
    connect(mTcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),
            this,SLOT(displayError(QAbstractSocket::SocketError)));
}

TcpClient::~TcpClient()
{
    delete mServerIP;

    if(mTcpSocket)
        closeConnect();
}


/**
 * @brief 关闭连接
 */
void TcpClient::closeConnect(void)
{
    if(isConnect)
    {
        isConnect = false;
        mTcpSocket->abort(); //取消已有的连接
        mSentData.clear();
    }
}

/**
 * @brief 建立新的连接
 * @param host 目标主机
 * @return true
 */
bool TcpClient::newConnect(const QString &host, int port)
{
    QHostAddress address(host);
    if((*mServerIP == address) && (isConnect)) return true;

    bool ret = mServerIP->setAddress(host);
    if(ret)
    {
        if(isConnect)
            closeConnect(); //先关闭

        mTcpSocket->connectToHost(*mServerIP,port);//连接到主机，机地址和端口号
        mSentData.clear();
    }
    else
        qDebug() << "newConnect set ip err" << host;

    return ret;
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
    mSentData.append(data);
    QTimer::singleShot(1,this,SLOT(timeoutDone()));

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
        if(mTcpSocket->isWritable())
        {

            rtn = mTcpSocket->write(data);
            if(rtn == data.size()) {
                mTcpSocket->waitForBytesWritten();
            } else {
                 emit connectSig(UP_CMD_ERR);
                emit sentErr(mServerIP->toString());
            }
        }
    }
    data.clear();

    return rtn;
}


void TcpClient::timeoutDone(void)
{
    if(isConnect && mSentData.size()) {
        QByteArray data = mSentData.first();
        writeMessage(data);
        mSentData.removeFirst();
    }
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
    while(mTcpSocket->bytesAvailable() > 0)
    {
        QByteArray datagram;
        datagram.resize(mTcpSocket->bytesAvailable());
        mTcpSocket->read(datagram.data(), datagram.size());
        mRecvData = datagram;
        emit connectSig(UP_CMD_READ);

        // qDebug() << datagram.data();
    }
}

/**
 * @brief 出现异常
 */
void TcpClient::displayError(QAbstractSocket::SocketError)
{
    closeConnect();
    emit sentErr(mServerIP->toString());
    emit connectSig(UP_CMD_ERR); // 连接错误
    //    qDebug() << mTcpSocket->errorString(); //输出错误信息
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
