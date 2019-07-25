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
    mLen = 121;
    mSerial = NULL;
    mBuf = (uchar *)malloc(RTU_BUF_SIZE);
    mBuf2 = (uchar *)malloc(RTU_BUF_SIZE);
    mMutex = new QMutex();

    isRun = false;
    m_Timer = 400;
    mRtuSent = new SI_RtuSent();
    mRtuPkt = new Rtu_recv; //传输数据结构
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
    memset(mRtuPkt,0,sizeof(Rtu_recv));
    mQstringList.clear();
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


    emit cmdSig(strArray , 0 );
    memset(mBuf,0,RTU_BUF_SIZE);
    if(mSerial) {
        int rtn = mSerial->transmit(buf, len, mBuf, 10);

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
        int color = 1;
        if(rtn == 0xE6+5)
        {
            len = rtn;
            strArray+="-----------------"+localTime2;
            if(rtn > 0)
                rtu_recv_packet(mBuf, rtn, mRtuPkt); // 解析数据 data - len - it
            if(mBuf[0] < 0x20)
            {
                strArray +=tr("     第一条母线:");
                if(mBuf[0]==0) strArray +=tr("     始端箱:");
                else strArray +=tr("     插接箱%1:").arg(mBuf[0]);
            }
            else if(mBuf[0] >= 0x20&& mBuf[0] < 0x40)
            {
                strArray +=tr("     第二条母线:");
                if(mBuf[0]-0x20 == 0) strArray +=tr("     始端箱:");
                else strArray +=tr("     插接箱%1:").arg(mBuf[0]-0x20);
            }
            else if(mBuf[0] >= 0x40&& mBuf[0] < 0x60)
            {
                strArray +=tr("     第三条母线:");
                if(mBuf[0]-0x40 == 0) strArray +=tr("     始端箱:");
                else strArray +=tr("     插接箱%1:").arg(mBuf[0]-0x40);
            }
            else if(mBuf[0] >= 0x60)
            {
                strArray +=tr("     第四条母线:");
                if(mBuf[0]-0x60 == 0) strArray +=tr("     始端箱:");
                else strArray +=tr("     插接箱%1:").arg(mBuf[0]-0x60);
            }

            mQstringList<<QString::number(mBuf[0]);
            for(int i = 0 ; i < RTU_TH_NUM ; i++)
            {
                mQstringList<< QString::number(mRtuPkt->env[i].tem.value);
                strArray += QString("     温度%1:").arg(i+1)+QString::number(mRtuPkt->env[i].tem.value);
            }


            for(int i = 0 ; i < RTU_TH_NUM ; i++)
            {
                if(mRtuPkt->data[i].sw == 1 )
                 {
                    mQstringList<<tr("闭合");
                    strArray += QString("     断路器%1:").arg(i+1)+tr("闭合");
                }
                else
                {
                    mQstringList<<tr("断开");
                    strArray += QString("     断路器%1:").arg(i+1)+tr("断开");
                }
                if(mBuf[0] == 0x00 || mBuf[0] == 0x20 || mBuf[0] == 0x40 || mBuf[0] == 0x60)
                {
                    if(i == 0)
                    break;
                }
            }

            for(int i = 0 ; i < RTU_LINE_NUM ; i++)
            {
                if(mBuf[0] == 0x00 || mBuf[0] == 0x20 || mBuf[0] == 0x40 || mBuf[0] == 0x60)
                {
                    if(i < 3)
                    {
                        mQstringList<< QString::number(mRtuPkt->data[i].cur);
                        strArray += QString("     电流%1:").arg(i+1)+QString::number(mRtuPkt->data[i].cur/10.0,'f',1);
                    }
                    else
                    {
                        mQstringList<< QString::number(mRtuPkt->data[i].cur);
                        strArray += QString("     零线电流:")+QString::number(mRtuPkt->data[i].cur/10.0,'f',1);
                    }
                    if(i == 3)
                        break;
                }
                else
                {
                    mQstringList<< QString::number(mRtuPkt->data[i].cur);
                    strArray += QString("     电流%1:").arg(i+1)+QString::number(mRtuPkt->data[i].cur/10.0,'f',1);
                }
            }
            for(int i = 0 ; i < RTU_LINE_NUM ; i++)
             {
                mQstringList<< QString::number(mRtuPkt->data[i].vol);
                strArray += QString("     电压%1:").arg(i+1)+QString::number(mRtuPkt->data[i].vol);
                if(mBuf[0] == 0x00 || mBuf[0] == 0x20 || mBuf[0] == 0x40 || mBuf[0] == 0x60)
                {
                    if(i == 2)
                        break;
                }
            }
            if(mBuf[0] == 0x00 || mBuf[0] == 0x20 || mBuf[0] == 0x40 || mBuf[0] == 0x60)
            {
                if(mRtuPkt->lps == 0)
                 {
                    mQstringList<<tr("正常");
                    strArray += tr("          防雷状态：正常");
                }
                else
                 {
                    mQstringList<<tr("损坏");
                    strArray += tr("          防雷状态：损坏");
                }
            }
        }

        ans += strArray;


        qDebug()<< "recv:" << ans;
        emit cmdSig(ans,color );
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


    for(int i=0; i<mList.size(); ++i)
    {
        bool ret = sentSetCmd(mList.at(i).addr,mList.at(i).fn,mList.at(i).reg,mList.at(i).value, 1);
        if(ret) {
        } else {
            ++j;
            emit countSig(j);
        }
        sleep(2);
    }
    mList.clear();
}

