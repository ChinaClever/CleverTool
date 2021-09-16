#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QtGui>
#include <QtCore>
#include <QSerialPort>

class SerialPort : public QThread
{
    Q_OBJECT
    explicit SerialPort(QObject *parent = nullptr);
public:
    static SerialPort *bulid(QObject *parent = nullptr);
    bool open(const QString &name,qint32 baudRate = QSerialPort::Baud19200);
    bool transmit(const QByteArray &witeArray, int msecs=1);

signals:

protected slots:
    void writeSlot();

private:
    QSerialPort  *mSerial;
    QByteArray mWriteArrays;
};

#endif // SERIALPORT_H
