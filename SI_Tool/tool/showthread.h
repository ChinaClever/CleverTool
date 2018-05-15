#ifndef SHOWTHREAD_H
#define SHOWTHREAD_H

#include <QObject>
#include <QThread>
#include "serialport/serialportoperate.h"
#include "datadefine.h"
#include <QByteArray>

class ShowThread : public QThread
{    
    Q_OBJECT
public:
    explicit ShowThread(SerialportOperate *Port);
    ~ShowThread();
    void timerData(int flaga, int flagb,QString name, int timer = 1000);
    void timerStop();

signals:

public slots:

private:
    void run();

    void sendCmd(int flag1,int flag2);//fasong数据
   // void getCmd(QByteArray &data); //获取数据
   // void onReadAnswer(); //数据更新
public:
    bool isRun;
    int flag1, flag2;
    int mTimer;
    QString mCurrentPort;
    SerialportOperate *myPort;
};

#endif // SHOWTHREAD_H
