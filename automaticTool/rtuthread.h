#ifndef RTUTHREAD_H
#define RTUTHREAD_H

#include <QObject>
#include <QThread>
#include "serialportwid.h"
#include "commondata.h"

class ReturnTableData{
public:
   quint8 sign;            //执行板返回标志
   quint8 addr;            //执行板地址
   quint8 len;             //长度
   quint8 onoffState;      //八位开关状态
   quint8 current[8][2];   //电流，第一位到第八位，每位两个字节
   quint8 volate[8][2];    //电压，第一个和第八个
   quint8 power[8][2];     //功率，第一位到第八位，每位两个字节 功率
   quint8 xornumber;       //校验码
   ReturnTableData():sign(0), addr(0), len(0), onoffState(0), xornumber(0){
       for(int i =0; i < 8; i++){
           for(int j = 0; j < 2; j++){
               current[i][j] = volate[i][j] = power[i][j] = 0;
           }
       }

   }
   ~ReturnTableData(){}
};

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

protected:
    void run();
    void workDown();
    bool stationV1(); //工位一 | 握手判断存在
    bool stationV2(); //工位二 | 等待标准源起来
    bool stationV3(); //工位三 | 重新校准
    bool stationV4(); //工位四 | 控制标准源 - 解决重启 开关全开 读取
    bool stationV5(); //工位五 | 取走料

    void resSource(); //复位标准员
    void resSourceOutput(QString V = "V100", QString A = "A100"); //复位标准源输出

protected:
    bool isRun;
    bool isRead;

    int mRet;
    int mPosition;
    bool mPass;

signals:
    void showDataSignal(int value, ReturnTableData *data);
    void sendStation(QString str);
    void sendEnd();
    void sendClear();
    void sendTime(QString str);

    void sendWriteSignal(uchar *sent, int len);
    void sendWriteSignal(QByteArray array);

protected slots:
    void onWrite(uchar *sent, int len);
    void onWrite(QByteArray array); //标准源

public:
    SerialPort *mSerialPortPlate;
    SerialPort *mSerialPortSource;
    int mAddr, mMethodType, mCount, mOffCount;
    bool reset; //重置

};

#endif // RTUTHREAD_H
