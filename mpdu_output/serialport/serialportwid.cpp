/*
 * 串口窗口
 *
 *  Created on: 2018年1月1日
 *      Author: Lzy
 */
#include "serialportwid.h"
#include "ui_serialportwid.h"

SerialPortWid::SerialPortWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SerialPortWid)
{
    ui->setupUi(this);
    mSerialPort = new SerialPort(this);

    initSerialPort();
    initWidget();
}

SerialPortWid::~SerialPortWid()
{
    delete ui;
}


void SerialPortWid::initSerialPort()
{
    QStringList com = mSerialPort->getList(); // 端口列表
    ui->comBox->clear();
    ui->comBox->addItems(com);

    QString name = mSerialPort->getSerialName(); // 已打开端口
    if(!name.isEmpty())
        ui->comBox->setCurrentText(name);
}


void SerialPortWid::initWidget()
{
    connect(ui->comBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(serialPortChanged(QString)));
    connect(ui->workBtn,SIGNAL(clicked()),this,SLOT(workBtnSlot()));
    connect(ui->updateBtn,SIGNAL(clicked()),this,SLOT(updateBtnSlot()));
}



/**
 * @brief 选择不同串口号时重新打开
 * @param com
 */
void SerialPortWid::serialPortChanged(QString com)
{
    if(!com.isEmpty())
    {
        mSerialPort->close();
        qint32 baudRate = ui->baudComBox->currentText().toInt();
        bool ret = mSerialPort->open(com, baudRate);
        if(ret) {
            ui->workBtn->setText(tr("关闭串口"));
            ui->stateLab->setText(tr("串口已打开"));
            ui->comBox->setEnabled(false);
            ui->baudComBox->setEnabled(false);
        }  else {
            ui->workBtn->setText(tr("打开串口"));
            ui->stateLab->setText(tr("串口打开失败"));
            CriticalMsgBox box(this, tr("打开串口%1失败").arg(com));
        }
    }
}



void SerialPortWid::openSerialSlot()
{
    if(mSerialPort->isOpened()) /*已打开，现在是关闭窗口*/
    {
        mSerialPort->close();
        ui->workBtn->setText(tr("打开串口"));
        ui->stateLab->setText(tr("串口已关闭"));
        ui->comBox->setEnabled(false);
        ui->baudComBox->setEnabled(true);
    } else { /*未打开，现在打开*/
        QString com = ui->comBox->currentText();
        if(!com.isEmpty())
            serialPortChanged(com);
        else
            CriticalMsgBox box(this, tr("没有串口"));
    }
}


void SerialPortWid::workBtnSlot()
{
    openSerialSlot();
}


void SerialPortWid::updateBtnSlot()
{
    disconnect(ui->comBox,SIGNAL(currentIndexChanged(QString)), this,SLOT(serialPortChanged(QString)));  //断开信号

    initSerialPort();
    connect(ui->comBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(serialPortChanged(QString)));
    InfoMsgBox box(this, tr("串口刷新成功"));
}

void SerialPortWid::on_loopBtn_clicked()
{
    if(mSerialPort->isOpened())
    {
        bool ret = mSerialPort->loopTest();
        if(ret) {
             InfoMsgBox box(this, tr("串口:%1回环测试成功").arg(mSerialPort->getSerialName()));
        } else {
             CriticalMsgBox box(this, tr("串口:%1回环测试失败").arg(mSerialPort->getSerialName()));
        }
    } else {
         CriticalMsgBox box(this, tr("请先打开串口"));
    }
}
