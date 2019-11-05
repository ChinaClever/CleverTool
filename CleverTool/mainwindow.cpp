#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"datadefine.h"
#include<qtimer.h>
#include <QSettings>
#include <QTime>

#define isSet 1

static returntableData returnData;
static collectedData sentData;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    cSUM = 7;
    initText();
    initData();
    port = new SerialportOperate;
    initGroupboxThree(0);
    initTablewidget();
    initComboxDa(0);
    timer2 = new QTimer(this);
    connect(timer2, SIGNAL(timeout()), this, SLOT(ontimeout()));


    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeoutDone()));
    timer->start(1*1000);

    updateStateAndButton();

    //去掉不用的条目
    ui->groupBox_2->setDisabled(true);
    ui->pushButton_3->setText(tr("电压电流校准"));
    ui->groupBox->setTitle("电压电流校准");
    ui->pushButton_3->setMinimumSize(110,30);
    ui->pushButton_3->setGeometry(300,120,110,30);

    onInitIni();
    ui->comboBox_3->setCurrentIndex(1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief combox初始化串口信息
 */
void MainWindow::initComboxDa( int flag)
{
    QStringList list;
    list.clear();
    ui->comboBox->clear();
    if(flag == 0)
        list = port->initPortInfo();
    else if (flag == 1)
        list = port->readPortInfo();
    for(int i=0;i < list.size();i++)
    {
        QString str;
        str = list.at(i);
        ui->comboBox->addItem(str);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    QString portName = ui->comboBox->currentText();
    if(ui->pushButton_2->text() == "打开串口")
    {
        port->enablePort(portName);
    }
    else if (ui->pushButton_2->text() == "关闭串口")
    {
        port->disablePort(portName);
    }
    updateStateAndButton();
    //--------------[发送]----------
    on_comboBox_3_currentIndexChanged(1);
}

/**
 * @brief combox当前值改变，修改状态栏以及按钮text
 * @param arg1
 */
void MainWindow::on_comboBox_currentTextChanged(const QString &)
{
    //    port->disablePort();//关闭之前的串口
    updateStateAndButton();
}


/**
 * @brief 刷新状态栏以及按钮
 */
void MainWindow::updateStateAndButton()
{
    mCurrentPortName = ui->comboBox->currentText();
    //    qDebug()<<"串口状态："<<mCurrentPortName<<port->checkPortState(mCurrentPortName);
    if(port->checkPortState(mCurrentPortName))
    {
        ui->pushButton_2->setText("关闭串口");
        ui->label_2->setText(QString("串口%1").arg(mCurrentPortName)+"已开启");
    }else
    {
        ui->pushButton_2->setText("打开串口");
        ui->label_2->setText(QString("串口%1").arg(mCurrentPortName)+"已关闭");
    }
    port->checkAllState();
}

void MainWindow::sleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}


/**
 * @brief 定时刷新串口
 */
void MainWindow::timeoutDone()
{
    int index = ui->comboBox->currentIndex();
    //    initComboxDa(1);
    if(ui->comboBox->count() > index)
        ui->comboBox->setCurrentIndex(index);
    else
        ui->comboBox->setCurrentIndex(0);

    readAnswer();  //读取数据

    if( mflag == 2 ) //如果点击采集按钮，那么循环时开始发送采集指令
        collectLoopDone();
}

/**
 * @brief 地址码切换,刷新发送命令及校验码
 * @param index
 */
void MainWindow::on_comboBox_2_currentIndexChanged(int index)
{
    mAddr = index + 1;  //当前地址码
    initGroupboxOne(index);
    initGroupboxTwo(index);
    initGroupboxThree(index);

    //--------------[发送]----------
    on_comboBox_3_currentIndexChanged(1);
}

void MainWindow::initGroupboxOne(int index)
{
    switch (index) {
    case 0:
        ui->lineEdit_3->setText(tr("04"));
        break;
    case 1:
        ui->lineEdit_3->setText(tr("07"));
        break;
    case 2:
        ui->lineEdit_3->setText(tr("06"));
        break;
    case 3:
        ui->lineEdit_3->setText(tr("01"));
        break;

    }
}

void MainWindow::initGroupboxTwo(int index)
{
    switch (index) {
    case 0:
        ui->lineEdit_6->setText(tr("07"));
        break;
    case 1:
        ui->lineEdit_6->setText(tr("04"));
        break;
    case 2:
        ui->lineEdit_6->setText(tr("05"));
        break;
    case 3:
        ui->lineEdit_6->setText(tr("02"));
        break;

    }
}

void MainWindow::initGroupboxThree(int index)
{
    ui->lineEdit_8->clear();
    quint8 addr = index + 1;
    quint8 flag = 0xA2;
    quint8 onbuf[8];//发送采集命令时，这两个参数无用
    quint8 offbuf[8];
    memset(onbuf,0,sizeof(onbuf));
    memset(offbuf,0,sizeof(offbuf));
    bool symbol = false; //表示此时发送采集命令

    sentDataToPacket(addr, flag,onbuf,offbuf ,symbol);
    QString str = quintToStr(sendData,sentData.length);
    ui->lineEdit_8->setText(str);

}

/**
 * @brief 增益校准按钮
 */
void MainWindow::on_pushButton_3_clicked()
{
    if(!ui->pushButton_5->isEnabled()) on_pushButton_6_clicked(); //如果是采集状态， 停止采集

    ui->pushButton_3->setEnabled(false);
    //    str.clear();
    //    is_read = false;
    if(mflag == 2)
        on_pushButton_6_clicked();//改变了mflag为3
    //    mflag = 0;

    mflag = 0;

    if(is_read && port->checkIsOpen(mCurrentPortName))
    {
        is_read = false;
        sendCmd(mflag);
        //        ui->label_11->setMovie(movie);
        //        movie->start();
    }
    else if(!port->checkIsOpen(mCurrentPortName))
    {
        QString warningstr = QObject::tr("串口%1未打开").arg(mCurrentPortName);
        QMessageBox::information(this,QObject::tr("Information"),warningstr,QObject::tr("确定"));
    }

}

/**
 * @brief 初始化标志位
 */
void MainWindow::initData()
{
    switchFlag = false;
    switchNum = 9 ;
    mCurrentButtonRow = 9;
    mCurrentClearButtonRow = 9;
    mflag = 3;
    is_gather = true;
    is_once = true;
    is_read = true; //默认已读，才能保证发送
    is_ele = true;
    mAddr = 1;
    //    mTimer = new QTimer(this);
    //    movie = new QMovie(":/new/prefix1/image/camEffectTarget.gif");

    if(isSet){
        pass = 0;
        for(int j = 0 ; j < 8 ;j++){
                init[j] = 1;
                onbuffer[j] = 0;
                offbuffer[j] = 1;
        }
    }

}


/**
 * @brief 相位校准按钮
 */
void MainWindow::on_pushButton_4_clicked()
{
    //    is_read = false;
    if(mflag == 2)
        on_pushButton_6_clicked();//改变了mflag为3

    mflag = 1;

    if(is_read && port->checkIsOpen(mCurrentPortName))
    {
        is_read = false;
        sendCmd(mflag);

    }
    else if(!port->checkIsOpen(mCurrentPortName))
    {
        QString warningstr = QObject::tr("串口%1未打开").arg(mCurrentPortName);
        QMessageBox::information(this,QObject::tr("Information"),warningstr,QObject::tr("确定"));
    }


}

/**
 * @brief 发送命令
 * @param flag
 */
void MainWindow::sendCmd(int flag)
{
    qDebug()<<"校准命令已发送";
    quint8 buf[6];
    memset(buf,0,sizeof(buf));
    //    qDebug()<<"cmd:"<<mAddr;
    if(flag == 0)  //增益校准
        for(int i = 0; i < 6; i++)
        {

            buf[i] = transCmd[mAddr-1][i];
            //            qDebug("buf:%x",buf[i]);
        }
    else if(flag == 1)  //相位校准
        for(int i = 0; i < 6; i++)
        {

            buf[i] = phaseCmd[mAddr-1][i];
            //        qDebug()<<"transCmd:"<<transCmd[1][1];
            //            printf("%x",transCmd[1][0]);
            //            qDebug("buf:%x",buf[i]);
        }

    port->sendDataToPort(mCurrentPortName,buf,sizeof(buf));
}

/**
 * @brief 从串口读取数据显示,根据mflag标志决定数据流向
 */
void MainWindow::readAnswer()
{
   // qDebug()<<"判断uuuu mflg："<<mflag;

    QByteArray answer;
    answer.clear();
    answer = port->readDataFromPort(mCurrentPortName);
   // qDebug()<<"answer -len:"<<answer.length();

    qDebug() << "get:" << answer.toHex();
   // qDebug() << "mflag"<< mflag << "is_gather"<< is_gather << "is_ele"<< is_ele;
    quint8 data[BUFSIZ];
    QString str;

    memset(data,0,sizeof(data));
    for(int i = 0; i < answer.length() ; i++)
    {
        data[i] = answer.at(i);
        //        qDebug("data:%x",data[i]);
        str.append(QString::number(data[i],16));
        str.append(" ");
    }

    if(!answer.isEmpty())
    {
        is_read = true;  //将读取数据标志位置1

        switch (mflag)
        {
        case 0:
            updateGroupboxOne(data,mflag,answer.length()-1,answer.length());
            //qDebug()<<"updateGroupboxOne()";
            break;
        case 1:
            updateGroupboxTwo(data,mflag,answer.length()-1,answer.length());
            break;
        case 2:  //当数据为采集信息数据时，需要判断是显示到采集应答还是表格
            if(is_ele)
            {
              // qDebug()<<"is_gather";
                updateGroupboxThree(data,answer.length());


//               qDebug()<<"is_not_gather";
//                                QString str = quintToStr(data, answer.length());
//                                ui->lineEdit_11->setText(str);

                returnToPacket(answer);
                updataTableData();
            }

            break;
        }
    }
    else //数据未收到，如果is_read一直为false，那么后面数据将一直无法发送，所以延迟置1，延迟时间内无法发送
    {

       // qDebug()<<"数据未收到";

        if(!port->checkIsOpen(mCurrentPortName) && mflag !=3)
        {
            QTimer::singleShot(2*1000 , this , SLOT(delaySetTrue()));  //3s延时置1
        }

    }

}

/**
 * @brief 针对表格数据更新
 */
void MainWindow::readTableAnswer()
{
  //  qDebug()<<"判断mflg："<<mflag;

    QByteArray answer;

    answer.clear();

    if(port->checkIsOpen(mCurrentPortName))
        answer= port->readDataFromPort(mCurrentPortName);

   // qDebug()<<"answer -len:"<<answer.length();

    quint8 data[30];
    QString str;

    memset(data,0,sizeof(data));
    for(int i = 0; i < answer.length() ; i++)
    {
        data[i] = answer.at(i);
     //   qDebug("data:%x",data[i]);
        str.append(QString::number(data[i],16));
        str.append(" ");
    }

    if(!answer.isEmpty() && mflag == 2)
    {
        if(is_gather)
        {
            updateGroupboxThree(data,answer.length());
        }
        else
        {
            returnToPacket(answer);
            updataTableData();
        }
    }

}

/**
 * @brief 获取异或校验码,主要针对于返回值校验码，最后一位为校验码，所以要减1
 */
quint8 MainWindow::getXorNumber(quint8 *data,int len)
{

    quint8 xorNumber = 0;
    for(int i = 0;i<len ;i++)
    {
        //        qDebug() <<"数据长度："<<sizeof(data);
        xorNumber ^= *(data+i);
        //        qDebug("*data:%x",*(data+i));
    }
    //    qDebug("校验码：%x",xorNumber);
    return xorNumber;

}


/**
 * @brief 应答是否正确,分别为要校验的数据，标志位1代表增益校准，2代表相位校准，长度为数据长度，防止sizeof中碰到0
 */
bool MainWindow::responseIsRight(quint8 *data ,int flag ,int length)
{

    //mAddr  sendCmd xor
    dataTopacket(data);

    if((getXorNumber(data,length) == recv.xornumber) && cmdIsRight(flag))
    {
        return true;
    }
    //    qDebug()<<"cmdIsRight:"<<cmdIsRight(flag);
    return false;

}

/**
 * @brief flag为1表示增益校准，为2表示相位校准
 * @param data
 * @param flag
 * @return
 */
bool MainWindow::cmdIsRight(int flag)
{
    if(recv.addr == mAddr)
    {
        for(int i = 0; i <5 ;i++)
        {
            //            qDebug("recv:%x   cmd:%x",recv.cmd[i],recvCmd[flag][i]);
            if(recv.cmd[i] == recvCmd[flag][i])
                continue;
            return false;
        }
    }else
        return false;

    return true;
}

/**
 * @brief 相位及增益校准返回数据打包
 * @param Data
 */
void MainWindow::dataTopacket(quint8 *Data)
{
    int i = 0;
    recv.addr = Data[i++];

    recv.cmd[0] = Data[i++];
    recv.cmd[1] = Data[i++];
    recv.cmd[2] = Data[i++];
    recv.cmd[3] = Data[i++];
    recv.cmd[4] = Data[i++];

    recv.xornumber = Data[i++];

}

void MainWindow::initTablewidget()
{
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);  //设置不可编辑

    initTablewidgetOfButton();

}

