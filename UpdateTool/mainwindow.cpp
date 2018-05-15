#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <windows.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    myPort = new MySeriorport;
    sendData = new SendData(myPort);
    connect(sendData, SIGNAL(sendOk(int)), this, SLOT(onOSendOkSlot(int)));
    connect(sendData, SIGNAL(sendProgress(QString)), this, SLOT(onProgressSlot(QString))); //进度条

    sendDataAll = new SendDataAll(myPort);
    connect(sendDataAll, SIGNAL(sendOk(int)), this, SLOT(onOSendAllOkSlot(int)));
    connect(sendDataAll, SIGNAL(sendProgress(QString)), this, SLOT(onProgressAllSlot(QString))); //tiaoshi
    connect(sendDataAll, SIGNAL(sendProgress2(QString)), this, SLOT(onProgressAllSlot2(QString))); //进度条

    QStringList portList = myPort->initSeriorPortInfos();
    initPortCombox(portList); //初始化串口信息  -- 初始化界面

    //--- 没必要的功能
    ui->checkBox->hide();
    mCbaud = QSerialPort::Baud9600;

}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief 刷新串口信息
 */
void MainWindow::initPortCombox(QStringList &portList)
{
    QString portName = ui->comboBox_port->currentText();

    ui->comboBox_port->clear();
    int ret = portList.size();

    if(ret)
    {
        //重新设置combox
        for(int i = 0 ; i < ret ; i++)
        {
            QString str = portList.at(i);
            //            qDebug() << "str:" <<str;
            ui->comboBox_port->insertItem(i,str);
        }

        //如果刷新后刷新前当前项仍存在，那么切换到该项，如果已不存在，那么切换到第一项
        if(portList.contains(portName))  //如果portlist含有之前的串口
        {
            int index = getIndex(portList,portName);
            qDebug() << "index:" << index;
            if(index != -1)
            {
                ui->comboBox_port->setCurrentIndex(index);
            }else
            {
                qDebug() << "串口不存在!!!";
            }
        }else
        {
            qDebug()<< "该串口已不存在";
            ui->comboBox_port->setCurrentText(0);
        }

    }else
    {
        qDebug() << "当前无串口连接";
        ui->pushButton_port->setText(tr(""));
    }
    //刷新串口信息，将会触发combox的index变化，将在对应槽函数中刷新buttontext；
}

/**
 * @brief 点击按钮刷新串口信息
 */
void MainWindow::on_pushButton_12_clicked()
{
    QStringList portList = myPort->updataSeriorPortinfos();
    initPortCombox(portList);
}

int MainWindow::getIndex(QStringList &list,QString &str)
{
    if(list.contains(str))
    {
        for(int i = 0 ; i < list.size() ;i++)
        {
            if(list.at(i) == str)
            {
                return i;
            }
        }
    }
    return -1;
}

/**
 * @brief 初始化按钮文字
 */
void MainWindow::initButtonText()
{
    QString str = ui->comboBox_port->currentText();

    if(!str.isEmpty())
    {
        int baud;
        if(myPort->portIsOpen(str,baud))
        {
            ui->pushButton_port->setText(tr("关闭串口"));
        }else
            ui->pushButton_port->setText(tr("打开串口"));

        if(9600 == baud){
            ui->baudBox->setCurrentIndex(0);
        }else if(19200 == baud){
            ui->baudBox->setCurrentIndex(1);
        }else if(38400 == baud){
            ui->baudBox->setCurrentIndex(2);
        }

        qDebug() << baud;
    }else
        ui->pushButton_port->setText(tr(""));
}

void MainWindow::on_comboBox_port_currentIndexChanged(int index)
{

    mCurrentPort = ui->comboBox_port->currentText();
    if(!mCurrentPort.isEmpty())
    {
        initButtonText();
        myPort->viewAllPortStatus();
    }
}

/**
 * @brief 打开或者关闭串口
 */
void MainWindow::on_pushButton_port_clicked()
{
    QString str = ui->pushButton_port->text();
    QString portName = ui->comboBox_port->currentText();
    if(str == "打开串口")
    {
        myPort->openPort(portName, mCbaud);
    }else if(str == "关闭串口")
        myPort->closePort(portName);
    else
        qDebug() << "当前串口不存在！";

    initButtonText();
}

void MainWindow::sleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

/**
 * @brief MainWindow::on_selectfileBtn_clicked  选择升级文件
 */
