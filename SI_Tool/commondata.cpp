#include "commondata.h"
#include <QDebug>
#include <QTime>
#include <QCoreApplication>

const unsigned short wCRCTalbeAbs[] =
{
    0x0000, 0xCC01, 0xD801, 0x1400, 0xF001, 0x3C00, 0x2800, 0xE401, 0xA001, 0x6C00, 0x7800, 0xB401, 0x5000, 0x9C01, 0x8801, 0x4400,
};

/**
 * @brief CRC16_2
 * @param pchMsg
 * @param wDataLen  pchMsg的长度
 * @return
 */
int CRC16_2(char* pchMsg, int wDataLen)
{
    unsigned short wCRC = 0xFFFF;
    int i;
    uchar chChar;

    for (i = 0; i < wDataLen; i++)
    {
        chChar = *pchMsg++;
        wCRC = wCRCTalbeAbs[(chChar ^ wCRC) & 15] ^ (wCRC >> 4);
        wCRC = wCRCTalbeAbs[((chChar >> 4) ^ wCRC) & 15] ^ (wCRC >> 4);
    }
   // qDebug() << "校验码：" << wCRC << QString::number(wCRC,16);
    return wCRC;
}

/**
 * @brief CRC16_2
 * @param pchMsg
 * @param wDataLen  pchMsg的长度
 * @return
 */
int CRC16_22(char* pchMsg, int start ,int wDataLen)
{
    unsigned short wCRC = 0xFFFF;
    int i;
    uchar chChar;

    for (i = start; i < wDataLen; i++)
    {
        chChar = *pchMsg++;
        wCRC = wCRCTalbeAbs[(chChar ^ wCRC) & 15] ^ (wCRC >> 4);
        wCRC = wCRCTalbeAbs[((chChar >> 4) ^ wCRC) & 15] ^ (wCRC >> 4);
    }
   // qDebug() << "校验码：" << wCRC << QString::number(wCRC,16);
    return wCRC;
}

QByteArray quintToByte(quint8 *cmd, int len)
{
    QByteArray array;
    for(int i = 0; i<len ; i++)
    {
        array.append(*(cmd + i));
    }
    return array;
}

char ConvertHexChar(char ch)
{
    if((ch >= '0') && (ch <= '9'))
               return ch-0x30;
         else if((ch >= 'A') && (ch <= 'F'))
             return ch-'A'+10;
         else if((ch >= 'a') && (ch <= 'f'))
               return ch-'a'+10;
    else return (-1);
}

void StringToHex(QString str, QByteArray &senddata)
{
    int hexdata,lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    senddata.resize(len/2);
    char lstr,hstr;
    for(int i=0; i<len; )
      {
            //char lstr,
         hstr = str[i].toLatin1();
        if(hstr == ' ')
         {
               i++;
            continue;
          }
        i++;
          if(i >= len)
               break;
         lstr = str[i].toLatin1();
           hexdata = ConvertHexChar(hstr);
           lowhexdata = ConvertHexChar(lstr);
          if((hexdata == 16) || (lowhexdata == 16))
              break;
          else
              hexdata = hexdata*16+lowhexdata;
           i++;
           senddata[hexdatalen] = (char)hexdata;
           hexdatalen++;
        }
      senddata.resize(hexdatalen);
}

uchar BCC(QByteArray array)
{
    uchar XOR = 0;
    for(int i = 0; i < array.length(); i++){
        XOR ^= array.at(i);
    }
    return XOR;
}

static ushort calccrc(ushort crc, uchar crcbuf)
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
    *(ptr++); // 防雷开关
     // 读取负载百分比
    for(int i=0; i<3; ++i) *(ptr++);
    *(ptr++);

    int len = 32;
    if(msg->addr) len = 3;
    for(int i=0; i<len; ++i){
        (*ptr) * 256 + *(ptr+1);  ptr += 2;
    }

    return (1+3+1+len*2);
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
         len = buf[2];
        if(len != RTU_SENT_LEN) {
            ret = -3;
            qDebug() << "rtu recv len Err!!"<< len << rtn  << RTU_SENT_LEN;
        }
    }
    return ret;
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
        //qDebug() << len << RTU_SENT_DC_LEN;
        uchar *ptr=buf;
        ptr += rtu_recv_head(ptr, pkt); //指针偏移

        pkt->dc = *(ptr++);  //[交直流]
        pkt->rate = *(ptr++);
        for(int i=0; i<RTU_TH_NUM; ++i) // 读取环境 数据
            ptr += rtu_recv_env(ptr, &(pkt->env[i].tem));
        pkt->lineNum = *(ptr++); //[输出位]
        pkt->version = *(ptr++); //[输出位]
        ptr += 2;

        int lineSum = 0;
        if(pkt->dc)//交流
            lineSum = RTU_LINE_NUM;
        else
            lineSum = 4; //[暂时未加宏]
        for(int i=0; i<lineSum; ++i) // 读取电参数
            ptr += rtu_recv_data(ptr, &(pkt->data[i]));

        if(pkt->dc)
        {
            ptr += rtu_recv_thd(ptr , pkt);
        }
        else{
            ptr++; // 直流此字节没有用
            // 读取负载百分比
            for(int i=0; i<2; ++i)  *(ptr++);
            ptr++; // 此字节没有用，直流只有两路负载百分比
            ptr++; // 此字节没有用，直流谐波通道预留位
            //----------------------[二分二路直流][显示]----------------------------
            if(2 == pkt->rate && 2 == pkt->lineNum && 0 == pkt->dc){ //交换2-3数据
                RtuRecvLine data;
                data = pkt->data[1];
                pkt->data[1] = pkt->data[2];
                pkt->data[2] = data;
                //swap(pkt->data[1], pkt->data[2]);
            }
        }


        //---------------------------------------------------------------

#if 0
        pkt->crc = (ptr[1]*256) + ptr[0]; // 获取校验码
        ret = rtu_recv_crc(buf, len, pkt); //校验码
#else
        ret = true;
#endif
    }
    return ret;
}


void sleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
