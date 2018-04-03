/**
 * mainwindow.cpp
 * 激活码生成器，主界面
 *
 *  Created on: 2016年10月29日
 *      Author: Lzy
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"machinecode.h"
#include<QDate>
#include"activecode.h"
#include<QPalette>
#include<QTime>
#include<QDateTime>
#include<QByteArray>
#include<QClipboard>
#include<QApplication>
#include<registrykey.h>
#include<machinecode.h>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    is_free=false;




    QPixmap img1(":/wdl/image/4.png");
    QPixmap img=img1.scaled(this->width(),this->height());

    QPalette back_img;
    back_img.setBrush(QPalette::Background,QBrush(img));
    this->setPalette(back_img);
    this->setAutoFillBackground(true);


    ui->label->setStyleSheet("background:transparent");
    ui->label_2->setStyleSheet("background:transparent");
    ui->label_3->setStyleSheet("background:transparent");
    ui->label_4->setStyleSheet("background:transparent");
    ui->label_5->setStyleSheet("background:transparent");//设置label透明
    ui->label_6->setStyleSheet("background:transparent");
    ui->label_8->setStyleSheet("background:transparent");
    ui->label_10->setStyleSheet("background:transparent");
    ui->label_11->setStyleSheet("background:transparent");
    ui->label_12->setStyleSheet("background:transparent");

    QPalette palw,palq,pale;
    palw.setColor(QPalette::ButtonText,QColor(0,100,0));
    ui->pushButton->setPalette(palw);
    ui->pushButton_2->setPalette(palw);
    //ui->pushButton_3->setPalette(palw);
    ui->pushButton_3->setPalette(palw);
    ui->pushButton_4->setPalette(palw);
    ui->pushButton_5->setPalette(palw);

    palq.setColor(QPalette::Text,QColor(47,79,79));
    ui->lineEdit->setPalette(palq);
    ui->lineEdit_2->setPalette(palq);


    MachineCode computer;

    // str=computer.getMachineCode();  //获取机器码
    //ui->lineEdit_2->setText(str);

    pale.setColor(QPalette::WindowText,QColor(0,0,0));
    ui->lineEdit_2->setPalette(pale);
    ui->label_9->setPalette(pale);
    ui->label_8->setPalette(pale);

    pal.setColor(QPalette::WindowText, Qt::red);
    ui->label_8->setPalette(pal);
    ui->label_10->setPalette(pal);
    ui->label_12->setPalette(pal);
    //ui->label_13->setPalette(pal);

    //ui->pushButton_3->setToolTip(tr("点击激活本机"));
    ui->pushButton_2->setToolTip(tr("复制激活码"));
    ui->pushButton->setToolTip(tr("点击获取激活码"));

    ui->pushButton_3->setVisible(false);




}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

    ActiveCode active;
    bool is_nuab;
     is_free=false;



    is_nuab=com_info();
    str = str.trimmed();  //机器码去除空格

    if(num&&is_late&&(!str.isEmpty())&&is_nuab)

        str5=active.createActiveCode(date,num,str);
    ui->label_9->setText(str5);

}


/*
 * 判断注册信息是否正确
 */
bool MainWindow::com_info()
{
    bool get;
    str5.fill(0,str5.size());
    num=0;

    getdate();
    if(is_late)
        getenum();

    //获取机器码
    str=ui->lineEdit_2->text();
    if(str.isEmpty()&&num&&is_late)
    {
        if(!is_free)
            ui->label_10->setText(tr("请输入机器码！"));
        sleep(800);
        ui->label_10->clear();

    }
    else if(!str.isEmpty()&&num)
    {
        get=is_right(str);  //判断机器码是否为数字及字母
        if(!get) //如果不是纯数字或字母
        {
            ui->label_10->setText(tr("机器码格式错误"));
            sleep(800);
            ui->label_10->clear();

        }

    }

    return get;
}


/*
 * 延迟函数
 */
void MainWindow::sleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}


/*
 *获取编辑框中的日期，并与当前时间比较
 */
void MainWindow::getdate()
{
    QDate currentdate,date1;

    QDateTime time=QDateTime::currentDateTime();
    currentdate=time.date();

    date1=ui->dateEdit->date();
    is_late=date1.operator >(currentdate);
    if(is_late)
    {
        date=date1;
    }else{
        //QMessageBox::information(this,tr("information"),tr("时间已失效"));

        ui->label_6->setPalette(pal);
        ui->label_6->setText(tr("日期已失效！"));
        ui->label_9->clear();


        sleep(800);
        ui->label_6->clear();


    }
}


/*
 *获取数量，判断是否为空以及是否为数字格式
 */
