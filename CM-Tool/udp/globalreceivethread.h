#ifndef GLOBALRECEIVETHREAD_H
#define GLOBALRECEIVETHREAD_H

#include <QThread>
#include "heartbeat.h"
#include <QTimer>


class GlobalReceiveThread : public QThread
{
    Q_OBJECT
public:
    GlobalReceiveThread();
    ~GlobalReceiveThread();

private:
    bool isRunning;
    HeartBeat *mSocket;

private slots:
    void timeOutDone();

protected:
    void run();
};


extern int get_data_packet(QByteArray array);
#endif // GLOBALRECEIVETHREAD_H
