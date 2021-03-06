﻿/*
 * RTU线程
 *
 *  Created on: 2018年1月1日
 *      Author: Lzy
 */
#include "si_rtuthread.h"

SI_RtuThread::SI_RtuThread(QObject *parent) : QThread(parent)
{
    mreg = 0;
    mLen = 121;
    mSerial = NULL;
    mBuf = (uchar *)malloc(RTU_BUF_SIZE);
    mBuf2 = (uchar *)malloc(RTU_BUF_SIZE);
    mMutex = new QMutex();

    isRun = false;
    m_Timer = 400;
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
 * @brief 设置串口并开启线程
 * @param serial
 */
void SI_RtuThread::setLen(int len)
{
    mLen = len;
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
    strArray.clear();
    array.clear();
    QDateTime local(QDateTime::currentDateTime());
    QString localTime = local.toString("yyyy-MM-dd:hh:mm:ss:zzz");
    array.append((char *)buf, len);
    strArray = array.toHex(); // 十六进制
    for(int i=0; i<array.size(); ++i)
        strArray.insert(2+3*i, " "); // 插入空格
    strArray = "send:"+strArray+"-----------------"+localTime;
    //qDebug()<< "send:" << strArray;


    emit cmdSig(strArray , 0);
    memset(mBuf,0,RTU_BUF_SIZE);
    if(mSerial) {
        int rtn = mSerial->transmit(buf, len, mBuf, msecs);
        //if(fn == 6)
        {
                //if(len == rtn)
            {
                //if(memcmp(buf, mBuf,rtn) == 0)
                {
                    strArray.clear();
                    array.clear();
                    array.append((char *)mBuf, rtn);
                    strArray = array.toHex(); // 十六进制
                    for(int i=0; i<array.size(); ++i)
                        strArray.insert(2+3*i, " "); // 插入空格
                    QDateTime local2(QDateTime::currentDateTime());
                    QString localTime2 = local2.toString("yyyy-MM-dd:hh:mm:ss:zzz");
                    strArray = "recv:"+strArray;
                    //qDebug()<< "recv:" << strArray;

                    QString ans = "";
                    int color = 0;
                    if(checkData(array,ret,ans,color,rtn))
                        strArray+="-----------------"+localTime2;
                    else
                    {
                        msleep(m_Timer);
                        memset(mBuf2,0,RTU_BUF_SIZE);
                        int rtn2 = mSerial->read(mBuf2,1);
                        array.append((char *)mBuf2, rtn2);
                        if(checkData(array,ret,ans,color,rtn+rtn2))
                        {
                            strArray = array.toHex(); // 十六进制
                            for(int i=rtn; i<array.size(); ++i)
                                strArray.insert(2+3*i, " "); // 插入空格
                            QDateTime local3(QDateTime::currentDateTime());
                            QString localTime3 = local3.toString("yyyy-MM-dd:hh:mm:ss:zzz");
                            strArray+="-----------------"+localTime3;
                        }
                    }
                    ans += strArray;
                    qDebug()<< "recv:" << ans;
                    emit cmdSig(ans,color);
                }

           }
        }
//        else
//        {
//            if(memcmp(buf, mBuf,2) == 0)
//            {
//                array.append((char *)mBuf, len);
//                strArray = array.toHex(); // 十六进制
//                for(int i=0; i<array.size(); ++i)
//                    strArray.insert(2+3*i, " "); // 插入空格
//                strArray = "recv:"+strArray;
//                qDebug()<< "recv:" << strArray;
//                emit cmdSig(strArray);
//                ret = true;
//            }
//            else
//                qDebug() << "SI sent Set Cmd Err";
//        }
    }

    return ret;
}

bool SI_RtuThread::checkData(QByteArray mBuf,bool& ret,QString &ans,int &color,int rtn)
{
    if(mBuf.at(0) == 1)
    {
        if(mBuf.at(1) == 3)
        {
            if(rtn == mLen)
            {
                ret = true;
                ans = tr("正确");
                color = 2;
            }
            else
            {
                ans = tr("长度错误");
                color = 1;
                ret = false;
            }
         }
        else
        {
            ans = tr("功能码错误");
            color = 1;
            ret = false;
        }
    }
    else if(mBuf.at(0)==0&&mBuf.at(1) == 0&&mBuf.at(2) == 0)
    {
        ans = tr("未响应");
        color = 1;
        ret = false;
    }
    else
    {
      ans = tr("设备地址错误");
      color = 1;
      ret = false;
    }
   return ret;
}

void SI_RtuThread::run()
{
    int j = 0;
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
        //mList.clear();
    }
}

void SI_RtuThread::stopThread()
{
    isRun = false;
}

void SI_RtuThread::setTimer(int timer)
{
    m_Timer = timer;
}
