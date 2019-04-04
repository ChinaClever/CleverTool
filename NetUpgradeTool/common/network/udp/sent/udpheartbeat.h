#ifndef UDPHEARTBEAT_H
#define UDPHEARTBEAT_H

#include "udpbdsent.h"

class UdpHeartBeat : public QThread
{
    Q_OBJECT
    explicit UdpHeartBeat(QObject *parent = 0);
public:
    ~UdpHeartBeat();

    static UdpHeartBeat *bulid(QObject *parent);

protected:
    void run(void);
     void heartbeatPacket(void);
     void startSent(void);

signals:

public slots:
    void timeoutDone(void);

private:
    bool isRun;
    UdpSentSocket *mSocket;
    QTimer *timer;

    UdpBDSent *mUdpBDSent;
};

#endif // UDPHEARTBEAT_H
