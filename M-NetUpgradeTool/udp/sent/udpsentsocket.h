#ifndef UDPSENTSOCKET_H
#define UDPSENTSOCKET_H

#include <QtCore>
#include <QtNetwork>

#define UDP_HB_PORT     38725 /*UDP心跳端口*/
#define UDP_SENT_PORT   38750 /*UDP发送端口*/

class UdpSentSocket : public QObject
{
    Q_OBJECT
public:
    explicit UdpSentSocket(QObject *parent = 0);
    ~UdpSentSocket();

    bool sentData(QHostAddress &addr,uchar *buf, ushort len);
    bool sentData(QHostAddress &addr,QByteArray &msg);
    bool sentBroadcastData(uchar *buf, ushort len, quint16 port);
    bool sentBroadcastData(QByteArray &msg, quint16 port);

signals:

public slots:

private:
    QUdpSocket *mUdpSocket;
    QReadWriteLock  *sentLock;
};
void udp_sent_data(const QString &ip, uchar *buf, ushort len);
#endif // UDPSENTSOCKET_H
