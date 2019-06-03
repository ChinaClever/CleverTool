#include "rtuthread.h"
#include "datamodel.h"
#include "commondata.h"
#include <QDateTime>
QReadWriteLock lock;
static uchar recvData[512];

RtuThread::RtuThread(QObject *parent) : QThread(parent)
{
    isRun = false;
    isRead = false;
    mAddr = 0;
    mDc = 0;
    mDcType = 0;
    mSerialPortPlate = NULL;
    pkt = (Rtu_recv*)malloc(sizeof(Rtu_recv));
    //free(pkt);

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
    emit sendEnd();
}

void RtuThread::workDown()
{
    if(mDc){ //交流
        emit sendStr(tr("[地址%1_开始校准]_交流...").arg(mAddr));
        communication();
    }else{ //直流
        emit sendStr(tr("[地址%1_开始校准]_直流...").arg(mAddr));
        directd();
    }
    emit sendStr(" "); //分界线
}

void RtuThread::communication()
{
    static uchar sentData[7] =
    {0x7B, 0xA1, 0x00, 0x00, 0x00, 0xBB,  0x61};
    //----------------- [固定] --------------------------
    // sentData[2] = mAddr;
    // sentData[6] = BCC(QByteArray((char*)sentData, 6));
    //-----------------------------------------------------
    emit sendWriteSignal(sentData, sizeof(sentData));
    qDebug() << "send" << QByteArray((char*)sentData, sizeof(sentData)).toHex();

    msleep(500);
    int ret = 0;
    memset(recvData,0,sizeof(recvData));
    for(int i = 0; i < 80; i++){
        if(isRun == false) return;
        msleep(500);
        ret = mSerialPortPlate->read(recvData, 2);
        if(ret > 3) break;
    }

    //解析data
    if(ret > 3){
        bool ok = true;
        uchar recvCmd[7] = {0x7B, 0xA1, 0xDD, 0xDD, 0xDD, 0xBB, 0xBC};
        for(int i = 1; i < ret-1; i++){
            if(recvData[i] != recvCmd[i])
                ok = false;
        }
        if(ok) emit sendStr(tr("[地址%1_校准通过]_回复正确").arg(mAddr));
        else emit sendStr(tr("[地址%1_校准通过]_回复错误").arg(mAddr));
    }else{
        emit sendStr(tr("[地址%1_校准失败]_无回复").arg(mAddr));
    }

    qDebug() << "get3:" << QByteArray((char*)recvData, ret).toHex();
}

void RtuThread::directd()
{
    if(mDcType){ //1 - 220v 10A:校准电流电压
        emit sendStr(tr("[地址%1_校准电流电压校准]_直流<<<").arg(mAddr));
        directd10();
    }else{ // 0 - 220v 0A：校准偏移量
        emit sendStr(tr("[地址%1_校准偏移量]_直流<<<").arg(mAddr));
        directd0();
    }
}

void RtuThread::directd0()
{
    static uchar sentData[7] =
    {0x7b, 0xA2, 0x00, 0x00, 0x00, 0xBB, 0x62};
    emit sendWriteSignal(sentData, sizeof(sentData));
    qDebug() << "send" << QByteArray((char*)sentData, sizeof(sentData)).toHex();

    msleep(500);
    int ret = 0;
    memset(recvData,0,sizeof(recvData));
    for(int i = 0; i < 80; i++){
        if(isRun == false) return;
        msleep(500);
        ret = mSerialPortPlate->read(recvData, 2);
        if(ret > 3) break;
    }

    //解析data
    if(ret > 3){
        bool ok = true;
        uchar recvCmd[7] = {0x7B, 0xA2, 0xCC, 0xCC, 0xCC, 0xBB, 0xAE};
        for(int i = 1; i < ret-1; i++){
            if(recvData[i] != recvCmd[i])
                ok = false;
        }
        if(ok) emit sendStr(tr("[地址%1_校准通过]_回复正确").arg(mAddr));
        else emit sendStr(tr("[地址%1_校准通过]_回复错误").arg(mAddr));
    }else{
        emit sendStr(tr("[地址%1_校准失败]_无回复").arg(mAddr));
    }
    qDebug() << "get3:" << QByteArray((char*)recvData, ret).toHex();
}