void MainWindow::on_selectfileBtn_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "bin/",
                                                    tr("Images (*.bin)"));
    ui->nameEdit->setText(filename);
    mUpdateFile = filename;
}

/**
 * @brief MainWindow::on_startBtn_clicked  点击升级按钮
 */
void MainWindow::on_startBtn_clicked()
{
    this->setEnabled(false);
    if(!ui->checkBox->isChecked()){ //从机没有进入升级模式
        int ret = 0 ;
        if(!sendUpdateCmd()) { this->setEnabled(true);  return; } //发送升级标志

        qDebug() <<"";
        qDebug() <<"";
        qDebug() <<"";
        qDebug() << QString("[>>↓↓↓↓↓↓↓ 定点升级%1 ↓↓↓↓↓↓↓<<]").arg(ui->addrEdit->text().toInt());

        bool isPass = false;
        do {  //升级回应
            if(ret == 3 || ret == 6) sendUpdateCmd(); //再次发送
            isPass = responseUpdate();
            if(isPass)  break;  //收到回复 跳出循环
            else qDebug() << "NG>" << ret;
            sleep(1*1000);
            ret++;
        } while (ret < 10);  //收到应答，立即向下执行，否则等待5s再向下执行 */

        if(!isPass) {
             QMessageBox::warning(this,tr("waring"),tr("请确定从机是否启动"),tr("确定"));
            this->setEnabled(true);
            return;
        } //十次依旧无结果
    }

    if(!mUpdateFile.isEmpty() && !ui->addrEdit->text().isEmpty())  //触发线程 发送文件
    {
        sendData->init(mUpdateFile, mCurrentPort,ui->addrEdit->text().toInt());
        sendData->start();  //不能点第二下  待优化

    }

}

void MainWindow::onOSendOkSlot(int value)
{
    this->setEnabled(true);
    if(value){ //传输OK
         QMessageBox::information(this,tr("information"),tr("软件升级完毕！"),tr("确定"));
    }else{  //传输NG
         QMessageBox::warning(this,tr("information"),tr("数据发送可能存在丢失，请检查！"),tr("确定"));
    }
}

void MainWindow::onProgressSlot(QString progress)
{
    ui->showProgress->setText(progress);
}

void MainWindow::onOSendAllOkSlot(int)
{
    this->setEnabled(true);
    QMessageBox::information(this,tr("information"),tr("批处理执行完毕！"),tr("确定"));
}

void MainWindow::onProgressAllSlot(QString str)
{
    ui->textEdit->append(str);
}

void MainWindow::onProgressAllSlot2(QString str)
{
    ui->labelS->setText(str);
}

/**
 * @brief MainWindow::sendUpdateCmd  发送升级命令
 */
bool MainWindow::sendUpdateCmd()  //bool型
{
    QByteArray array;
    uchar addr;

    if(!ui->addrEdit->text().isEmpty())
        addr = uchar(ui->addrEdit->text().toInt());
    else{
        QMessageBox::warning(this,tr("waring"),tr("请选择执行板地址"),tr("确定"));
        return false;
        }

    send_to_packet(addr,array); //打包到array
    qDebug() << "initData" << array.toHex();

    int buad;
    if(!mCurrentPort.isEmpty() && myPort->portIsOpen(mCurrentPort,buad))
        myPort->sendData(array,mCurrentPort);

    return true;
}

/**
 * @brief MainWindow::responseUpdate  升级命令回应
 */
bool MainWindow::responseUpdate()
{
    QString responseStr = tr("Start Updata");

    int buad;
    if(!mCurrentPort.isEmpty() && myPort->portIsOpen(mCurrentPort, buad))
    {
        QByteArray array = myPort->readData(mCurrentPort);
        qDebug() << "Data1" << array.toHex();
        QString str = QString(array);
        if(str == responseStr)
            return true;
    }
    return false;
}

/**
 * @brief MainWindow::sendFile  发送文件
 */
