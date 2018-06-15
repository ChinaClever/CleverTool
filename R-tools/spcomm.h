#ifndef SPCOMM_H
#define SPCOMM_H

#include <QObject>
#include <QtSerialPort/QtSerialPort>

class SPComm : public QObject
{
    Q_OBJECT
public:
    explicit SPComm(QObject *parent = 0);
    ~SPComm();
    bool isOpen() const;

    void setPortName(const QString &name);
    QString portName() const;

    void setBaudRate(int baudRate);
    int baudRate() const;

    virtual bool open();
    virtual void close();
    virtual bool clear();

    int readData(char *buffer, int count, int timeout = 1000);
    int writeData(char *data, int size);
    int write(char ch);

protected:
    QString m_portName;
    int m_baudRate;
    QSerialPort *m_serialPort;

public slots:

};

#endif // SPCOMM_H
