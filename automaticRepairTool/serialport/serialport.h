#ifndef SERIALPORT_H
#define SERIALPORT_H

#include "common.h"
#include <QSerialPort>

#define SERIAL_LEN  2*1024

class SerialPort
{
public:
    explicit SerialPort();
    ~SerialPort();

    bool open(const QString &name,qint32 baudRate = QSerialPort::Baud9600);
    bool isOpened(){return isOpen;}
    bool close(void);

    QStringList getList();
    QString getSerialName();
    bool isContains(const QString &name);

    int write(const QByteArray &array);
    int write(uchar *sent, int len);

    int read(QByteArray &array, int msecs=1);
    int read(uchar *recv, int msecs=1);

    int transmit(const QByteArray &witeArray, QByteArray &readArray, int msecs=1);
    int transmit(uchar *sent, int len, uchar *recv, int msecs=1);

    bool loopTest();

    void msleep(unsigned int msec);


protected:
    int recv(QByteArray &array);
    bool reOpen();
//private slots:
    void serialReadSlot(void);

private:
    bool isOpen;
    int mCount;
    QSerialPort  *mSerial;
    QReadWriteLock  mRwLock;
    QByteArray mSerialData;
};

#endif // SERIALPORT_H
