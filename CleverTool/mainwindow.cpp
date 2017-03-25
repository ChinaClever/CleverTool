#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"datadefine.h"
#include<qtimer.h>

static returntableData returnData;
static collectedData sentData;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initText();
    initData();
    port = new SerialportOperate;
    initGroupboxThree(0);
    initTablewidget();
    initComboxDa(0);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeoutDone()));
    timer->start(1*1000);

    updateStateAndButton();

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
    //            qDebug()<<"success 0";
    ui->comboBox->clear();
    if(flag == 0)
        list = port->initPortInfo();
    else if (flag == 1)
        list = port->readPortInfo();

    //        qDebug()<<"success 1";

    for(int i=0;i < list.size();i++)
    {
        QString str;
        //            str.clear();
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
    mflag = 3;
    is_gather = true;
    is_once = true;
    is_read = true; //默认已读，才能保证发送
    mAddr = 1;
    //    mTimer = new QTimer(this);
    //    movie = new QMovie(":/new/prefix1/image/camEffectTarget.gif");
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
    qDebug()<<"判断uuuu mflg："<<mflag;

    QByteArray answer;
    answer.clear();
    answer = port->readDataFromPort(mCurrentPortName);
    qDebug()<<"answer -len:"<<answer.length();

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
            break;
        case 1:
            updateGroupboxTwo(data,mflag,answer.length()-1,answer.length());
            break;
        case 2:  //当数据为采集信息数据时，需要判断是显示到采集应答还是表格
            if(is_gather)
            {
                qDebug()<<"is_gather";
                updateGroupboxThree(data,answer.length());
            }
            else
            {
                qDebug()<<"is_not_gather";
                //                 QString str = quintToStr(data, answer.length());
                //                 ui->lineEdit_11->setText(str);

                returnToPacket(answer);
                updataTableData();
            }

            break;
        }
    }
    else //数据未收到，如果is_read一直为false，那么后面数据将一直无法发送，所以延迟置1，延迟时间内无法发送
    {

        qDebug()<<"数据未收到";

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
    qDebug()<<"判断mflg："<<mflag;

    QByteArray answer;

    answer.clear();

    if(port->checkIsOpen(mCurrentPortName))
        answer= port->readDataFromPort(mCurrentPortName);

    qDebug()<<"answer -len:"<<answer.length();

    quint8 data[30];
    QString str;

    memset(data,0,sizeof(data));
    for(int i = 0; i < answer.length() ; i++)
    {
        data[i] = answer.at(i);
        qDebug("data:%x",data[i]);
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
            if(j != (ui->tableWidget->columnCount() - 1))
            {
                QTableWidgetItem *item = new QTableWidgetItem(tr("---"));
                item->setTextAlignment(Qt::AlignHCenter);
                ui->tableWidget->setItem(i,j,item);
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
                break;
            }
        }
    }

    qDebug()<<"mCurrentButtonRow:"<<mCurrentButtonRow<<"switchData:"<<switchData;
    //    setSwitch(mCurrentButtonRow,switchData);


}

/**
 * @brief 开始采集按钮的点击事件
 */
