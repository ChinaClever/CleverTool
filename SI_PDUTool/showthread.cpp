#include "showthread.h"

#include <QDebug>

ShowThread::ShowThread(SerialportOperate *Port)
{
    mCurrentPort = "";
    flag1 = flag2 = 0;
    myPort = Port;
    mTimer = 1000;
    isRun = false;
}

ShowThread::~ShowThread()
{

}

void ShowThread::timerData(int flaga, int flagb, QString name, int timer)
{
    mCurrentPort = name;
    flag1 = flaga;
    flag2 = flagb;
    mTimer = timer;
    isRun = true;
    start();
}

void ShowThread::timerStop()
{
    isRun = false;
}

void ShowThread::run()
{
    while (isRun) {

        qDebug() << "##############";
        if(myPort->checkIsOpen(mCurrentPort))
        {
            //发送
           sendCmd(flag1, flag2);
            //回收
       //    onReadAnswer();
        } else if(!myPort->checkIsOpen(mCurrentPort)){
            QString warningstr = QObject::tr("串口%1未打开").arg(mCurrentPort);
            //QMessageBox::information(this,QObject::tr("Information"),warningstr,QObject::tr("确定"));
            qDebug() << warningstr;
            isRun = false;
        }
        msleep(mTimer);
    }
}

void ShowThread::sendCmd(int flag1, int flag2)
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
    myPort->sendDataToPort(mCurrentPort,buf,sizeof(buf));
}
/*
void ShowThread::getCmd(QByteArray &data)
{
    data.clear();
    data = myPort->readDataFromPort(mCurrentPort);  //读取数据
}

void ShowThread::onReadAnswer()
{
    QByteArray data;
    for(int i = 0; i < 10; i++){
        msleep(500);
        getCmd(data);
        if(data.length() > 0) break;
    }
    qDebug() << data.toHex();
    /*
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


  /*  QByteArray answer;
    answer.clear();
    answer = port->readDataFromPort(mCurrentPortName);  //读取数据
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

    }
    else //数据未收到，如果is_read一直为false，那么后面数据将一直无法发送，所以延迟置1，延迟时间内无法发送
    {
        qDebug()<<"数据未收到";
    }* /
}
*/

