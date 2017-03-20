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

    initCompareList();
    initList(); //重新初始化list表
    initMlist();  //重新初始化

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
int SerialportOperate::sendDataToPort(QString &portName,QString &data)
{
    char buffer[7]={0x01,0x04,0x04,0x01,0x04,0x04};

    int writeBytes;
    int ret = serachPort(portName);
    if(data.isEmpty() && mList.at(ret)->isOpen())
    {
//        writeBytes = mList.at(ret)->write(data.toLatin1());  //QString转化为QByteArray类型
        writeBytes = mList.at(ret)->write(buffer);  //QString转化为QByteArray类型
        return writeBytes;
    }
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
    if(mList.at(id)->isOpen())
    {
        while (mList.at(id)->waitForReadyRead(1)); // 等待窗口接收完全
        while (!mList.at(id)->atEnd())
            recv += mList.at(id)->readAll();     //因为串口是不稳定的，也许读到的是部分数据而已，但也可能是全部数据
    }else
    {
        QString warningstr = QObject::tr("串口%1未打开").arg(portName);
        QMessageBox::information(NULL,QObject::tr("Information"),warningstr,QObject::tr("确定"));
        return 0;
    }

    return recv;
}

QString SerialportOperate::readStringFromPort(QString &portName)
{
    QString str = readDataFromPort(portName);
    return str;
}

/**
 * @brief 第一次初始化串口信息
 * @return
 */
QStringList SerialportOperate::initPortInfo()
{
    mPortList.clear();
    mPortList = QSerialPortInfo::availablePorts();


    list.clear();
    for(int i=0;i < mPortList.size();i++)
    {
        list.append(mPortList.at(i).portName());

        mSerialport = new QSerialPort;
        mSerialport->setPort(mPortList.at(i));
        mList.append(mSerialport);

    }
    return list;
}

void SerialportOperate::initList()
{
    mPortList.clear();
    mPortList = QSerialPortInfo::availablePorts();


    list.clear();
    for(int i=0;i < mPortList.size();i++)
    {
        list.append(mPortList.at(i).portName());
    }
}


void SerialportOperate::initMlist()
{
    int ret = list.size() - mCompareList.size();

    if(ret > 0)  //增加串口
        for(int i = 0; i < list.size() ;i++)
        {
            if(!mCompareList.contains(list.at(i)))//旧表不包含新表的串口,更新mlist
            {
                qDebug()<<"增加串口";
                mSerialport = new QSerialPort;
                mSerialport->setPort(mPortList.at(i));
                mList.append(mSerialport);
            }
        }
    else if(ret < 0)  //减少串口
    {
        for(int i = 0; i < mCompareList.size(); i++)
        {
            if(!list.contains(mCompareList.at(i))) //新表不包含旧表串口,更新mlist
            {
                for( int j = 0; j < mList.size() ; j++ )
                {
                    if( mList.at(j)->portName() == mCompareList.at(i))
                    {
                        qDebug()<<"减少串口";
                        // qDeleteAll(mList.at(j));
                        mList.at(j)->deleteLater();
                        mList.removeAt(j);
                        break;

                    }
                }
            }
        }
    }
}

void SerialportOperate::initCompareList()
{
    mCompareList.clear();
    for(int i=0; i < list.size(); i++)
    {
        mCompareList.append(list.at(i));
    }
}





