/**
  *解析udp心跳包发送出去后返回的数据
  *
  *
  */
#include "analysisdata.h"

bool checkXor(data_packet *packet)
{
    int calcXor = 0 ;
    calcXor += packet->addr;
    calcXor += (packet->predictValue >> 8);
    calcXor += (packet->predictValue & 0xFF);
    calcXor += (packet->practicalValue >> 8);
    calcXor += (packet->practicalValue & 0xFF);
    calcXor += (packet->reserve >> 8);
    calcXor += (packet->reserve & 0xFF);
    calcXor += packet->result;

    //    qDebug() << "校验码：" <<calcXor <<packet->XOR;

    if(calcXor == packet->XOR)
        return true;
    else
        return false;

}

/**
 * @brief analysis_data
 * @param deviceData  解析后的结果存在该结构体中
 * @return
 */
bool analysis_data(data_packet &netPacket, QByteArray &array)
{

    if(array.size() == 16)
    {
        int ret = 0 ;
        netPacket.har = array.at(ret++);
        netPacket.stx = array.at(ret++);
        netPacket.trans = array.at(ret++);
        netPacket.fn = array.at(ret++);
        netPacket.len = (array.at(ret++)<<8) + array.at(ret++) ;
        netPacket.addr = array.at(ret++);
        netPacket.predictValue = (array.at(ret++)<<8) + array.at(ret++);
        netPacket.practicalValue = (array.at(ret++)<<8) + array.at(ret++);
        netPacket.reserve = (array.at(ret++)<<8) + array.at(ret++);
        netPacket.result = array.at(ret++);
        netPacket.XOR = array.at(ret++);
        netPacket.ED = array.at(ret++);

        if(ret != 16)
        {
            qDebug() <<"数据解析错误！" << ret;
            return false;
        }

    }else
    {
        qDebug() << "数据字节长度错误" << array.size();
        return false;
    }


    //判断数据包格式是否正确
    if((netPacket.har == 0x7E) && (netPacket.stx == 0xAE) && (netPacket.ED == 0x23))
    {
        if(netPacket.trans == 2)
        {
            if((netPacket.fn >>4 > 0) && (netPacket.fn >> 4 < 5))
            {
                if(netPacket.len == 8)
                {
                    if(!checkXor(&netPacket))
                    {
                        qDebug()<< "校验码错误";
                        return false;
                    }
                }else
                {
                    qDebug() << "数据长度错误！" << netPacket.len;
                    return false;
                }

            }else
            {
                qDebug() << "功能码错误！" << (netPacket.fn >> 4);
                return false;
            }
        }else
        {
            qDebug() << "方向识别码错误！" << netPacket.trans;
            return false;
        }
    }else
    {
        qDebug() << "同步头||识别码||结束符错误" << netPacket.har << netPacket.stx << netPacket.ED;
        return false;
    }

    return true;

}

void get_information(data_packet &netPacket , data_dev *deviceData)
{
    //    data_dev deviceData;
    //    memset(&deviceData,0,sizeof(deviceData));


    //根据功能码检查获取信息
    if(netPacket.fn >> 4 == 1)
    {

        deviceData->info.devType = netPacket.addr;
        deviceData->info.phaseNum = netPacket.predictValue;
        deviceData->info.outNum = netPacket.practicalValue;

    }else if(netPacket.fn == 0x21)
    {
        qDebug() << "2";
        int ret = netPacket.addr;
        *(deviceData->phase.cur + ret) = netPacket.practicalValue;
    }else if(netPacket.fn == 0x22)
    {
        qDebug() << "3";
        int ret = netPacket.addr;
        *(deviceData->phase.vol + ret) = netPacket.practicalValue;
    }else if(netPacket.fn == 0x31)
    {
        qDebug() << "4";
        int ret = netPacket.addr;
        *(deviceData->out.cur + ret) = netPacket.practicalValue;
    }else if(netPacket.fn == 0x32)
    {
        qDebug() << "5";
        int ret = netPacket.addr;
        *(deviceData->out.vol + ret ) = netPacket.practicalValue;
    }else
        qDebug() << "功能码不在解析范围内！";
}

