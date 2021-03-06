﻿#ifndef DATADEFINE_H
#define DATADEFINE_H
#include<qglobal.h>

quint8 snedCmd[3][7] ={ //校准发送
   {0x7B, 0xA1, 0x00, 0x00, 0x00, 0xBB,  0x61}, //L1
   {0x7B, 0xA1, 0x01, 0x00, 0x00, 0xBB,  0x60}, //L2
   {0x7B, 0xA1, 0x02, 0x00, 0x00, 0xBB,  0x63}  //L3
};

quint8 CsendCmd[7] ={ //其他发送
    0x7B, 0xA2, 0x00, 0x00, 0x00, 0xBB,  0x62
};
quint8 recvdCmd[7] ={ //其他回复
    0x7B, 0xA2, 0xCC, 0xCC, 0xCC, 0xBB,  0xAE
};

quint8 recvCmd[3][7] ={ //校准回复
    {0x7B, 0xA1, 0xDD, 0xDD, 0xDD, 0xBB, 0xBC}, //L1
    {0x7B, 0xA1, 0xDD, 0xDD, 0xDD, 0xBB, 0xBC}, //L2
    {0x7B, 0xA1, 0xDD, 0xDD, 0xDD, 0xBB, 0xBC}  //L3
};

quint8 get1Cmd[4][8] ={ //获取单相数据
    {0x01, 0x03, 0x00, 0x00, 0x00, 0x1A, 0xC4, 0x01},
    {0x02, 0x03, 0x00, 0x00, 0x00, 0x1A, 0xC4, 0x32},
    {0x03, 0x03, 0x00, 0x00, 0x00, 0x1A, 0xC5, 0xE3},
    {0x04, 0x03, 0x00, 0x00, 0x00, 0x1A, 0xC4, 0x54}
};

quint8 get3Cmd[4][8] ={ //获取三相数据
    {0x01, 0x03, 0x00, 0x00, 0x00, 0x4A, 0xC4, 0x3D},
    {0x02, 0x03, 0x00, 0x00, 0x00, 0x4A, 0xC4, 0x0E},
    {0x03, 0x03, 0x00, 0x00, 0x00, 0x4A, 0xC5, 0xDF},
    {0x04, 0x03, 0x00, 0x00, 0x00, 0x4A, 0xC4, 0x68}
};

#endif // DATADEFINE_H
