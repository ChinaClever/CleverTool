#include "widget.h"
#include "ui_widget.h"

//#include <windows.h>
#include <QDebug>
#include "datadefine.h"
#include <QTime>
//static returntableData1 returnData1;
//static returntableData3 returnData3;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    port = new SerialportOperate;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimer()));

    initComboxDa(0);
    updateStateAndButton();

    ui->comboBox_3->setCurrentIndex(1);
}

Widget::~Widget()
{
    delete ui;
   // delete showThread;
    delete port;
}

void Widget::sleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

/**
 * *********************************************************************************
 * :串口通用部分
 * *********************************************************************************
 */
void Widget::initComboxDa(int flag)
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

void Widget::updateStateAndButton()
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

void Widget::initText() //状态
{
    ui->L1B->setText("---");
    ui->L2B->setText("---");
    ui->L3B->setText("---");
}

void Widget::on_pushButton_2_clicked()
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

void Widget::on_comboBox_currentIndexChanged(const QString &arg1)
{
    updateStateAndButton();
}

/**
 * *********************************************************************************
 * :校准部分
 * *********************************************************************************
 */
//发送数据
void Widget::sendCmd(int flag)
{
    qDebug()<<"校准命令已发送";
    quint8 buf[7];
    memset(buf,0,sizeof(buf));
    if(flag == 0)  //1
        for(int i = 0; i < 7; i++)
        {
         buf[i] = snedCmd[0][i];
        }
    else if(flag == 1)  //3
        for(int i = 0; i < 7; i++)
        {
         buf[i] = snedCmd[1][i];
        }
    else if(flag == 2)  //3相
        for(int i = 0; i < 7; i++)
        {
         buf[i] = snedCmd[2][i];
        }
    else if(flag == 3)  //其他校准
        for(int i = 0; i < 7; i++)
        {
         buf[i] = CsendCmd[i];
        }

    port->sendDataToPort(mCurrentPortName,buf,sizeof(buf));
}

//获取数据
void Widget::getCmd(QByteArray &datas)
{
    datas.clear();
    datas = port->readDataFromPort(mCurrentPortName);  //读取数据
}

void Widget::on_pushButton_B1_clicked()
{
    if(!ui->pushButton_3->isEnabled()) on_pushButton_4_clicked();
    ui->pushButton_B1->setEnabled(false);
    ui->L1B->setText("---");
    if(port->checkIsOpen(mCurrentPortName))
    {
        sendCmd(0);
        QByteArray data;
        for(int i = 0; i < 10; i++){
            sleep(500);
            getCmd(data);
            if(data.length() > 0) break;
        }
        qDebug() << data.toHex();
        if(!data.isEmpty()){ //收到数据
            quint8 datas[BUFSIZ];
            memset(datas,0,sizeof(datas));
            bool ok = true;
            for(int i = 0; i < data.length() ; i++)
            {
                datas[i] = data.at(i);
                if(recvCmd[0][i] != datas[i]){
                    ok = false;
                    break;
                }
            }
            if(ok){
                 ui->L1B->setText("完成");
            }else{
                 ui->L1B->setText("回复有误");
            }
        }else{
             ui->L1B->setText("未收到回复");
        }
    } else if(!port->checkIsOpen(mCurrentPortName)){
        QString warningstr = QObject::tr("串口%1未打开").arg(mCurrentPortName);
        QMessageBox::information(this,QObject::tr("Information"),warningstr,QObject::tr("确定"));
    }
    ui->pushButton_B1->setEnabled(true);
}

void Widget::on_pushButton_B2_clicked()
{
    if(!ui->pushButton_3->isEnabled()) on_pushButton_4_clicked();
    ui->pushButton_B2->setEnabled(false);
    ui->L2B->setText("---");
    int time = 500;
    if(port->checkIsOpen(mCurrentPortName))
    {
         if(ui->comboBox_3->currentIndex()){ //交流
             time = 500;
            sendCmd(1);
         }else{  //其他校准
             time = 1000;
            sendCmd(3);
         }

        QByteArray data;
        for(int i = 0; i < 10; i++){
            sleep(time);
            getCmd(data);
            if(data.length() > 0) break;
        }
        qDebug() << data.toHex();
        if(!data.isEmpty()){ //收到数据
            quint8 datas[BUFSIZ];
            memset(datas,0,sizeof(datas));
            bool ok = true;
            for(int i = 0; i < data.length() ; i++)
            {
                datas[i] = data.at(i);

                if(ui->comboBox_3->currentIndex()){ //交流
                    if(recvCmd[1][i] != datas[i]){
                        ok = false;
                        break;
                    }
                }else{  //其他校准
                    if(recvdCmd[i] != datas[i]){
                        ok = false;
                        break;
                    }
                }

            }
            if(ok){
                 ui->L2B->setText("完成");
            }else{
                 ui->L2B->setText("回复有误");
            }
        }else{
             ui->L2B->setText("未收到回复");
        }




    } else if(!port->checkIsOpen(mCurrentPortName)){
        QString warningstr = QObject::tr("串口%1未打开").arg(mCurrentPortName);
        QMessageBox::information(this,QObject::tr("Information"),warningstr,QObject::tr("确定"));
    }
    ui->pushButton_B2->setEnabled(true);
}

