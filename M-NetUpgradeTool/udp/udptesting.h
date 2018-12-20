#ifndef UDPTESTING_H
#define UDPTESTING_H

#include "sent/udpheartbeat.h"
#include "recv/udprecvsocket.h"

class UdpTesting : public QObject
{
    Q_OBJECT
public:
    explicit UdpTesting(QObject *parent = nullptr);

    bool check(QStringList &ips);

signals:

public slots:
    void timeoutDone(void);

private:
    UdpRecvSocket *mRecv;
    QTimer *timer;
    QStringList mIPs;
};

#endif // UDPTESTING_H