/**
 * @brief 初始化一个带有按钮的表格
 */
void MainWindow::initTablewidgetOfButton()
{
    for(int i =0;i <ui->tableWidget->rowCount(); i++)
        for(int j = 0;j <ui->tableWidget->columnCount(); j++)
        {
            if(j != (ui->tableWidget->columnCount() - 1)&&j != (ui->tableWidget->columnCount() - 2))
            {
                QTableWidgetItem *item = new QTableWidgetItem(tr("---"));
                item->setTextAlignment(Qt::AlignHCenter);
                ui->tableWidget->setItem(i,j,item);
            }
            else if(j == (ui->tableWidget->columnCount() - 2))
            {
                //QWidget *buttonWidget=new QWidget(this);

                QPushButton *button_clear=new QPushButton(this); //在倒数第二栏添加按钮

                button_clear->setText(tr("清零"));

                mAddrMap.insert(i,button_clear);

                connect(button_clear,SIGNAL(clicked()),this,SLOT(button_clear_clicked()));

                ui->tableWidget->setCellWidget(i,j,button_clear);
            }
            else
            {
                QWidget *buttonWidget=new QWidget(this);
                QHBoxLayout *buttonLayout=new QHBoxLayout(buttonWidget);

                QPushButton *button_on=new QPushButton(this); //在最后一栏添加按钮
                //button_on->setFixedHeight(30);
                button_on->setText(tr("开启"));

                QPushButton *button_off=new QPushButton(this);
                button_off->setText(tr("关闭"));

                buttonLayout->addWidget(button_on);
                buttonLayout->addWidget(button_off);
                buttonLayout->setContentsMargins(10, 2, 10, 3);

                mAddressMap.insert(i,button_on);
                mAddressMap.insert(i,button_off);

                connect(button_on,SIGNAL(clicked()),this,SLOT(button_clicked()));
                connect(button_off,SIGNAL(clicked()),this,SLOT(button_clicked()));

                ui->tableWidget->setCellWidget(i,j,buttonWidget);
            }
        }
}

