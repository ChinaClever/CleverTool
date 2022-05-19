#include "senddata.h"

#include <QFile>
#include <QWindow>

SendData::SendData(MySeriorport *Port)
{
    myPort = Port;
    mWifiSiBusbarFlag = false;
}
void SendData::init(QString fileName, QString currectPort, int addr , bool flag)
{
    mfileName = fileName;
    mCurrentPort = currectPort;
    mAddr = addr;
    mWifiSiBusbarFlag = flag;
    mLastPacketNum = 0;
}

void SendData::run()
{
    QFile file(mfileName);

    int packetNum = getPacketNum(file.size()) ;
 //   qDebug() << "packet num" << packetNum;  //包数

    if(packetNum == 0)  //升级文件是否为空
    {
      //  QMessageBox::warning(this,"waring",QObject::tr("升级文件为空，请重新选择！"),QObject::tr("确定"));
        return;
    }

    if(!file.open(QIODevice::ReadOnly)) //升级文件是否打开
    {
       // QMessageBox::warning(this,tr("warning"),tr("文件打开失败"),tr("确定"));
        return;
    }

    qDebug() <<"";
    qDebug() <<"";
    qDebug() <<"";
    qDebug() << QString("[对象切换_>>地址%1<<]").arg(mAddr);
//    qDebug() << QString("[object switching_>>address %1<<]").arg(mAddr);
    QByteArray allArray = file.readAll();

    //qDebug() << allArray.size() % TEXT_MAX_LEN; //取余


   // QTextStream in(&file);  //字符流可能有问题  -- 改 QDataStream 或直接 QFile
    int ret = 0; //第几包
    bool recvsuccessful = false; //数据包接收成功

    while (ret < packetNum)
    {
        QByteArray array;
        uchar addr;
        ret++;
        recvsuccessful = false; //数据包接收成功
        if(ret >= packetNum){  //最后一包
           //int len = allArray.size() % TEXT_MAX_LEN; //取余
            mLastPacketNum = ret;
            QByteArray da;
            da = allArray;
            //da= str.toLatin1();
            char *data =  da.data();

            addr = uchar(mAddr);
          //  qDebug() << "EndLen:" << allArray.size() % TEXT_MAX_LEN << da.size();
            text_send_packet(addr ,data,array, da.size());  //组装数据

        }else{

          QByteArray da;
          for(int i=0; i<TEXT_MAX_LEN; ++i)      da.append(allArray.at (i));

            allArray.remove(0, TEXT_MAX_LEN);
           // da = file.read(TEXT_MAX_LEN);
            //da= str.toLatin1();
            char *data =  da.data();
            addr = uchar(mAddr);
            text_send_packet(addr ,data,array);  //组装数据
        }

//        QByteArray xxx;
//        send_to_packet(0,xxx);
        QString progress = QString::number(ret) + "/" + QString::number(packetNum);
        emit sendProgress(progress);
        //QString info = QString("packet %1 size is %2").arg(ret).arg(array.size());
       // qDebug() << "[Sen尝试:]_" << info;

        int buad;

        int pass = 0;
        do{
            pass++;

            if (mWifiSiBusbarFlag)//小于200
            {
                int k = 0,len = array.length();
                int endpos = len / TEXT_LEN ;
                endpos += len / TEXT_LEN? 1: 0;
                QByteArray temp = array;
                while(k < endpos)
                {
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
//                    qDebug() << "[Set right data:]_" <<arr.toHex();
                    k++;
                    msleep(500);
                }

            }
            else//1024
            {
                if(!mCurrentPort.isEmpty() && myPort->portIsOpen(mCurrentPort, buad))
                    myPort->sendData(array,mCurrentPort);

                qDebug() << "[Set正确数据:]_" <<array.toHex();
//                qDebug() << "[Set right data:]_" <<array.toHex();
            }


            int tick = 0;//用于延时
            int recvError = 0;

            do {
                tick++;
                recvError = responseSendFile(ret);
                if(recvError == 1)
                {
                    recvsuccessful = true; //接收成功
                    break;
                }else if(recvError == -1){
                   qDebug() << "responseSendFile err" << tick << ret;
                    break;
                }
                sleep(1);
            } while (tick < 3);
            if(recvError == -1)
            {
                recvsuccessful = false;
                break;
            }
        } while (!recvsuccessful && pass < 5); //发送数据包，直到该数据包接收成功

        qDebug() << "-------------------- [分割线] --------------------------";
//       qDebug() << "-------------------- [split line] --------------------------";
        qDebug() << "";

        if(!recvsuccessful){
        /* 发送失败信号  -- 同时跳出当前升级对象*/  //QString::number(caddr)
           qDebug() << "[<定点升级失败>]_" << QString("从机数据发送第%2包失败跳出升级升级，请检查").arg(ret);
//            qDebug() << "[<upgrade failed>]_" << QString("Failed to send the %2 package of data from the machine, jump out of the upgrade, please check.").arg(ret);
            break;
         }

    }


   // qDebug() << "quie";

  /*  if(ret == packetNum)
        QMessageBox::information(this,tr("information"),tr("OK"),tr("quit"));
    else
        QMessageBox::warning(this,tr("information"),tr("NG"),tr("quit"));
*/

    if(ret == packetNum&&recvsuccessful) emit sendOk(1);
    else emit sendOk(0);
}

int SendData::getPacketNum(int bytes)
{
  //  qDebug() << "file size"<< bytes;
    int num = 0 ;

    if(bytes > 0)
        num = bytes/TEXT_MAX_LEN;

    if(bytes%TEXT_MAX_LEN != 0)
        num++;

    return num;
}

int SendData::responseSendFile(int num)
{
    QString responseStr = QString("Receive Packet %1 successful").arg(num);
    QString responseStr2 = QString("Receive Packet %1 successfu").arg(num);
    int buad;
    if(!mCurrentPort.isEmpty() && myPort->portIsOpen(mCurrentPort, buad))
    {
        QByteArray array = myPort->readData(mCurrentPort);

        QString str = QString(array);
        if(str.compare(responseStr) == 0 || str.compare(responseStr2) == 0){
             qDebug() << "[Get正确数据:]_" << str;
//            qDebug() << "[Get right data:]_" << str;
            return 1;
        }
        else {
            QString responseStr = QString("successful");
            QString responseStr2 = QString("successfu");
            if(num > 42) {
                QStringList list = str.split(" ");
                str = list.at(list.size()-1);
            }
            if(str.contains(responseStr)|| str.contains(responseStr2)){
                 qDebug() << "[Get正确数据:]_" << str;
//                qDebug() << "[Get right data:]_" << str;
                return 1;
            }
            if(mLastPacketNum == num&&(str.contains(responseStr)|| str.contains(responseStr2)))
            {
                    qDebug() << "[Get正确数据:]_" << str;
//                qDebug() << "[Get right data:]_" << str;
                   return 1;
             }
            if(!str.isEmpty()) {qDebug() << "[Get错误数据:]_" << str << QString("[参考数据：%1]").arg(responseStr);return -1;}
            else qDebug() << "[Get数据失败:]_" << "空";
//            if(!str.isEmpty()) {qDebug() << "[Get wrong data:]_" << str << QString("[Reference data：%1]").arg(responseStr);return -1;}
//            else qDebug() << "[Get wrong data:]_" << "null";
        }
    }

    return 0;
}
