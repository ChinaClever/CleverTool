#include "serialport.h"

SerialPort::SerialPort(QObject *parent) : QThread(parent)
{
    mSerial = new QSerialPort(this);
}

SerialPort *SerialPort::bulid(QObject *parent)
{
    static SerialPort* sington = nullptr;
    if(sington == nullptr)
        sington = new SerialPort(parent);
    return sington;
}


bool SerialPort::open(const QString &name,qint32 baudRate)
{
    mSerial->setPortName(name);
    bool ret = mSerial->open(QIODevice::ReadWrite);
    if(ret) {
        mSerial->setBaudRate(baudRate);  //波特率
        mSerial->setDataBits(QSerialPort::Data8); //数据位
        mSerial->setParity(QSerialPort::NoParity);    //无奇偶校验
        mSerial->setStopBits(QSerialPort::OneStop);   //无停止位
        mSerial->setFlowControl(QSerialPort::NoFlowControl);  //无控制
        // connect(mSerial,SIGNAL(readyRead()),this,SLOT(serialReadSlot()));    //连接槽
    } else qDebug() << "open Serial:" << name << "Err";

    return  ret;
}


void SerialPort::writeSlot()
{
    mSerial->clear();
    if(mSerial->isOpen() && mWriteArrays.size()) {
        int len = mSerial->write(mWriteArrays);
        if(len > 0) mSerial->flush();
    }
}


bool SerialPort::transmit(const QByteArray &witeArray, int msecs)
{
    mWriteArrays = witeArray;
    return mSerial->waitForReadyRead(msecs);
}