void MainWindow::on_pushButton_5_clicked()
{
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
    sentData.flag = sendData[j++] = flag;


    if(!symbol) //发送采集命令
    {
        int i=0;
        sentData.openeffective[i++] = sendData[j++] = 0X11;  //第一块执行板的开关控制
        sentData.openeffective[i++] = sendData[j++] = 0X11;
        sentData.openeffective[i++] = sendData[j++] = 0X11;

        sentData.openeffective[i++] = sendData[j++] = 0X11;
        sentData.openeffective[i++] = sendData[j++] = 0X11;
        sentData.openeffective[i++] = sendData[j++] = 0X11;

        i=0;
        sentData.closeffective[i++] = sendData[j++] = 0X11;  //第一块执行板的开关控制
        sentData.closeffective[i++] = sendData[j++] = 0X11;
        sentData.closeffective[i++] = sendData[j++] = 0X11;

        sentData.closeffective[i++] = sendData[j++] = 0X11;
        sentData.closeffective[i++] = sendData[j++] = 0X11;
        sentData.closeffective[i++] = sendData[j++] = 0X11;

        i=0;
        sentData.obligate[i++] = sendData[j++] = 0x22;
        sentData.obligate[i++] = sendData[j++] = 0x22;
        sentData.obligate[i++] = sendData[j++] = 0x22;
    }
    else    //发送控制命令
    {
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
 * @brief 返回数据打包
 * @param array
 */
void MainWindow::returnToPacket(QByteArray &array)
{


    int i = 0;

    //执行板返回标志
    returnData.sign = array.at(i++);

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
    for(int m = 0; m < 2; m++)
        for(int n = 0 ; n <2 ; n++)
            returnData.volate[m][n] = array.at(i++);

    //功率
    for(int m = 0; m < 2; m++)
        for(int n = 0 ; n <2 ; n++)
            returnData.power[m][n] = array.at(i++);

    //异或校验码
    returnData.xornumber = array.at(i++);

}

/**
 * @brief 更新表格数据
 */
void MainWindow::updataTableData()
{
    qDebug()<<"刷新表格";
    for(int i = 0 ; i < 8 ;i++ )
    {
        int j = 0;
        setOnOffState(i,j++);
        setCurretnt(i,j++);

    }
    setFirstVolate();
    setSecondVolate();
    setFirstPower();
    setSecondPower();
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
    if(state) //开
        item->setText(tr("打开"));
    else
        item->setText(tr("关闭"));
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
    QString str = QString("%1A").arg(data);
    item->setText(str);
}

void MainWindow::setFirstVolate()
{

    int data = returnData.volate[0][0]<<8 | returnData.volate[0][1];
    QString str = QString("%1V").arg(data);
    ui->label_18->setText(str);
}

void MainWindow::setSecondVolate()
{

    int data = returnData.volate[1][0]<<8 | returnData.volate[1][1];
    QString str = QString("%1V").arg(data);
    ui->label_21->setText(str);
}

void MainWindow::setFirstPower()
{

    int data = returnData.power[0][0]<<8 | returnData.power[0][1];
    QString str = QString("%1KW").arg(data);
    ui->label_23->setText(str);
}

void MainWindow::setSecondPower()
{

    int data = returnData.power[1][0]<<8 | returnData.power[1][1];
    QString str = QString("%1KW").arg(data);
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
    memset(onbuf,0,sizeof(onbuf));
    memset(offbuf,0,sizeof(offbuf));
    if(onOroff)  //开，那么填充开有效位，关有效位不用管
    {
        onbuf[row] = 1;

    }else        //关，与上面正好相反
    {
        offbuf[row] = 1;

    }

    if(is_read)
        sendControlCmd(onbuf,offbuf);
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
    //    qDebug("buf+i:%x",*(buf + len-1));
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

    if(ret)
        ui->label_11->setText("正确");
    else
        ui->label_11->setText("错误");

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
    }
}

/**
 * @brief 开始采集按钮点击之后采集循环,轮流发送，因为一次只能读取一次数据，同时发送两个数据，前一次的返回将被覆盖
 */
void MainWindow::collectLoopDone()
{
    qDebug()<<"循环开始";
    //    is_read = false;
    //    mflag = 2;

    //        is_read = false;
    qDebug()<<"mCurrentButtonRow:"<<mCurrentButtonRow;

    if( switchFlag ) //已点击切换按钮
    {
        sendControlCmd(onbuffer,offbuffer);
        switchFlag = false;
    }
    else if(mCurrentButtonRow == 9) //当前行为0 - 7，为9表示初始并无按钮点击
    {
        if(is_once)
        {
            qDebug()<<"采集指令---------------------------------";
            //            sendGatherCmd(); //发送采集数据命令
            //            is_once = false;

            if(is_read)
            {
                sendGatherCmd(); //发送采集数据命令
                is_once = false;
            }
        }
        else
        {
            qDebug()<<"表格指令---------------------------------";
            delayTimeDone();
            is_once = true;
        }
    }
    else
    {
        qDebug()<<"按钮指令---------------------------------";
        setSwitch(mCurrentButtonRow,switchData);
        mCurrentButtonRow = 9;
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
    QMessageBox::information(this,tr("information"),tr("停止数据采集！"),tr("确定"));
    //    initTablewidgetOfButton();//如果停止采集初始化表格
    clearTalbeText();
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
    port->sendDataToPort(mCurrentPortName,recalibrateCmd,sizeof(recalibrateCmd));
    ui->lineEdit_4->clear();
    ui->lineEdit_7->clear();
}

/**
 * @brief 清空表格数据
 */
void MainWindow::clearTalbeText()
{
    for(int i = 0; i < ui->tableWidget->rowCount() ; i++)
        for(int j = 0 ; j < (ui->tableWidget->columnCount() - 1) ; j++)
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
    switchFlag = true;
//    quint8 onbuffer[8]; /*= {1,0,0,0,0,0,0,0}; */
//    quint8 offbuffer[8]; /*= {0,1,1,1,1,1,1,1}; */
    memset(onbuffer,0,sizeof(onbuffer));
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

    //    sendControlCmd(onbuffer,offbuffer);
}
