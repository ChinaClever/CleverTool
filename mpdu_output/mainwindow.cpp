#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mpdu_modbus_test/mpdu_test_threshold/mpdu_outputthresholdwid.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mSerialPortDlg = new SerialPortDlg(this);
    mSerialPort = mSerialPortDlg->getSerialPort();

    MPDU_OutputThresholdWid *wid = new MPDU_OutputThresholdWid(ui->tabWidget);
    wid->initwid(0, mSerialPort);
    ui->tabWidget->addTab(wid, tr("输出位最小值"));

    wid = new MPDU_OutputThresholdWid(ui->tabWidget);
    wid->initwid(1, mSerialPort);
    ui->tabWidget->addTab(wid, tr("输出位最大值"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    mSerialPortDlg->exec();
    QString str = "---";
    if(mSerialPort->isOpened()) {
        str = mSerialPort->getSerialName();
    }
    ui->comLab->setText(str);
}