void Widget::on_pushButton_B3_clicked()
{
    ui->pushButton_B3->setEnabled(false);
    if(!ui->pushButton_3->isEnabled()) on_pushButton_4_clicked();
    ui->L3B->setText("---");
    if(port->checkIsOpen(mCurrentPortName))
    {
        sendCmd(2);
        QByteArray data;
        for(int i = 0; i < 10; i++){
            sleep(500);
            getCmd(data);
            if(data.length() > 0) break;
        }
        qDebug() << data.toHex();
        if(!data.isEmpty()){ //收到数据
            quint8 datas[BUFSIZ];
            memset(datas,0,sizeof(datas));
            bool ok = true;
            for(int i = 0; i < data.length() ; i++)
            {
                datas[i] = data.at(i);
                if(recvCmd[2][i] != datas[i]){
                    ok = false;
                    break;
                }
            }
            if(ok){
                 ui->L3B->setText("完成");
            }else{
                 ui->L3B->setText("回复有误");
            }
        }else{
             ui->L3B->setText("未收到回复");
        }
    } else if(!port->checkIsOpen(mCurrentPortName)){
        QString warningstr = QObject::tr("串口%1未打开").arg(mCurrentPortName);
        QMessageBox::information(this,QObject::tr("Information"),warningstr,QObject::tr("确定"));
    }
    ui->pushButton_B3->setEnabled(true);
}

void Widget::on_pushButton_BAll_clicked()
{
    on_pushButton_clicked();
    on_pushButton_B1_clicked();
    on_pushButton_B2_clicked();
    on_pushButton_B3_clicked();
}

void Widget::on_pushButton_clicked()
{
    initText();
}


//------------------------------------------------------------------------
//发送数据
void Widget::sendCmd(int flag1, int flag2) //1 - 单三   2 - 地址
{
    qDebug()<<"校准命令已发送";
    quint8 buf[8];
    memset(buf,0,sizeof(buf));
    if(flag1  == 0){ //单相
            for(int i = 0; i < 8; i++)
             buf[i] = get1Cmd[flag2][i];
    }else if(flag1 == 1){ //3
        for(int i = 0; i < 8; i++)
         buf[i] = get3Cmd[flag2][i];
    }
    port->sendDataToPort(mCurrentPortName,buf,sizeof(buf));
}

void Widget::on_pushButton_3_clicked()
{
    ui->pushButton_3->setEnabled(false);
    int time = ui->lineEdit->text().toInt();
    timer->start(time);
}

void Widget::on_pushButton_4_clicked()
{
    timer->stop();
    ui->pushButton_3->setEnabled(true);

}

void Widget::onTimer() //时时获取
{
    qDebug() << "##############";
    if(port->checkIsOpen(mCurrentPortName))
    {
        //发送
       sendCmd(ui->comboBox_3->currentIndex(), ui->comboBox_2->currentIndex());
        //回收
       onReadAnswer();
    } else if(!port->checkIsOpen(mCurrentPortName)){
        QString warningstr = QObject::tr("串口%1未打开").arg(mCurrentPortName);
        QMessageBox::information(this,QObject::tr("Information"),warningstr,QObject::tr("确定"));
        on_pushButton_4_clicked();
    }
}

void Widget::onReadAnswer()
{

    QByteArray data;
    for(int i = 0; i < 5; i++){
        sleep(500);
        getCmd(data);
        if(data.length() > 0) break;
    }
    qDebug() << data.toHex();

    if(!data.isEmpty()){ //收到数据
        upDataShow(returnToPacket(data));

    }else{
         qDebug() <<"未收到回复";
    }
}


