#ifndef SI_RTU_SENT_H
#define SI_RTU_SENT_H

#include "serialportwid.h"

#define SI_RTU_ONE_LEN 0x1A  // 单相数据长度
#define SI_RTU_THREE_LEN 0x4A  // 三相数据长度

struct SI_Rtu_Sent {
    SI_Rtu_Sent():fn(3),reg(0),len(SI_RTU_ONE_LEN){}
    uchar addr; // 表示从机地址码
    uchar fn;  // 表示功能码
    ushort reg; // 表示寄存器首地址
    ushort len; // 表示数据长度
    ushort crc; // 表示CRC校验
};

class SI_RtuSent
{
public:
    int sentDataBuff(uchar addr, uchar line, uchar *buf);
    int sentCmdBuff(uchar addr, ushort reg, ushort value, uchar *buf);
};

ushort si_rtu_crc(uchar *buf, int len);

#endif // SI_RTU_SENT_H
