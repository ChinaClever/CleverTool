/*
 *
 *
 *  Created on: 2018年10月1日
 *      Author: Lzy
 */
#include "testresultwid.h"
#include "ui_testresultwid.h"

TestResultWid::TestResultWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestResultWid)
{
    ui->setupUi(this);
    groupBox_background_icon(this);
    mItem = TestConfig::bulid()->item;
    initSerialPort();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),this, SLOT(progressSlot()));
}

TestResultWid::~TestResultWid()
{
    delete ui;
}


void TestResultWid::startSlot()
{
    QString name = mItem->sn.name;
    QString batch = mItem->sn.batch;
    QString sn = mItem->sn.sn;
    mItem->progress.errNum = mItem->progress.finishNum = mItem->progress.okNum = 0;

    QString str = tr("产品:%1  批次：%2").arg(name).arg(batch);
    ui->titleTab->setText(str);
    str = tr("条码：%1").arg(sn);
    ui->snLab->setText(str);

    ui->itemNumLab->clear();
    ui->statusLab->clear();
    ui->progressBarWater->setValue(0);

    timer->start(200);
    ui->startBtn->setText(tr("停止测试"));
}

void TestResultWid::resultSlot()
{
    bool p = true;
    if(mItem->progress.errNum)  p = false;

    timer->stop();

    mItem->progress.allNum = mItem->progress.finishNum;
    progressSlot();
    ui->statusLab->setText(tr("测试结束!!!"));
    ui->startBtn->setText(tr("立即测试"));
}

void TestResultWid::progressSlot()
{
    sTestProgress *arg = &(mItem->progress);

    int progress = (arg->finishNum * 100.0) / arg->allNum;
    //ui->progressBarWater->setValue(progress);
    ui->statusLab->setText(arg->status);

    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::black);
    int ok = (arg->okNum * 100.0) / arg->allNum;
    if(arg->errNum)  pe.setColor(QPalette::WindowText,Qt::red);
    QString str = tr("测试项目数:%1  失败项目数：%2  项目测试通过率：%3%").arg(arg->allNum).arg(arg->errNum).arg(ok);
    ui->itemNumLab->setText(str);
    ui->itemNumLab->setPalette(pe);
}



void TestResultWid::on_startBtn_clicked()
{
    bool ret = checkSerial();
    if(ret) {
        int mode = Test_Over;
        if(mItem->mode != Test_Start) {
            mode = Test_Start;
        }

        emit startSig(mode);
    }
}

bool TestResultWid::checkSerial()
{
    SerialPort *serial = mSerialDlg->getSerialPort();
    bool ret = serial->isOpened();
    if(!ret)
    {
        QuMsgBox box(this, tr("请先打开串口!!"));
        ret = box.Exec();
        if(ret) {
            on_comBtn_clicked();
        }
    }
    return ret;
}

void TestResultWid::on_comBtn_clicked()
{
    if(mItem->mode != Test_Start) {
        mSerialDlg->exec();
        updateSerialWid();
    } else {
        CriticalMsgBox box(this, tr("请先停止测试！！！"));
    }
}


/**
 * @brief 初始化串口
 * @return
 */
bool TestResultWid::initSerialPort()
{
    bool ret = false;

    mSerialDlg = new SerialPortDlg(this);
    SerialPort *serial = mSerialDlg->getSerialPort();
    mItem->serial = serial;

    QString com = TestConfig::bulid()->getSerialName();
    if(!com.isEmpty())
    {
        ret = serial->isContains(com);
        if(ret) {
            ret = serial->open(com);
            updateSerialWid();
        }
    }

    return ret;
}



/**
 * @brief 更新串口窗口
 */
void TestResultWid::updateSerialWid()
{
    QPalette pe;
    SerialPort *serial = mSerialDlg->getSerialPort();
    QString str = serial->getSerialName();

    if(serial->isOpened()) {
        TestConfig::bulid()->setSerialName(str);
        str += tr(" 已打开");
        pe.setColor(QPalette::WindowText,Qt::black);
    } else {
        str += tr(" 串口未打开");
        pe.setColor(QPalette::WindowText,Qt::red);
    }

    ui->serialLab->setText(str);
    ui->serialLab->setPalette(pe);
}