/**
 * @brief 初始化一个不带按钮的表格
 */
void MainWindow::initTablewidgetNoButton()
{
    for(int i =0;i <ui->tableWidget->rowCount(); i++)
        for(int j = 0;j <ui->tableWidget->columnCount(); j++)
        {
            QTableWidgetItem *item = new QTableWidgetItem(tr("---"));
            item->setTextAlignment(Qt::AlignHCenter);
            ui->tableWidget->setItem(i,j,item);
        }
}
/**
 * 点击清零按钮的点击事件
 */
void MainWindow::button_clear_clicked()
{

    QPushButton *button = dynamic_cast<QPushButton *>(QObject::sender()); //找到信号发送者
    //    int mCurrentButtonRow;
    //遍历map，通过比较地址，获取当前按钮所在回路

    int count = ui->tableWidget->rowCount(); //同时也等于rowcount
    QPushButton* address;
    int i;
    for(i = 0;i < count; i++)
    {
        address = NULL;
        address = mAddrMap.value(i);
        if(address == button)
            {
                mCurrentClearButtonRow = i; //当前行数

                break;
            }
    }

//    qDebug()<<"mCurrentButtonRow:"<<mCurrentButtonRow<<"switchData:"<<switchData;
    //    setSwitch(mCurrentButtonRow,switchData);


}
/**
 * 点击关闭按钮的点击事件
 */
void MainWindow::button_clicked()
{

    QPushButton *button = dynamic_cast<QPushButton *>(QObject::sender()); //找到信号发送者
    //    int mCurrentButtonRow;
    //遍历map，通过比较地址，获取当前按钮所在回路

    int count = ui->tableWidget->rowCount(); //同时也等于rowcount
    QList<QPushButton *> addresslist;
    int i;
    for(i = 0;i < count; i++)
    {
        addresslist.clear();
        addresslist = mAddressMap.values(i);
        for(int j= 0;j < addresslist.size(); j++)
        {
            if(addresslist[j] == button)
            {
                mCurrentButtonRow = i; //当前行数
                switchData = j;        //开关正好与其存值相同
                if(j == 1) cSUM = i;
                break;
            }
        }
    }

//    qDebug()<<"mCurrentButtonRow:"<<mCurrentButtonRow<<"switchData:"<<switchData;
    //    setSwitch(mCurrentButtonRow,switchData);


}

/**
 * @brief 开始采集按钮的点击事件
 */
