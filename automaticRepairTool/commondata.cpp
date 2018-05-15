#include "commondata.h"
#include <QDebug>

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
    qDebug() << "校验码：" << wCRC << QString::number(wCRC,16);
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
