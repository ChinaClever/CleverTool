/*
 * RTU线程
 *
 *  Created on: 2018年1月1日
 *      Author: Lzy
 */
#include "si_rtuthread.h"

SI_RtuThread::SI_RtuThread(QObject *parent) : QThread(parent)
{
    mreg = 0;
    mSerial = NULL;
    mBuf = (uchar *)malloc(RTU_BUF_SIZE);
    mMutex = new QMutex();

    isRun = false;
    mRtuSent = new SI_RtuSent();
}

SI_RtuThread::~SI_RtuThread()
{
    isRun = false;
    wait();
}


SI_RtuThread *SI_RtuThread::bulid(QObject *parent)
{
    static SI_RtuThread* sington = NULL;
    if(sington == NULL) {
        sington = new SI_RtuThread(parent);
    }
    return sington;
}

/**
 * @brief 设置串口并开启线程
 * @param serial
 */
void SI_RtuThread::init(SerialPort *serial)
{
    mSerial = serial;
}


/**
 * @brief 发送设置命令
 * @param addr 地址
 * @param reg 寄存吕
 * @param value 值
 * @return true
 */
bool SI_RtuThread::sentSetCmd(int addr, int reg, ushort value, int msecs)
{
    bool ret = false;
    static uchar buf[RTU_BUF_SIZE] = {0};
    QMutexLocker locker(mMutex);

    int len = mRtuSent->sentCmdBuff(addr, reg, value, buf);
    if(mSerial) {
        int rtn = mSerial->transmit(buf, len, mBuf, msecs);
        if(len == rtn) {
            if(memcmp(buf, mBuf,rtn) == 0)
                ret = true;
            else
                qDebug() << "SI sent Set Cmd Err";
        }
    }

    return ret;
}

void SI_RtuThread::run()
{
    if(isRun == false) {
        isRun = true;

        for(int i=0; i<mList.size(); ++i)
        {
            QString str = tr("输出位 %1 命令执行").arg(mList.at(i).reg - mreg+1);
            bool ret = sentSetCmd(mList.at(i).addr,mList.at(i).reg,mList.at(i).value, 5);
            if(ret) {
                str += tr("成功!!");
            } else {
                 str += tr("失败!!");
            }

            emit cmdSig(str);
        }

        mList.clear();
        isRun = false;
    }
}
