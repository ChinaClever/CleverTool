#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>

class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QHostAddress &addr,int &port ,QObject *parent = 0);

    void createConnect();
    void breakConnect();
    void writeData(QByteArray array);
    bool getConnectingStatus();

signals:
    void receiveTcpData(QByteArray &array);

public slots:
    void socketConnected();
    void socketDisconnected();
    void readData();

private:
    QTcpSocket *tcpsocket;
    int mPort;
    QHostAddress mAddr;
    bool isConnecting;

};

#endif // TCPCLIENT_H