void SI_RtuThread::stopThread()
{
    isRun = false;
}

void SI_RtuThread::setTimer(int timer)
{
    m_Timer = timer;
}


static int rtu_recv_len_dc(uchar *buf, int len)
{
    int ret = 0;
    int rtn = RTU_SENT_DC_LEN+5;

    if(len < rtn) {
        ret = -1;
        //        qDebug() << "rtu recv Err: len too short!!" << len  << rtn;
    } else if(len > rtn) {
        ret = -2;
        //        qDebug() << "rtu recv Err: len too long!!" << len << rtn ;
    } else {
        //        len = buf[2]*256 + buf[3];
        len = buf[2];
        if(len != RTU_SENT_DC_LEN) {
            ret = -3;
            qDebug() << "rtu recv len Err!!"<< len << rtn  << RTU_SENT_DC_LEN;
        }
    }
    return ret;
}



/**
  * 功　能：长度 校验
  * 入口参数：buf -> 缓冲区  len -> 长度
  * 返回值：0 正确
  */
static int rtu_recv_len(uchar *buf, int len)
{
    int ret = 0;
    int rtn = RTU_SENT_LEN+5;

    if(0 == rtu_recv_len_dc(buf, len)){ //先判断是否是直流数据
        return ret;
    }

    if(len < rtn) {  //判断是否为交流数据
        ret = -1;
        //        qDebug() << "rtu recv Err: len too short!!" << len  << rtn;
    } else if(len > rtn) {
        ret = -2;
        //        qDebug() << "rtu recv Err: len too long!!" << len << rtn ;
    } else {
        //        len = buf[2]*256 + buf[3];
        len = buf[2];
        if(len != RTU_SENT_LEN) {
            ret = -3;
            qDebug() << "rtu recv len Err!!"<< len << rtn  << RTU_SENT_LEN;
        }
    }

    return ret;
}


/**
  * 功　能：读取数据包头部
  * 入口参数：ptr -> 缓冲区
  * 出口参数：pkt -> 结构体
  * 返回值：0 正确
  */
static int rtu_recv_head(uchar *ptr,  Rtu_recv *pkt)
{
    pkt->addr = *(ptr++);// 从机地址码
    pkt->fn = *(ptr++);  /*功能码*/

    pkt->len = (*ptr) ; /*数据长度*/
    return 3;
}

/**
  * 功　能：读取电参数 数据
  * 入口参数：ptr -> 缓冲区
  * 出口参数：pkt -> 结构体
  * 返回值：12 正确
  */
static int rtu_recv_data(uchar *ptr, RtuRecvLine *msg)
{
    msg->vol =  (*ptr) * 256 + *(ptr+1);  ptr += 2; // 读取电压
    msg->cur =  (*ptr) * 256 + *(ptr+1);  ptr += 2; // 读取电流
    msg->pow =  (*ptr) * 256 + *(ptr+1);  ptr += 2; // 读取功率
    msg->ele =  (*ptr) * 256 + *(ptr+1);  ptr += 2; // 读取电能高8位
    msg->ele <<= 8; // 左移8位
    msg->ele +=  (*ptr) * 256 + *(ptr+1);  ptr += 2; // 读取电能底8位

    msg->maxVol =  (*ptr) * 256 + *(ptr+1);  ptr += 2;
    msg->minVol =  (*ptr) * 256 + *(ptr+1);  ptr += 2;
    msg->maxCur =  (*ptr) * 256 + *(ptr+1);  ptr += 2;
    msg->minCur =  (*ptr) * 256 + *(ptr+1);  ptr += 2;

    msg->wave =  (*ptr) * 256 + *(ptr+1);  ptr += 2;    // 谐波值
    msg->pf =  *(ptr++);// 功率因素
    msg->sw =  *(ptr++);// 开关状态

    msg->apPow = msg->vol * msg->cur / 10.0; // 视在功率

    return 22;   ////============ 加上开关，功率因素之后，是为14
}

/**
  * 功　能：读取环境 数据
  * 入口参数：ptr -> 缓冲区
  * 出口参数：pkt -> 结构体
  * 返回值：12 正确
  */
static int rtu_recv_env(uchar *ptr, RtuEnvUnit *msg)
{
    msg->value = *(ptr++);
    msg->max = *(ptr++);
    msg->min = *(ptr++);

    return 3;
}

/**
  * 功　能：检验码 数据
  * 入口参数：buf -> 缓冲区
  * 出口参数：pkt -> 结构体
  * 返回值：true
  */
