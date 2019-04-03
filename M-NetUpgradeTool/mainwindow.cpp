#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    DataPacket::bulid()->data;
    mIpsWid = new IpsWid(ui->ipWid);
    mTftpWid = new TftpWidget(ui->tftpWid);

    mIpsTabWid = new IpsTabWid(ui->ipsTabWid);
    mOksTabWid = new OksTabWid(ui->oksTabWid);
    mErrsTabWid = new ErrsTabWid(ui->errsTabWid);
//    mUdpTesting = new UdpTesting(this);

//    timer = new QTimer(this);
//    timer->start(200);
//    connect(timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::timeoutDone(void)
{
//    QStringList ips;
//    bool ret = mUdpTesting->check(ips);
//    if(ret) {
//        QString str = tr("有其它电脑已打开升级工具：");
//        for(int i=0; i<ips.size(); ++i) {
//            str += ips.at(i) + "; ";
//        }
//        ui->statusBar->showMessage(str);
//    }
}
