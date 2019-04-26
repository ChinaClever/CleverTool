#include "serialportdlg.h"
#include "ui_serialportdlg.h"
extern void groupBox_background_icon(QWidget *target);

SerialPortDlg::SerialPortDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SerialPortDlg)
{
    ui->setupUi(this);
    groupBox_background_icon(this);
    serial = new SerialPortWid(this);
    connect(serial,SIGNAL(closeSignal()),this,SLOT(close()));
}

SerialPortDlg::~SerialPortDlg()
{
    delete ui;
}
