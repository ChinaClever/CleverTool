#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QTextStream>
#include <QFile>
#include <QTime>
#include <QCoreApplication>
#include <QEventLoop>

class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = 0);

    void createConnect(QHostAddress &addr, int &port);
    void breakConnect();
    void writeData(QByteArray array);
    bool getConnectingStatus();
    void sendFile(QString &str);

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

private:
    void sleep(unsigned int msec);

};

#endif // TCPCLIENT_H