void MainWindow::getenum()
{
    QString str3;
    num=0;
    str3=ui->lineEdit->text();
    str3 = str3.trimmed();
    if(!str3.isEmpty()&&is_number(str3))
    {

        num=str3.toInt();
    }else{
        //弹出对话框，提示输入数量
        // QMessageBox::information(this,tr("information"),tr("sorry,The number is lost!"));
        // pal.setColor(QPalette::WindowText, Qt::red);
        ui->label_5->setPalette(pal);
        if(str3.isEmpty())
        {
            ui->label_5->setText(tr("请输入数量!"));
            ui->label_9->clear();
        }else {
            ui->label_5->setText(tr("数量输入错误!"));
            ui->lineEdit->clear();
            ui->label_9->clear();
        }
        //sleep(3000);
        sleep(1000);
        ui->label_5->clear();
    }
    qDebug("%d",num);

}


/*
  *获取数量输入是否为纯数字格式
  */
bool MainWindow::is_number(QString src)
{
    QByteArray ba = src.toLatin1();//QString 转换为 char*
    const char *s = ba.data();

    while(*s && *s>='0' && *s<='9') s++;

    if (*s)
    {
        return false;
    }
    else
    {
        return true;
    }
}


/*
  * 点击复制按钮，复制激活码到剪切板
  */
void MainWindow::on_pushButton_2_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->label_9->text());
    QString str9=NULL;

    str9=ui->label_9->text();
    if(!str9.isNull())
        ui->label_8->setText(tr("文本复制成功"));
    sleep(500);
    ui->label_8->clear();
}

bool MainWindow::is_right(QString str)
{
    QByteArray ba = str.toLatin1();//QString 转换为 char*
    const char *s = ba.data();

    while(*s && *s>='0' && *s<='9'||(*s>='a'&&*s<='z')||(*s>='A'&&*s<='Z')) s++;

    if (*s)  //不是纯数字及字母
    {
        if(*s!=32)  //如果是空格
            return false;
    }
    else
    {
        return true;
    }

}

/*
 *将注册信息写入注册表
 */
void MainWindow::register_info()
{
    RegistryKey *key;
    MachineCode *macv;
    bool  is_ok;
    ActiveCode active;
    QString str8;//机器码

    is_ok=com_info();
    str8=macv->getMachineCode();
    ui->lineEdit_2->setText(str8);
    //qDebug("%d\n",is_ok);
    if(num&&is_late&&(!str8.isEmpty()))
    {
        str5=active.createActiveCode(date,num,str8);

        qDebug("success");

        ui->label_9->setText(str5);
    }
    if(!str5.isEmpty())
    {
        key->writeDate(date);
        key->writeCount(num);
        key->writeMachineCode(str);
        key->writeActiveCode(str5);
    }
}




void MainWindow::on_pushButton_3_clicked()
{
    is_free=true;
    QPixmap map1(":/wdl/image/1203585.png");
    QPixmap map2=map1.scaled(50,50);


    register_info();
    if(num&&is_late)
    {
        ui->label_7->setPixmap(map2);
        ui->label_12->setText(tr("激活成功"));
        ui->label_12->setAlignment(Qt::AlignCenter);
    }
    sleep(1000);
    ui->label_12->clear();

}


/*
 * 获取本机机器码
 */
void MainWindow::on_pushButton_4_clicked()
{
    MachineCode code;
    str=code.getMachineCode();
    ui->lineEdit_2->setText(str);
    ui->pushButton_3->setVisible(true);
}

void MainWindow::on_lineEdit_2_textChanged(const QString &arg1)
{
    ui->pushButton_3->setVisible(false);
}

/*
 * 注销本机
 */
void MainWindow::logout()
{
    RegistryKey key;
    QPixmap map(":/wdl/image/526912.png");
    QPixmap mapp=map.scaled(50,50);
    key.writeMachineCode("helloworld");//注销
    //ui->label_13->setText(tr("本机已注销！"));
    ui->label_7->setPixmap(mapp);
    ui->label_12->setText(tr("已注销"));
    ui->label_12->setAlignment(Qt::AlignCenter);
    sleep(1000);
    ui->label_12->clear();
}

void MainWindow::on_pushButton_5_clicked()
{
    logout();
}


/*
 * 检测本机是否注册
 */
void MainWindow::is_register()
{
    //检测本机有没有注册
    QPixmap map1(":/wdl/image/1203585.png");
    QPixmap map2=map1.scaled(50,50);
    QPixmap map(":/wdl/image/526912.png");
    QPixmap mapp=map.scaled(50,50);

    RegistryKey key;
    str=key.readMachineCode();
    if(str=="helloworld")
    {
        ui->label_7->setPixmap(mapp);
        ui->label_12->setText("本机未注册");
        ui->label_12->setAlignment(Qt::AlignCenter);
    }
    else
    {
        ui->label_7->setPixmap(map2);
        ui->label_12->setText("本机已注册");
        ui->label_12->setAlignment(Qt::AlignCenter);
    }
    sleep(10000);
   ui->label_12->clear();

}
