#ifndef RTUTHREAD_H
#define RTUTHREAD_H

#include <QObject>
#include <QThread>
#include "serialportwid.h"
#include "commondata.h"
class RtuThread : public QThread
{
    Q_OBJECT
public:
    explicit RtuThread(QObject *parent = nullptr);
    ~RtuThread();

    void startThread();
    void stopThread();
    void initState();
    bool getRun(){return isRun;}
    void setRead(bool value){ isRead = value; }
    bool getRead(){return isRead;}

protected:
    void run();
    void workDown();
    void communication(); //交流
    void directd(); //直流
    void directd0(); //直流 - 220v 0A：校准偏移量
    void directd10(); //直流 - 220v 10A:校准电流电压
    void readData(); //读取数据

protected:
    bool isRun;
    bool isRead;

signals:
    void showDataSignal(int value, Rtu_recv *data);
    void sendStr(QString str);
    void sendEnd();
    void sendWriteSignal(uchar*, int);
    void sendWriteSignal(QByteArray);
public slots:
     void onWrite(uchar *sent, int len);
     void onWrite(QByteArray array);
public:
    SerialPort *mSerialPortPlate;
    int mAddr;
    int mDc; // 0-直流 1-交流
    int mDcType; //校准模式 _ 仅直流有 【0 - 220v 0A：校准偏移量】 【1 - 220v 10A:校准电流电压】
    Rtu_recv *pkt;
};

#endif // RTUTHREAD_H