//返回数据打包
int Widget::returnToPacket(QByteArray &array)
{
  //  qDebug() << array.length();
    int i = 0;
    if(array.length() < 0x30){
        qDebug() << "--------------1-----------";
        //执行板地址
        returnData1.addr = array.at(i++);
        //执行板功能码
        returnData1.sign = array.at(i++);
        //长度
        for(int n = 0 ; n <2 ; n++)
            returnData1.len[n] = array.at(i++);
        //电压值
        for(int n = 0 ; n <2 ; n++)
            returnData1.bufV[n] = array.at(i++);
        //电流值
        for(int n = 0 ; n <2 ; n++)
            returnData1.bufA[n] = array.at(i++);
        //功率
        for(int n = 0 ; n <2 ; n++)
            returnData1.bufW[n] = array.at(i++);
        //电能
        for(int n = 0 ; n <4 ; n++){
            returnData1.bufEnergy[n] = array.at(i++);
          //  qDebug() << "aaa " <<returnData1.bufEnergy[i];
        }

        //压Min
        for(int n = 0 ; n <2 ; n++)
            returnData1.bufVMin[n] = array.at(i++);
        //压Max
        for(int n = 0 ; n <2 ; n++)
            returnData1.bufVMax[n] = array.at(i++);
        //压Min
        for(int n = 0 ; n <2 ; n++)
            returnData1.bufAMin[n] = array.at(i++);
        //压Max
        for(int n = 0 ; n <2 ; n++)
            returnData1.bufAMax[n] = array.at(i++);
        //温度
        returnData1.bufTem = array.at(i++);
        //湿度
        returnData1.bufHum = array.at(i++);
        //波特率
        returnData1.bufBaud = array.at(i++);
        //有用功
        for(int n = 0 ; n <2 ; n++)
            returnData1.bufUsefulW[n] = array.at(i++);
        //功率因素
        returnData1.bufPowerFa = array.at(i++);

        return 1;
    }else{
        qDebug() << "--------------3-----------";
        //执行板地址
        returnData3.addr = array.at(i++);
        //执行板功能码
        returnData3.sign = array.at(i++);
        //长度
        for(int n = 0 ; n <2 ; n++)
            returnData3.len[n] = array.at(i++);
        //电压值
        for(int m = 0 ; m <3 ; m++)
            for(int n = 0 ; n <2 ; n++)
                returnData3.bufV[m][n] = array.at(i++);
        //电流值
        for(int m = 0 ; m <3 ; m++)
            for(int n = 0 ; n <2 ; n++)
                returnData3.bufA[m][n] = array.at(i++);
        //功率
        for(int m = 0 ; m <3 ; m++)
            for(int n = 0 ; n <2 ; n++)
                returnData3.bufW[m][n] = array.at(i++);

        //电能
        for(int m = 0 ; m <3 ; m++)
            for(int n = 0 ; n <4 ; n++)
                returnData3.bufEnergy[m][n] = array.at(i++);

        //Min-Max电压
        for(int m = 0 ; m <3 ; m++){
            for(int n = 0 ; n <2 ; n++)
                returnData3.bufVMin[m][n] = array.at(i++);
            for(int n = 0 ; n <2 ; n++)
                returnData3.bufVMax[m][n] = array.at(i++);

        }

        //Min-Max电流
        for(int m = 0 ; m <3 ; m++){
            for(int n = 0 ; n <2 ; n++)
                returnData3.bufAMin[m][n] = array.at(i++);
            for(int n = 0 ; n <2 ; n++)
                returnData3.bufAMax[m][n] = array.at(i++);
        }

        //温度
        returnData3.bufTem = array.at(i++);
        //湿度
        returnData3.bufHum = array.at(i++);

        //温度Max
        returnData3.bufTemMax = array.at(i++);
        //温度min
        returnData3.bufTemMin = array.at(i++);
        //湿度
        returnData3.bufHumMax = array.at(i++);
        //湿度
        returnData3.bufHumMin = array.at(i++);

        i++;
        //波特率
        returnData3.bufBaud = array.at(i++);
        //有用功
        for(int m = 0 ; m <3 ; m++)
            for(int n = 0 ; n <2 ; n++)
                returnData3.bufUsefulW[m][n] = array.at(i++);
        //功率因素
        for(int n = 0 ; n <2 ; n++)
            returnData3.bufPowerFa[n] = array.at(i++);

        return 3;
    }
    return 0;
}

