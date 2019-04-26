#ifndef SERIALPORTWID_H
#define SERIALPORTWID_H

#include <QWidget>
#include "serialport.h"

namespace Ui {
class SerialPortWid;
}

class SerialPortWid : public QWidget
{
    Q_OBJECT

public:
    explicit SerialPortWid(QWidget *parent = 0);
    ~SerialPortWid();

    SerialPort *getSerialPort(){return mSerialPort;}

protected:
    void initSerialPort();
    void initWidget();
signals:
    void closeSignal();
protected slots:
    void serialPortChanged(QString com);
    void workBtnSlot();
    void updateBtnSlot();
    void openSerialSlot();

private slots:
    void on_loopBtn_clicked();

private:
    Ui::SerialPortWid *ui;

    SerialPort *mSerialPort;
};
extern void groupBox_background_icon(QWidget *target);

#endif // SERIALPORTWID_H
