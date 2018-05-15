#include "rtuthread.h"
#include "datamodel.h"
#include <QDateTime>

static uchar recvData[512];

RtuThread::RtuThread(QObject *parent) : QThread(parent)
{
    isRun = false;
    reset = false;
    mRet = 0;
    mPosition = 1;
    mPass = false;
    mAddr = mMethodType = mCount = mOffCount = 1;
    mSerialPortPlate = NULL;
    mSerialPortSource = NULL;
    connect(this, SIGNAL(sendWriteSignal(uchar*, int)), this, SLOT(onWrite(uchar*,int)));
    connect(this, SIGNAL(sendWriteSignal(QByteArray)), this, SLOT(onWrite(QByteArray)));
}

RtuThread::~RtuThread()
{
    isRun = false;
    wait();
}



/**
 * @brief 开启线程
 */
void RtuThread::startThread()
{
    if(isRun==false) {
        isRun = true;
        start();
    }
}

/**
 * @brief 停止线程
 */
void RtuThread::stopThread()
{
    isRun = false;
}

void RtuThread::initState()
{
    reset = true; //回到工位1
}

void RtuThread::workDown()
{
    /* 初始化各参数 */
    reset = false;
    mRet = 0;
    mPosition = 1;
    mPass = false;
    /* 工位一 | 握手判断存在 */
    if(!stationV1()) return;
        emit sendClear(); //清空上次的记录 -- 零时性放这
    /* 工位二 | 等待标准源起来 */
    if(!stationV2()) return;
    /* 工位三 | 重新校准 */
    if(!stationV3()) return;
    /* 工位四 | 控制标准源 - 解决重启 开关全开 读取*/
    if(!stationV4()) return;
        resSourceOutput("V0", "A0"); //断电 -- 标准源
    /* 工位五 | 取走料*/
    if(!stationV5()) return;
    /* 结束 */
    emit sendEnd();
}

/* 工位一 | 握手判断存在 */
bool RtuThread::stationV1()
{
    qDebug() << tr("[当前工位]_ 工位%1").arg(mPosition);
    emit sendStation(tr("%1.放板").arg(mPosition++));
    while (!mPass) {
        static uchar sentData[21] =
        {0x7B, 0xC1, 0x01, 0x15, 0x00, 0xFF, 0xFF, 0xFF, 0xC7, 0xC8, 0xC9, 0x00, 0x00, 0x00, 0xD7, 0xD8, 0xD9, 0x00, 0x10, 0x00, 0xF0};
        sentData[2] = mAddr;
        sentData[20] = BCC(QByteArray((char*)sentData, 20));
        emit sendWriteSignal(sentData, sizeof(sentData));
        if(reset) return false;
        if(isRun == false) return false;

        msleep(500);
        mRet = mSerialPortPlate->read(recvData, 2);
        if(mRet > 10) break;
        msleep(10);
    }
    return true;
}

 /* 工位二 | 等待标准源起来 */
bool RtuThread::stationV2()
{
    qDebug() << tr("[当前工位]_ 工位%1").arg(mPosition);
    emit sendStation(tr("%1.等源").arg(mPosition++));
    //-----------------[等待10--标准源起来]-------------------
    for(int i = 0; i < 15; i++){
        msleep(1000);
        if(reset) return false;
        if(isRun == false) return false;
    }
    return true;
}

/* 工位三 | 重新校准 */
bool RtuThread::stationV3()
{
    msleep(1000);
    qDebug() << tr("[当前工位]_ 工位%1").arg(mPosition);
    emit sendStation(tr("%1.校准").arg(mPosition++));
    while (!mPass) {
        //重置
        emit sendWriteSignal(recalibrateCmd,  sizeof(recalibrateCmd));
        if(isRun == false) return false;
        if(reset) return false;
        msleep(1000);

        //选择
        if(mMethodType == 1){ //一键校准
            emit sendWriteSignal(modelCmd1[mAddr - 1],  sizeof(modelCmd1[mAddr - 1]));
        }else{ //轮询校准
             emit sendWriteSignal(modelCmd2[mAddr - 1],  sizeof(modelCmd2[mAddr - 1]));
        }
        memset(recvData,0,sizeof(recvData));
        for(int i = 0; i < 5; i++){
            if(reset) return false;
            if(isRun == false) return false;
            msleep(500);
            mRet = mSerialPortPlate->read(recvData, 2);
            if(mRet > 3)  break;
        }
       // qDebug() << "get1:" << QByteArray((char*)recvData, ret).toHex();

        //开始
        emit sendWriteSignal(transCmd[mAddr-1],  sizeof(transCmd[mAddr-1]));
        mRet = 0;
        memset(recvData,0,sizeof(recvData));
        for(int i = 0; i < 80; i++){
            if(reset) return false;
            if(isRun == false) return false;
            msleep(500);
            mRet = mSerialPortPlate->read(recvData, 2);
            if(mRet > 3) break;
        }

        //解析data
        if(mRet > 3){
            bool ok = true;
            for(int i = 1; i<mRet-1; i++){
                if(recvData[i] != recvCmd[0][i-1] && recvData[i] != recvCmd[1][i-1])
                    ok = false;
            }
            if(ok) break;
        }
    }
    return true;
}

