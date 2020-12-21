#ifndef SERIALPORTDLG_H
#define SERIALPORTDLG_H

#include <QDialog>
#include "serialportwid.h"

namespace Ui {
class SerialPortDlg;
}

class SerialPortDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SerialPortDlg(QWidget *parent = 0);
    ~SerialPortDlg();

     SerialPort *getSerialPort(){return serial->getSerialPort();}

private:
    Ui::SerialPortDlg *ui;
    SerialPortWid *serial;
};

#endif // SERIALPORTDLG_H
