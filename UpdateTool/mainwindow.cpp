#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <windows.h>
#include <QDebug>
#include<QFile>
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

    mIsOpenSerial = false;

    ui->label_8->hide();
    ui->spinBox->hide();
    ui->SetChannelBtn->hide();
    ui->Statuslabel->hide();

    ui->label_9->hide();
    ui->SetBaudBtn->hide();
    ui->StatusBaudlabel->hide();

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

    QString iniFile = QCoreApplication::applicationDirPath() + "/config.ini";
    //qDebug()<<iniFile;
    ini = new QSettings(iniFile, QSettings::IniFormat);
   if(isDirExist(iniFile ,ini))
    {
        ini->beginGroup("File");
        mUpdateFile = ini->value("FilePath").toString();
        ini->endGroup();

        ui->nameEdit->setText(mUpdateFile);
    }
}

/**
 * @brief 判断ini文件是否存在，不存在则创建
 * @param [in] fullPath 文件路径
 * @param [in&out] ini  文件设置
 * @return 存在或者创建成功返回true ， 创建失败返回false
 */
bool MainWindow::isDirExist(QString fullPath ,QSettings *ini)
{
    QFile isFile(fullPath);
    bool ok = isFile.exists();
    if( ok )
    {
      return true;
    }
    else
    {
      ini->setValue("/File/FilePath","");
      return true;
    }
    return false;
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

        int index = 0;
        switch (baud) {
        case 9600: index = 0; break;
        case 19200: index = 1; break;
        case 38400: index = 2; break;
        case 115200: index = 3; break;
        default:
            break;
        }
        ui->baudBox->setCurrentIndex(index);

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
       mIsOpenSerial =  myPort->openPort(portName, mCbaud);
    }else if(str == "关闭串口")
    {
        myPort->closePort(portName);
        mIsOpenSerial = false;
    }
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

    QString iniFile = QCoreApplication::applicationDirPath() + "/config.ini";

   if(isDirExist(iniFile ,ini))
    {
        ini->setValue("/File/FilePath",filename);
    }
    ui->nameEdit->setText(filename);
    mUpdateFile = filename;
}

/**
 * @brief MainWindow::on_startBtn_clicked  点击升级按钮
 */