/* 工位四 | 控制标准源 - 解决重启 开关全开 读取*/
bool RtuThread::stationV4()
{
    qDebug() << tr("[当前工位]_ 工位%1").arg(mPosition);
    emit sendStation(tr("%1.采集").arg(mPosition++));
    msleep(3000); //重启

   // resSource();// 消除校准源报警

    int value = 0;
    while (!mPass) {
        if(reset) return false;
        if(isRun == false) return false;
        static uchar sentData[21] =
        {0x7B, 0xC1, 0x01, 0x15, 0xA1, 0x00, 0x00, 0x00, 0xC7, 0xC8, 0xC9, 0x00, 0x00, 0x00, 0xD7, 0xD8, 0xD9, 0x00, 0x10, 0x00, 0xF0};
        sentData[2] = mAddr;
        sentData[mAddr + 4] = 0xFF;
        sentData[20] = BCC(QByteArray((char*)sentData, 20));
        emit sendWriteSignal(sentData, sizeof(sentData));

        memset(recvData,0,sizeof(recvData));
        for(int i = 0; i < 5; i++){
            if(reset) return false;
            if(isRun == false) return false;
            msleep(500);
            mRet = mSerialPortPlate->read(recvData, 2);
            if(mRet > 3) break;
        }
       // qDebug() << "get3:" << QByteArray((char*)recvData, ret).toHex();

        //解析数据
        ReturnTableData *returnData = new ReturnTableData;
        int i = 0;
        //执行板返回标志
        returnData->sign = recvData[i++];
        //执行板地址
        returnData->addr = recvData[i++];
        //长度
        returnData->len = recvData[i++];
        //开关状态
        returnData->onoffState = recvData[i++];
        //八位电流值
        for(int m = 0; m < 8; m++)
            for(int n = 0 ; n <2 ; n++)
                returnData->current[m][n] = recvData[i++];
        //电压值
        for(int m = 0; m < 2; m++){
            for(int n = 0 ; n <2 ; n++)
                returnData->volate[m*4][n] =  returnData->volate[m*4 +1][n] =
                        returnData->volate[m*4 +2][n] =  returnData->volate[m*4 +3][n] = recvData[i++];
        }
        //功率
        for(int m = 0; m < 8; m++)
            for(int n = 0 ; n <2 ; n++)
                returnData->power[m][n] = recvData[i++];
        //异或校验码
        returnData->xornumber = recvData[i++];
     //   qDebug() << "BCC" << returnData.power[7][1]  <<   returnData.xornumber;
        emit showDataSignal(value, returnData); //确定第几次刷新OK NG
        if(++value > mCount) break;
    }
    return true;
}

/* 工位五 | 取走料*/
bool RtuThread::stationV5()
{
    qDebug() << tr("[当前工位]_ 工位%1").arg(mPosition);
    emit sendStation(tr("%1.取板").arg(mPosition++));
    int value = 0;  //三次通信无果  判定为取走 回到工位1
    while (!mPass) {
        if(reset) return false;
        if(isRun == false) return false;
        static uchar sentData[21] =
        {0x7B, 0xC1, 0x01, 0x15, 0x00, 0xFF, 0xFF, 0xFF, 0xC7, 0xC8, 0xC9, 0x00, 0x00, 0x00, 0xD7, 0xD8, 0xD9, 0x00, 0x10, 0x00, 0xF0};
        sentData[2] = mAddr;
        sentData[20] = BCC(QByteArray((char*)sentData, 20));
        emit sendWriteSignal(sentData, sizeof(sentData));
        msleep(500);
        memset(recvData,0,sizeof(recvData));
        mRet = mSerialPortPlate->read(recvData, 2);
        if(mRet == 0 && ++value >= mOffCount) break;
        if(mRet > 10) value = 0; //复原  要求连续mOffCount次无回复
        msleep(10);
    }
    return true;
}

void RtuThread::resSource()
{
    // 过载复位		r+回车	即r\r
    QByteArray array;
    QString data;
    data ="r";
    int len = data.length();
    for(int i = 0; i < len; i++){
        array.append(data.at(i));
    }
    array.append(0x0D); //回车
    qDebug() << "[标准源]_" <<array.toHex();
    if(mSerialPortSource && mSerialPortSource->isOpened()) //串口是打开状态
        emit sendWriteSignal(array);; //通过串口 把数据发出

}

void RtuThread::resSourceOutput(QString V, QString A) //220V 6A
{
    for(int n = 0; n < 3; n++){
        QByteArray array;
        QString data;
        if(n == 0) { //量程
            data ="I0 220 6 15 1200";
        }else if(n == 1){ //V
            data = V;
        }else if(n == 2){  //A
            data = A;
        }
        int len = data.length();
        for(int i = 0; i < len; i++){
            array.append(data.at(i));
        }
        array.append(0x0D);
        if(mSerialPortSource && mSerialPortSource->isOpened()) //串口是打开状态
            emit sendWriteSignal(array); //通过串口 把数据发出
        msleep(200);
        if(n == 1) msleep(1000);
    }
}

void RtuThread::onWrite(uchar *sent, int len) //PDU串口
{
    mSerialPortPlate->write(sent, len);
}

void RtuThread::onWrite(QByteArray array) //标准源串口
{
    mSerialPortSource->write(array);
}

void RtuThread::run()
{
    while (isRun) {
        QString str = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
       // qDebug() << str;
        emit sendTime(str);
        workDown();
    }
}
