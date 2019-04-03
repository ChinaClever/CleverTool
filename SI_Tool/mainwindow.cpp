#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTableWidgetItem>

extern QReadWriteLock lock;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();

    QTimer::singleShot(100,this,SLOT(onInit())); //延时初始化
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    mSerialPortPlateWid = new SerialPortWid(ui->gBoxAWid);
    mSerialPortPlateWid->setBaudSlot(2); //默认波特率
    mRtuThread = new RtuThread(this);
    mRtuThread->mSerialPortPlate = mSerialPortPlateWid->getSerialPort();
    connect(mRtuThread, SIGNAL(showDataSignal(int,Rtu_recv*)), this, SLOT(onGetShowData(int,Rtu_recv*)));
    connect(mRtuThread, SIGNAL(sendStr(QString)), this, SLOT(onDebugEidt(QString)));
    connect(mRtuThread, SIGNAL(sendEnd()), this, SLOT(onEnd()));
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);  //设置不可编辑
}

void MainWindow::onInit()
{
    ui->readBox->setChecked(true);
    on_readBox_clicked(true);
}

void MainWindow::on_startOrStop_clicked()
{
    mRtuThread->mAddr = ui->addr->text().toInt();
    if(mRtuThread->getRun()){ //STOP
        mRtuThread->stopThread();
        if(mRtuThread->getRead()) ui->startOrStop->setText("读取");
        else ui->startOrStop->setText("校准");
        ui->gBoxA->setEnabled(true);
        ui->gBoxC->setEnabled(true);
    }else{ //start
        if(mRtuThread->mSerialPortPlate->isOpened()) {
            mRtuThread->mDcType = ui->radBtn0->isChecked() ? 0 : 1;  //为直流校准引入
            ui->tableWidget->clearContents(); //清空
            ui->vLabel->clear();//清空
            mRtuThread->startThread();
            ui->startOrStop->setText("停止");
            ui->gBoxA->setEnabled(false);
            ui->gBoxC->setEnabled(false);
        } else {
            CriticalMsgBox box(this, tr("请打开串口"));
        }
    }
}

/**
 * @brief 设置值
 * @param row
 * @param column
 */
bool MainWindow::setItem(int row, int column , uint value, int rate)
{
    //int data = (valueH<<8 | valueD)/rate;
    QString str = QString("%1.%2").arg(value/rate).arg(value%rate);
    QTableWidgetItem *item = new QTableWidgetItem("Apple");
    item->setText(str);
    ui->tableWidget->setItem(row, column,item);
    return true;
}

void MainWindow::onGetShowData(int value, Rtu_recv *data)
{
    QReadLocker locker(&lock); //上锁
    int len = 0;
    if(data->dc) len = 9;
    else len = 4;
    ui->vLabel->setText(tr("v%1.%2").arg(data->version/10).arg(data->version%10));
    for(int i =0; i < len; i++){
        int j = 0;
        setItem(i, j++, data->data[i].cur, 10); //电流
         setItem(i, j++, data->data[i].vol, 1); //电压
         setItem(i, j++, data->data[i].ele, 10); //电能
         if(i < 3){
             setItem(i, j++, data->env[i].tem.value, 1); //温度
         }else
             j++;
         setItem(i, j++, data->data[i].pow, 1000); //功率

        setItem(i, j++, data->data[i].minCur, 10); //电流min
        setItem(i, j++, data->data[i].maxCur, 10); //电流max
        setItem(i, j++, data->data[i].minVol, 1); //电压min
        setItem(i, j++, data->data[i].maxVol, 1); //电压max

        if(i < 3){
            setItem(i, j++, data->env[i].tem.min, 1); //温度min
            setItem(i, j++, data->env[i].tem.max, 1); //温度max
        }else{
            j += 2;
        }

    }
}