void MainWindow::on_pushButton_5_clicked()
{
    on_pushButton_6_clicked(); //先停止采集
    ui->pushButton_5->setEnabled(false);

    if(isSet){
        pass = 1;
    }
    if(port->checkIsOpen(mCurrentPortName))
    {
        mflag = 2;
    }
    else
    {
        QString warningstr = QObject::tr("串口%1未打开").arg(mCurrentPortName);
        QMessageBox::information(this,QObject::tr("Information"),warningstr,QObject::tr("确定"));
    }
#if 0
    //    qDebug()<<"isopen:"<<port->checkIsOpen(mCurrentPortName)<<"portname:"<<mCurrentPortName;

    if(port->checkIsOpen(mCurrentPortName)) //如果串口打开，再开始循环
    {
        connect(mTimer, SIGNAL(timeout()), this, SLOT(collectLoopDone()));  //循环采集数据
        mTimer->start(10*1000);
    }
    else
    {
        QString warningstr = QObject::tr("串口%1未打开").arg(mCurrentPortName);
        QMessageBox::information(this,QObject::tr("Information"),warningstr,QObject::tr("确定"));
    }
#endif

}

/**
 * @brief 开始采集时发送命令打包,传入参数为执行板地址，以及读写及按钮标志位,symbol用于采集命令及发送命令的区别
 */
void MainWindow::sentDataToPacket(quint8 &addr, quint8 &flag,quint8 *onbit,quint8 *offbit ,bool symbol)
{

    //    quint8 sendData[32];
    memset(sendData,0,sizeof(sendData));
    int j = 0;

    //填充包头
    sentData.head[0] = sendData[j++] = 0x7B;
    sentData.head[1] = sendData[j++] = 0xC1;

    //填充地址，可变，需要传参
    //    sentData.addr = sendData[j++] = 0x01;
    sentData.addr = sendData[j++] = addr;

    //长度
    sentData.length = sendData[j++] = 0x15;

    //标志位
    //    sentData.flag = sendData[j++] = 0xA1;



    if(!symbol) //发送采集命令
    {
        sentData.flag = sendData[j++] = 0x00;
        int i=0;
        sentData.openeffective[i++] = sendData[j++] = 0X00;  //第一块执行板的开关控制
        sentData.openeffective[i++] = sendData[j++] = 0X00;
        sentData.openeffective[i++] = sendData[j++] = 0X00;

        sentData.openeffective[i++] = sendData[j++] = 0X00;
        sentData.openeffective[i++] = sendData[j++] = 0X00;
        sentData.openeffective[i++] = sendData[j++] = 0X00;

        i=0;
        sentData.closeffective[i++] = sendData[j++] = 0X00;  //第一块执行板的开关控制
        sentData.closeffective[i++] = sendData[j++] = 0X00;
        sentData.closeffective[i++] = sendData[j++] = 0X00;

        sentData.closeffective[i++] = sendData[j++] = 0X00;
        sentData.closeffective[i++] = sendData[j++] = 0X00;
        sentData.closeffective[i++] = sendData[j++] = 0X00;

        i=0;
        sentData.obligate[i++] = sendData[j++] = 0x00;
        sentData.obligate[i++] = sendData[j++] = 0x00;
        sentData.obligate[i++] = sendData[j++] = 0x00;
    }
    else    //发送控制命令
    {
        sentData.flag = sendData[j++] = flag;
        //开启有效位
        int i = 0;
        for(int k = 0; k < 6 ; k++)
        {
            if((i+1) == addr) //i=0时，第一块执行板，此时mAddr=1
                sentData.openeffective[i++] = sendData[j++] = bitControl(onbit);  //第一块执行板的开关控制
            else
                sentData.openeffective[i++] = sendData[j++] = 0;
        }

        //关闭有效位
        i = 0;
        for(int k = 0; k < 6 ; k++)
        {
            if((i+1) == addr) //i=0时，第一块执行板，此时mAddr=1
                sentData.closeffective[i++] = sendData[j++] = bitControl(offbit);  //第一块执行板的开关控制
            else
                sentData.closeffective[i++] = sendData[j++] = 0;
        }

        //预留
        i = 0;
        sentData.obligate[i++] = sendData[j++] = 0;
        sentData.obligate[i++] = sendData[j++] = 0;
        sentData.obligate[i++] = sendData[j++] = 0;


        sendData[8] = 0xC7;
        sendData[9] = 0xC8;
        sendData[10] = 0xC9;
        sendData[14] =   0xD7;
        sendData[15] =   0xD8;
        sendData[16] =     0xD9;
        sendData[18] = sendData[2];
    }

    //异或校验码
    sentData.xornumber = sendData[j++] = getXorNumber(sendData,sizeof(sendData));
    //    qDebug("校验码为：%x" , sentData.xornumber );

}

/**
 * @brief 开关有效位的赋值
 */
quint8 MainWindow::bitControl(quint8 *buffer)
{


    //    quint8 buffer[8] = {1,0,0,0,0,0,0,0}; //第一位到第八位开关，结果为10000000，即80
    quint8 on = 0;
    for(int i = 0;i < 8;i++)
    {
        //        qDebug("buffer:%x",*(buffer+i));
        if(buffer[i] == 1)
        {

            on = on|(1<<(7-i));  //如果该位为1，则该位开关打开，置1，否则默认为0，此时无效
        }
    }
    //    qDebug("------------------------on------------------------:%x",on);
    return on;

}

/**
 * @brief 发送采集命令
 */
void MainWindow::sendGatherCmd()
{
    is_gather = true;
    quint8 addr = mAddr;
    quint8 flag =0xA2;
    quint8 buffer[8] = {0X11,0X11,0X11,0X11,0X11,0X11,0X11,0X11}; //第一位到第八位开关，结果为10000000，即80
    quint8 buffer1[8] = {0X11,0X11,0X11,0X11,0X11,0X11,0X11,0X11};
    sentDataToPacket(addr,flag,buffer,buffer1,0);
    port->sendDataToPort(mCurrentPortName,sendData,21);
}
/**
 * @brief 发送采集电能命令
 */
