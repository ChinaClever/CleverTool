/*****
 * 1.实现udp服务器端接收收心跳包
 * 2.提供udp协议发送数据的接口sendData();
 */
#include "heartbeat.h"


static HeartBeat *heartbeat =NULL;

HeartBeat *get_HeartBeat()
{
    if(!heartbeat)
    {
        heartbeat = new HeartBeat(NULL,18725);
        //        qDebug()<<"get_HeartBeat";
    }
    return  heartbeat;
}





HeartBeat::HeartBeat(QWidget *parent, int port) : QObject(parent)
{
    mUdpScoket = new QUdpSocket(this);
    mAddr = new QHostAddress;
    mPort = new quint16;

    qDebug()<<"绑定的端口号："<<port;
    bool ret = mUdpScoket->bind(port); //只能绑定18725才能收到心跳包
    if(!ret)
    {
        //        QMessageBox::warning(this,tr("警告"),tr("绑定端口失败"),tr("确定"));
        qDebug()<<"绑定端口失败！";
    }
    connect(mUdpScoket,SIGNAL(readyRead()),this,SLOT(dataReceived()));
}

/**
 * @brief 当有数据到来时，接收数据
 */
void HeartBeat::dataReceived()
{

    initData();
    while(mUdpScoket->hasPendingDatagrams())
    {

        array.resize(mUdpScoket->pendingDatagramSize());
        //        qDebug() <<array.size();

        quint64 number  = mUdpScoket->readDatagram(array.data(),array.size(),mAddr,mPort); //maddr和port存储接收到的心跳包心跳包IP

        data = array.data();
        emit receiveMessage(data);
        //        data = QString(array.data(),)
    }
}

/**
 * @brief 获取心跳包地址
 */
QHostAddress HeartBeat::getAddr()
{
    return *mAddr;
}

/**
 * @brief 获取心跳包端口
 */
quint16 HeartBeat::getPort()
{
    return *mPort;
}

/**
 * @brief 每次接收心跳包时先清空数据
 */
void HeartBeat::initData()
{
    *mAddr = 0;
    *mPort = 0;
    data.clear();
}

QString HeartBeat::getData()
{
    QString str = data;
    data.clear();
    return str;
}

QByteArray HeartBeat::getArray()
{
    QByteArray str;
    str = array;
    array.clear();
    return str;
}

bool HeartBeat::clearData()
{
    data.clear();
    if(data.isEmpty())
        return true;
    else
        return false;
}


/**
 * @brief 传入QByteArray，将数据发送出去
 */
void HeartBeat::sendData(QByteArray &array)
{

    QHostAddress addr = *mAddr; //心跳包地址
    //        qDebug()<<"*mAddr:"<<*mAddr;
    //    QHostAddress addr("192.168.1.112");

    mUdpScoket->writeDatagram(array,addr,28720); //向接收到心跳包的ip指定端口发送数据
}



