#ifndef SERIALPORTOPERATE_H
#define SERIALPORTOPERATE_H
#include<QSerialPort>
#include<QSerialPortInfo>
#include<QList>
#include<QStringList>
#include<QDebug>
#include<QMessageBox>
#include <QObject>
//#pragma execution_character_set("utf-8")


class SerialportOperate
{
public:
    SerialportOperate();
     ~SerialportOperate();

    QByteArray datra;

    QStringList readPortInfo();     //读取串口信息
    QStringList initPortInfo();//初始化串口信息
    void enablePort(QString &portName, qint32 baudRate = QSerialPort::Baud9600);       //打开串口
    void disablePort(QString &portName);      //关闭串口
    QByteArray test();
    int sendDataToPort(QString &portName, quint8 *cmd, int len);   //发送数据
    QByteArray readDataFromPort(QString &portName);
    QString readStringFromPort(QString &portName);
//    void sendDataToPort(QByteArray &data);
    int serachPort(QString &portName);   //遍历
    bool checkPortState(QString &portName);       //检测
    void checkAllState();
    bool checkIsOpen(QString &portName);

protected:
//    void copyList();
    void initList();
    void initMlist();
    void initCompareList();
    QByteArray quintToByte(quint8 *cmd,int len);

private:
    QSerialPort *mSerialport;
    //    QSerialPort *mStatePort;  //用于查看串口状态
    QList<QSerialPortInfo> mPortList;
    QStringList mCompareList;    // 用于对比初始列表，检查串口在线
    QList<QSerialPort *> mList;  //存储mportlist对应的端口
    QStringList list;
/*
private:
    int mCPortSun;  //当前绑定串口
private:
    void openPor(QSerialPort *, int);
    void stopPor(QSerialPort *, int);
signals:
    void sendDataSig(QByteArray);
private slots:
    void onGetData();
    */
};

#endif // SERIALPORTOPERATE_H
