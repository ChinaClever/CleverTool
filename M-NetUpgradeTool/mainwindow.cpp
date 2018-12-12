#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mIpsWid = new IpsWid(ui->ipWid);
    mTftpWid = new TftpWidget(ui->tftpWid);

    mIpsTabWid = new IpsTabWid(ui->ipsTabWid);
    mOksTabWid = new OksTabWid(ui->oksTabWid);
    mErrsTabWid = new ErrsTabWid(ui->errsTabWid);
}

MainWindow::~MainWindow()
{
    delete ui;
}
