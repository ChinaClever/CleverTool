/*
 * 对串口的常用操作：打开、读、写、关闭
 *
 *  Created on: 2018年1月1日
 *      Author: Lzy
 */
#include "serialport.h"
#include <QSerialPortInfo>
#include <QApplication>

#define SERIAL_READ_TIMEOUT  30  // 30MS

SerialPort::SerialPort(QObject *parent) : QThread(parent)
{
    isOpen = false;
    mSerial = NULL;
}

SerialPort::~SerialPort()
{
    close();
}

/**
 * @brief 打开串口
 * @param name 串口名  大写
 * @param baudRate 波特率
 * @return true
 */
bool SerialPort::open(const QString &name,qint32 baudRate)
{
    bool ret=false;

    if(!isOpen)
    {
        mCount = 1;
        mSerial = new QSerialPort(name);       //串口号，一定要对应好，大写！！！
        ret = mSerial->open(QIODevice::ReadWrite);      //读写打开
        if(ret)
        {
            mSerial->setBaudRate(baudRate);  //波特率
            mSerial->setDataBits(QSerialPort::Data8); //数据位
            mSerial->setParity(QSerialPort::NoParity);    //无奇偶校验
            mSerial->setStopBits(QSerialPort::OneStop);   //无停止位
            mSerial->setFlowControl(QSerialPort::NoFlowControl);  //无控制
            // connect(mSerial,SIGNAL(readyRead()),this,SLOT(serialReadSlot()));    //连接槽
            isOpen = true;
        }
        else
            qDebug() << "open Serial:" << name << "Err";
    }

    return ret;
}


/**
 * @brief 串口关闭
 * @return true
 */
bool SerialPort::close(void)
{
    if(isOpen)
    {
        QWriteLocker locker(&mRwLock); // 正在操作时不允许关闭
        isOpen = false;
        mSerial->close();
        delete mSerial;
        mSerial = NULL;
    }

    return true;
}

/**
 * @brief 重新打开串口 发送到一定程序，串口发送数据无反应
 * @param array
 * @return
 */
bool SerialPort::reOpen()
{
    QString com = getSerialName();
    int br = mSerial->baudRate();

    close();
    bool ret = open(com, br);
    if(!ret) qDebug() << "SerialPort reOpen " << ret << com << br;

    return ret;
}

/**
 * @brief 获取端口名称
 * @return
 */
QString SerialPort::getSerialName()
{
    QString com;

    if(mSerial)
        com =  mSerial->portName();

    return com;
}


/**
 * @brief 获取串口列表
 * @return
 */
QStringList SerialPort::getList()
{
    QStringList list;
    QList<QSerialPortInfo>  infos = QSerialPortInfo::availablePorts();
    if(!infos.isEmpty())
    {
        foreach (QSerialPortInfo info, infos) {
            list <<  info.portName();
        }
    }

    return list;
}

bool SerialPort::isContains(const QString &name)
{
    return getList().contains(name);
}


/**
 * @brief 写数据
 * @param array
 * @return
 */
int SerialPort::write(const QByteArray &array)
{
    int len=0;

    if(isOpen) {
        // 串口每过段时间重新打开一下
//        if((mCount++ % 555) == 0)  reOpen();  // 重新打开串口

        len = mSerial->write(array);
        if(len > 0) {
            mSerial->flush();
//            mSerial->waitForBytesWritten(SERIAL_READ_TIMEOUT);
//            msleep(255);
        }
    }

    return len;
}

int SerialPort::write(uchar *sent, int len)
{
    QByteArray array;
    array.append((char *)sent, len);

    return write(array);
}

/**
 * @brief 读串口
 * @param array
 * @return 长度
 */
int SerialPort::recv(QByteArray &array)
{
    QByteArray dataTemp;
    if(isOpen)
    {
        /* 处理所有还没有被处理的各类事件，主要是不用用户感觉到ka */
        QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
//        msleep(SERIAL_READ_TIMEOUT);
        while (mSerial->waitForReadyRead(SERIAL_READ_TIMEOUT)); // 等待窗口接收完全
        while (!mSerial->atEnd()) {
            dataTemp += mSerial->readAll();     //因为串口是不稳定的，也许读到的是部分数据而已，但也可能是全部数据
        }
        if(!mSerial->isReadable()) qDebug() << "Serial not Read Err!!! ###########";

        array += dataTemp;
    }

    return dataTemp.length();
}


int SerialPort::read(QByteArray &array, int msecs)
{
    int len=0, count=0;
    if(!isOpen) return len;

    QWriteLocker locker(&mRwLock);
    do
    {
        int rtn = recv(array);
        if(rtn > 0) {
            len += rtn;
            count = msecs-1;
        } else {
            count++;
        }
    } while (count < msecs) ;

    return len;
}

int SerialPort::read(uchar *recv, int msecs)
{
    QByteArray array;
    int ret = read(array, msecs);
    if(ret < SERIAL_LEN) {
        for(int i=0; i<ret; ++i)
            recv[i] = array.at(i);
    } else {
        qDebug() << "SerialPort read too long :" << ret;
        ret = 0;
    }

    return ret;
}



/**
 * @brief 串口接收糟函数
 */
void SerialPort::serialReadSlot(void)
{
    QByteArray dataTemp;
    recv(dataTemp);

    mSerialData += dataTemp;
    //emit readyReadSig();
}

/**
 * @brief 半双工传输
 * @param witeArray 要发送的数据
 * @param readArray 接收到的数据
 * @return 收到的数据长度
 */
int SerialPort::transmit(const QByteArray &witeArray, QByteArray &readArray, int msecs)
{
    int ret = write(witeArray);
    if(ret > 0) {
        ret = read(readArray, msecs);
        if((ret < 0) || (ret > SERIAL_LEN)) {

            ret = 0;
        } else if(ret == 0) {
            // qDebug() << "SerialPort transmit read no data"  << ret;
        }
    }

    return ret;
}

/**
  * 功　能：传输数据
  * 入口参数：sent -> 发送缓冲区, len ->  发送长度
  * 出口参数：recv -> 接收缓冲区
  * 返回值：读取的实际长度  <=0 出错
  */
int SerialPort::transmit(uchar *sent, int len, uchar *recv, int msecs)
{
    QByteArray witeArray, readArray;
    witeArray.append((char *)sent, len);

    int ret = transmit(witeArray, readArray, msecs);
    if(ret > 0) {
        for(int i=0; i<ret; ++i)
            recv[i] = readArray.at(i);
    }

    return ret;
}


/**
 * @brief 回环测试
 */
bool SerialPort::loopTest()
{
    QByteArray sentArray,recvArray;

    for(int i=0; i<255;++i)
        sentArray.append(i);

    int ret = transmit(sentArray, recvArray);
    if(ret != sentArray.size())
        qDebug() << "Serial test Err:" <<ret;
    else
        qDebug() << "Serial test OK";
    return ret;
}
