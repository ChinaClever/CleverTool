#include "tcpsocket.h"

TcpSocket::TcpSocket(QObject *parent) : QTcpSocket(parent)
{

    connect(this,SIGNAL(readyRead()),this,SLOT(dataReceived()));
    connect(this,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));
}

void TcpSocket::dataReceived()
{
    int len = this->bytesAvailable();
    if(len)
    {
        char buf[BUFSIZ];
        memset(buf,0,BUFSIZ);
        this->read(buf,len);

        QByteArray array(buf,len);
        qDebug()<<"收到信息："<<array.size()<<len;
        emit dataComing(array);
    }

}

void TcpSocket::slotDisconnected()
{
    //    qDebug()<<"连接已断开";
    emit socketDisconnected();
}

QString TcpSocket::getData()
{
    return str;
}
