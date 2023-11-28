#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "datadefine.h"
#include<QDoubleSpinBox>
#include<qtimer.h>
#include <QTime>
#include<QFile>
#define isSet 1

static returntableData returnData;
static collectedData sentData;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    cSUM = 0;

    initData();
    port = new SerialportOperate;
    //initGroupboxThree(0);
    initTablewidget();
    initComboxDa(0);
    timer2 = new QTimer(this);
    connect(timer2, SIGNAL(timeout()), this, SLOT(ontimeout()));


    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeoutDone()));
    timer->start(1000);

    updateStateAndButton();


    ui->GaincorrectBtn->setText(tr("电压电流校准"));
    ui->GaincorrectBtn->setMinimumSize(110,30);
    ui->GaincorrectBtn->setGeometry(300,120,110,30);

    onInitIni();
    ui->comBox_State->setCurrentIndex(1);
}

MainWindow::~MainWindow()
{
    if(ini != NULL)
    {
        delete ini;
        ini = NULL;
    }
    delete ui;
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
    //qDebug()<<iniFile;
    ini = new QSettings(iniFile, QSettings::IniFormat);
    if(isDirExist(iniFile ,ini))
    {
        ini->beginGroup("Value");
        Amin = ini->value("Amin").toDouble();
        Amax = ini->value("Amax").toDouble();
        Vmin = ini->value("Vmin").toDouble();
        Vmax = ini->value("Vmax").toDouble();
        Wmin = ini->value("Wmin").toDouble();
        Wmax = ini->value("Wmax").toDouble();
        ini->endGroup();

        ui->doubleSpinBoxAmin->setValue(Amin);
        ui->doubleSpinBoxAmax->setValue(Amax);
        ui->doubleSpinBoxVmin->setValue(Vmin);
        ui->doubleSpinBoxVmax->setValue(Vmax);
        ui->doubleSpinBoxWmin->setValue(Wmin);
        ui->doubleSpinBoxWmax->setValue(Wmax);
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
        ini->setValue("/Value/Amin",0);
        ini->setValue("/Value/Amax",32);
        ini->setValue("/Value/Vmin",0);
        ini->setValue("/Value/Vmax",270);
        ini->setValue("/Value/Wmin",0);
        ini->setValue("/Value/Wmax",8.64);
        return true;
    }
    return false;
}
/**
 * @brief combox初始化串口信息
 */
void MainWindow::initComboxDa( int flag)
{
    QStringList list;
    list.clear();
    ui->comBox->clear();
    if(flag == 0)
        list = port->initPortInfo();
    else if (flag == 1)
        list = port->readPortInfo();
    for(int i=0;i < list.size();i++)
    {
        QString str;
        str = list.at(i);
        ui->comBox->addItem(str);
    }
}
/**
 * @brief 初始化标志位
 */
void MainWindow::initData()
{
    //switchFlag = false;
    switchNum = 15 ;
    mCurrentButtonRow = 15;
    mCurrentClearButtonRow = 15;
    mflag = 3;
    is_gather = true;
    //is_once = true;
    is_read = true; //默认已读，才能保证发送
    mAddr = 1;

    if(isSet){
        pass = 0;
        for(int j = 0 ; j < 14 ;j++){
            init[j] = 1;
            onbuffer[j] = 0;
            offbuffer[j] = 1;
        }
    }

}