void MainWindow::sendGetEleCmd()
{
    is_gather = true;
    quint8 addr = mAddr;
    quint8 flag =0xE1;

    memset(sendData,0,sizeof(sendData));
    int j = 0;

    //填充包头
    sentData.head[0] = sendData[j++] = 0x7B;
    sentData.head[1] = sendData[j++] = flag;

    //填充地址，可变，需要传参
    //    sentData.addr = sendData[j++] = 0x01;
    sentData.addr = sendData[j++] = addr;

    //长度
    sentData.length = sendData[j++] = 0x10;

    for(int i = 0 ; i < 11; i ++)
        sendData[j++] = 0x00;
    //异或校验码
    sentData.xornumber = sendData[j++] = getXorNumber(sendData,sizeof(sendData));
    //    qDebug("校验码为：%x" , sentData.xornumber );
    port->sendDataToPort(mCurrentPortName,sendData,16);
}
/**
 * @brief 发送表格相关的命令
 */
void MainWindow::sendTableCmd()
{
    //电流电压
    is_gather = false;  //用于区别于采集命令
    quint8 addr = mAddr;
    quint8 flag = 0;
    quint8 buffer[8] = {1,1,1,1,1,1,1,1}; //第一位到第八位开关，结果为10000000，即80
    quint8 buffer1[8] = {0,0,0,0,0,0,0,0};
    bool symbol = true;
    sentDataToPacket(addr,flag,buffer,buffer1,symbol);
    port->sendDataToPort(mCurrentPortName,sendData,21);

}

/**
 * @brief 发送开关位的控制命令
 */
void MainWindow::sendControlCmd(quint8 *onBuf,quint8 *offBuf)
{
    quint8 addr = mAddr;
    quint8 flag = 0xA1;
    //    quint8 buffer[8] = {1,1,1,1,1,1,1,1};
    //    quint8 buffer1[8] = {0,0,0,0,0,0,0,0};
    bool symbol = true;
    sentDataToPacket(addr,flag,onBuf,offBuf,symbol);

    port->sendDataToPort(mCurrentPortName,sendData,21);
}

/**
 * @brief 返回数据打包，解析数据包
 * @param array
 */
void MainWindow::returnToPacket(QByteArray &array)
{

   // qDebug() << array.toHex();
    int i = 0;

    //执行板返回标志
    returnData.sign = array.at(i++);
    //qDebug()<<returnData.sign;
    if(returnData.sign==0XC1)
    {
        //执行板地址
        returnData.addr = array.at(i++);

        //长度
        returnData.len = array.at(i++);

        //开关状态
        returnData.onoffState = array.at(i++);

        //八位电流值
        for(int m = 0; m < 8; m++)
            for(int n = 0 ; n <2 ; n++)
                returnData.current[m][n] = array.at(i++);

        //电压值
        for(int m = 0; m < 8; m++){
            int j = i;
            for(int n = 0 ; n <2 ; n++)
                returnData.volate[m][n] = array.at(j++);
            if(3 == m || 7 == m) i += 2;
        }

        //功率
        for(int m = 0; m < 8; m++)
            for(int n = 0 ; n <2 ; n++)
                returnData.power[m][n] = array.at(i++);

        //异或校验码
        returnData.xornumber = array.at(i++);
    }
    else
    {

        //执行板地址
        returnData.addr = array.at(i++);

        //八位电能值
        for(int m = 0; m < 8; m++)
            for(int n = 0 ; n <3 ; n++)
                returnData.ele[m][n] = array.at(i++);

        //长度
        returnData.len = array.at(i++);
        //异或校验码
        returnData.xornumber = array.at(i++);
    }

}

/**
 * @brief 更新表格数据
 */
void MainWindow::updataTableData()
{
    //qDebug()<<"刷新表格";
    for(int i = 0 ; i < 8 ;i++ )
    {
       // int j = 0;
        setOnOffState(i , 0);
        setCurretnt(i , 1);
        setCurretVolate(i , 2);
        setCurretPower(i, 3);
        setCurretEle(i , 6);

        setAll();
    }
  /*  setFirstVolate();
    setSecondVolate();
    setFirstPower();
    setSecondPower(); */
}
/**
 * @brief 计算电流和功率的总和

 */
void MainWindow::setAll()
{
    QTableWidgetItem *itemA = ui->tableWidget->item(8, 1);
    QTableWidgetItem *itemW = ui->tableWidget->item(8, 3);
    int dataA = 0;
    int dataW = 0;
    for(int i = 0; i < 8; i++){
        int data1 = (returnData.current[i][0]<<8 | returnData.current[i][1])/10;
        dataA += data1;

        int data2 = returnData.power[i][0]<<8 | returnData.power[i][1];
        dataW += data2;

    }
    QString str = QString("%1.%2").arg(dataA/10).arg(dataA%10);
    itemA->setText(str+ "A");
    str = QString::number(dataW/1000.0,'f',3);
    itemW->setText(str+ "KW");
}

/**
 * @brief 开关状态显示
 * @param row
 * @param column
 */
void MainWindow::setOnOffState(int row, int column)
{
    QTableWidgetItem *item = ui->tableWidget->item(row,column);
    bool state = (returnData.onoffState>>(7-row))&1; //判断该位，先将该位移到第一位，再与1相与，则第一位不变，其他位置0
    if(state) {//开
        if(isSet && init[row]){
            init[row] = 0;
            onbuffer[row] = 1;
            offbuffer[row] = 0;
        }
        item->setText(tr("打开"));
    }else{
        if(isSet && init[row]){
            init[row] = 0;
            onbuffer[row] = 0;
            offbuffer[row] = 1;
        }
        item->setText(tr("关闭"));}
}

/**
 * @brief 设置电流值
 * @param row
 * @param column
 */
void MainWindow::setCurretnt(int row, int column)
{
    QTableWidgetItem *item = ui->tableWidget->item(row,column);
    int data = (returnData.current[row][0]<<8 | returnData.current[row][1])/10;
    QString str = QString("%1.%2").arg(data/10).arg(data%10);

    QTableWidgetItem *item2 = ui->tableWidget->item(row,column+3); //+++++++++

    if(ui->lineEditAmin->text().toDouble() <= str.toDouble() &&
       ui->lineEditAmax->text().toDouble() >= str.toDouble() ) {
        item->setForeground(QBrush(QColor(0, 0, 0)));
        item2->setForeground(QBrush(QColor(0, 0, 0)));
        }
    else  {
        item->setForeground(QBrush(QColor(255, 0, 0)));
        item2->setForeground(QBrush(QColor(255, 0, 0)));
    }

    if(onbuffer[row]){ //--Add : 不动添加
        item2->setText(str+ "A");
    }

    item->setText(str+ "A");
}
/**
 * @brief 设置电压值
 * @param row
 * @param column
 */
