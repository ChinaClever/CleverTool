#ifndef TCPTHREAD_H
#define TCPTHREAD_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include "tcpserver.h"
#include "analysisdata.h"

class TcpThread : public QThread
{
    Q_OBJECT
public:
    TcpThread();

protected:
    void run();

private:
    bool is_run;
    TcpServer *server;
    QByteArray data;
private slots:
    void timeoutdone();
};

#endif // TCPTHREAD_H
