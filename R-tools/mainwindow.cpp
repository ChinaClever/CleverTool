#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QTimer>
#include <QDebug>
int flag=0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_spcomm = new SPComm();

    this->setWindowFlags(Qt::WindowCloseButtonHint);    //去掉最小化、最大化按钮，保留关闭按钮

    setWindowTitle("校准测试工具");
    setWindowIcon(QIcon(":/new/prefix1/ico"));
    setMinimumSize(950, 600);
    setMaximumSize(950, 600);

    title = new QLabel(this);
    title->setGeometry(QRect(390,40,300,35));
    title->setText("RPDU主控板");
    title->setStyleSheet("font-size:27px; color:#01050A; font-weight:bold; font-style:Times");

    group_1 = new QGroupBox("串口设置", this);
    group_2 = new QGroupBox("设备设置", this);
    group_3 = new QGroupBox("校准测量", this);
    group_4 = new QGroupBox("输出信息", this);

    group_1->setGeometry(QRect(25,70,900,100));
    group_2->setGeometry(QRect(25,190,325,270));
    group_3->setGeometry(QRect(400,190,525,190));
    group_4->setGeometry(QRect(25,480,900,90));


    /**************************************串口设置**********************************************/
    label_1 = new QLabel("端口号：", this);
    label_2 = new QLabel("波特率：", this);
    label_1->setGeometry(QRect(75,95,100,25));
    label_2->setGeometry(QRect(75,125,100,25));

    combo_1 = new QComboBox(this);
    combo_2 = new QComboBox(this);
    combo_1->setGeometry(QRect(150,95,100,25));
    combo_2->setGeometry(QRect(150,125,100,25));
    QStringList str1;
    QStringList str2;
    str1<<"com1"<<"com2"<<"com3"<<"com4"<<"com5"<<"com6"<<"com7"<<"com8"<<"com9"<<"com10";
    str2<<"1200"<<"2400"<<"4800"<<"9600"<<"19200"<<"38400"<<"56000"<<"115200";
    combo_1->addItems(str1);
    combo_2->addItems(str2);


    button_1 = new QPushButton("打开串口", this);
    button_2 = new QPushButton("关闭串口", this);
    button_1->setGeometry(QRect(750,95,100,25));
    button_2->setGeometry(QRect(750,125,100,25));
    connect(button_1, SIGNAL(clicked()), this, SLOT(open_pushButton_clicked()));
    connect(button_2, SIGNAL(clicked()), this, SLOT(close_pushButton_clicked()));


    /**************************************设备设置**********************************************/
    type = new QLabel("选择类型：", this);
    type->setGeometry(QRect(50, 215, 65, 25));

    combo_3 = new QComboBox(this);
    combo_3->setGeometry(QRect(120,215,50,25));

    str3<<"1-A"<<"1-B"<<"1-C"<<"1-D"<<"2-A"<<"2-B"<<"2-C"<<"2-D"<<"3-A"<<"3-B"<<"3-C"<<"3-D";
    combo_3->addItems(str3);

    line1 = new QLabel("L1：", this);
    line2 = new QLabel("L2：", this);
    line3 = new QLabel("L3：", this);
    line1->setGeometry(QRect(50, 255, 65, 25));
    line2->setGeometry(QRect(50, 295, 65, 25));
    line3->setGeometry(QRect(50, 335, 65, 25));

    editLine1 = new QLineEdit(this);
    editLine2 = new QLineEdit(this);
    editLine3 = new QLineEdit(this);
    editLine1->setGeometry(QRect(120, 255, 50, 25));
    editLine2->setGeometry(QRect(120, 295, 50, 25));
    editLine3->setGeometry(QRect(120, 335, 50, 25));

    mac = new QLabel("物理地址：", this);
    mac->setGeometry(QRect(50, 375, 65, 25));

    editMac = new QLineEdit(this);
    editMac->setGeometry(QRect(120, 375, 150, 25));
    editMac->setMaxLength(17);//限制最长输入12位

    button_6 = new QPushButton("读取设置", this);
    button_7 = new QPushButton("保存设置", this);
    button_6->setGeometry(QRect(50, 415, 100, 25));
    button_7->setGeometry(QRect(170, 415, 100, 25));
    connect(button_6,SIGNAL(clicked()),this,SLOT(readDevice_pushButton_clicked()));
    connect(button_7,SIGNAL(clicked()),this,SLOT(writeDevice_pushButton_clicked()));


    /**************************************校准测量**********************************************/
    L1 = new QLabel("L1：", this);
    L2 = new QLabel("L2：", this);
    L3 = new QLabel("L3：", this);
    L1->setGeometry(QRect(425, 215, 30, 25));
    L2->setGeometry(QRect(425, 255, 30, 25));
    L3->setGeometry(QRect(425, 295, 30, 25));

    curL1 = new QLabel("电流", this);
    curL2 = new QLabel("电流", this);
    curL3 = new QLabel("电流", this);
    curL1->setGeometry(QRect(460, 215,30,25));
    curL2->setGeometry(QRect(460, 255,30,25));
    curL3->setGeometry(QRect(460, 295,30,25));

    editCurL1 = new QLineEdit(this);
    editCurL2 = new QLineEdit(this);
    editCurL3 = new QLineEdit(this);
    editCurL1->setGeometry(QRect(500, 215, 60, 25));
    editCurL2->setGeometry(QRect(500, 255, 60, 25));
    editCurL3->setGeometry(QRect(500, 295, 60, 25));

    curA = new QLabel("A", this);
    curA->setGeometry(QRect(565, 215, 10, 25));
    curB = new QLabel("A", this);
    curB->setGeometry(QRect(565, 255, 10, 25));
    curC = new QLabel("A", this);
    curC->setGeometry(QRect(565, 295, 10, 25));

    volL1 = new QLabel("电压", this);
    volL2 = new QLabel("电压", this);
    volL3 = new QLabel("电压", this);
    volL1->setGeometry(QRect(605, 215,30,25));
    volL2->setGeometry(QRect(605, 255,30,25));
    volL3->setGeometry(QRect(605, 295,30,25));

    editVolL1 = new QLineEdit(this);
    editVolL2 = new QLineEdit(this);
    editVolL3 = new QLineEdit(this);
    editVolL1->setGeometry(QRect(645, 215, 60, 25));
    editVolL2->setGeometry(QRect(645, 255, 60, 25));
    editVolL3->setGeometry(QRect(645, 295, 60, 25));

    volA = new QLabel("V", this);
    volB = new QLabel("V", this);
    volC = new QLabel("V", this);
    volA->setGeometry(QRect(710, 215, 10, 25));
    volB->setGeometry(QRect(710, 255, 10, 25));
    volC->setGeometry(QRect(710, 295, 10, 25));

    energyL1 = new QLabel("电能", this);
    energyL2 = new QLabel("电能", this);
    energyL3 = new QLabel("电能", this);
    energyL1->setGeometry(QRect(750, 215,30,25));
    energyL2->setGeometry(QRect(750, 255,30,25));
    energyL3->setGeometry(QRect(750, 295,30,25));

    editEngL1 = new QLineEdit(this);
    editEngL2 = new QLineEdit(this);
    editEngL3 = new QLineEdit(this);
    editEngL1->setGeometry(QRect(790, 215, 60, 25));
    editEngL2->setGeometry(QRect(790, 255, 60, 25));
    editEngL3->setGeometry(QRect(790, 295, 60, 25));

    energyA = new QLabel("kwh", this);
    energyB = new QLabel("kwh", this);
    energyC = new QLabel("kwh", this);
    energyA->setGeometry(QRect(855, 215, 20, 25));
    energyB->setGeometry(QRect(855, 255, 20, 25));
    energyC->setGeometry(QRect(855, 295, 20, 25));

    button_3 = new QPushButton("采集数据", this);
    button_4 = new QPushButton("电能清零", this);
    button_5 = new QPushButton("主控校准", this);
    button_3->setGeometry(QRect(450,335,100,25));
    button_4->setGeometry(QRect(600,335,100,25));
    button_5->setGeometry(QRect(750,335,100,25));
    connect(button_3,SIGNAL(clicked()),this,SLOT(readDate_pushButton_clicked()));
    connect(button_4,SIGNAL(clicked()),this,SLOT(cleanDate_pushButton_clicked()));
    connect(button_5,SIGNAL(clicked()),this,SLOT(adjustDate_pushButton_clicked()));

    label_3 = new QLabel("说明：串口波特率默认为9600", this);
    label_4 = new QLabel("      校准源基准电压为220V，基准电流为6A", this);
    label_3->setGeometry(QRect(400, 400, 300,25));
    label_4->setGeometry(QRect(400, 430, 300,25));

    textEdit = new QTextEdit(this);
    textEdit->setGeometry(QRect(40,500,870,60));

    /**************************************串口状态**********************************************/
    state = new QLabel("串口状态：",this);
    stateTag = new QLabel(this);
    state->setGeometry(QRect(25,570,300,25));
    stateTag->setGeometry(QRect(95,570,300,25));


    QTimer *timer = new QTimer( this );     //定时器
    connect( timer, SIGNAL(timeout()), this, SLOT(timerDone()) );
    timer->start( 2000 ); // 2秒单触发定时器

}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPixmap image(":/new/prefix1/ico");
    QPainter painter(this);
    painter.drawPixmap(850,395,80,80,image);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_spcomm;
}

