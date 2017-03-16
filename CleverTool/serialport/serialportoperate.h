#ifndef SERIALPORTOPERATE_H
#define SERIALPORTOPERATE_H
#include<QSerialPort>
#include<QSerialPortInfo>
#include<QList>
#include<QStringList>


class SerialportOperate
{
public:
    SerialportOperate();


protected:
    QList<QSerialPortInfo> readPortInfo();     //读取串口信息
    void enablePort();       //打开串口
    void disablePort();      //关闭串口
    void sendDataToPort();   //发送数据

private:
    QSerialPort *mSerialpost;
    QList<QSerialPortInfo> mPortList;
};

#endif // SERIALPORTOPERATE_H
