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
    isRun = true;
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
bool SI_RtuThread::sentSetCmd(int addr,int fn, int reg, ushort value, int msecs)
{
    bool ret = false;
    static uchar buf[RTU_BUF_SIZE] = {0};
    QMutexLocker locker(mMutex);

    int len = mRtuSent->sentCmdBuff(addr,fn , reg, value, buf);
    QByteArray array;
    QString strArray;
    array.append((char *)buf, len);
    strArray = array.toHex(); // 十六进制
    for(int i=0; i<array.size(); ++i)
        strArray.insert(2+3*i, " "); // 插入空格
    strArray = "send:"+strArray;
    qDebug()<< "send:" << strArray;
    emit cmdSig(strArray);
    if(mSerial) {
        int rtn = mSerial->transmit(buf, len, mBuf, msecs);
        if(fn == 6)
        {
                if(len == rtn) {
                if(memcmp(buf, mBuf,rtn) == 0)
                {
                    array.append((char *)mBuf, len);
                    strArray = array.toHex(); // 十六进制
                    for(int i=0; i<array.size(); ++i)
                        strArray.insert(2+3*i, " "); // 插入空格
                    strArray = "recv:"+strArray;
                    qDebug()<< "recv:" << strArray;
                    emit cmdSig(strArray);
                    ret = true;
                }
                else
                    qDebug() << "SI sent Set Cmd Err";
                }
        }
        else
        {
            if(memcmp(buf, mBuf,2) == 0)
            {
                array.append((char *)mBuf, len);
                strArray = array.toHex(); // 十六进制
                for(int i=0; i<array.size(); ++i)
                    strArray.insert(2+3*i, " "); // 插入空格
                strArray = "recv:"+strArray;
                qDebug()<< "recv:" << strArray;
                emit cmdSig(strArray);
                ret = true;
            }
            else
                qDebug() << "SI sent Set Cmd Err";
        }
    }

    return ret;
}

void SI_RtuThread::run()
{
    static int j = 0;
    isRun = true;
    while(isRun) {



        for(int i=0; i<mList.size(); ++i)
        {
            //QString str = tr("输出位 %1 命令执行").arg(i);
            bool ret = sentSetCmd(mList.at(i).addr,mList.at(i).fn,mList.at(i).reg,mList.at(i).value, 1);
            if(ret) {
                //str += tr("成功!!");
            } else {
                 //str += tr("失败!!");
                ++j;
                emit countSig(j);
            }
        }
        sleep(1);
        //mList.clear();
    }
}

void SI_RtuThread::stopThread()
{
    isRun = false;
}
