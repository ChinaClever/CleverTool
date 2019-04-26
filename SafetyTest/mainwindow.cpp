#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QTimer::singleShot(5,this,SLOT(initFunSLot())); //延时初始化
    set_background_icon(ui->widget,":/image/title_back.jpg");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::initFunSLot()
{
    initWid();
}


void MainWindow::initWid()
{
    mLogWid = new LogWid(ui->stackedWid);
    ui->stackedWid->addWidget(mLogWid);
}

void MainWindow::on_logBtn_clicked()
{
    ui->stackedWid->setCurrentWidget(mLogWid);
}
