#ifndef DATASENT_H
#define DATASENT_H

#include "datasouce.h"
#include "heartbeat.h"
#include <QByteArray>



typedef struct
{
    uchar addr;	/*地址*/
    uchar fn[2];	/*功能码*/
    ushort len; /*数据长度，为data的字符数*/
    quint8 *data; /*数据*/
}data_Filed;

typedef struct
{
    ushort len; /*数据长度，为数据域的长度，需根据数据域确定 */
    data_Filed filed;	/*数据域*/
    uchar XOR; /*校验码*/
}data_Segment;

typedef struct
{
    uchar devCode[4]; /*设备代号*/
    uchar type; /*通讯类型*/
    uchar version; /*版本号*/
    uchar trans;	/*服务端发送标志：10H 客户端应答标志：03H */
    uchar reserve[2]; /*预留2字节*/
}data_code;

typedef struct
{
    uchar hdr;	/*信息头 0x7E*/
    uchar stx;	/*标识字0x5E*/
    data_code code; /*代号段*/
    data_Segment segment;/*数据段*/
    uchar ED; /*结束符*/
}data_packet;

//data_packet *packet;
//HeartBeat *heartr;



extern void souceData_To_protcldata(); //提供将数据打包的接口
devSetings *get_devsetings();

#endif // DATASENT_H
