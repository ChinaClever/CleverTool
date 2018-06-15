#include "common.h"

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
  //  qDebug() << "校验码：" << wCRC << QString::number(wCRC,16);
    return wCRC;
}