void MainWindow::sleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MainWindow::on_readBox_clicked(bool checked)
{
    mRtuThread->setRead(checked);
    if(checked){
        if(mRtuThread->getRun()){
            ui->startOrStop->setText("停止");
        }else{
            ui->startOrStop->setText("读取");
        }
    }else{
        if(mRtuThread->getRun()){
            ui->startOrStop->setText("停止");
        }else{
            ui->startOrStop->setText("校准");
        }
    }
}

void MainWindow::onDebugEidt(QString str)
{
    ui->debugEdit->append(str);
}

void MainWindow::onEnd()
{
    bool en = ui->readBox->isChecked();
    on_readBox_clicked(en);
    ui->gBoxA->setEnabled(!mRtuThread->getRun());
    ui->gBoxC->setEnabled(!mRtuThread->getRun());
    // ui->debugEdit->clear();
}

void MainWindow::on_dcBox_currentIndexChanged(int index)
{
    mRtuThread->mDc =  index;
    if(index){
        ui->radio->hide();
    }else{
        ui->radio->show();
    }
}

void MainWindow::on_tip_clicked()
{
    ui->debugEdit->clear();
}

void MainWindow::on_clearBtn_clicked()
{
    /* 01 10 10 39 00 F0  14 80 直流
       01 10 10 39 01 FF  CRC   交流*/
    if(mRtuThread->mSerialPortPlate->isOpened()) {
        if(mRtuThread->getRun()){
            CriticalMsgBox box(this, tr("请先停止其他操作"));
        }else{
            static uchar sentDat[8] =
            {0x01, 0x10, 0x10, 0x39, 0x00, 0xF0, 0x14, 0x80};

            if(mRtuThread->mDc){ //  1-交流
                onDebugEidt(tr("[地址%1_电能清零_交流]_开始").arg(ui->addr->text()));
                sentDat[4] = 0x01;
                sentDat[5] = 0xFF;
            }else{ //0-直流
                onDebugEidt(tr("[地址%1_电能清零_直流]_开始").arg(ui->addr->text()));
                sentDat[4] = 0x00;
                sentDat[5] = 0xF0;
            }
            sentDat[0] = ui->addr->text().toInt();
            ushort crc = CRC16_2((char*)sentDat, 6);
            sentDat[6] = (0xff)&(crc); /*低8位*/
            sentDat[7] = ((crc) >> 8); /*高8位*/
            mRtuThread->onWrite(sentDat, sizeof(sentDat));
            qDebug() << "send" << QByteArray((char*)sentDat, sizeof(sentDat)).toHex();
            if(mRtuThread->mDc){ //  1-交流
                onDebugEidt(tr("[地址%1_电能清零_交流]_已发送>[%2]").arg(ui->addr->text())
                            .arg(QString(QByteArray((char*)sentDat, sizeof(sentDat)).toHex())));
            }else{ //0-直流
                onDebugEidt(tr("[地址%1_电能清零_直流]_已发送>[%2]").arg(ui->addr->text())
                            .arg(QString(QByteArray((char*)sentDat, sizeof(sentDat)).toHex())));
            }

            sleep(500);
            uchar recvDat[20];
            int ret = 0;
            memset(recvDat,0,sizeof(recvDat));
            for(int i = 0; i < 8; i++){
                sleep(500);
                ret = mRtuThread->mSerialPortPlate->read(recvDat, 2);
                if(ret > 3) break;
            }
            qDebug() << "get3:" << QByteArray((char*)recvDat, ret).toHex();
            if(ret > 3) onDebugEidt(tr("[地址%1_电能清零]_完成").arg(mRtuThread->mAddr));
            else onDebugEidt(tr("[地址%1_电能清零]_无回复").arg(mRtuThread->mAddr));
            onDebugEidt(tr(" "));
        }
    } else {
        CriticalMsgBox box(this, tr("请打开串口"));
    }

}

