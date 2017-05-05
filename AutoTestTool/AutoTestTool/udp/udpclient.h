#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <QObject>
#include <QUdpSocket>
#include <QTimer>
#include "datadefine.h"

class UdpClient : public QObject
{
    Q_OBJECT
public:
    explicit UdpClient(QObject *parent = 0,int port = 0);

    QByteArray getUdpBackData();
    QHostAddress getAddr();
    quint16 getPort();

signals:
    void receiveData(QByteArray &array);

public slots:
    void timeoutDone();
    void readData();

private:
    QUdpSocket *udpsocket;
    int mPort;
    QHostAddress *mAddr;
    quint16 *mServerPort;
    QByteArray mArray;
};
extern bool analysis_data(data_packet &netPacket,QByteArray &array);
void get_information(data_packet &netPacket , data_dev *deviceData);
#endif // UDPCLIENT_H
