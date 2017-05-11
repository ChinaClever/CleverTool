#include "tcpclient.h"

TcpClient::TcpClient(QObject *parent) : QObject(parent)
{
    isConnecting = false;

    tcpsocket = new QTcpSocket;
    connect(tcpsocket,SIGNAL(connected()),this,SLOT(socketConnected()));
    connect(tcpsocket,SIGNAL(disconnected()),this,SLOT(socketDisconnected()));
    connect(tcpsocket,SIGNAL(readyRead()),this,SLOT(readData()));
}

void TcpClient::createConnect(QHostAddress &addr,int &port )
{
    mAddr = addr;
    mPort = port;
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
    //    tcpsocket->close();//关闭io
    tcpsocket->abort();  //断开当前连接
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

/**
 * @brief 发送文件
 * @param str
 */
void TcpClient::sendFile(QString &str)
{
    QFile file(str);

    if(file.open(QFile::ReadOnly))
    {
        QTextStream out(&file);
        while(!out.atEnd())
        {
            QString str = out.readLine();
            writeData(str.toLatin1());
            sleep(2);
        }
    }else
        qDebug() << "打开文件失败！";
}

void TcpClient::sleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}