void MainWindow::setCurretVolate(int row, int column)
{
    QTableWidgetItem *item = ui->tableWidget->item(row,column);
    int data = (returnData.volate[row][0]<<8 | returnData.volate[row][1]);
    QString str = QString("%1.%2").arg(data/10).arg(data%10);

    if(ui->lineEditVmin->text().toDouble() <= str.toDouble() &&
       ui->lineEditVmax->text().toDouble() >= str.toDouble() ) item->setForeground(QBrush(QColor(0, 0, 0)));
    else  item->setForeground(QBrush(QColor(255, 0, 0)));
    item->setText(str + "V");
}
/**
 * @brief 设置电能值
 * @param row
 * @param column
 */
void MainWindow::setCurretEle(int row, int column)
{
    QTableWidgetItem *item = ui->tableWidget->item(row,column);
    int data = (returnData.ele[row][0]<<8 | returnData.ele[row][1]);
    data = (data<<8 | returnData.ele[row][2]);
    //qDebug()<<data;
    QString str = QString("%1.%2").arg(data/10).arg(data%10);


    item->setText(str + "KWh");
}


void MainWindow::setCurretPower(int row, int column)
{
    QTableWidgetItem *item = ui->tableWidget->item(row,column);
    int data = (returnData.power[row][0]<<8 | returnData.power[row][1]);
    QString str = QString::number(data/1000.0,'f',3);

    QTableWidgetItem *item2 = ui->tableWidget->item(row,column+2); //+++++++++

    if(ui->lineEditWmin->text().toDouble() <= str.toDouble() &&
       ui->lineEditWmax->text().toDouble() >= str.toDouble() ) {
        item->setForeground(QBrush(QColor(0, 0, 0)));
        item2->setForeground(QBrush(QColor(0, 0, 0)));
    }
    else  {
        item->setForeground(QBrush(QColor(255, 0, 0)));
        item2->setForeground(QBrush(QColor(255, 0, 0)));

    }

    if(onbuffer[row]){ //--Add : 不动添加
        item2->setText(str+ "KW");
    }

    item->setText(str + "KW");
}

void MainWindow::setFirstVolate()
{

    int data = returnData.volate[0][0]<<8 | returnData.volate[0][1];
    QString str = QString("%1.%2V").arg(data/10).arg(data%10);
    ui->label_18->setText(str);
}

void MainWindow::setSecondVolate()
{

    int data = returnData.volate[1][0]<<8 | returnData.volate[1][1];
    QString str = QString("%1.%2V").arg(data/10).arg(data%10);
    ui->label_21->setText(str);
}

void MainWindow::setFirstPower()
{

    int data = returnData.power[0][0]<<8 | returnData.power[0][1];
    QString str = QString("%1.%2KW").arg(data/10).arg(data%10);
    ui->label_23->setText(str);
}

void MainWindow::setSecondPower()
{

    int data = returnData.power[1][0]<<8 | returnData.power[1][1];
    QString str = QString("%1.%2KW").arg(data/10).arg(data%10);
    ui->label_25->setText(str);
}

/**
 * @brief 设置开关
 * @param row为当前所按按钮所在行数
 * @param onOroff为是开是关,决定是填充开有效位还是关有效位
 */
void MainWindow::setSwitch(int row,int onOroff)
{
    quint8 onbuf[8];
    quint8 offbuf[8];

    if(onOroff)  //开，那么填充开有效位，关有效位不用管
    {
        memset(onbuf,0,sizeof(onbuf));
        memset(offbuf,1,sizeof(offbuf));


        for(int j = 0 ; j < 8 ;j++)
        {
           onbuffer[j] = onbuf[j] = 0;
           offbuffer[j] = offbuf[j] = 1;
        }

       onbuffer[row] = onbuf[row] = 1;
       offbuffer[row] = offbuf[row] = 0;

    }else        //关，与上面正好相反
    {
        memset(onbuf,0,sizeof(onbuf));
        memset(offbuf,0,sizeof(offbuf));
        offbuf[row] = 1;
    }
     //qDebug()<<row;
    if(is_read)
        sendControlCmd(onbuf,offbuf);
}
/**
 * @brief 清零
 * @param row为当前所按按钮所在行数
 */
void MainWindow::setClearEle(int row)
{

    if(is_read)
    {

        quint8 flag = 0xD1;

        //    quint8 sendData[32];
        memset(sendData,0,sizeof(sendData));
        int j = 0;

        //填充包头
        sentData.head[0] = sendData[j++] = 0x7B;
        sentData.head[1] = sendData[j++] = 0xC1;

        sentData.addr = sendData[j++] = mAddr;

        sentData.length = sendData[j++] = 0x15;//长度

        sentData.flag = sendData[j++] = flag;

        for(int m = 0 ; m < 3 ; m ++ , j++)
        {
            //qDebug()<<"AAA"<<row;
            if( m == sentData.addr-1)
            {
                if(row <= 7)
                    sendData[j] = (row==0?0x80:0x80>>row);
                else
                    sendData[j] = 0xFF;
            }
            else
            sendData[j] = 0x00;
        }

        for(int i = 0 ; i < 12 ; i ++)
            sendData[j++] = 0x00 ;

        //异或校验码
        sentData.xornumber = sendData[j++] = getXorNumber(sendData,sizeof(sendData));

        port->sendDataToPort(mCurrentPortName,sendData,21);
    }
}

//void MainWindow::updateResponse(QString &str)
//{

//    //    ui->lineEdit_10->setText(str);
//}

/**
 * @brief 将quint8数组转化为字符串返回
 * @param buf
 */
QString MainWindow::quintToStr(quint8 *buf , int len)
{
    QString str;
    str.clear();
    for(int i = 0 ;i < len ; i++)
    {
        str.append(QString::number(*(buf + i),16));
        str.append(" ");
    }
    return str;
}