void MainWindow::on_startBtn_clicked()
{
    ui->Statuslabel->hide();
    if(!mIsOpenSerial)   {QMessageBox::warning(this,tr("waring"),tr("请确认是否有打开串口"),tr("确定"));return;}
    if(ui->nameEdit->text().isEmpty()){QMessageBox::warning(this,tr("waring"),tr("请选择升级文件"),tr("确定"));return;}
    this->setEnabled(false);
    if(!ui->checkBox->isChecked()){ //从机没有进入升级模式
        int ret = 0 ;
        if(!sendUpdateCmd()) { this->setEnabled(true);  return; } //发送升级标志

        qDebug() <<"";
        qDebug() <<"";
        qDebug() <<"";
        qDebug() << QString("[>>↓↓↓↓↓↓↓ 定点升级%1 ↓↓↓↓↓↓↓<<]").arg(ui->addrEdit->text().toInt());

        int isPass = -1;//-1失败 1成功 0接收到其它的数据
        do {  //升级回应
            if(ret % 3 == 0) sendUpdateCmd(); //再次发送
            isPass = responseUpdate();
            if(isPass == 1)
             {
                if( ui->radioButton->isChecked())
                    sleep(1000);
                    break;  //收到回复 跳出循环
            }
            else if(isPass == 0)
            {ret = 0;}
            else qDebug() << "NG>" << ret;
            sleep(1*1000);
            ret++;
        } while (ret < 15);  //收到应答，立即向下执行，否则等待5s再向下执行 */

        if(!isPass) {
            QMessageBox::warning(this,tr("waring"),tr("请确定从机是否启动"),tr("确定"));
            this->setEnabled(true);
            return;
        } //十次依旧无结果
    }

    if(!mUpdateFile.isEmpty() && !ui->addrEdit->text().isEmpty())  //触发线程 发送文件
    {
        sendData->init(mUpdateFile, mCurrentPort,ui->addrEdit->text().toInt() , ui->radioButton->isChecked());
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
int MainWindow::responseUpdate()
{
    QString responseStr1 = tr("Start Updata");
    QString responseStr2 = tr("Start Updat");

    int buad;
    if(!mCurrentPort.isEmpty() && myPort->portIsOpen(mCurrentPort, buad))
    {
        QByteArray array = myPort->readData(mCurrentPort);
        qDebug() << "Data1" << array.toHex();
        QString str = QString(array);
        if(str.contains(responseStr1)||str.contains(responseStr2))
            return 1;
        if(!str.isEmpty())
            return 0;
    }
    return -1;
}

/**
 * @brief MainWindow::sendFile  发送文件
 */
void MainWindow::sendFile()
{
    QFile file(mUpdateFile);

    int packetNum = getPacketNum(file.size()) ;
    //qDebug() << "packet num" << packetNum;  //包数

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
    if(ui->addrEdit->text().isEmpty()) //升级文件是否打开
    {
        QMessageBox::warning(this,tr("warning"),tr("没有填写地址"),tr("确定"));
        return;
    }



    QTextStream in(&file);  //字符流可能有问题  -- 改 QDataStream 或直接 QFile
    int ret = 0; //第几包
    bool recvsuccessful = false; //数据包接收成功

    while (!in.atEnd())
    {
        QByteArray array;
        uchar addr = 0;
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
        else{
        QString responseStr = QString("successful");
        if(num > 42) {
           if(str.contains(responseStr))return true;
        }
        if(str == responseStr)
            return true;
        }
    }
    return false;
}

void MainWindow::on_pushButton_clicked() //批量
{
    ui->Statuslabel->hide();
    if(!mIsOpenSerial)   {QMessageBox::warning(this,tr("waring"),tr("请确认是否有打开串口"),tr("确定"));return;}
    if(ui->nameEdit->text().isEmpty()){QMessageBox::warning(this,tr("waring"),tr("请选择升级文件"),tr("确定"));return;}
    this->setEnabled(false);

    //  if(ui->lEditMin->text().isDetached())

    if(!mUpdateFile.isEmpty() && !ui->lEditMin->text().isEmpty() && !ui->lEditMax->text().isEmpty())  //触发线程 发送文件
    {
        sendDataAll->init(mUpdateFile, mCurrentPort,ui->lEditMin->text().toInt(), ui->lEditMax->text().toInt() , ui->radioButton->isChecked());
        sendDataAll->start();  //不能点第二下  待优化
    }else{
        QMessageBox::warning(this,tr("waring"),tr("上下地址或升级文件有空白"),tr("确定"));
        this->setEnabled(true);
    }
}

void MainWindow::on_baudBox_currentIndexChanged(int index) //botelv
{
    switch (index) {
    case 0: mCbaud = QSerialPort::Baud9600; break;
    case 1: mCbaud = QSerialPort::Baud19200; break;
    case 2: mCbaud = QSerialPort::Baud38400; break;
    case 3: mCbaud = QSerialPort::Baud115200; break;
    default:
        break;
    }
}

void MainWindow::on_radioButton_clicked()
{
    if(ui->radioButton->isChecked())
    {
        ui->label_8->show();
        ui->spinBox->show();
        ui->SetChannelBtn->show();

        ui->label_9->show();
        ui->SetBaudBtn->show();
    }
    else
    {
        ui->label_8->hide();
        ui->spinBox->hide();
        ui->SetChannelBtn->hide();
        ui->Statuslabel->hide();

        ui->label_9->hide();
        ui->SetBaudBtn->hide();
        ui->StatusBaudlabel->hide();
    }
}

void MainWindow::on_SetChannelBtn_clicked()
{
    ui->Statuslabel->hide();
    ui->StatusBaudlabel->hide();
    if(!mIsOpenSerial)   {QMessageBox::warning(this,tr("waring"),tr("请确认是否有打开串口"),tr("确定"));return;}
    if(ui->spinBox->value() < 398 || ui->spinBox->value() > 525)
    {
        QMessageBox::warning(this,tr("information"),tr("通道不能小于398MHz或者大于525MHz"),tr("确定"));
        return;
    }
    QString  str("");
    bool flag = false;
    bool channelOrbaud = true;//true: channel false: baud
    int i = 0;//发送次数计数
    int times = 2;//重复发送次数
    int steps = 0;//计算步骤数目

    steps ++;
    while(i < times)//重复发两次
    {
        SetChannelSendarray("+++");//发送"+++"
        if(!SetChannelRecvarray("a")) //接收"a"
        {
            if(i == 1)
            {
                QMessageBox::warning(this,tr("information"),tr("可能串口线松了。"),tr("确定"));
                SetChannelStatus(channelOrbaud ,flag , steps);
                return;
            }
        }
        else
            flag = true;
        if(flag) break;
        i++;
    }

    i= 0;//重置计数
    flag = false;//重置标志
    steps ++;//步骤次数累加
    while(i < times)
    {
        SetChannelSendarray("a");//发送"a"
        if(!SetChannelRecvarray("OK")) //接收"+OK"
        {
            if(i == 1)
            {
                QMessageBox::warning(this,tr("information"),tr("可能串口线松了。"),tr("确定"));
                SetChannelStatus(channelOrbaud , flag, steps);
                return ;
            }
        }
        else
            flag = true;
        if(flag) break;
        i++;
    }
    i= 0;
    flag = false;
    steps ++;
    while(i < times)
    {
        str = QString("AT+CH=%1\r\n").arg(ui->spinBox->value() - 398);
        SetChannelSendarray(str);//发送"AT+CH=398\r\n"
        if(!SetChannelRecvarray("OK")) //接收"OK"
        {
            if(i == 1)
            {
                QMessageBox::warning(this,tr("information"),tr("可能串口线松了。"),tr("确定"));
                SetChannelStatus(channelOrbaud , flag, steps);
                return;
            }
        }
        else
            flag = true;
        if(flag) break;
        i++;
    }



    i= 0;
    flag = false;
    steps++;
    while(i < times)
    {
        str = QString("AT+Z\r\n");
        SetChannelSendarray(str);//发送"AT+CH=398\r\n"
        if(!SetChannelRecvarray("OK")) //接收"OK"
        {
            flag = false;
            if(i == 1)
            {
                QMessageBox::warning(this,tr("information"),tr("可能串口线松了。"),tr("确定"));
                SetChannelStatus(channelOrbaud , flag ,steps);//失败时，设置状态
                return;
            }
        }
        else
            flag = true;
        if(flag) break;
        i++;
    }

    SetChannelStatus(channelOrbaud , flag ,steps);//成功时，设置状态
}


void MainWindow::SetChannelSendarray(QString str)
{
    QByteArray sendarray = str.toLatin1();
    int buad;
    if(!mCurrentPort.isEmpty() && myPort->portIsOpen(mCurrentPort,buad))
        myPort->sendData(sendarray,mCurrentPort);
    sleep(200);
}

bool MainWindow::SetChannelRecvarray(QString comparestr)
{
    int buad;
    if(!mCurrentPort.isEmpty() && myPort->portIsOpen(mCurrentPort, buad))
    {
        QByteArray recvarray = myPort->readData(mCurrentPort);

        QString str = QString(recvarray);
        QString pattern = ".*"+comparestr+".*";
        QRegExp rx(pattern);
        bool match = rx.exactMatch(str);
        if(match)
        {
            qDebug() << "[Get正确数据:]_" << str;
            sleep(200);
            return true;
        }
        else
        {
            if(!str.isEmpty())
                qDebug() << "[Get错误数据:]_" << str << QString("参考数据")+comparestr;
            else
            {
                qDebug() << "[Get数据失败:]_" << "空";
            }
        }
    }
    sleep(200);
    return false;
}

void MainWindow::SetChannelStatus(bool channelOrbaud , bool flag , int steps)
{


    if(flag && steps == 4)
     {
        if(channelOrbaud)
        {
            ui->Statuslabel->show();
            ui->Statuslabel->setText(tr("成功"));
            ui->Statuslabel->setStyleSheet("color:green");
        }
        else
        {
            ui->StatusBaudlabel->show();
            ui->StatusBaudlabel->setText(tr("成功"));
            ui->StatusBaudlabel->setStyleSheet("color:green");
        }
    }
    else if(!flag)
    {
        if(channelOrbaud)
        {
            ui->Statuslabel->show();
            ui->Statuslabel->setText(tr("失败"));
            ui->Statuslabel->setStyleSheet("color:red");
        }
        else
        {
            ui->StatusBaudlabel->show();
            ui->StatusBaudlabel->setText(tr("失败"));
            ui->StatusBaudlabel->setStyleSheet("color:red");
        }
    }
}

void MainWindow::on_SetBaudBtn_clicked()
{
    ui->StatusBaudlabel->hide();
    if(!mIsOpenSerial)   {QMessageBox::warning(this,tr("waring"),tr("请确认是否有打开串口"),tr("确定"));return;}

    QString  str("");
    bool flag = false;
    bool channelOrbaud = false;//true: channel false: baud
    int i = 0;//发送次数计数
    int times = 2;//重复发送次数
    int steps = 0;//计算步骤数目

    steps ++;
    while(i < times)//重复发两次
    {
        SetChannelSendarray("+++");//发送"+++"
        if(!SetChannelRecvarray("a")) //接收"a"
        {
            if(i == 1)
            {
                QMessageBox::warning(this,tr("information"),tr("可能串口线松了。"),tr("确定"));
                SetChannelStatus(channelOrbaud , flag , steps);
                return;
            }
        }
        else
            flag = true;
        if(flag) break;
        i++;
    }

    i= 0;//重置计数
    flag = false;//重置标志
    steps ++;//步骤次数累加
    while(i < times)
    {
        SetChannelSendarray("a");//发送"a"
        if(!SetChannelRecvarray("OK")) //接收"+OK"
        {
            if(i == 1)
            {
                QMessageBox::warning(this,tr("information"),tr("可能串口线松了。"),tr("确定"));
                SetChannelStatus(channelOrbaud , flag, steps);
                return ;
            }
        }
        else
            flag = true;
        if(flag) break;
        i++;
    }

    i= 0;
    flag = false;
    steps ++;
    while(i < times)
    {
        str = QString("AT+UART=9600,8,1,NONE,NFC\r\n");
        SetChannelSendarray(str);//发送"AT+CH=398\r\n"
        if(!SetChannelRecvarray("OK")) //接收"OK"
        {
            if(i == 1)
            {
                QMessageBox::warning(this,tr("information"),tr("可能串口线松了。"),tr("确定"));
                SetChannelStatus(channelOrbaud , flag, steps);
                return;
            }
        }
        else
            flag = true;
        if(flag) break;
        i++;
    }



    i= 0;
    flag = false;
    steps++;
    while(i < times)
    {
        str = QString("AT+Z\r\n");
        SetChannelSendarray(str);//发送"AT+CH=398\r\n"
        if(!SetChannelRecvarray("OK")) //接收"OK"
        {
            flag = false;
            if(i == 1)
            {
                QMessageBox::warning(this,tr("information"),tr("可能串口线松了。"),tr("确定"));
                SetChannelStatus(channelOrbaud , flag ,steps);//失败时，设置状态
                return;
            }
        }
        else
            flag = true;
        if(flag) break;
        i++;
    }

    SetChannelStatus(channelOrbaud , flag ,steps);//成功时，设置状态
}
