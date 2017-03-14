#include "serialportoperate.h"


SerialportOperate::SerialportOperate()
{
    //    mSerialport = new QSerialPort ;  //只作为串口操作，如打开、关闭，其他时候不改变该串口指向
    //    mStatePort = new QSerialPort;    //用于查看状态，在其他时候不改变
    //    mSerialport->close();


}

/**
 * @brief 读取当前串口,返回串口名
 *
 */
QStringList SerialportOperate::readPortInfo()
{
    qDeleteAll(mList);  //释放内存
    mPortList = QSerialPortInfo::availablePorts();
    QStringList list;
    for(int i=0;i < mPortList.size();i++)
    {
        list.append(mPortList.at(i).portName());

        mSerialport = new QSerialPort;
        mSerialport->setPort(mPortList.at(i));
        mList.append(mSerialport);

    }

    //   for(int i=0;i<mList.size();i++)
    //       qDebug()<<"mList:"<<mList.at(i)->portName();

    return list;
}

/**
 * @brief 打开串口
 */
void SerialportOperate::enablePort(QString &portName,qint32 baudRate)
{
    //    qDebug()<<"要关闭的串口："<<mSerialport->portName();
    //    mSerialport->close();

    int ret = serachPort(portName);

    qDebug()<<"要打开的串口："<<mSerialport->portName();
    if(!mList.at(ret)->isOpen())
    {
        mList.at(ret)->open(QSerialPort::ReadWrite);
        mList.at(ret)->setBaudRate(baudRate);  //波特率
        mList.at(ret)->setDataBits(QSerialPort::Data8); //数据位
        mList.at(ret)->setParity(QSerialPort::NoParity);    //无奇偶校验
        mList.at(ret)->setStopBits(QSerialPort::OneStop);   //无停止位
        mList.at(ret)->setFlowControl(QSerialPort::NoFlowControl);  //无控制
        qDebug()<<"打开串口";
    }

}

/**
           * @brief 关闭串口
           * @param portName
           */
void SerialportOperate::disablePort(QString &portName)
{
    //    qDebug()<<"要关闭的串口："<<portName;
    int ret = serachPort(portName);
    if(mList.at(ret)->isOpen())
    {
        qDebug()<<"要关闭的串口："<<mList.at(ret)->portName();
        mList.at(ret)->close();
    }

}

/**
 * @brief 关闭当前串口，例如切换combox时
 */
//void SerialportOperate::disablePort()
//{
//    mSerialport->close();
//}

/**
           * @brief 发送数据到串口
           * @param portName
           * @param data
           */
void SerialportOperate::sendDataToPort(QString &portName,QString &data)
{
    int ret = serachPort(portName);
    if(!data.isEmpty() && mList.at(ret)->isOpen())
        mList.at(ret)->write(data.toLatin1());  //QString转化为QByteArray类型

}

/**
           * @brief 遍历串口表
           * @param portName
           * @return
           */
int SerialportOperate::serachPort(QString &portName)
{
    for(int i = 0;i <mList.size(); i++)
    {
        if( mList.at(i)->portName() == portName)
        {
            return i;
            break;
        }
    }
}

/**
 * @brief 根据串口名检测串口状态
 * @param 注：监测状态时不需要改变mSerialport
 * @return
 */
bool SerialportOperate::checkPortState(QString &portName)
{

    int ret = serachPort(portName);
    if(!mList.at(ret)->isOpen()){
        return false;
    }

    return true;
}

void SerialportOperate::checkAllState()
{
    for(int i=0;i<mList.size();i++)
        qDebug()<<"i:"<<i<<"端口名："<<mList.at(i)->portName()<<"状态："<<mList.at(i)->isOpen();

}

QByteArray SerialportOperate::test()
{
    QByteArray sentArray,recvArray;

    for(int i=0; i<255;++i)
        sentArray.append(i);
    mList.at(1)->write(sentArray);

    recvArray = mList.at(1)->readAll();

    return recvArray;
}

QByteArray SerialportOperate::readDataFromPort(QString &portName)
{
    int id =serachPort(portName);
    QByteArray recv;
    while (mList.at(id)->waitForReadyRead(1)); // 等待窗口接收完全
    while (!mList.at(id)->atEnd())
        recv += mList.at(id)->readAll();     //因为串口是不稳定的，也许读到的是部分数据而已，但也可能是全部数据
    return recv;
}

QString SerialportOperate::readStringFromPort(QString &portName)
{
    QString str = readDataFromPort(portName);
    return str;
}





