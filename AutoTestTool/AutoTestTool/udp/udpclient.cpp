#include "udpclient.h"

static UdpClient *client = NULL;
UdpClient *getUdpClient()
{
    if(client == NULL)
        client =  new UdpClient(NULL,40050);

    return client;

}

UdpClient::UdpClient(QObject *parent, int port) : QObject(parent)
{
    mAddr = new QHostAddress;
    mServerPort = new quint16;
    mPort = port;
    udpsocket = new QUdpSocket;
    QTimer *timer = new QTimer;
    connect(timer,SIGNAL(timeout()),this,SLOT(timeoutDone()));
    //    timer->start(3*1000);

    connect(udpsocket,SIGNAL(readyRead()),this,SLOT(readData()));
}

void UdpClient::timeoutDone()
{
    QString str("auto test tool!");
    if(udpsocket->writeDatagram(str.toLatin1(),str.length(),QHostAddress::Broadcast,mPort) != str.length())
    {
        qDebug() << "数据传输不完整";
    }else
        qDebug() << "心跳包发送成功！";
}

/**
 * @brief 读取数据
 */
void UdpClient::readData()
{
    if(udpsocket->hasPendingDatagrams())
    {
        mArray.clear();
        mArray.resize(udpsocket->pendingDatagramSize());
        if(mArray.size() == udpsocket->readDatagram(mArray.data(),mArray.size(),mAddr,mServerPort))
        {
            //            qDebug() << "IP:" << mAddr->toString() << "PORT:" << mPort;
            emit receiveData(mArray,mAddr);

        }else
            qDebug() << "数据不完整！";

    }
}

/**
 * @brief 获取该端口返回的数据
 * @return
 */
QByteArray UdpClient::getUdpBackData()
{
    QByteArray array = mArray;
    mArray.clear();
    return mArray;
}

/**
 * @brief 获取心跳包地址
 */
QHostAddress UdpClient::getAddr()
{
    return *mAddr;
}

/**
 * @brief 获取心跳包端口
 */
quint16 UdpClient::getPort()
{
    return *mServerPort;
}

void UdpClient::sendHeartBeart()
{
    QString str("auto test tool!");
    if(udpsocket->writeDatagram(str.toLatin1(),str.length(),QHostAddress::Broadcast,mPort) != str.length())
    {
        qDebug() << "数据传输不完整";
    }else
        qDebug() << "心跳包发送成功！";
}
