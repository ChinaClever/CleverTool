/*
 * RTU发送数据
 *
 *  Created on: 2018年1月1日
 *      Author: Lzy
 */
#include "si_rtu_sent.h"


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
ushort si_rtu_crc(uchar *buf, int len)
{
    ushort crc = 0xffff;
    for(int i=0; i<len; i++)
        crc = calccrc(crc, buf[i]);
    return crc;
}

/**
  * 功　能：发送数据打包
  * 入口参数：pkt -> 发送结构体
  * 出口参数：ptr -> 缓冲区
  * 返回值：打包后的长度
  */
static int rtu_sent_packet(SI_Rtu_Sent *pkt, uchar *ptr)
{
    uchar *buf = ptr;
    *(ptr++) = pkt->addr;  /*地址码*/
    *(ptr++) = pkt->fn; /*功能码*/

    /*填入寄存器首地址*/
    *(ptr++) = ((pkt->reg) >> 8); /*高8位*/
    *(ptr++) = (0xff)&(pkt->reg); /*低8位*/

    /*填入数据长度*/
    *(ptr++) = ((pkt->len) >> 8); /*长度高8位*/
    *(ptr++) = (0xff)&(pkt->len); /*低8位*/

    /*填入CRC*/
    pkt->crc = si_rtu_crc(buf, 6);
    *(ptr++) = (0xff)&(pkt->crc); /*低8位*/
    *(ptr++) = ((pkt->crc) >> 8); /*高8位*/

    return 8;
}

/**
  * 功　能：发送数据打包
  * 入口参数：pkt -> 发送结构体
  * 出口参数：ptr -> 缓冲区
  * 返回值：打包后的长度
  */
static int rtu_sent_buff(uchar addr, uchar line, uchar *buf)
{
    static SI_Rtu_Sent msg;
    static QMutex mutex; // 互拆锁
    QMutexLocker locker(&mutex);

    msg.addr = addr;
    if(line == 1)
        msg.len = SI_RTU_ONE_LEN;
    else
        msg.len = SI_RTU_THREE_LEN;
    return rtu_sent_packet(&msg, buf);
}


/**
  * 功　能：发送命令数据打包
  * 入口参数：pkt -> 发送结构体
  * 出口参数：ptr -> 缓冲区
  * 返回值：打包后的长度
  */
static int rtu_sent_setCmd(uchar addr, ushort reg, ushort value, uchar *buf)
{
    static SI_Rtu_Sent msg;
    static QMutex mutex; // 互拆锁
    QMutexLocker locker(&mutex);

    msg.addr = addr;
    msg.fn = 0x06;
    msg.reg = reg;
    msg.len = value;

    return rtu_sent_packet(&msg, buf);
}




int SI_RtuSent::sentDataBuff(uchar addr, uchar line, uchar *buf)
{
    return rtu_sent_buff(addr, line, buf);
}

int SI_RtuSent::sentCmdBuff(uchar addr, ushort reg, ushort value, uchar *buf)
{
    return rtu_sent_setCmd(addr, reg, value, buf);
}