/**
 * @brief 更新增益按钮应答数据及其相关状态,参数主要用于判断状态正确错误
 * length用于检测状态时获取校验码，此时长度一定要比
 */
void MainWindow::updateGroupboxOne(quint8 *data, int flag, int length ,int len)
{
    bool ret = responseIsRight(data,flag,length);  //判断应答是否正确

    if(ret){
        ui->label_11->setText("正确");
        on_pushButton_5_clicked(); //开始采集

        on_pushButton12_clicked(); //自动记录

    }else{
        ui->label_11->setText("错误");
        on_pushButton_7_clicked(); //重新校准
    }

    QString str = quintToStr(data, len);
    ui->lineEdit_4->setText(str);
}

/**
 * @brief 刷新相位校准应答数据以及状态
 * @param data
 * @param flag
 * @param length
 * @param len
 */
void MainWindow::updateGroupboxTwo(quint8 *data, int flag, int length ,int len)
{
    bool ret = responseIsRight(data,flag,length);  //判断应答是否正确

    if(ret)
        ui->label_16->setText("正确");
    else
        ui->label_16->setText("错误");

    QString str = quintToStr(data, len);
    ui->lineEdit_7->setText(str);
}

/**
 * @brief 刷新采集应答数据以及状态
 * @param data
 * @param len
 */
void MainWindow::updateGroupboxThree(quint8 *data, int len)
{
    QString str = quintToStr(data, len);
    ui->lineEdit_10->setText(str);

    if(*(data + len - 1) == getXorNumber(data,len - 1) )
        ui->label_27->setText(tr("正常"));
    else
        ui->label_27->setText(tr("错误"));
}

/**
 * @brief 延时函数
 */
void MainWindow::delayTimeDone()
{
    if(is_read)
    {
        is_read = false;
        sendTableCmd();  //表格信息采集
        //qDebug()<<"sendTableCmd()";
    }
}

/**
 * @brief 开始采集按钮点击之后采集循环,轮流发送，因为一次只能读取一次数据，同时发送两个数据，前一次的返回将被覆盖
 */
void MainWindow::collectLoopDone()
{

    //    is_read = false;
    //    mflag = 2;

    //        is_read = false;


    if( switchFlag ) //已点击切换按钮
    {
        sendControlCmd(onbuffer,offbuffer);
        switchFlag = false;
    }
    else if(mCurrentButtonRow == 9 && mCurrentClearButtonRow == 9) //当前行为0 - 7，为9表示初始并无按钮点击
    {
        if(is_once)
        {
          //  qDebug()<<"采集指令---------------------------------";
            //            sendGatherCmd(); //发送采集数据命令
            //            is_once = false;

            if(is_read&&is_ele)
            {
                sendGatherCmd(); //发送采集数据命令
                //qDebug()<<"sendGatherCmd()";
                is_once = false;
                is_ele = false;
            }
            else if(is_read&&!is_ele)
            {
                sendGetEleCmd(); //发送采集数据命令
                //qDebug()<<"sendGetEleCmd()";
                is_once = false;
                is_ele = true;
            }
        }
        else
        {
//         //   qDebug()<<"表格指令---------------------------------";
            delayTimeDone();
            is_once = true;
        }
    }
    else if(mCurrentButtonRow != 9 && mCurrentClearButtonRow == 9)//点击开启和关闭响应
    {
      //  qDebug()<<"按钮指令---------------------------------";
        if(isSet){
            onbuffer[mCurrentButtonRow] = switchData;
            offbuffer[mCurrentButtonRow] = 1-onbuffer[mCurrentButtonRow];
        }

        setSwitch(mCurrentButtonRow,switchData);
        mCurrentButtonRow = 9;
    }
    else if(mCurrentButtonRow == 9 && mCurrentClearButtonRow != 9)//点击清零响应
    {
        setClearEle(mCurrentClearButtonRow);
        mCurrentClearButtonRow = 9;
    }

    //    QTimer::singleShot(2*1000 , this , SLOT(delayTimeDone()));  //2s延时后再发送表格信息命令

}

/**
 * @brief 停止采集
 */
void MainWindow::on_pushButton_6_clicked()
{
    mflag = 3;
    is_read = true;
    //    mTimer->stop();
 //   QMessageBox::information(this,tr("information"),tr("停止数据采集！"),tr("确定"));
    //    initTablewidgetOfButton();//如果停止采集初始化表格
    clearTalbeText();

    ui->pushButton_5->setEnabled(true);

}

/**
 * @brief 延迟置1
 */
void MainWindow::delaySetTrue()
{
    is_read = true;
    mflag = 3; //0为增益，1为相位，2为采集，3表示恢复初始状态
    //    qDebug()<<"延迟置1";
    //    QString warningstr = QObject::tr("串口%1未打开").arg(mCurrentPortName);
    //    QMessageBox::information(this,QObject::tr("Information"),warningstr,QObject::tr("确定"));
}

/**
 * @brief 初始化操作说明
 */
void MainWindow::initText()
{
    QFile file("data.txt");
    bool ret = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if(ret) {
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            //        ui->textEdit->setText(line);
            ui->textEdit->append(line);
        }
    }
}

/**
 * @brief 重新校准
 */
void MainWindow::on_pushButton_7_clicked()
{
    is_read = true; //清空标志位，使命令可发
    mflag = 4;

    port->sendDataToPort(mCurrentPortName,activationCmd,sizeof(activationCmd));
    sleep(200);
    //--------------[发送]----------
    on_comboBox_3_currentIndexChanged(1);
    //port->sendDataToPort(mCurrentPortName,recalibrateCmd[ui->comboBox_2->currentIndex()],sizeof(recalibrateCmd[ui->comboBox_2->currentIndex()]));
    ui->lineEdit_4->clear();
    ui->lineEdit_7->clear();

    ui->pushButton_3->setEnabled(true);
}

/**
 * @brief 清空表格数据
 */