void MainWindow::initTablewidget()
{
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);//设置表格是否充满，即行位不留空
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);  //设置不可编辑
    //    ui->tableWidget->setStyleSheet("QTableWidget::item:alternate:!selected{background-color: rgb(255, 255, 245);}"
    //                                   "QTableWidget::item:!alternate:!selected{ background-color: rgb(245, 245, 245);}");


    initTablewidgetOfButton();
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setFrameShape(QFrame::NoFrame);                  //设置无边框
    ui->tableWidget->setMouseTracking(true);                          //设置鼠标捕捉，不设置，如鼠标进入等动作检测不到
    ui->tableWidget->setStyleSheet("QTableWidget{ background-color: rgb(202, 232, 234); alternate-background-color: rgb(245, 245, 245); }");
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
                button_clear->setStyleSheet(

                            "QPushButton:hover{background-color: qconicalgradient(cx:0.5, cy:0.522909, angle:179.9, stop:0.494318 rgba(181, 225, 250, 255), stop:0.5 rgba(222, 242, 251, 255)); border-radius:5px; border: 1px solid #3C80B1;}"
                            "QPushButton:focus{background-color: qconicalgradient(cx:0.5, cy:0.522909, angle:179.9, stop:0.494318 rgba(134, 198, 233, 255), stop:0.5 rgba(206, 234, 248, 255));border-radius:5px; border: 1px solid #5F92B2;}");

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

                if(i == ui->tableWidget->rowCount()-1)
                    button_on->setText(tr("全开"));
                else
                    button_on->setText(tr("开启"));
                button_on->setStyleSheet(

                            "QPushButton:hover{background-color: qconicalgradient(cx:0.5, cy:0.522909, angle:179.9, stop:0.494318 rgba(181, 225, 250, 255), stop:0.5 rgba(222, 242, 251, 255)); border-radius:5px; border: 1px solid #3C80B1;}"
                            "QPushButton:focus{background-color: qconicalgradient(cx:0.5, cy:0.522909, angle:179.9, stop:0.494318 rgba(134, 198, 233, 255), stop:0.5 rgba(206, 234, 248, 255));border-radius:5px; border: 1px solid #5F92B2;}");

                QPushButton *button_off=new QPushButton(this);
                if(i == ui->tableWidget->rowCount()-1)
                    button_off->setText(tr("全关"));
                else
                    button_off->setText(tr("关闭"));
                button_off->setStyleSheet(

                            "QPushButton:hover{background-color: qconicalgradient(cx:0.5, cy:0.522909, angle:179.9, stop:0.494318 rgba(181, 225, 250, 255), stop:0.5 rgba(222, 242, 251, 255)); border-radius:5px; border: 1px solid #3C80B1;}"
                            "QPushButton:focus{background-color: qconicalgradient(cx:0.5, cy:0.522909, angle:179.9, stop:0.494318 rgba(134, 198, 233, 255), stop:0.5 rgba(206, 234, 248, 255));border-radius:5px; border: 1px solid #5F92B2;}");

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
 * @brief 开始采集时发送命令打包,传入参数为执行板地址，以及读写及按钮标志位,symbol用于采集命令及发送命令的区别
 */
void MainWindow::sentDataToPacket(quint8 &addr)
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

    if(ui->readApduBtn->isChecked())
    {
        sentData.funcode[0] = sendData[j++] = 0xA9;
        sentData.funcode[1] = sendData[j++] = 0xB9;
        for(int i = 0 ; i < 61 ; i++)
            sentData.obligate[i] = sendData[j++] = 0x00;
    }
    else{
        //功能码
        sentData.funcode[0] = sendData[j++] = 0xA1;
        sentData.funcode[1] = sendData[j++] = 0xB1;
        sentData.obligate[0] = sendData[j++] = 0x01;
        for(int i = 1 ; i < 61 ; i++)
            sentData.obligate[i] = sendData[j++] = 0x00;
    }


    //长度
    sentData.length = sendData[j++] = 0x44;


    //异或校验码
    sentData.xornumber = sendData[j++] = getXorNumber(sendData,sizeof(sendData));
    //    qDebug("校验码为：%x" , sentData.xornumber );

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
 * @brief 刷新状态栏以及按钮
 */
void MainWindow::updateStateAndButton()
{
    mCurrentPortName = ui->comBox->currentText();
    //    qDebug()<<"串口状态："<<mCurrentPortName<<port->checkPortState(mCurrentPortName);
    if(port->checkPortState(mCurrentPortName))
    {
        ui->workBtn->setText("关闭串口");
        ui->SerialStatelabel->setText(QString("串口%1").arg(mCurrentPortName)+"已开启");
    }else
    {
        ui->workBtn->setText("打开串口");
        ui->SerialStatelabel->setText(QString("串口%1").arg(mCurrentPortName)+"已关闭");
    }
    port->checkAllState();
}
/**
 * @brief 定时刷新串口
 */
void MainWindow::timeoutDone()
{
    int index = ui->comBox->currentIndex();

    if(ui->comBox->count() > index)
        ui->comBox->setCurrentIndex(index);
    else
        ui->comBox->setCurrentIndex(0);

    readAnswer();  //读取数据

    if( mflag == 2 ) //如果点击采集按钮，那么循环时开始发送采集指令
        collectLoopDone();
}
/**
 * @brief 开始采集按钮点击之后采集循环,轮流发送，因为一次只能读取一次数据，同时发送两个数据，前一次的返回将被覆盖
 */