void MainWindow::open_pushButton_clicked()
{
    if(m_spcomm->isOpen())
        return;

    QString PortName=combo_1->currentText();
    int BaudRate = combo_2->currentText().toInt();

    qDebug() << PortName;
    qDebug() << BaudRate;

    m_spcomm->setPortName(PortName);
    m_spcomm->setBaudRate(BaudRate);
    if(m_spcomm->open())
        textEdit->append("打开串口成功");
}

void MainWindow::close_pushButton_clicked()
{
    if(m_spcomm->isOpen())
    {
         m_spcomm->close();
         textEdit->append("关闭串口成功");
    }
}

char Xor(char *buf, int len)
{
    char bytemp=0;
    int i;
    for(i=0;i<len;i++)
    {
        bytemp ^= buf[i];
    }
    return bytemp;
}

void Printf(char *buf, int len, char *str)
{
    int i;
    char data[32];
    for(i=0;i<len;i++)
    {
        memset(data,0,sizeof(data));
        sprintf(data,"%.2x ", buf[i]);
        strcat(str,data);
    }
}

void sleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MainWindow::readDevice_pushButton_clicked()
{
    m_spcomm->clear();
    char buf[32];
    memset(buf,0,sizeof(buf));
    buf[0]=0x5A;
    buf[1]=0x2B;
    buf[2]=0x11;
    buf[24]=Xor(buf,24);
    m_spcomm->writeData(buf, 25);

    char str[128]={0};
    Printf(buf,25,str);
    textEdit->append(str);

    memset(buf,0,sizeof(buf));
    m_spcomm->readData(buf,24,1000);


    if(buf[0]==0x2B && buf[1]==0x11)
    {
        combo_3->setCurrentIndex(buf[2]-1);
//        QString str();
        editLine1->setText(QString(buf[3]));
        editLine2->setText(QString(buf[4]));
        editLine3->setText(QString(buf[5]));
        char mac[32];
        memset(mac,0,sizeof(mac));
        strncpy(mac,&buf[6],17);
        editMac->setText(mac);
    }
}

