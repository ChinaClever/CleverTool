#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<qtimer.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    port = new SerialportOperate;

    initComboxData(0);
    QTimer *mTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(timeoutDone()));
    mTimer->start(3*1000);

    updateStateAndButton();

}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief combox初始化串口信息
 */
void MainWindow::initComboxData( int flag)
{
    QStringList list;
    list.clear();
    qDebug()<<"success 0";
    ui->comboBox->clear();
    if(flag == 0)
        list = port->initPortInfo();
    else if (flag == 1)
        list = port->readPortInfo();

    qDebug()<<"success 1";

    for(int i=0;i < list.size();i++)
    {
        QString str;
        str.clear();
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
    qDebug()<<"串口状态："<<mCurrentPortName<<port->checkPortState(mCurrentPortName);
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




void MainWindow::on_pushButton_clicked()
{

#if 1
    QString portName=ui->comboBox->currentText();
    QString data = ui->lineEdit->text();
    int writeBytes = 0;
    writeBytes = port->sendDataToPort(portName,data);
    qDebug()<<"发送字节数："<<writeBytes;
    //    if( writeBytes == data.size() && data.size() )
    //    {
    //        QMessageBox::information(this,tr("Information"),tr("数据发送成功"),tr("确定"));
    //    }

    QString str;
    str.clear();
    str =port->readDataFromPort(portName);
    if(!str.isEmpty())
        ui->textEdit->setText(str);
#else
    QByteArray recv = port->test();//
    if( recv.size() == 256)
    {
        for(int i=0;i<0xff;i++)
            if(recv.at(i) != i)
            {
                qDebug()<<"数据接收失败！";
            }
    }else
    {
        qDebug()<<"接收数据成功！";
    }
#endif

}

/**
 * @brief 定时刷新串口
 */
void MainWindow::timeoutDone()
{
    int index = ui->comboBox->currentIndex();
    initComboxData(1);
    if(ui->comboBox->count() > index)
        ui->comboBox->setCurrentIndex(index);
    else
        ui->comboBox->setCurrentIndex(0);
}