void MainWindow::collectLoopDone()
{


    if(mCurrentButtonRow == 15 && mCurrentClearButtonRow == 15) //当前行为0 - 7，为9表示初始并无按钮点击
    {
        //  qDebug()<<"采集指令---------------------------------";
        //            sendGatherCmd(); //发送采集数据命令
        //            is_once = false;

        if(is_read)
        {
            sendGatherCmd(); //发送采集数据命令
            //qDebug()<<"sendGatherCmd()";
        }
    }
    else if(mCurrentButtonRow != 15 && mCurrentClearButtonRow == 15)//点击开启和关闭响应
    {
        //  qDebug()<<"按钮指令---------------------------------";
        if(isSet){
            onbuffer[mCurrentButtonRow] = switchData;
            offbuffer[mCurrentButtonRow] = 1-onbuffer[mCurrentButtonRow];
        }
        //qDebug()<<mCurrentButtonRow<<switchData;
        setSwitch(mCurrentButtonRow,switchData,true);
        sleep(1000);
        setSwitch(mCurrentButtonRow,switchData,false);
        mCurrentButtonRow = 15;
    }
    else if(mCurrentButtonRow == 15 && mCurrentClearButtonRow != 15)//点击清零响应
    {
        setClearEle(mCurrentClearButtonRow);
        mCurrentClearButtonRow = 15;
    }

    //    QTimer::singleShot(2*1000 , this , SLOT(delayTimeDone()));  //2s延时后再发送表格信息命令

}
/**
 * @brief 设置开关
 * @param row为当前所按按钮所在行数
 * @param onOroff为是开是关,决定是填充开有效位还是关有效位
 */
void MainWindow::setSwitch(int row,int onOroff ,bool flag)
{
    quint8 onbuf[14];
    quint8 offbuf[14];

    if(onOroff)  //开，那么填充开有效位，关有效位不用管
    {
        memset(onbuf,0,sizeof(onbuf));
        memset(offbuf,1,sizeof(offbuf));
        memset(onbuffer,0,sizeof(onbuffer));
        memset(offbuffer,1,sizeof(offbuffer));


        for(int j = 0 ; j < 14 ;j++)
        {
            onbuffer[j] = onbuf[j] = 0;
            offbuffer[j] = offbuf[j] = 1;
        }

        onbuffer[row] = onbuf[row] = 1;
        offbuffer[row] = offbuf[row] = 0;

    }else        //关，与上面正好相反
    {
        memset(offbuffer,0,sizeof(offbuffer));
        memset(onbuf,0,sizeof(onbuf));
        memset(offbuf,0,sizeof(offbuf));
        offbuffer[row] = offbuf[row] = 1;
    }

    if(is_read)//可读状态
    {
        memset(sendData,0,sizeof(sendData));
        int j = 0;

        //填充包头
        sentData.head[0] = sendData[j++] = 0x7B;//0
        sentData.head[1] = sendData[j++] = 0xC1;//1

        sendData[j++] = 0x00;//预留//2

        //功能码
        sentData.funcode[0] = sendData[j++] = 0xA2;//3
        sentData.funcode[1] = sendData[j++] = 0xB2;//4
        if(returnData.opnum>=8){
            //打开有效位
            for(int k = 0; k < 6 ; k++)
            {
                if(row == 14 && onOroff)
                    sentData.openeffective[k] = sendData[j++] = 0xFF;//10
                else
                    sentData.openeffective[k] = sendData[j++] = 0x00;//10

            }
            if(row != 14 && onOroff)
            {
                for(int k = 0 ; k < returnData.opnum ; k++)
                {
                    if(onbuffer[k] == 1)
                    {
                        int index = (returnData.opnum * (mAddr-1) +k) / 8 ;
                        int key = (returnData.opnum * (mAddr-1) +k) % 8 ;
                        //qDebug()<<"open"<<j<<j-6+index<<k<<index<<key;
                        sentData.openeffective[index] |= 0x80 >> key;
                        sendData[j - 6 + index] |= 0x80 >> key;
                    }
                }
            }
            //关闭有效位
            for(int k = 0 ; k < 6 ; k++)
            {
                if(row == 14 && !onOroff)
                {
                    sentData.closeffective[k] = sendData[j++] = 0xFF;//10
                }
                else
                {
                    sentData.closeffective[k] = sendData[j++] = 0x00;
                }
            }
            if(row != 14 && !flag)
            {
                for(int k = 0 ; k < returnData.opnum ; k++)
                {
                    if(offbuffer[k] == 1)
                    {
                        int index = (returnData.opnum * (mAddr-1) +k) / 8 ;
                        int key = (returnData.opnum * (mAddr-1) +k) % 8 ;
                        //qDebug()<<"close"<<j<<k<<index<<key<<j - 6 + index;
                        sentData.closeffective[index] |= 0x80 >> key;
                        sendData[j - 6 + index] |= 0x80 >> key;
                        //qDebug()<<sendData[j - 6 + index];
                    }
                }
            }
        }
        else if(returnData.opnum == 4)
        {
            //打开有效位
            for(int k = 0; k < 6 ; k++)
            {
                if(row == 14 && onOroff)
                    sentData.openeffective[k] = sendData[j++] = 0xFF;//10
                else
                    sentData.openeffective[k] = sendData[j++] = 0x00;//10

            }
            if(row != 14 && onOroff)
            {
                for(int k = 0 ; k < returnData.opnum ; k++)
                {
                    if(onbuffer[k] == 1)
                    {
                        if(mAddr==3)
                            sendData[j - 4 ] |= 0x80>>k ;
                        else if(mAddr==2)
                            sendData[j - 5 ] |= 0x80>>k ;
                    }
                }
            }
            //关闭有效位
            for(int k = 0 ; k < 6 ; k++)
            {
                if(row == 14 && !onOroff)
                {
                    sentData.closeffective[k] = sendData[j++] = 0xFF;//10
                }
                else
                {
                    sentData.closeffective[k] = sendData[j++] = 0x00;
                }
            }
            if(row != 14 && !flag)
            {
                for(int k = 0 ; k < returnData.opnum ; k++)
                {
                    if(offbuffer[k] == 1)
                    {
                        if(mAddr==3)
                            sendData[j - 4 ] |= 0x80>>k ;
                        else if(mAddr==2)
                            sendData[j - 5 ] |= 0x80>>k ;
                    }
                }
            }
        }
        //预留位
        sendData[j++] =0xC7;
        sendData[j++] =0xC8;
        sendData[j++] =0xC9;
        sendData[j++] =0xD7;
        sendData[j++] =0xD8;
        sendData[j++] =0xD9;
        for(int k = 0 ; k < 39 ; k ++ )
            sendData[j++] = 0x00;

        if(row != 14)
            sentData.swstate = sendData[j++] = 0x00;
        else
            sentData.swstate = sendData[j++] = 0x01;

        for(int k = 0 ; k < 3 ; k ++)
            sentData.opbit[k] = sendData[j++] = 0x0E;
        sentData.length = sendData[j++] = 0x44;
        //异或校验码
        sentData.xornumber = sendData[j++] = getXorNumber(sendData,sizeof(sendData));

        //        QByteArray array = port->quintToByte(sendData,68);
        //        qDebug() << "Array:" << array.toHex();

        port->sendDataToPort(mCurrentPortName,sendData,68);
    }
}
/**
 * @brief 清零
 * @param row为当前所按按钮所在行数
 */
