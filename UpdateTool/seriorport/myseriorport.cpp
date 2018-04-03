

#include "myseriorport.h"

MySeriorport::MySeriorport()
{
    mPort = new QSerialPort;
}

/**
 * @brief 返回串口信息链表
 * @return
 */
QList<QSerialPortInfo> MySeriorport::getQSerialPortInfo()
{
    QList<QSerialPortInfo> portList;
    portList = QSerialPortInfo::availablePorts();

    return portList;
}

/**
 * @brief 返回串口名链表
 * @return
 */
QStringList MySeriorport::getSeriorportInfos()
{
    getMportList();



    for(int i = 0 ; i < mPortlist.size() ; i++)
    {

        mList.append(mPortlist.at(i)->portName());

    }

    return mList;
}

/**
 * @brief 获取系统维护的串口链表，全局变量
 */
void MySeriorport::getMportList()
{
    QList<QSerialPortInfo> portList = getQSerialPortInfo();

    for(int i = 0 ; i < portList.size() ; i++)
    {
        QSerialPort *port = new QSerialPort;
        port->setPort(portList.at(i));
        mPortlist.append(port);
        mbaudlist[i] = 9600;

    }
}


/**
 * @brief 打开串口
 * @param portName
 */
bool MySeriorport::openPort(QString &portName, int baud)
{
    int ret = searchPortIndex(portName);
    if(ret != -1)
    {
        QSerialPort *port = mPortlist.at(ret);
        if(!port->isOpen())
        {
            mbaudlist[ret] = baud;
           // baud = 9600;
            port->setPortName(portName);
            port->open(QSerialPort::ReadWrite);
            port->setBaudRate(baud);  //波特率
            port->setDataBits(QSerialPort::Data8); //数据位
            port->setParity(QSerialPort::NoParity);    //无奇偶校验
            port->setStopBits(QSerialPort::OneStop);   //无停止位
            port->setFlowControl(QSerialPort::NoFlowControl);  //无控制

        }else
            qDebug() << "串口已打开！";
        return true;
    }
    qDebug() << "串口不存在";
    return false;
}

int MySeriorport::searchPortIndex(QString &portName)
{
    for(int i = 0 ; i < mPortlist.size() ; i++)
    {
        if(mPortlist.at(i)->portName() == portName)
            return i;
    }

    return -1;
}

void MySeriorport::closePort(QString &portName)
{
    int ret = searchPortIndex(portName);
    mPortlist.at(ret)->close();
}

bool MySeriorport::sendData(QByteArray &array ,QString &portName)
{
    int ret = searchPortIndex(portName);
    QSerialPort *port = mPortlist.at(ret);
    int bytes = port->write(array);
    if(bytes == array.size())
    {
        return true;
    } else {
       qDebug() << "数据发送失败！";
    }

    return false;
}

QByteArray MySeriorport::readData(QString &portName)
{
    int ret = searchPortIndex(portName);
    QSerialPort *port = mPortlist.at(ret);
    QByteArray recv;
    if(port->isOpen())
    {
        while (port->waitForReadyRead(100)); // 等待窗口接收完全
        while (!port->atEnd())
            recv += port->readAll();     //因为串口是不稳定的，也许读到的是部分数据而已，但也可能是全部数据

       // qDebug() << recv.toHex();
       /* for(int i = 0 ; i < recv.size() ; i++)
            qDebug("recv:%x",(uchar)recv.at(i)); */
    }else
    {
       // QString warningstr = QObject::tr("串口%1未打开").arg(portName);
    }
    return recv;
}

/**
 * @brief 串口是否打开
 * @param portName
 * @return
 */
bool MySeriorport::portIsOpen(QString &portName, int &baud)
{
    int ret = searchPortIndex(portName);
    baud = mbaudlist[ret];
    if(ret != -1)
    {
        if(mPortlist.at(ret)->isOpen()){
            return true;
        }
    }
    return false;
}

/**
 * @brief 查看所有的串口状态
 */
void MySeriorport::viewAllPortStatus()
{

    for(int i = 0 ; i < mPortlist.size() ; i++)
    {
        qDebug() << "i" <<mPortlist.at(i)->portName() << mPortlist.at(i)->isOpen();
    }
}

/**
 * @brief 刷新串口时确定是否缺少或多了串口
 */
void MySeriorport::strcmpMportlist()
{
#if 1
    QList<QSerialPortInfo> portList = getQSerialPortInfo();
    QStringList QSerialPortInfoList;
    for(int i = 0 ; i < portList.size() ; i++)
    {
        QSerialPortInfoList.append(portList.at(i).portName());
    }

    //是否缺少
    for(int i = 0 ; i < portList.size() ; i++)
    {
        QString str = portList.at(i).portName();
        if(!mList.contains(str)) //如果有串口不存在于链表中
        {
            QSerialPort *port = new QSerialPort;
            port->setPort(portList.at(i));

            mPortlist.append(port);
            mList.append(str);
        }
    }

    //是否多了
    for(int i = 0 ; i < mList.size() ; i++)
    {
        QString str = mList.at(i);
        if(!QSerialPortInfoList.contains(str))  //不存在的串口存在
        {
            mList.removeAt(i);
            mPortlist.removeAt(i);
        }
    }
    /*注：mPortlist和list应当始终保持一致*/
#endif
}

QStringList MySeriorport::initSeriorPortInfos()
{
    QStringList list = getSeriorportInfos();

    return list;
}

QStringList MySeriorport::updataSeriorPortinfos()
{
    strcmpMportlist();
    return mList;
}

/**
 * @brief 如果超时，清理串口中的数据
 * @param portName
 */
bool MySeriorport::clearPort(QString &portName)
{
    int ret = searchPortIndex(portName);
    if(ret != -1)
    {
        if(mPortlist.at(ret)->isOpen())
        {
            if(mPortlist.at(ret)->clear(QSerialPort::Input))
                return true;
        }
    }
    return false;
}

