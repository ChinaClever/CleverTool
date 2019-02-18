#include "senddataall.h"

#include <QFile>
#include <windows.h>

SendDataAll::SendDataAll(MySeriorport *Port)
{
    myPort = Port;
    mWifiSiBusbarFlag = false;
}

void SendDataAll::init(QString fileName, QString currectPort, int min, int max , bool flag)
{
    mfileName = fileName;
    mCurrentPort = currectPort;
    mMin = min;
    mMax = max;
    mWifiSiBusbarFlag = flag;
    mLastPacketNum = 0;
}

void SendDataAll::run()
{
    qDebug() <<"";
    qDebug() <<"";
    qDebug() <<"";
    qDebug() << QString("[>>↓↓↓↓↓↓↓ 批量升级%1到%2 ↓↓↓↓↓↓↓<<]").arg(mMin).arg(mMax);
    //读文件
    QFile file(mfileName);
    int packetNum = getPacketNum(file.size()) ;
    if(packetNum == 0)  //升级文件是否为空
    {
        emit sendProgress("升级文件为空，请重新选择");
      //  QMessageBox::warning(this,"waring",QObject::tr("升级文件为空，请重新选择！"),QObject::tr("确定"));
        return;
    }
    if(!file.open(QIODevice::ReadOnly)) //升级文件是否打开
    {
        emit sendProgress("文件打开失败");
       // QMessageBox::warning(this,tr("warning"),tr("文件打开失败"),tr("确定"));
        return;
    }
    QByteArray allArrayEnd = file.readAll();

    for(int caddr = mMin; caddr <= mMax; caddr++){
        //升级命令
        qDebug() <<"";
        qDebug() <<"";
        qDebug() <<"";
        qDebug() << QString("[对象切换_>>地址%1<<]").arg(caddr);
        int cret = 0 ;
        sendUpdateCmd(caddr);
        int isPass = -1;
        do {  //升级回应
            if(cret == 3 || cret == 6) sendUpdateCmd(caddr); //再次发送
            isPass = responseUpdate();
            if(isPass == 1)
            {
                if(mWifiSiBusbarFlag)
                    Sleep(1000);
                break;  //收到回复 跳出循环
            }
            else if(isPass == 0)
            {cret = 0;}
            else qDebug() <<"[Get>NG>尝试:]_"<< cret;
            Sleep(1*1000);
            cret++;
        } while (cret < 10);  //收到应答，立即向下执行，否则等待5s再向下执行 */

        if(!isPass) {
             emit sendProgress("从机"+ QString::number(caddr) + "是否启动，请检查--------------------[NG]");  // 是否启动
             continue;
        } //十次依旧无结果
        qDebug() << "";

        //-------------------------------[发送数据]------------------------------------------------------

        QByteArray allArray = allArrayEnd; //初始化数据
        int ret = 0; //第几包
        bool recvsuccessful;

        while (ret < packetNum)
        {
            recvsuccessful = false; //数据包接收成功
            QByteArray array;
            uchar addr;
            ret++;
            if(ret >= packetNum){  //最后一包
              //  int len = allArray.size() % TEXT_MAX_LEN; //取余
                mLastPacketNum = ret;
                QByteArray da;
                da = allArray;
                //da= str.toLatin1();
                char *data =  da.data();

                addr = uchar(caddr);
               // qDebug() << "EndLen:" << allArray.size() % TEXT_MAX_LEN << da.size();
                text_send_packet(addr ,data,array, da.size());  //组装数据

            }else{

              QByteArray da;
              for(int i=0; i<TEXT_MAX_LEN; ++i)      da.append(allArray.at (i));

                allArray.remove(0, TEXT_MAX_LEN);
               // da = file.read(TEXT_MAX_LEN);
                //da= str.toLatin1();
                char *data =  da.data();
                addr = uchar(caddr);
                text_send_packet(addr ,data,array);  //组装数据
            }

            QString progress = "从机" + QString::number(caddr) + "完成" + QString::number(ret) + "/" + QString::number(packetNum);
            emit sendProgress2(progress);
           // QString info = QString("packet %1 size is %2").arg(ret).arg(array.size());
           // qDebug() << info;
            int buad;
            int pass = 0;
            do {
                pass ++;

                if (mWifiSiBusbarFlag)//小于200
                {
                    int k = 0,len = array.length();
                    int endpos = len / TEXT_LEN ;
                    endpos += len / TEXT_LEN? 1: 0;
                    QByteArray temp = array;
                    do{
                        QByteArray arr;
                        if(len-k*TEXT_LEN > TEXT_LEN)
                        {
                            for(int i=0; i<TEXT_LEN; ++i)
                                arr.append(temp.at(i));
                            temp.remove(0, TEXT_LEN);
                        }
                        else
                        {
                            arr = temp;
                        }
                        if(!mCurrentPort.isEmpty() && myPort->portIsOpen(mCurrentPort, buad))
                            myPort->sendData(arr,mCurrentPort);
                        qDebug() << "[Set正确数据:]_" <<arr.toHex();
                        k++;
                        msleep(500);
                    }while(k < endpos);
                }
                else//1024
                {
                    if(!mCurrentPort.isEmpty() && myPort->portIsOpen(mCurrentPort, buad))
                        myPort->sendData(array,mCurrentPort);

                    qDebug() << "[Set正确数据:]_" <<array.toHex();
                }

                int tick = 0;//用于延时
                do {
                    tick++;
                    if(responseSendFile(ret) == true)
                    {
                        recvsuccessful = true; //接收成功
                        break;
                    }else {
                      //  qDebug() << "responseSendFile err" << tick << ret << pass;
                    }
                    sleep(1);
                } while (tick < 3);

            } while (!recvsuccessful && pass < 5); //发送数据包，直到该数据包接收成功

            qDebug() << "-------------------- [分割线] --------------------------";
            qDebug() << "";

            if(!recvsuccessful){
            /* 发送失败信号  -- 同时跳出当前升级对象*/  //QString::number(caddr)
                emit sendProgress(QString("从机%1数据发送第%2包失败，跳出升级升级，请检查---------[NG]").arg(caddr).arg(ret));
                break;
             }

        }
        if(ret == packetNum) emit sendProgress("从机"+ QString::number(caddr) + "软件升级完毕");  //当前OK
        else emit sendProgress("从机"+ QString::number(caddr) + "数据发送可能存在丢失，请检查-------------------[NG]");  //当前NG
    }

    emit sendOk(1);  //全部执行完

}