void MainWindow::setClearEle(int row)
{

    if(is_read)
    {

        memset(sendData,0,sizeof(sendData));
        int j = 0;

        //填充包头
        sentData.head[0] = sendData[j++] = 0x7B;//0
        sentData.head[1] = sendData[j++] = 0xC1;//1

        sendData[j++] = 0x00;//预留//2
        //功能码
        sentData.funcode[0] = sendData[j++] = 0xA4;//3
        sentData.funcode[1] = sendData[j++] = 0xB4;//4

        for(int k = 0; k < 6 ; k++)
        {
            if(row == 14 )
                sentData.clearEle[k] = sendData[j++] = 0xFF;//10
            else
                sentData.clearEle[k] = sendData[j++] = 0x00;//10

        }
        if(row < 14)
        {

            int index = (returnData.opnum * (mAddr-1) +row) / 8 ;
            int key = (returnData.opnum * (mAddr-1) +row) % 8 ;
            //qDebug()<<"clear"<< j- 6 + index<<row<<index<<key;
            sentData.clearEle[index] |= 0x80 >> key;
            sendData[j - 6 + index] |= 0x80 >> key;
        }

        //预留位
        for(int k = 0 ; k < 52 ; k ++ )
            sendData[j++] = 0x00;


        for(int k = 0 ; k < 3 ; k ++)
            sentData.opbit[k] = sendData[j++] = 0x0E;
        sentData.length = sendData[j++] = 0x44;
        //异或校验码
        sentData.xornumber = sendData[j++] = getXorNumber(sendData,sizeof(sendData));

        port->sendDataToPort(mCurrentPortName,sendData,68);
    }
}

/**
 * @brief 从串口读取数据显示,根据mflag标志决定数据流向
 */
