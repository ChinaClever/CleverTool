#ifndef UDPBDSENT_H
#define UDPBDSENT_H

#include "udpsentsocket.h"

class UdpBDSent : public QThread
{
    Q_OBJECT
public:
    explicit UdpBDSent(QObject *parent = 0);

protected:
    void run(void);
    void sentData(void);

signals:

public slots:
      void timeoutDone(void);

private:
    bool isRun;
    UdpSentSocket *mSocket;
    QTimer *timer;

    QReadWriteLock  *mLock;
    QQueue<QByteArray>  *mArrayQue;
};
void udp_queue_append(uchar *buf, int len);
#endif // UDPBDSENT_H