int SendDataAll::getPacketNum(int bytes)
{
   // qDebug() << "file size"<< bytes;
    int num = 0 ;

    if(bytes > 0)
        num = bytes/TEXT_MAX_LEN;

    if(bytes%TEXT_MAX_LEN != 0)
        num++;

    return num;
}

bool SendDataAll::responseSendFile(int num)
{
    QString responseStr = QString("Receive Packet %1 successful").arg(num);
    QString responseStr2 = QString("Receive Packet %1 successfu").arg(num);
    int buad;
    if(!mCurrentPort.isEmpty() && myPort->portIsOpen(mCurrentPort, buad))
    {
        QByteArray array = myPort->readData(mCurrentPort);

        QString str = QString(array);

        qDebug()<<str;
        if(str.compare(responseStr) == 0 || str.compare(responseStr2) == 0){
             qDebug() << "[Get正确数据:]_" << str;
            return true;
        }else {
            QString responseStr = QString("successful");
            QString responseStr2 = QString("successfu");
            if(num > 42) {
                QStringList list = str.split(" ");
                str = list.at(list.size()-1);
            }
            if(str.compare(responseStr) == 0 || str.compare(responseStr2) == 0){
                 qDebug() << "[Get正确数据:]_" << str;
                return true;
            }
            if(mLastPacketNum == num&&(str.contains(responseStr)|| str.contains(responseStr2)))
            {
                    qDebug() << "[Get正确数据:]_" << str;
                   return true;
             }

            if(!str.isEmpty()) qDebug() << "[Get错误数据:]_" << str << array<< QString("[参考数据：%1]").arg(responseStr);
            else qDebug() << "[Get数据失败:]_" << "空";
        }
    }
    return false;
}


/**
 * @brief MainWindow::sendUpdateCmd  发送升级命令
 */
bool SendDataAll::sendUpdateCmd(int add)  //bool型
{
    QByteArray array;
    uchar addr = uchar(add);

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
bool SendDataAll::responseUpdate()
{
   // QString responseStr = tr();

    int buad;
    if(!mCurrentPort.isEmpty() && myPort->portIsOpen(mCurrentPort, buad))
    {
        QByteArray array = myPort->readData(mCurrentPort);
        QString str = QString(array);
        qDebug() << "Data1" << str;

        if(!str.compare("Start Updata") || !str.compare("Start Updat"))
            return true;
    }
    return false;
}
