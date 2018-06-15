#ifndef MYUDPTHREAD_H
#define MYUDPTHREAD_H
#include <qthread.h>
#include <QTimer>
#include "datasent.h"
#include "heartbeat.h"



class MyUdpThread : public QThread
{

    Q_OBJECT

public:
    MyUdpThread();
    ~MyUdpThread();

    void run();

private:
    bool is_run;
    HeartBeat *heartbeat;
    DataSouce *souce;

private slots:
    void timeoutdone();
};

#endif // MYUDPTHREAD_H