void MainWindow::writeDevice_pushButton_clicked()
{
    m_spcomm->clear();
    int Typetm=combo_3->currentIndex();
    int L1=editLine1->text().toInt();
    int L2=editLine2->text().toInt();
    int L3=editLine3->text().toInt();

    char a[8];
    char b[8];
    char c[8];
    memset(a,0,8);
    memset(b,0,8);
    memset(c,0,8);
    sprintf(a,"%x", L1);
    sprintf(b,"%x", L2);
    sprintf(c,"%x", L3);

    char buf[16];
    memset(buf,0,sizeof(buf));
    buf[0]=0x5A;
    buf[1]=0x2C;
    buf[2]=0x11;
    buf[3]=Typetm+1;
    buf[4]=a[0];
    buf[5]=b[0];
    buf[6]=c[0];
    char *mac = editMac->text().toLatin1().data();    //    11:22:33:44:55:66

    char sendStr[128];
    memset(sendStr,0,sizeof(sendStr));
    memcpy(sendStr,buf,7);
//    strcat(&sendStr[7],mac);
     memcpy(&sendStr[7],mac,strlen(mac) );

    sendStr[strlen(sendStr)] = Xor(sendStr,strlen(sendStr));
    m_spcomm->writeData(sendStr, sizeof(sendStr)+1);

    char str[128]={0};
    Printf(sendStr,25,str);
    textEdit->append(str);


}

