#ifndef COMMONDATA_H
#define COMMONDATA_H
#include <QByteArray>
#include <QDebug>
#include "datamodel.h"

extern int CRC16_2(char* pchMsg, int wDataLen);//CRC校验
extern int CRC16_22(char* pchMsg,int start, int wDataLen);//CRC校验
extern uchar BCC(QByteArray array); //异或校验

void sleep(unsigned int msec);

extern QByteArray quintToByte(quint8 *cmd, int len); //数据转换
extern void StringToHex(QString str, QByteArray &senddata);//将QString字符串转换为16进制的形式
static ushort calccrc(ushort crc, uchar crcbuf);
ushort rtu_crc(uchar *buf, int len);
static int rtu_recv_head(uchar *ptr,  Rtu_recv *pkt);
static int rtu_recv_data(uchar *ptr, RtuRecvLine *msg);
static int rtu_recv_env(uchar *ptr, RtuEnvUnit *msg);
static bool rtu_recv_crc(uchar *buf, int len, Rtu_recv *msg);
static int rtu_recv_len_dc(uchar *buf, int len);
static int rtu_recv_len(uchar *buf, int len);
bool rtu_recv_packet(uchar *buf, int len, Rtu_recv *pkt);


#endif // COMMONDATA_H
