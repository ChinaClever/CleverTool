#include "serialportoperate.h"


SerialportOperate::SerialportOperate()
{

}

/**
 * @brief 读取当前串口
 *
 */
QList<QSerialPortInfo> SerialportOperate::readPortInfo()
{
    mPortList = QSerialPortInfo::availablePorts();
    QStringList list;
    for(int i=0;i < mPortList.size();i++)
    {
        list.append(mPortList.at(i).portName());
    }
    return mPortList;
}

void SerialportOperate::enablePort()
{

}


