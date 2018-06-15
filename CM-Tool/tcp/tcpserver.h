#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include "tcpsocket.h"

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = 0);

    QByteArray getData();
    bool isConnecting();

signals:

public slots:
    void disconnected();
    void readData(QByteArray str);

private:
//    bool isConnected;
    TcpSocket *socket;
    QByteArray mData;

//    QTcpServer *server;
    bool isInconnected;  //判断是否处于正常的tcp连接状态


protected:
    void incomingConnection(int socketDescriptor);

};

#endif // TCPSERVER_H