void MainWindow::readAnswer()
{
    // qDebug()<<"判断uuuu mflg："<<mflag;

    QByteArray answer;
    answer.clear();
    //qDebug()<<"answer -len:"<<mCurrentPortName;
    answer = port->readDataFromPort(mCurrentPortName);


    //qDebug() << "get:" << answer.toHex();
    quint8 data[BUFSIZ];
    QString str;

    memset(data,0,sizeof(data));
    for(int i = 0; i < answer.length() ; i++)
    {
        data[i] = answer.at(i);
        //qDebug("data:%x",data[i]);
        str.append(QString::number(data[i],16));
        str.append(" ");
    }

    if(!answer.isEmpty())
    {
        is_read = true;  //将读取数据标志位置1

        switch (mflag)
        {
        case 0:
            if(data[0]!=0x7B)//忽略两个底层发送7B开头的两个命令
                updateGroupboxOne(data,mflag,answer.length()-1,answer.length());
            break;
        case 1:
            //updateGroupboxTwo(data,mflag,answer.length()-1,answer.length());
            break;
        case 2:  //当数据为采集信息数据时，需要判断是显示到采集应答还是表格
            if(is_gather)
            {
                if(data[0] == 0x7B && (answer.length() == 127||answer.length() == 62))//只对开头是7B和长度是105进行解析
                {
                    qDebug()<<answer.length();
                    updateGroupboxThree(data,answer.length());

                    returnToPacket(answer);
                    updataTableData();
                }
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
 * @brief 更新表格数据
 */
void MainWindow::updataTableData()
{
    //qDebug()<<"刷新表格";
    if(returnData.opnum > 14)//大于14开关位不符合
        return ;
    ui->Hzlabel->setText(QString(tr("%1 Hz")).arg(returnData.conversefreq));
    for(int i = 0 ; i < returnData.opnum ;i++ )
    {
        // int j = 0;
        setOnOffState(i,0);
        setCurretnt(i,1);
        setCurretVolate(i,2);
        setCurretPower(i, 3);

        setCurretEle(i, 5);

        setAll();
        setPF(i , 6);
    }

}
void MainWindow::setAll()////////
{
    QTableWidgetItem *itemA = ui->tableWidget->item(14, 1);
    QTableWidgetItem *itemW = ui->tableWidget->item(14, 3);
    float dataA = 0;
    double dataW = 0;
    for(int i = 0; i < returnData.opnum; i++){
        float data1 = (returnData.current[i][0]<<8 | returnData.current[i][1])/10.0;
        dataA += data1;

        double data2 = returnData.power[i];
        dataW += data2;

    }
    QString str = QString::number(dataA/10.0, 'f', 2);
    itemA->setText(str+ "A");
    str = QString::number(dataW, 'f', 3);//取一位小数点，并且判断小数点后一位是否大于等于5，则进一，否则舍弃
    itemW->setText(str+ "kW");
}

/**
 * @brief 开关状态显示
 * @param row
 * @param column
 */
void MainWindow::setOnOffState(int row, int column)
{
    QTableWidgetItem *item = ui->tableWidget->item(row,column);
    bool state = false;
    if(returnData.opnum == 8)
    {
        state = (returnData.onoffState[0]>>(7-row))&1; //判断该位，先将该位移到第一位，再与1相与，则第一位不变，其他位置0
    }
    else
    {
        if(row <= 7)
            state = (returnData.onoffState[0]>>(7-row))&1;
        else
            state = (returnData.onoffState[1]>>(15-row))&1;
    }
    //qDebug()<<state;
    if(state)
    {//开
        //qDebug()<<"aaaaaaa"<<init[row];
        if(isSet && init[row]){
            init[row] = 0;
            onbuffer[row] = 1;
            offbuffer[row] = 0;
        }
        item->setText(tr("打开"));
    }
    else
    {
        if(isSet && init[row]){
            init[row] = 0;
            onbuffer[row] = 0;
            offbuffer[row] = 1;
        }
        item->setText(tr("关闭"));
    }
}

/**
 * @brief 设置电流值
 * @param row
 * @param column
 */
void MainWindow::setCurretnt(int row, int column)
{
    QTableWidgetItem *item = ui->tableWidget->item(row,column);
    float data = (returnData.current[row][0]<<8 | returnData.current[row][1])/10.0;
    //QString str = QString("%1.%2").arg(data/10).arg(data%10);
    QString str = QString::number(data/10.0, 'f', 2);

    QTableWidgetItem *item2 = ui->tableWidget->item(row,column+3); //+++++++++

    if(ui->doubleSpinBoxAmin->value()<= str.toDouble() &&
            ui->doubleSpinBoxAmax->value() >= str.toDouble() ) {
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
 * @brief 设置电流值
 * @param row
 * @param column
 */
void MainWindow::setPF(int row, int column)
{
    QTableWidgetItem *item = ui->tableWidget->item(row,column);
    int data = returnData.powerfactor[row];
    QString str = QString::number(data/100.0, 'f', 2);

    item->setText(str);
}

void MainWindow::setCurretVolate(int row, int column)
{
    QTableWidgetItem *item = ui->tableWidget->item(row,column);
    int data = 0;
    QString str="";
    if(row < returnData.opnum)
        data= (returnData.vol[row][0]<<8 | returnData.vol[row][1]);
    str = QString("%1.%2").arg(data/10).arg(data%10);

    if(ui->doubleSpinBoxVmin->value() <= str.toDouble() &&
            ui->doubleSpinBoxVmax->value() >= str.toDouble() )
        item->setForeground(QBrush(QColor(0, 0, 0)));
    else
        item->setForeground(QBrush(QColor(255, 0, 0)));
    item->setText(str + "V");
}

void MainWindow::setCurretPower(int row, int column)
{
    QTableWidgetItem *item = ui->tableWidget->item(row,column);
    int voldata = 0;
    voldata= (returnData.vol[row][0]<<8 | returnData.vol[row][1]);
    int curdata = returnData.current[row][0]<<8 | returnData.current[row][1];
    int pfdata = returnData.powerfactor[row];
    if(curdata == 0 || pfdata == 0 || voldata == 0 )
        returnData.power[row] = 0;
    else
        returnData.power[row] = (voldata * curdata /(10.0*10*10*100)/1000.0)* pfdata;
    QString str = QString::number(returnData.power[row], 'f', 3);

    if(ui->doubleSpinBoxWmin->value() <= str.toDouble() &&
            ui->doubleSpinBoxWmax->value() >= str.toDouble() ) {
        item->setForeground(QBrush(QColor(0, 0, 0)));
    }
    else  {
        item->setForeground(QBrush(QColor(255, 0, 0)));
    }
    item->setText(str + "KW");
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


    item->setText(str + "kWh");
}
/**
 * @brief 更新增益按钮应答数据及其相关状态,参数主要用于判断状态正确错误
 * length用于检测状态时获取校验码，此时长度一定要比
 */
void MainWindow::updateGroupboxOne(quint8 *data, int flag, int length ,int len)
{
    bool ret = responseIsRight(data,flag,length);  //判断应答是否正确

    if(ret){
        ui->ResponseStatelabel->setText("正确");
        on_StartBtn_clicked(); //开始采集

        on_AllOpenBtn_clicked(); //自动记录

    }else{
        ui->ResponseStatelabel->setText("错误");
        on_RecorrectBtn_clicked(); //重新校准
    }

    QString str = quintToStr(data, len);
    ui->ResponseDatalineEdit->setText(str);
}



/**
 * @brief 刷新采集应答数据以及状态
 * @param data
 * @param len
 */
void MainWindow::updateGroupboxThree(quint8 *data, int len)
{
    QString str = quintToStr(data, len);
    ui->CollectResponsetextEdit->setText(str);

    if(*(data + len - 1) == getXorNumber(data,len - 1) )
        ui->CollectStatelable->setText(tr("正常"));
    else
        ui->CollectStatelable->setText(tr("错误"));
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
        ui->CRCCodelineEdit->setText(QString::number(recv.xornumber,16));
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
/**
 * @brief 返回数据打包
 * @param array
 */
void MainWindow::returnToPacket(QByteArray &array)
{

    //qDebug() << array.toHex();
    //qDebug() << array.length();
    int i = 0;
    if(ui->readApduBtn->isChecked()){
        //头码
        returnData.head[0] = array.at(i++);//0
        //头码
        returnData.head[1] = array.at(i++);//1
        returnData.opnum = 6;
        //执行板地址
        returnData.addr = array.at(i++);//2
        //频率
        returnData.conversefreq = array.at(i++);//3

        for(int j = 0 ; j < 6 ; j ++)
        {
            for(int n = 0 ; n < 2 ; n ++)
            returnData.vol[j][n] = array.at(i++);//6
        }

        for(int j = 0 ; j < 6 ; j ++)
        {
            for(int n = 0 ; n < 2 ; n ++)
            returnData.current[j][n] = array.at(i++);//6
        }
        for(int j = 0 ; j < 6 ; j ++)
        {
            returnData.power[j] = array.at(i++)<<8;//6
            returnData.power[j] += array.at(i++);//6
        }
        //电能
        for(int m = 0; m < 6; m++)
            for(int n = 0 ; n < 3 ; n++)
                returnData.ele[m][n] = array.at(i++);//94

//        uchar sw = array.at(i++); // 开关状态 1表示开，0表示关
//        for(int i=0; i<6; ++i)  returnData.sw[i] = (sw >> (6-i)) & 1;
//        mData->version = *ptr++;
//        ptr++;
    }
    else{
        //头码
        returnData.head[0] = array.at(i++);//0
        //头码
        returnData.head[1] = array.at(i++);//1

        //执行板地址
        returnData.addr = array.at(i++);//2

        //输出位的个数////////////////////////////////////////////////////////////////////
        returnData.opnum = array.at(i++);//3

        //频率
        returnData.conversefreq = array.at(i++);//4


        for(int n = 0 ; n < 2 ; n ++)
            returnData.vol[0][n] = array.at(i++);//6

//        if(returnData.opnum < 8)
//            for(int n = 0 ; n < 2 ; n ++)
//                array.at(i++);
//        else
        for(int n = 0 ; n < 2 ; n ++)
            returnData.vol[returnData.opnum-1][n] = array.at(i++);//8

        //开关状态1
        returnData.onoffState[0] = array.at(i++);//9
        //开关状态2
        returnData.onoffState[1] = array.at(i++);//10

        //十四位电流值
        for(int m = 0; m < 14; m++)
            for(int n = 0 ; n <2 ; n++)
                returnData.current[m][n] = array.at(i++);//38

        for(int m = 0 ;m < 14; m++)
            returnData.powerfactor[m] = array.at(i++);//52

        //电能
        for(int m = 0; m < 14; m++)
            for(int n = 0 ; n < 3 ; n++)
                returnData.ele[m][n] = array.at(i++);//94

        i += 3;//忽略三位97
        returnData.version = array.at(i++);//98

        i += 2;//忽略四位100

//        if(returnData.opnum<8)
//        {
//            for(int m = 1 ; m < returnData.opnum ; m ++)
//                for(int n = 0 ; n < 2 ; n ++)
//                    returnData.vol[m][n] = array.at(i++);//124
//            if(returnData.opnum < 14)
//            {
//                for(int m = returnData.opnum ; m < 13; m ++)
//                    for(int n = 0 ; n < 2 ; n ++)
//                        i ++;
//            }
//        }
//        else
        {
            for(int m = 1 ; m < returnData.opnum - 1 ; m ++)
                for(int n = 0 ; n < 2 ; n ++)
                    returnData.vol[m][n] = array.at(i++);//124
            if(returnData.opnum < 14)
            {
                i += 2;
                for(int m = returnData.opnum ; m < 13; m ++)
                    for(int n = 0 ; n < 2 ; n ++)
                        i ++;
            }
        }
        returnData.len = array.at(i++);//125
        //异或校验码
        returnData.xornumber = array.at(i);//126
    }

}
/**
 * @brief 发送采集命令
 */
void MainWindow::sendGatherCmd()
{
    is_gather = true;
    quint8 addr = mAddr;

    sentDataToPacket(addr);
    port->sendDataToPort(mCurrentPortName,sendData,68);
    if(is_gather)
    {
        QString str = quintToStr(sendData, 68);
        ui->CollectCommandlinetext->setText(str);
    }
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
void MainWindow::ontimeout()
{
    on_DownBtn_clicked();
    if(cSUM == 14) timer2->stop();
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

}
void MainWindow::on_comBox_State_currentIndexChanged(int index)
{
    if(!ui->StartBtn->isEnabled()) on_StopBtn_clicked(); //如果是采集状态， 停止采集
    quint8 buf[16];
    index = ui->comBox_State->currentIndex();
    if(index == 0){ //一键
        for(int i = 0; i < 16; i++)
        {
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

void MainWindow::on_comBox_Addr_currentIndexChanged(int index)
{
    mAddr = index + 1;  //当前地址码
    initGroupboxOne(index);

    //initGroupboxThree(index);

    //--------------[发送]----------
    //on_comBox_State_currentIndexChanged(1);
}
void MainWindow::initGroupboxOne(int index)
{
    switch (index) {
    case 0:
        ui->CRCCodelineEdit->setText(tr("04"));
        break;
    case 1:
        ui->CRCCodelineEdit->setText(tr("07"));
        break;
    case 2:
        ui->CRCCodelineEdit->setText(tr("06"));
        break;
    case 3:
        ui->CRCCodelineEdit->setText(tr("01"));
        break;

    }
}




/**
 * @brief 发送命令
 * @param flag
 */
void MainWindow::sendCmd(int flag)
{
    //qDebug()<<"校准命令已发送";
    quint8 buf[6];
    memset(buf,0,sizeof(buf));
    //    qDebug()<<"cmd:"<<mAddr;
    if(flag == 0)  //增益校准
        for(int i = 0; i < 6; i++)
        {
            buf[i] = transCmd[mAddr-1][i];
        }

    port->sendDataToPort(mCurrentPortName,buf,sizeof(buf));
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

    ui->CollectResponsetextEdit->clear();

}
void MainWindow::on_RecorrectBtn_clicked()
{
    is_read = true; //清空标志位，使命令可发
    mflag = 4;
    on_comBox_State_currentIndexChanged(1);//模式选择

    sleep(200);

    port->sendDataToPort(mCurrentPortName,activationCmd,sizeof(activationCmd));
    //    sleep(2100);
    //    port->sendDataToPort(mCurrentPortName,recalibrateCmd[ui->comBox_Addr->currentIndex()],sizeof(recalibrateCmd[ui->comBox_Addr->currentIndex()]));
    ui->ResponseDatalineEdit->clear();
    ui->CRCCodelineEdit->clear();

    ui->GaincorrectBtn->setEnabled(true);
}
void MainWindow::sleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
void MainWindow::on_comBox_currentTextChanged(const QString &)
{
    updateStateAndButton();
}

void MainWindow::on_workBtn_clicked()
{
    QString portName = ui->comBox->currentText();
    if(ui->workBtn->text() == "打开串口")
    {
        port->enablePort(portName);
    }
    else if (ui->workBtn->text() == "关闭串口")
    {
        port->disablePort(portName);
    }
    updateStateAndButton();
    //--------------[发送]----------
    //on_comBox_State_currentIndexChanged(1);
}

void MainWindow::on_GaincorrectBtn_clicked()
{
    if(!ui->StartBtn->isEnabled()) on_StopBtn_clicked(); //如果是采集状态， 停止采集

    ui->GaincorrectBtn->setEnabled(false);

    if(mflag == 2)
        on_StopBtn_clicked();//改变了mflag为3

    mflag = 0;

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

void MainWindow::on_StartBtn_clicked()
{
    on_StopBtn_clicked(); //先停止采集



    if(isSet){
        pass = 1;
    }
    if(port->checkIsOpen(mCurrentPortName))
    {
        ui->StartBtn->setEnabled(false);
        ui->GaincorrectBtn->setEnabled(true);
        mflag = 2;
    }
    else
    {
        QString warningstr = QObject::tr("串口%1未打开").arg(mCurrentPortName);
        QMessageBox::information(this,QObject::tr("Information"),warningstr,QObject::tr("确定"));
    }
}

void MainWindow::on_StopBtn_clicked()
{
    mflag = 3;
    is_read = true;
    //    mTimer->stop();
    //   QMessageBox::information(this,tr("information"),tr("停止数据采集！"),tr("确定"));
    //    initTablewidgetOfButton();//如果停止采集初始化表格
    clearTalbeText();

    ui->StartBtn->setEnabled(true);
}


void MainWindow::on_SaveBtn_clicked()
{
    double Amin = ui->doubleSpinBoxAmin->value();
    double Amax = ui->doubleSpinBoxAmax->value();
    double Vmin = ui->doubleSpinBoxVmin->value();
    double Vmax = ui->doubleSpinBoxVmax->value();
    double Wmin = ui->doubleSpinBoxWmin->value();
    double Wmax = ui->doubleSpinBoxWmax->value();


    ini->beginGroup("Value");
    ini->setValue("Amin", Amin);
    ini->setValue("Amax", Amax);
    ini->setValue("Vmin", Vmin);
    ini->setValue("Vmax", Vmax);
    ini->setValue("Wmin", Wmin);
    ini->setValue("Wmax", Wmax);
    ini->endGroup();
}

void MainWindow::on_AllOpenBtn_clicked()
{
    sleep(5000);
    if(isSet && !pass) return;
    //switchFlag = true;
    if(isSet)
    {
        if(is_read)//可读状态
        {
            memset(sendData,0,sizeof(sendData));
            int j = 0;

            //填充包头
            sentData.head[0] = sendData[j++] = 0x7B;//0
            sentData.head[1] = sendData[j++] = 0xC1;//1

            sendData[j++] = 0x00;//预留//2

            //功能码
            sentData.funcode[0] = sendData[j++] = 0xA2;//3
            sentData.funcode[1] = sendData[j++] = 0xB2;//4
            //打开有效位
            for(int k = 0; k < 6 ; k++)
            {
                sentData.openeffective[k] = sendData[j++] = 0xFF;//10
            }

            //关闭有效位
            for(int k = 0 ; k < 6 ; k++)
            {
                sentData.closeffective[k] = sendData[j++] = 0x00;
            }

            //预留位
            sendData[j++] =0xC7;
            sendData[j++] =0xC8;
            sendData[j++] =0xC9;
            sendData[j++] =0xD7;
            sendData[j++] =0xD8;
            sendData[j++] =0xD9;
            for(int k = 0 ; k < 39 ; k ++ )
                sendData[j++] = 0x00;

            sentData.swstate = sendData[j++] = 0x01;

            for(int k = 0 ; k < 3 ; k ++)
                sentData.opbit[k] = sendData[j++] = 0x0E;
            sentData.length = sendData[j++] = 0x44;
            //异或校验码
            sentData.xornumber = sendData[j++] = getXorNumber(sendData,sizeof(sendData));
            port->sendDataToPort(mCurrentPortName,sendData,68);
        }
    }
}

void MainWindow::on_AutoChangeBtn_clicked()///////
{
    cSUM = 0;
    int timers = ui->timerline->text().toInt() ? ui->timerline->text().toInt() : 3000;
    timer2->start(timers);
}

void MainWindow::on_UpBtn_clicked()///////
{
    mCurrentButtonRow = cSUM--; //当前行数
    switchData =   1;       //开关正好与其存值相同
    if( cSUM > 14)  cSUM = 0;
    if( cSUM < 0)  cSUM = 14;
}

void MainWindow::on_DownBtn_clicked()//////
{
    mCurrentButtonRow = cSUM++; //当前行数
    switchData =   1;       //开关正好与其存值相同
    if( cSUM > 14)  cSUM = 0;
}
