/*
 * tftpthread.cpp
 * tftp上传线程类
 *  1、设置相应的目标IP，及文件
 *  2、启动线程发送文件，void startSent(void);
 *  3、发送过程中，根据实际情况，会发出相应的信号及进度信号
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "tftpthread.h"
#include <QTcpSocket>

TftpThread::TftpThread(QObject *parent) : QThread(parent)
{
    mTftp = new Tftp(this);
    connect(mTftp, SIGNAL(progressSig(float,QString)), this, SLOT(subProgressSlot(float,QString)));
    mData = DataPacket::bulid()->data;
}

TftpThread::~TftpThread()
{
    wait();
}

/**
 * @brief 设置需要发送的IP，和文件
 * @param ip
 * @param file
 */
bool TftpThread::startSend()
{    
    if(mData->isRun) return false;
    start();

    return true;
}

void TftpThread::subProgressSlot(float f, QString s)
{
    mData->subPorgress = (int)f;
    mData->subStatus = s;
}

void TftpThread::saveLogs(const QString &ip, bool f, bool exist)
{
    QStringList str;

    str << QString::number(mData->progress);
    str << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    str << ip;
    if(exist){
        if(f) str << tr("成功");
        else str << tr("失败");
    } else {
        str << tr("IP不存在");
    }

    mData->logs << str;
}

/**
 * @brief 发送文件
 * @return
 */
bool TftpThread::sentFile(void)
{    
    QString ip = mIps.first();

    bool exist = checkIp(ip);
    bool ret = false;
    if(exist) {
        mData->status = tr("开始传输：%1").arg(ip);
        ret = mTftp->upload(mData->file, ip);
        if(ret){
            mData->oks << ip;
            mData->status = tr("传输成功：%1").arg(ip);
        } else {
            mData->errs << ip;
            mData->status = tr("传输失败：%1").arg(ip);
        }
    }  else {
        mData->errs << ip;
        mData->status = tr("目标IP不存在：%1").arg(ip);
    }

    mData->progress++;
    saveLogs(ip, ret , exist);
    mIps.removeFirst();
    return ret && exist;
}

void TftpThread::initSend()
{
    mIps = mData->ips;
    mData->isRun = true;

    mData->progress = 0;
    mData->status.clear();
    mData->errs.clear();
    mData->oks.clear();
    mData->logs.clear();
}

void TftpThread::overSend()
{
    mData->isRun = false;

    QString str;
    if(mData->errs.size()) {
        str = tr("%1台失败 %2台成功 ").arg(mData->errs.size()).arg(mData->oks.size());
    } else {
        str = tr("%1台成功 ").arg(mData->oks.size());
    }
    mData->status = str;
}

void TftpThread::run(void)
{
    initSend();

    while (mIps.size() > 0) {
        sentFile();
        msleep(800);
    }
    overSend();
}

bool TftpThread::checkIp(const QString& ip)
{
    QProcess pingProcess;
    QString strArg = "ping " + ip + " -n 1 -i 2";  //strPingIP 为设备IP地址
    pingProcess.start(strArg,QIODevice::ReadOnly);
    pingProcess.waitForFinished(-1);

    QString p_stdout = QString::fromLocal8Bit( pingProcess.readAllStandardOutput());
    bool bPingSuccess = false;

    if(p_stdout.contains("TTL=")) //我采用这个字符来判断 对不对？
    {
        bPingSuccess = true;
    }else
    {
        bPingSuccess = false;
    }
    return bPingSuccess;
}
