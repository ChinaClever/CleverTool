/****
 * 1，解析从管理软件发送过来的数据
 *
 */
#include "analysisdata.h"

/**
 * @brief tcp收到的数据转化成bytearray格式，再转化成数据包的格式
 * @param array
 */
int get_data_packet(QByteArray array)
{

    data_packet packet;
    packet.segment.filed.data = (quint8 *)malloc(sizeof(quint8)*BUFSIZ);
    //数据包完整检查1.包头 2.包尾 3.数据包长度

    int len = array.size();
    if(len < (2 + 9 + 2 + 1)) //分别代表了包头、代号段、数据长度、包尾，小于该长度则说明数据包错误
    {
        qDebug()<<"数据包长度错误！" <<len;
        return false;
    }

    int offset = 0;
    //包头
    packet.hdr = array.at(offset++);
    packet.stx = array.at(offset++);


    //代号段
    for(int i = 0 ; i < 4; i++)
        packet.code.devCode[i] = array.at(offset++);

    packet.code.type = array.at(offset++);
    packet.code.version = array.at(offset++);
    packet.code.trans = array.at(offset++);

    packet.code.reserve[0] = array.at(offset++);
    packet.code.reserve[1] = array.at(offset++);

    //数据段
    packet.segment.len = array.at(offset++)<<8 + array.at(offset++);

    packet.segment.filed.addr = array.at(offset++);
    packet.segment.filed.fn[0] = array.at(offset++);
    packet.segment.filed.fn[1] = array.at(offset++);
    packet.segment.filed.len = array.at(offset++) + array.at(offset++);

    int length = packet.segment.filed.len;
    for(int i = 0 ;i < length ; i++)
    {
        *(packet.segment.filed.data + i) = array.at(offset++);
    }

    packet.segment.XOR = array.at(offset++);

    //包尾
    packet.ED = array.at(offset++);

    if(offset != len)
        qDebug() <<"解析错误,请重新解析！";
    else
        qDebug() << "offset:"<<offset;

    for(int j = 0 ; j < length ; j++)
        qDebug() << "data:" <<*(packet.segment.filed.data + j);


    DataSouce *souce = get_datasouce();
    qDebug()<<"get_data_packe";
    parameterData *soucedata = souce->getSouceData();

    int num = packet.segment.filed.fn[1];
    qDebug()<<"num："<<num;
    if(packet.segment.filed.fn[0] == 0)
        souce->setData(&(soucedata->outData.cur),num,packet.segment.filed.data);
    else if(packet.segment.filed.fn[0] == 0x71 )
        souce->setData(&(soucedata->loopData.cur),num,packet.segment.filed.data);
    else if(packet.segment.filed.fn[0] == 1 )
        souce->setData(&(soucedata->inputPhrase.vol),num,packet.segment.filed.data);
    else if(packet.segment.filed.fn[0] == 2 )
        souce->setData(&(soucedata->inputPhrase.cur),num,packet.segment.filed.data);
    else if(packet.segment.filed.fn[0] == 3 )
        souce->setData(&(soucedata->envData.tem),num,packet.segment.filed.data);
    else if(packet.segment.filed.fn[0] == 4 )
        souce->setData(&(soucedata->envData.hum),num,packet.segment.filed.data);
    else if(packet.segment.filed.fn[0] == 13)  //输出位开关
    {
        int num = packet.segment.filed.fn[1];
        souce->setSwitchData(&(soucedata->outData),num,*(packet.segment.filed.data));

    }else if(packet.segment.filed.fn[0] == 18)
        souce->setAllSwitchData(&(soucedata->outData),*(packet.segment.filed.data));

}