void MainWindow::sendFile()
{
    QFile file(mUpdateFile);

    int packetNum = getPacketNum(file.size()) ;
    qDebug() << "packet num" << packetNum;  //包数

    if(packetNum == 0)  //升级文件是否为空
    {
        QMessageBox::warning(this,tr("waring"),tr("升级文件为空，请重新选择！"),tr("确定"));
        return;
    }

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) //升级文件是否打开
    {
        QMessageBox::warning(this,tr("warning"),tr("文件打开失败"),tr("确定"));
        return;
    }



    QTextStream in(&file);  //字符流可能有问题  -- 改 QDataStream 或直接 QFile
    int ret = 0; //第几包
    bool recvsuccessful = false; //数据包接收成功

    while (!in.atEnd())
    {
        QByteArray array;
        uchar addr;
        ret++;
        ret = packetNum;
        if(ret == packetNum){  //最后一包
            int len = file.size() % TEXT_MAX_LEN; //取余
            QByteArray da;
            da = file.read(len);
            //da= str.toLatin1();
            char *data =  da.data();
            if(!ui->addrEdit->text().isEmpty())
                addr = uchar(ui->addrEdit->text().toInt());
            text_send_packet(addr ,data,array, da.size());  //组装数据

        }else{


            //QString str=in.read(TEXT_MAX_LEN);//读取长度为TEXT_MAX_LEN的内容
            QByteArray da;
            da = file.read(TEXT_MAX_LEN);
            //da= str.toLatin1();
            char *data =  da.data();
            if(!ui->addrEdit->text().isEmpty())
                addr = uchar(ui->addrEdit->text().toInt());
            text_send_packet(addr ,data,array);  //组装数据
        }


        QString info = QString("packet %1 size is %2").arg(ret).arg(array.size());
        qDebug() << info;

        int buad;
        do {
            if(!mCurrentPort.isEmpty() && myPort->portIsOpen(mCurrentPort, buad))
                myPort->sendData(array,mCurrentPort);

            qDebug() << "End:"  <<array.toHex();

            int tick = 0;//用于延时
            do {
                tick++;
                if(responseSendFile(ret) == true)
                {
                    recvsuccessful = true; //接收成功
                    break;
                }else {
                    qDebug() << "responseSendFile err" << tick;
                }
                sleep(1*1000);
            } while (tick < 3);

        } while (!recvsuccessful); //发送数据包，直到该数据包接收成功
    }

    if(ret >= packetNum)
        QMessageBox::information(this,tr("information"),tr("软件升级完毕！"),tr("确定"));
    else
        QMessageBox::warning(this,tr("information"),tr("数据发送可能存在丢失，请检查！"),tr("确定"));
}

/**
 * @brief MainWindow::getPacketNum 获取文件数据包个数
 * @param bytes 该文件大小(字节总数)
 * @return
 */
int MainWindow::getPacketNum(int bytes)
{
    qDebug() << "file size"<< bytes;
    int num = 0 ;

    if(bytes > 0)
        num = bytes/TEXT_MAX_LEN;

    if(bytes%TEXT_MAX_LEN != 0)
        num++;

    return num;
}

void MainWindow::closeEvent(QCloseEvent *) //退出事件
{
     /*sendData->wait();
     Sleep(20);
     delete sendData; */
}

/**
 * @brief MainWindow::responseSendFile 发送数据包接收成功应答
 * @param num
 */
bool MainWindow::responseSendFile(int num)
{
    QString responseStr = QString("Receive Packet %1 successful").arg(num);
    int buad;
    if(!mCurrentPort.isEmpty() && myPort->portIsOpen(mCurrentPort, buad))
    {
        QByteArray array = myPort->readData(mCurrentPort);
        qDebug() << "getData:" << array.toHex();
        QString str = QString(array);
        if(str == responseStr)
            return true;
    }
    return false;
}

void MainWindow::on_pushButton_clicked() //批量
{
    this->setEnabled(false);

  //  if(ui->lEditMin->text().isDetached())

    if(!mUpdateFile.isEmpty() && !ui->lEditMin->text().isEmpty() && !ui->lEditMax->text().isEmpty())  //触发线程 发送文件
    {
        sendDataAll->init(mUpdateFile, mCurrentPort,ui->lEditMin->text().toInt(), ui->lEditMax->text().toInt());
        sendDataAll->start();  //不能点第二下  待优化
    }else{
        QMessageBox::warning(this,tr("waring"),tr("上下地址或升级文件有空白"),tr("确定"));
        this->setEnabled(true);
    }
}

void MainWindow::on_baudBox_currentIndexChanged(int index) //botelv
{
    //mCbaud
    if(0 == index){
        mCbaud = QSerialPort::Baud9600;
    }else if(1 == index){
        mCbaud = QSerialPort::Baud19200;
    }else if(2 == index){
        mCbaud = QSerialPort::Baud38400;
    }
  //  qDebug() << "out";
}
