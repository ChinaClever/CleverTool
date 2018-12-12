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

TftpThread::TftpThread(QObject *parent) : QThread(parent)
{
    mTftp = new Tftp(this);
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

void TftpThread::saveLogs(const QString &ip, bool f)
{
    QStringList str;

    str << QString::number(mData->progress);
    str << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    str << ip;
    if(f) str << tr("成功");
    else str << tr("失败");

    mData->logs << str;
}

/**
 * @brief 发送文件
 * @return
 */
bool TftpThread::sentFile(void)
{    
    QString ip = mIps.first();

    mData->progress++;
    mData->status = tr("开始传输：%1").arg(ip);
    bool ret = mTftp->upload(mData->file, ip);
    if(ret){
        mData->oks << ip;
        mData->status = tr("传输成功：%1").arg(ip);
    } else {
        mData->errs << ip;
        mData->status = tr("传输失败：%1").arg(ip);
    }
    saveLogs(ip, ret);
    mIps.removeFirst();

    return ret;

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
        str = tr("共%1台升级失败 共%2台升级失败!").arg(mData->errs.size()).arg(mData->oks.size());
    } else {
        str = tr("全部升级成功 共升级%1台!").arg(mData->oks.size());
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

