#ifndef COMMONDATA_H
#define COMMONDATA_H
#include <QByteArray>

extern int CRC16_2(char* pchMsg, int wDataLen);//CRC校验
extern uchar BCC(QByteArray array); //异或校验

extern QByteArray quintToByte(quint8 *cmd, int len); //数据转换
extern void StringToHex(QString str, QByteArray &senddata);//将QString字符串转换为16进制的形式

typedef struct responseCollected{
    quint8 head[2]; //头码
    quint8 addr;    //地址位
    quint8 length;  //长度
    quint8 flag;    //标志位，00:表示只读取电流电压，A1:表示读取电流电压并可以控制继电器
    quint8 openeffective[6];   //开启有效
    quint8 closeffective[6];   //关闭有效
    quint8 obligate[3];        //预留
    quint8 xornumber;          //校验码
}collectedData;



#endif // COMMONDATA_H