void RtuThread::directd10()
{
    static uchar sentData[7] =
    {0x7b, 0xA1, 0x00, 0x00, 0x00, 0xBB, 0x61};
    emit sendWriteSignal(sentData, sizeof(sentData));
    qDebug() << "send" << QByteArray((char*)sentData, sizeof(sentData)).toHex();

    msleep(500);
    int ret = 0;
    memset(recvData,0,sizeof(recvData));
    for(int i = 0; i < 80; i++){
        if(isRun == false) return;
        msleep(500);
        ret = mSerialPortPlate->read(recvData, 2);
        if(ret > 3) break;
    }

    //解析data
    if(ret > 3){
        bool ok = true;
        uchar recvCmd[7] = {0x7B, 0xA1, 0xDD, 0xDD, 0xDD, 0xBB, 0xBC};
        for(int i = 1; i < ret-1; i++){
            if(recvData[i] != recvCmd[i])
                ok = false;
        }
        if(ok) emit sendStr(tr("[地址%1_校准通过]_回复正确").arg(mAddr));
        else emit sendStr(tr("[地址%1_校准通过]_回复错误").arg(mAddr));
    }else{
        emit sendStr(tr("[地址%1_校准失败]_无回复").arg(mAddr));
    }
    qDebug() << "get3:" << QByteArray((char*)recvData, ret).toHex();
}

void RtuThread::readData()
{
    /* 01 03 00 00 00 D1 05 F8*/
    static uchar sentDataAc[8] = {0x01, 0x03, 0x00, 0x00, 0x00, 0xE0, 0x05, 0xF8};
    static uchar sentDataDc[8] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x67, 0x04, 0x20};//72 61 64 69 6F 61 64 72
    //static uchar sentDataAc[16] = {0x72, 0x61, 0x64, 0x69, 0x6F, 0x61, 0x64, 0x72, 0x01, 0x03, 0x00, 0x00, 0x00, 0xE0, 0x05, 0xF8};
    //static uchar sentDataDc[16] = {0x72, 0x61, 0x64, 0x69, 0x6F, 0x61, 0x64, 0x72, 0x01, 0x03, 0x00, 0x00, 0x00, 0x6C, 0x04, 0x20};
    uchar *sentData = sentDataDc;
    if(mDc) sentData = sentDataAc;

    sentData[0] = mAddr;
    //ushort crc = CRC16_22((char*)sentData,8,14);
    ushort crc = CRC16_2((char*)sentData, 6);
    sentData[6] = (0xff)&(crc); /*低8位*/
    sentData[7] = ((crc) >> 8); /*高8位*/
    emit sendWriteSignal(sentData, sizeof(sentDataDc));
    //emit sendStr(QByteArray((char*)sentData, sizeof(sentDataDc)).toHex());
    qDebug() << "send" << QByteArray((char*)sentData, sizeof(sentDataDc)).toHex();

    msleep(500);
    int ret = 0;
    memset(recvData,0,sizeof(recvData));
    for(int i = 0; i < 8; i++){
        if(isRun == false) return;
        msleep(500);
        ret = mSerialPortPlate->read(recvData, 2);
        if(ret > 3) break;
    }
    uchar * buf = recvData;
    qDebug() << "get3:" << QByteArray((char*)recvData, ret).toHex();
    //emit sendStr(QByteArray((char*)recvData, ret).toHex());

    QWriteLocker locker(&lock); //上锁
    memset(pkt,0,sizeof(pkt));
    //qDebug() << "len" << RTU_SENT_LEN + 5 << ret;
    //emit sendStr(tr("recv len:%1").arg(ret));
    if(rtu_recv_packet(buf, ret,  pkt , mDc)) {qDebug() << "crc" << pkt->dc << QString::number(pkt->crc, 16);
        emit showDataSignal(0, pkt); //发送到主线程
    }
}

void RtuThread::run()
{
    while (isRun) {
        if(isRead){
            readData();
        }else{
            workDown();
            stopThread();
        }
    }
}

void RtuThread::onWrite(uchar *sent, int len) //PDU串口
{
    mSerialPortPlate->write(sent, len);
}

void RtuThread::onWrite(QByteArray array) //PDU串口
{
    mSerialPortPlate->write(array);
}
