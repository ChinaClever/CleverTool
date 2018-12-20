#ifndef UDPRECVSOCKET_H
#define UDPRECVSOCKET_H

#include <QObject>
#include <QtCore>
#include <QtNetwork>


/**
 * UDP层接收到的数据包
 */
typedef struct
{
    QHostAddress addr;
    quint16 port;
    QByteArray datagram;
}UdpBaseData;


class UdpRecvSocket : public QThread
{
    Q_OBJECT
public:
    explicit UdpRecvSocket(QObject *parent = 0);
    ~UdpRecvSocket();

    bool initSocket(int port);
    UdpBaseData *getData(void);

protected:
    bool dataReceived(void);
    void run(void);

signals:

public slots:

private:
    bool isRun;
    QReadWriteLock  *mRecLock;
    QUdpSocket *mUdpSocket;
    QQueue<UdpBaseData*> *mUdpQueueData; // UDP套接字接收数据队列
};

#endif // UDPRECVSOCKET_H
