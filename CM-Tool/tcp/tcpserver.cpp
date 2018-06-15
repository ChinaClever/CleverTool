#include "tcpserver.h"

TcpServer::TcpServer(QObject *parent) : QTcpServer(parent)
{
    isInconnected = false;
    int port = 12707;
    this->listen(QHostAddress::Any ,port);
}

/**
 * @brief 重写incomingConnection，服务器在接收到新连接时会触发
 * @param socketDescriptor
 */
void TcpServer::incomingConnection(int socketDescriptor)
{
    socket = new TcpSocket;
    connect(socket,SIGNAL(socketDisconnected()),this,SLOT(disconnected())); //如果socket断开，那么发送信号
    connect(socket,SIGNAL(dataComing(QByteArray)),this,SLOT(readData(QByteArray)));
    socket->setSocketDescriptor(socketDescriptor);
    qDebug()<<"连接成功";
    isInconnected = true;
}

void TcpServer::disconnected()
{
    isInconnected = false;
    qDebug()<<"服务器中socket断开";
}

void TcpServer::readData(QByteArray str)
{
//    qDebug()<<"服务器中的数据："<<str.size();
    mData = str;

}

QByteArray TcpServer::getData()
{
    QByteArray array = mData;
    mData.clear();
    return array;
}

bool TcpServer::isConnecting()
{
    return isInconnected;
}
