#include "datapacket.h"

const unsigned short wCRCTalbeAbs[] =
{
    0x0000, 0xCC01, 0xD801, 0x1400, 0xF001, 0x3C00, 0x2800, 0xE401, 0xA001, 0x6C00, 0x7800, 0xB401, 0x5000, 0x9C01, 0x8801, 0x4400,
};
int CRC16_2(char* pchMsg, char wDataLen)
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

/**
 * @brief 检验回收数据包的校验码是否正确
 * @param array
 * @return
 */
int crc_recv_check(QByteArray array)
{
    QByteArray marray;

    int len = array.size() - 2;

    for(int i = 0 ; i < len ;i++)
    {
        marray.append(array.at(i));
        qDebug() << "marray :" << QString::number(marray.at(i),16);
    }
    qDebug()<< "src - len:" << marray.size();
    int src  = CRC16_2(marray.data(),marray.size());

    return src;
}

/**
 * @brief 把数据从解析后的数据包转化成只含必要信息的数据包
 * @param compacket
 * @param recvpacket
 */
void coming_to_recv(coming_packet *compacket,receiveData *recvpacket,basicinfo info)
{
    recvpacket->isUnitTest = info.isUnitTest;
    recvpacket->currentPhase = info.currentPhase;
    int ret = 0;

    recvpacket->vol[0] = (compacket->data[ret++]<<8) + compacket->data[ret++];
    recvpacket->vol[1] = (compacket->data[ret++]<<8) + compacket->data[ret++];
    recvpacket->vol[2] = (compacket->data[ret++]<<8) + compacket->data[ret++];

    recvpacket->cur[0] = (compacket->data[ret++]<<8) + compacket->data[ret++];
    recvpacket->cur[1] = (compacket->data[ret++]<<8) + compacket->data[ret++];
    recvpacket->cur[2] = (compacket->data[ret++]<<8) + compacket->data[ret++];

    //跳过功率
    ret += 6;

    recvpacket->fre[0] = (compacket->data[ret++]<<8) + compacket->data[ret++];
    recvpacket->fre[1] = (compacket->data[ret++]<<8) + compacket->data[ret++];
    recvpacket->fre[2] = (compacket->data[ret++]<<8) + compacket->data[ret++];

    recvpacket->ery[0] = (compacket->data[ret++]<<24) + (compacket->data[ret++]<<16)
                       + (compacket->data[ret++]<<8 ) + compacket->data[ret++];
    recvpacket->ery[1] = (compacket->data[ret++]<<24) + (compacket->data[ret++]<<16)
                       + (compacket->data[ret++]<<8 ) + compacket->data[ret++];
    recvpacket->ery[2] = (compacket->data[ret++]<<24) + (compacket->data[ret++]<<16)
                       + (compacket->data[ret++]<<8 ) + compacket->data[ret++];


}


/**
 * @brief 一键测试 + 单元测试
 * @param info
 */
void cmd_to_packet(basicinfo info,data_packet *packet)
{
    packet->addr = info.addr;
    packet->fn = 0x03;
    packet->registerAddr = 0;  //从第一相开始
   // packet->len = 6; //返回三相所有电压电流信息
    packet->len = 18; //返回三相所有电压电流信息

}


QByteArray packet_to_array(basicinfo info )
{
    data_packet packet;

    cmd_to_packet(info,&packet);

    QByteArray array ;
    array.append(packet.addr);
    array.append(packet.fn);

    array.append(packet.registerAddr >> 8);
    array.append(packet.registerAddr & 0xFF);

    array.append(packet.len >> 8);
    array.append(packet.len & 0xFF);

    packet.CRC = CRC16_2(array.data(), array.size());  //冗余校验

    //        array.append((packet.CRC >> 8)& 0xFF);
    array.append((packet.CRC & 0xFF));
    array.append((packet.CRC >> 8) & 0xFF );

    return array;
}

int net_to_packet(QByteArray recvArray,coming_packet *packet)
{
    qDebug() << "1-0";
    int ret = 0;
    packet->addr = recvArray.at(ret++);
    packet->fn = recvArray.at(ret++);

    packet->len = recvArray.at(ret++);

    int len = packet->len;
    for(int i = 0 ; i<len ;i++)
    {
        packet->data[i] = recvArray.at(ret++);
    }

    packet->CRC = (recvArray.at(ret++) & 0xFF ) + (recvArray.at(ret++) << 8);

    qDebug() << "1-1";

    return ret;

}

/**
 * @brief 将串口返回数据解析成所需格式
 * info 包含从机地址及数据长度信息，用于判断返回数据从机地址及数据长度是否正确
 * recvArray 为返回数据包
 * packet 信息提取，将解析后的数据包提取重要信息
 */
bool net_to_local(basicinfo info,QByteArray recvArray,receiveData *packet)
{
    coming_packet datapacket;
    int len =3 + 12*2 + 3*4 +2;  //返回数据的正确长度
    if(recvArray.size() == len)
    {
        qDebug() << "1";
        //如果长度正确，那么就先将返回数据转化成数据包格式
        int ret = net_to_packet(recvArray,&datapacket);
        if(ret == len)
        {
            qDebug() << "2";
            if(datapacket.addr == info.addr)
            {
                qDebug() << "3";
                if(datapacket.fn == 0x03)
                {
                    qDebug() << "4";
                    if(datapacket.len == 36)
                    {
                        qDebug() << "5";
                        int crc = crc_recv_check(recvArray);
                        if(crc == datapacket.CRC)
                        {
                            qDebug() << "6";
                            coming_to_recv(&datapacket,packet,info);
                            return true;
                        }else
                            qDebug()<< "校验码错误"<<crc <<datapacket.CRC;

                    }else
                        //                        qDebug() << "数据长度错误" <<　datapacket.len;
                        qDebug() << "数据长度错误" << datapacket.len;

                }else
                    qDebug() << "功能码错误" << datapacket.fn;
            }else
                qDebug()<< "从机地址错误" << datapacket.addr;
        }else
            qDebug() << "数据解析错误";

    }else
        qDebug() << "数据总长度错误：" << recvArray.size();

    return false;
}










