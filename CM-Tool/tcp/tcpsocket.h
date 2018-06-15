#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QObject>
#include <QTcpSocket>

class TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpSocket(QObject *parent = 0);

signals:
    void socketDisconnected();
    void dataComing(QByteArray str);

public slots:
    void dataReceived();
    void slotDisconnected();

public:
    QString getData();

private:
    QString str;



};

#endif // TCPSOCKET_H