void MainWindow::on_ChangeLineBtn_clicked()
{
    if(mRtuThread->mSerialPortPlate->isOpened()) {
        if(mRtuThread->getRun()){
            CriticalMsgBox box(this, tr("请先停止其他操作"));
        }else{
            static uchar sentDat[8] =
            {0x01, 0x10, 0x10, 0x41, 0x00, 0x00, 0x14, 0x80};

            sentDat[0] = ui->addr->text().toInt();
            ushort crc = CRC16_2((char*)sentDat, 6);
            sentDat[6] = (0xff)&(crc); /*低8位*/
            sentDat[7] = ((crc) >> 8); /*高8位*/
            mRtuThread->onWrite(sentDat, sizeof(sentDat));
            qDebug() << "send" << QByteArray((char*)sentDat, sizeof(sentDat)).toHex();

            onDebugEidt(tr("[%1_改变有线通信]_已发送>[%2]").arg(ui->addr->text())
                        .arg(QString(QByteArray((char*)sentDat, sizeof(sentDat)).toHex())));


            sleep(500);
            uchar recvDat[20];
            int ret = 0;
            memset(recvDat,0,sizeof(recvDat));
            for(int i = 0; i < 8; i++){
                sleep(500);
                ret = mRtuThread->mSerialPortPlate->read(recvDat, 2);
                if(ret > 3) break;
            }
            qDebug() << "get3:" << QByteArray((char*)recvDat, ret).toHex();
            if(ret > 3)
            {
                ui->Statuslab->setText(tr("改有线成功"));
                onDebugEidt(tr("[地址%1_改变有线通信]_完成").arg(mRtuThread->mAddr));
            }
            else
            {
                ui->Statuslab->setText(tr("改有线失败"));
                onDebugEidt(tr("[地址%1_改变有线通信]_无回复").arg(mRtuThread->mAddr));
            }
            onDebugEidt(tr(" "));
        }
    } else {
        CriticalMsgBox box(this, tr("请打开串口"));
    }
}

void MainWindow::on_ChangeWifiBtn_clicked()
{
    if(mRtuThread->mSerialPortPlate->isOpened()) {
        if(mRtuThread->getRun()){
            CriticalMsgBox box(this, tr("请先停止其他操作"));
        }else{
            static uchar sentDat[8] =
            {0x01, 0x10, 0x10, 0x41, 0x00, 0x01, 0x14, 0x80};

            sentDat[0] = ui->addr->text().toInt();
            ushort crc = CRC16_2((char*)sentDat, 6);
            sentDat[6] = (0xff)&(crc); /*低8位*/
            sentDat[7] = ((crc) >> 8); /*高8位*/
            mRtuThread->onWrite(sentDat, sizeof(sentDat));
            qDebug() << "send" << QByteArray((char*)sentDat, sizeof(sentDat)).toHex();

            onDebugEidt(tr("[%1_改变无线通信]_已发送>[%2]").arg(ui->addr->text())
                        .arg(QString(QByteArray((char*)sentDat, sizeof(sentDat)).toHex())));


            sleep(500);
            uchar recvDat[20];
            int ret = 0;
            memset(recvDat,0,sizeof(recvDat));
            for(int i = 0; i < 8; i++){
                sleep(500);
                ret = mRtuThread->mSerialPortPlate->read(recvDat, 2);
                if(ret > 3) break;
            }
            qDebug() << "get3:" << QByteArray((char*)recvDat, ret).toHex();
            if(ret > 3)
            {
                ui->Statuslab->setText(tr("改无线成功"));
                onDebugEidt(tr("[地址%1_改变无线通信]_完成").arg(mRtuThread->mAddr));
            }
            else
            {
                ui->Statuslab->setText(tr("改无线成功"));
                onDebugEidt(tr("[地址%1_改变无线通信]_无回复").arg(mRtuThread->mAddr));
            }
            onDebugEidt(tr(" "));
        }
    } else {
        CriticalMsgBox box(this, tr("请打开串口"));
    }
}
