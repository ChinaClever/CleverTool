#include "tcpclient.h"

TcpClient::TcpClient(QHostAddress &addr, int &port, QObject *parent) : QObject(parent)
{


    mAddr = addr;
    mPort = port;
    isConnecting = false;

    tcpsocket = new QTcpSocket;
    connect(tcpsocket,SIGNAL(connected()),this,SLOT(socketConnected()));
    connect(tcpsocket,SIGNAL(disconnected()),this,SLOT(socketDisconnected()));
    connect(tcpsocket,SIGNAL(readyRead()),this,SLOT(readData()));
}

void TcpClient::createConnect()
{
    tcpsocket->connectToHost(mAddr,mPort);
}

void TcpClient::socketConnected()
{
    isConnecting = true;
    qDebug() << "建立连接：" << mAddr << mPort;
}

void TcpClient::socketDisconnected()
{
    isConnecting = false;
    qDebug() << "断开连接：" << mAddr << mPort;
}

void TcpClient::breakConnect()
{
    tcpsocket->close();
}

void TcpClient::writeData(QByteArray array)
{
    tcpsocket->write(array.data(),array.size());
}

void TcpClient::readData()
{
    if(tcpsocket->bytesAvailable() > 0)
    {
        QByteArray array;
        array.resize(tcpsocket->bytesAvailable());
        tcpsocket->read(array.data(),array.size());
        emit receiveTcpData(array);
    }
}

/**
 * @brief 获取连接状态
 * @return
 */
bool TcpClient::getConnectingStatus()
{
    return isConnecting;
}


