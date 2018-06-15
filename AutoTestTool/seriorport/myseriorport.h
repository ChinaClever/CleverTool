#ifndef MYSERIORPORT_H
#define MYSERIORPORT_H

#include <QSerialPort> 
#include <QSerialPortInfo>
#include <QDebug>


class MySeriorport
{

public:
    MySeriorport();
    ~MySeriorport();

    QStringList initSeriorPortInfos();
    QStringList updataSeriorPortinfos();

    bool openPort(QString &portName);
    void closePort(QString &portName);
    bool sendData(QByteArray &array, QString &portName);
    QByteArray readData(QString &portName);
    bool portIsOpen(QString &portName);
    void viewAllPortStatus();
    bool clearPort(QString &portName);

private:
    void strcmpMportlist();
    QList<QSerialPortInfo> getQSerialPortInfo();
    QStringList getSeriorportInfos();
    void getMportList();
    int searchPortIndex(QString &portName);

private slots:



private:
    QSerialPort *mPort;
    QList<QSerialPort *> mPortlist;  //维护一个串口链表
    QStringList mList;

    
    
};

#endif // MYSERIORPORT_H