void MainWindow::readDate_pushButton_clicked()
{
    m_spcomm->clear();
    uchar buf[32];
    memset(buf,0,sizeof(buf));
    buf[0]=0x5A;
    buf[1]=0x2B;
    buf[2]=0x22;
    buf[24]=Xor((char *)buf,24);
    m_spcomm->writeData((char *)buf, 25);

    char str[128]={0};
    Printf((char *)buf,25,str);
    textEdit->append(str);

    memset(buf,0,sizeof(buf));
    m_spcomm->readData((char *)buf,20,1000);
//    memset(str,0,128);
//    Printf((char *)buf,20,str);
//    textEdit->append(str);
//    sleep(100);
    if(buf[0]==0x2B && buf[1]==0x22)
    {
        editVolL1->setText(QString::number(buf[2]));
        editVolL2->setText(QString::number(buf[3]));
        editVolL3->setText(QString::number(buf[4]));
        editCurL1->setText(QString::number((buf[5]*256+buf[6])/10.0, 'f', 1));
        editCurL2->setText(QString::number((buf[7]*256+buf[8])/10.0, 'f', 1));
        editCurL3->setText(QString::number((buf[9]*256+buf[10])/10.0, 'f', 1));
        editEngL1->setText(QString::number((buf[11]*256*256+buf[12]*256+buf[13])/10.0, 'f', 1));
        editEngL2->setText(QString::number((buf[14]*256*256+buf[15]*256+buf[16])/10.0, 'f', 1));
        editEngL3->setText(QString::number((buf[17]*256*256+buf[18]*256+buf[19])/10.0, 'f', 1));
    }

}

void MainWindow::cleanDate_pushButton_clicked()
{
    m_spcomm->clear();
    char buf[32];
    memset(buf,0,sizeof(buf));
    buf[0]=0x5A;
    buf[1]=0x2C;
    buf[2]=0x22;
    buf[24]=Xor(buf,24);
    m_spcomm->writeData(buf, 25);

    char str[128]={0};
    Printf(buf,25,str);
    textEdit->append(str);
}

void MainWindow::adjustDate_pushButton_clicked()
{
    m_spcomm->clear();
    char buf[32];
    memset(buf,0,sizeof(buf));
    buf[0]=0x5A;
    buf[1]=0x2C;
    buf[2]=0x33;
    buf[24]=Xor(buf,24);
    m_spcomm->writeData(buf, 25);

    char str[128]={0};
    Printf(buf,25,str);
    textEdit->append(str);
}

void MainWindow::timerDone()
{
    m_spcomm->clear();
    char buf[32];
    memset(buf,0,sizeof(buf));
    buf[0]=0x5A;
    buf[1]=0x2B;
    buf[2]=0x33;
    buf[4]=Xor(buf,4);
    m_spcomm->writeData(buf, 5);


    memset(buf,0,sizeof(buf));
    if(5==m_spcomm->readData(buf,5,1000) && buf[0]==0x2B && buf[1]==0x33)
    {
        stateTag->setText("已连接");
        flag=0;
    }
    else
    {
        if(flag>=3)
        {
            stateTag->setText("未连接");
        }
        flag++;
    }
}


