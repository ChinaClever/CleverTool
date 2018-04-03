#ifndef HEARTBEAT_H
#define HEARTBEAT_H

#include <QWidget>
#include<QUdpSocket>
#include<QHostAddress>
#include<QMessageBox>
#include<QDebug>
#include <QMessageBox>
//#include <QThread>

class HeartBeat : public QObject
{
    Q_OBJECT
public:
    explicit HeartBeat(QWidget *parent = 0,int port = 0);

    QHostAddress getAddr();
    quint16 getPort();
    QString getData();
    QByteArray getArray();
    bool clearData();
    void sendData(QByteArray &array);

signals:
    void receiveMessage(QString str);

public slots:
    void dataReceived();

protected:
    QUdpSocket *mUdpScoket;
    QHostAddress *mAddr;
    quint16 *mPort;
    QString data;
    QByteArray array;

protected:
    void initData();
};
HeartBeat *get_HeartBeat();

#endif // HEARTBEAT_H