void Widget::upDataShow(int flag)
{
    QString data;
    qint64 value;
    if(flag == 1){ //1
        //L1
         value = (returnData1.bufV[0]<<8) | returnData1.bufV[1];
         data = QString::number(value);
         ui->V_2->setText(data);
         value = (returnData1.bufA[0]<<8) | returnData1.bufA[1];
         data = QString("%1.%2").arg(value / 10).arg(value %10);
         ui->A_2->setText(data);
         value = (returnData1.bufW[0]<<8) | returnData1.bufW[1];
         data = QString::number(value);
         ui->w_2->setText(data);
         value = (returnData1.bufUsefulW[0]<<8) | returnData1.bufUsefulW[1];
         data = QString::number(value);
         ui->W_2->setText(data);
         value = (returnData1.bufEnergy[0]<<24) | returnData1.bufEnergy[1]<<16 |
                  returnData1.bufEnergy[2]<<8 | returnData1.bufEnergy[3];
         qDebug() << returnData1.bufEnergy[0] << returnData1.bufEnergy[1] << returnData1.bufEnergy[2] << returnData1.bufEnergy[3];


         data = QString("%1.%2").arg(value / 10).arg(value %10);
         ui->N_2->setText(data);

         //L2
          data = "---";
          ui->V_3->setText(data);
          ui->A_3->setText(data);
          ui->w_3->setText(data);
          ui->W_3->setText(data);
          ui->N_3->setText(data);
          //L3
           ui->V_4->setText(data);
           ui->A_4->setText(data);
           ui->w_4->setText(data);
           ui->W_4->setText(data);
           ui->N_4->setText(data);
    }else if(flag == 3){ //3
       //L1
        value = (returnData3.bufV[0][0]<<8) | returnData3.bufV[0][1];
        data = QString::number(value);
        ui->V_2->setText(data);
        value = (returnData3.bufA[0][0]<<8) | returnData3.bufA[0][1];
        data = QString("%1.%2").arg(value / 10).arg(value %10);
        ui->A_2->setText(data);
        value = (returnData3.bufW[0][0]<<8) | returnData3.bufW[0][1];
        data = QString::number(value);
        ui->w_2->setText(data);
        value = (returnData3.bufUsefulW[0][0]<<8) | returnData3.bufUsefulW[0][1];
        data = QString::number(value);
        ui->W_2->setText(data);
        value = (returnData3.bufEnergy[0][0]<<24) | returnData3.bufEnergy[0][1]<<16 |
                 returnData3.bufEnergy[0][2]<<8  | returnData3.bufEnergy[0][3];

        data = QString("%1.%2").arg(value / 10).arg(value %10);
        ui->N_2->setText(data);
        //L2
         value = (returnData3.bufV[1][0]<<8) | returnData3.bufV[1][1];
         data = QString::number(value);
         ui->V_3->setText(data);
         value = (returnData3.bufA[1][0]<<8) | returnData3.bufA[1][1];
         data = QString("%1.%2").arg(value / 10).arg(value %10);
         ui->A_3->setText(data);
         value = (returnData3.bufW[1][0]<<8) | returnData3.bufW[1][1];
         data = QString::number(value);
         ui->w_3->setText(data);
         value = (returnData3.bufUsefulW[1][0]<<8) | returnData3.bufUsefulW[1][1];
         data = QString::number(value);
         ui->W_3->setText(data);
         value = (returnData3.bufEnergy[1][0]<<24) | returnData3.bufEnergy[1][1]<<16 |
                  returnData3.bufEnergy[1][2]<<8  | returnData3.bufEnergy[1][3];
         data = QString("%1.%2").arg(value / 10).arg(value %10);
         ui->N_3->setText(data);

         //L3
          value =(returnData3.bufV[2][0]<<8) | returnData3.bufV[2][1];
          data = QString::number(value);
          ui->V_4->setText(data);
          value = (returnData3.bufA[2][0]<<8) | returnData3.bufA[2][1];
          data = QString("%1.%2").arg(value / 10).arg(value %10);
          ui->A_4->setText(data);
          value = (returnData3.bufW[2][0]<<8) | returnData3.bufW[2][1];
          data = QString::number(value);
          ui->w_4->setText(data);
          value = (returnData3.bufUsefulW[2][0]<<8) | returnData3.bufUsefulW[2][1];
          data = QString::number(value);
          ui->W_4->setText(data);
          value = (returnData3.bufEnergy[2][0]<<24) | returnData3.bufEnergy[2][1]<<16 |
                   returnData3.bufEnergy[2][2]<<8  | returnData3.bufEnergy[2][3];

          data = QString("%1.%2").arg(value / 10).arg(value %10);
          ui->N_4->setText(data);
         // value = returnData3.bufHumMax;
         // qDebug()<< "000" << value;
    }
}

void Widget::on_comboBox_3_currentIndexChanged(int index)
{
    if(index){ //交流
        ui->pushButton_B2->setText("L2校准");
        ui->pushButton_B3->show();
        ui->pushButton_BAll->show();
        ui->L3B->show();
    }else{ //直流
        ui->pushButton_B2->setText("偏移校准");
        ui->pushButton_B3->hide();
        ui->pushButton_BAll->hide();
        ui->L3B->hide();
    }
}

void Widget::on_allBtn_clicked()
{
    
}
