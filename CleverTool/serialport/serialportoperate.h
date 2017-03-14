#ifndef SERIALPORTOPERATE_H
#define SERIALPORTOPERATE_H
#include<QSerialPort>
#include<QSerialPortInfo>
#include<QList>
#include<QStringList>
#include<QDebug>
//#pragma execution_character_set("utf-8")


class SerialportOperate
{
public:
    SerialportOperate();

    QByteArray datra;

    QStringList readPortInfo();     //读取串口信息
    void enablePort(QString &portName, qint32 baudRate = QSerialPort::Baud19200);       //打开串口
    void disablePort(QString &portName);      //关闭串口
    QByteArray test();


    void sendDataToPort(QString &portName, QString &data);   //发送数据
    QByteArray readDataFromPort(QString &portName);
    QString readStringFromPort(QString &portName);

    void sendDataToPort(QByteArray &data);

    int serachPort(QString &portName);   //遍历
    bool checkPortState(QString &portName);       //检测
    void checkAllState();

private:
    QSerialPort *mSerialport;
    //    QSerialPort *mStatePort;  //用于查看串口状态
    QList<QSerialPortInfo> mPortList;
    QList<QSerialPort *> mList;  //存储mportlist对应的端口
};

#endif // SERIALPORTOPERATE_H
