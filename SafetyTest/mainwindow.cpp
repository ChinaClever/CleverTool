#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mItem = TestConfig::bulid()->item;
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
    mSerialNumDlg = new TestSerialNumDlg(this);
    mDataSave = new TestDataSave(this);

    mResultWid = new TestResultWid(ui->stackedWid);
    ui->stackedWid->addWidget(mResultWid);
    connect(mResultWid, SIGNAL(startSig(int)), this, SLOT(updateWidSlot(int)));

    mCoreThread = new TestCoreThread(this);
    connect(mCoreThread, SIGNAL(overSig()), this, SLOT(overSlot()));

    mDataWid = new TestDataTableWid(ui->stackedWid);
    ui->stackedWid->addWidget(mDataWid);

    mItemWid = new TestItemTableWid(ui->stackedWid);
    ui->stackedWid->addWidget(mItemWid);

    mTgsWid = new TgsWid(ui->stackedWid);
    ui->stackedWid->addWidget(mTgsWid);

    mLogWid = new LogWid(ui->stackedWid);
    ui->stackedWid->addWidget(mLogWid);
}


void MainWindow::startTest()
{
    int ret = mSerialNumDlg->exec();
    if(ret == QDialog::Accepted ) {
        QString str = tr("测试前接线: \n 将测试议高压线");
        QuMsgBox box(this,str);
        if(box.Exec()) {
            mItem->mode = Test_Start;
            mCoreThread->startThread();
            mItemWid->startSlot();
            mResultWid->startSlot();
            mDataWid->startSLot();
        }
    }

}

void MainWindow::overSlot()
{
    mItem->mode = Test_Over;
    mResultWid->resultSlot();
    mDataSave->saveTestData();
    mWebSocket->saveTestData();
    mSerialNumDlg->getSerialNum();
}

void MainWindow::overTest()
{
    QuMsgBox box(this, tr("是否停止测试?"));
    if(box.Exec()){
        overSlot();
    }
}

void MainWindow:: updateWidSlot(int id)
{
    switch (id) {
    case Test_Start: startTest(); break;
    case Test_Over: overTest(); break;
    }
}


void MainWindow::on_logBtn_clicked()
{
    ui->stackedWid->setCurrentWidget(mLogWid);
}

void MainWindow::on_itemBtn_clicked()
{
    ui->stackedWid->setCurrentWidget(mItemWid);
}

void MainWindow::on_dataBtn_clicked()
{
    ui->stackedWid->setCurrentWidget(mDataWid);
}

void MainWindow::on_testBtn_clicked()
{
    ui->stackedWid->setCurrentWidget(mResultWid);
}

void MainWindow::on_tgBtn_clicked()
{
    ui->stackedWid->setCurrentWidget(mTgsWid);
}