static bool rtu_recv_crc(uchar *buf, int len, Rtu_recv *msg)
{
    bool ret = true;
    int rtn = len-2;

    ushort crc = rtu_crc(buf, rtn);
    if(crc != msg->crc) {
        ret = false;
        qDebug() << "rtu recv crc Err!";
    }

    return ret;
}


static int rtu_recv_thd(uchar *ptr, Rtu_recv *msg)
{
    msg->lps = *(ptr++); // 防雷开关
    // 读取负载百分比
    for(int i=0; i<3; ++i) msg->pl[i] = *(ptr++);

    {//定制零线电流解析
        if(msg->addr==0 || msg->addr-0x20 == 0 || msg->addr-0x40 == 0 || msg->addr-0x60 == 0)
        {
            (&(msg->data[N_Line]))->cur = (*ptr) * 256 + *(ptr+1);
        }
        ptr+=6;//始端箱需要两个字节零线电流/插接箱保留
    }

    msg->hc = *(ptr++);

    int len = 32;
    if(msg->addr) len = 3;
    for(int i=0; i<len; ++i){
        msg->thd[i] =  (*ptr) * 256 + *(ptr+1);  ptr += 2;
    }

    return (1+3+1+6+len*2);
}




/**
  * 功　能：还原数据包
  * 入口参数：buf -> 缓冲区   len -> 数据长度
  * 出口参数：pkt -> 结构体
  * 返回值：true
  */
bool rtu_recv_packet(uchar *buf, int len, Rtu_recv *pkt)
{
    bool ret = false;
    int rtn = rtu_recv_len(buf, len); //判断回收的数据是否完全
    if(rtn == 0) {
        uchar *ptr=buf;
        //ptr += rtu_recv_head(ptr, pkt); //指针偏移
        pkt->addr = *(ptr++);// 从机地址码
        pkt->fn = *(ptr++);  /*功能码*/

        pkt->len = (*ptr++) ; /*数据长度*/

        pkt->dc = *(ptr++);  //[交直流]
        pkt->rate = *(ptr++);

        for(int i=0; i<RTU_TH_NUM; ++i) // 读取环境 数据
            ptr += rtu_recv_env(ptr, &(pkt->env[i].tem));

        pkt->lineNum = *(ptr++); //[输出位]
        pkt->version = *(ptr++); // 版本
        //ptr += 2;
        ptr++;// 暂不解析 00:485  01：LORA WIFI

        bool state[RTU_LINE_NUM] = {false};
        {//定制IOF触点解析
            if(pkt->addr == 0 || pkt->addr-0x20 == 0 || pkt->addr-0x40 == 0 || pkt->addr-0x60 == 0)
            {
                state[0] = ((*(ptr++))&0x01)==1?true:false;
            }
            else
            {
                for(int i = 0 ; i < 3 ; i++)
                    state[i] = ((*ptr)>>(3-1-i)&0x01)==1?true:false;
                ptr++;
            }
        }

        int lineSum = pkt->lineNum; //交流
        if(!pkt->dc) lineSum = 4; //[暂时未加宏]
        for(int i=0; i<lineSum; ++i) // 读取电参数
        {
            RtuRecvLine *msg = &(pkt->data[i]);
            ptr += rtu_recv_data(ptr, msg);
            if(pkt->dc)//定制IOF触点状态重新赋值
                msg->sw = state[i]?1:0;
        }

        if(pkt->dc) { // 交流
            ptr += rtu_recv_thd(ptr, pkt);
        } else {

            ptr++; // 直流此字节没有用
            // 读取负载百分比
            for(int i=0; i<2; ++i) pkt->pl[i] = *(ptr++);
            ptr++; // 此字节没有用，直流只有两路负载百分比
            ptr++; // 此字节没有用，直流谐波通道预留位
            //----------------------[二分二路直流][显示]----------------------------
            if(2 == pkt->rate && 2 == pkt->lineNum ){ //交换2-3数据
                RtuRecvLine data;
                data = pkt->data[1];
                pkt->data[1] = pkt->data[2];
                pkt->data[2] = data;
            }
            //---------------------------------------------------------------
        }

        ret = true;
    }
    return ret;
}

static ushort calccrc (ushort crc, uchar crcbuf)
{
    uchar x, kkk=0;
    crc = crc^crcbuf;
    for(x=0;x<8;x++)
    {
        kkk = crc&1;
        crc >>= 1;
        crc &= 0x7FFF;
        if(kkk == 1)
            crc = crc^0xa001;
        crc=crc&0xffff;
    }
    return crc;
}

/**
  * 功　能：CRC校验
  * 入口参数：buf -> 缓冲区  len -> 长度
  * 返回值：CRC
  */
ushort rtu_crc(uchar *buf, int len)
{
    ushort crc = 0xffff;
    for(int i=0; i<len; i++)
        crc = calccrc(crc, buf[i]);
    return crc;
}
