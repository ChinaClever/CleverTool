#ifndef UDPTESTING_H
#define UDPTESTING_H

#include "udp/sent/udpheartbeat.h"
#include "udp/recv/udprecvsocket.h"

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