void MainWindow::clearTalbeText()
{
    for(int i = 0; i < ui->tableWidget->rowCount() ; i++)
        for(int j = 0 ; j < (ui->tableWidget->columnCount() - 2) ; j++)
        {
            QTableWidgetItem *item = ui->tableWidget->item(i,j);
            item->setText(tr("---"));

        }

    ui->lineEdit_10->clear();
    ui->label_18->clear();
    ui->label_21->clear();
    ui->label_23->clear();
    ui->label_25->clear();
    ui->label_27->clear();
}

/**
 * @brief 切换开关按钮
 */
void MainWindow::on_pushButton_8_clicked()
{
    if(isSet && !pass) return;

    switchFlag = true;
    if(isSet){
        for(int j = 0 ; j < 8 ;j++)
        {
                onbuffer[j] = 1 - onbuffer[j];
                offbuffer[j] = 1 - offbuffer[j];
        }
    }else{
        memset(onbuffer,0,sizeof(onbuffer)); //清零
        memset(offbuffer,0,sizeof(offbuffer));

        //    i = 0;
        if( switchNum > 7 ) //初始为9，循环到8则返回0
            for(int j = 0 ; j < 8 ;j++)
            {
                if( j == switchNum++ )
                {
                    onbuffer[j] = 1;
                    offbuffer[j] = 0;
                }
                else
                {
                    onbuffer[j] = 0;
                    offbuffer[j] = 1;
                }

            }
        else
            switchNum = 0;
    }
    //    sendControlCmd(onbuffer,offbuffer);
}

void MainWindow::on_comboBox_3_currentIndexChanged(int) //模式切换
{
    if(!ui->pushButton_5->isEnabled()) on_pushButton_6_clicked(); //如果是采集状态， 停止采集
    quint8 buf[16];
    int index = ui->comboBox_3->currentIndex();
    if(index == 0){ //一键
        for(int i = 0; i < 16; i++)
        {
            qDebug()<<"aaaaaaaaaaaa";
            buf[i] = modelCmd1[mAddr-1][i];
        }
    }else{ //继电器轮流开关模式
        for(int i = 0; i < 16; i++)
        {
            buf[i] = modelCmd2[mAddr-1][i];
        }
    }
    port->sendDataToPort(mCurrentPortName,buf,sizeof(buf));
}

void MainWindow::on_pushButton_clicked()
{
    double Amin = ui->lineEditAmin->text().toDouble();
    double Amax = ui->lineEditAmax->text().toDouble();
    double Vmin = ui->lineEditVmin->text().toDouble();
    double Vmax = ui->lineEditVmax->text().toDouble();
    double Wmin = ui->lineEditWmin->text().toDouble();
    double Wmax = ui->lineEditWmax->text().toDouble();

    QString iniFile = QCoreApplication::applicationDirPath() + "/config.ini";
    QSettings ini(iniFile, QSettings::IniFormat);
    ini.beginGroup("Value");
    ini.setValue("Amin", Amin);
    ini.setValue("Amax", Amax);
    ini.setValue("Vmin", Vmin);
    ini.setValue("Vmax", Vmax);
    ini.setValue("Wmin", Wmin);
    ini.setValue("Wmax", Wmax);
    ini.endGroup();
}

void MainWindow::onInitIni()
{

    double Amin;
    double Amax;
    double Vmin;
    double Vmax;
    double Wmin;
    double Wmax;

    QString iniFile = QCoreApplication::applicationDirPath() + "/config.ini";
    QSettings ini(iniFile, QSettings::IniFormat);
    ini.beginGroup("Value");
    Amin = ini.value("Amin", 0).toDouble();
    Amax = ini.value("Amax", 0).toDouble();
    Vmin = ini.value("Vmin", 0).toDouble();
    Vmax = ini.value("Vmax", 0).toDouble();
    Wmin = ini.value("Wmin", 0).toDouble();
    Wmax = ini.value("Wmax", 0).toDouble();
    ini.endGroup();

    ui->lineEditAmin->setText(QString::number(Amin));
    ui->lineEditAmax->setText(QString::number(Amax));
    ui->lineEditVmin->setText(QString::number(Vmin));
    ui->lineEditVmax->setText(QString::number(Vmax));
    ui->lineEditWmin->setText(QString::number(Wmin));
    ui->lineEditWmax->setText(QString::number(Wmax));

}

void MainWindow::on_pushButton_9_clicked()
{
    mCurrentButtonRow = cSUM--; //当前行数
    switchData =   1;       //开关正好与其存值相同
    if( cSUM > 7)  cSUM = 0;
    if( cSUM < 0)  cSUM = 7;
}

void MainWindow::on_pushButton_10_clicked()
{
    mCurrentButtonRow = cSUM++; //当前行数
    switchData =   1;       //开关正好与其存值相同
    if( cSUM > 7)  cSUM = 0;
}

void MainWindow::on_pushButton_11_clicked()
{
    cSUM = 7;
    int timers = ui->timerLine->text().toInt() ? ui->timerLine->text().toInt() : 3000;
    timer2->start(timers);
}

void MainWindow::ontimeout()
{
    on_pushButton_9_clicked();
    if(cSUM == 7) timer2->stop();
}

void MainWindow::on_pushButton12_clicked() //全开
{
    sleep(5000);
    if(isSet && !pass) return;
    switchFlag = true;
    if(isSet){
        for(int j = 0 ; j < 8 ;j++)
        {
                onbuffer[j] = 1;
                offbuffer[j] = 0;
        }
    }
}

void MainWindow::on_DelayZeroBtn_clicked()
{
    qDebug()<<"0s延时命令已发送";
    quint8 buf[21];
    memset(buf,0,sizeof(buf));
    //    qDebug()<<"cmd:"<<mAddr;


    delayCmd[sizeof(delayCmd)-1] = getXorNumber(delayCmd,sizeof(delayCmd)-1);
    for(int i = 0; i < 21; i++)
        buf[i] = delayCmd[i];

    port->sendDataToPort(mCurrentPortName,buf,sizeof(buf));
    ui->lineEdit_8->clear();
    ui->lineEdit_8->setText(quintToStr(buf,sizeof(buf)));
}
