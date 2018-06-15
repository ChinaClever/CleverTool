/***
 * 1.将源数据解析成bytesArray格式发送
 * 2.提供发送源数据的接口，调用后将通过udp发送数据
 * 3.源数据-》协议格式数据包-》array
 */
#include "datasent.h"
void fill_header(data_packet *data);
void fill_designationBlock(data_packet *data);
void fill_segment(data_packet *data,parameterData *packet);
void fill_datafiled(data_packet *data,parameterData *packet);
void environmentdata_filled(data_packet *data ,environmentdata *packet);
void devinfo_filled(data_packet *data ,devInfo *packet);
void electricData_filled(data_packet *data ,electricData *packet);
void unit_filled(data_packet *data , unit *packet, int length);
void sent_msg_xor(data_packet *data);
void set_vice_number(data_packet *data ,int id);
void sent_msg_end(data_packet *data);
void quint8_to_char(data_packet *data , quint8 *buffer , int length);
void ushort_to_char(data_packet *data ,quint16 *buffer ,int length);
void int_to_char(data_packet *data ,quint32 *buffer ,int length);
void read_msg(data_packet *data);
void get_bytearray(data_packet *data);


#if 0
void udp_data_sent()
{
    getSouceData(); //获取源数据



}

void getSouceData()
{

    DataSouce *souce = new DataSouce;
    souceData = souce->getSouceData();
}

/**
 * @brief 发送回路、输入相、输出位数据
 * @param data
 */
void sent_parameterData(parameterData *data)
{
    sent_electricData(data->loopData);
    sent_electricData(data->inputPhrase);
    sent_electricData(data->outData);
}

void sent_electricData(electricData *data)
{
    sent_unit(data->cur);
    sent_unit(data->vol);

}

void sent_unit(unit *data)
{

}

#endif

/**
 * @brief 将源数据转化为协议数据格式
 * soucedata -》data_packet
 */
void souceData_To_protcldata()
{

    //    DataSouce *souce = new DataSouce;  //更新数据

    DataSouce *souce = get_datasouce();
    //        qDebug()<<"souceData_To_protcldata";
    parameterData *data = souce->getSouceData();  //获取数据
    //    qDebug() <<"data:" <<data->mDevInfo.mDevType.devName;

    //    heart = new HeartBeat(this,18725);

    //    qDebug()<<"souceData_To_protcldata";
    //    qDebug()<<"souceData_To_protcldata"<<data->inputPhrase.len<<data->loopData.len<<data->outData.len;
    static data_packet msg; //协议格式数据类型
    msg.segment.filed.data = (quint8 *)malloc(sizeof(quint8)*180);
    fill_header(&msg);
    fill_designationBlock(&msg);
    fill_segment(&msg,data);

}

void fill_header(data_packet *data)
{
    data->hdr = 0x7E;
    data->stx = 0x5E;
}

/**
 * @brief 补充代号段
 * @param data
 */
void fill_designationBlock(data_packet *data)
{
    deviceSettings *settings = get_devsetings();

    int i = 0 ;
    data->code.devCode[i++] = 0x01; //pdu

    //    data->code.devCode[i++] = 0x02; //01 ip-pdu 02 mpdu
    data->code.devCode[i++] = settings->devType;

    data->code.devCode[i++] = 0x01; //交直流
    data->code.devCode[i] = 0x01;   //第一代

    data->code.type = 0x01 ; //udp
    data->code.version = 0x01 ; //第一个版本
    data->code.trans =0x03;
    data->code.reserve[0] = 0 ;//预留
    data->code.reserve[1] = 0 ;//预留
}

/**
 * @brief 补充数据段
 * @param data
 */
void fill_segment(data_packet *data,parameterData *packet)
{
    //    qDebug()<<"fill_segment";

    fill_datafiled(data,packet);


}

/**
 * @brief 补充数据域,确定数据域的地址及高八位
 * @param data
 */
void fill_datafiled(data_packet *data,parameterData *packet)
{
    //    qDebug()<<"fill_datafiled";
    data->segment.filed.addr = 0 ;  //数据域中的地址，代表主机或者副机1，2，3。。。
    int fc = 1;

    data->segment.filed.fn[0] = fc++;
    electricData_filled(data,&(packet->loopData));

#if 1
    data->segment.filed.fn[0] = fc++;
    electricData_filled(data,&(packet->inputPhrase));

    data->segment.filed.fn[0] = fc++  ; //
    electricData_filled(data,&(packet->outData));

    data->segment.filed.fn[0] = fc++  ;
    environmentdata_filled(data,&(packet->envData));

    data->segment.filed.fn[0] = fc++ ; //设备信息
    devinfo_filled(data ,&(packet->mDevInfo));
#endif
}

void devinfo_filled(data_packet *data ,devInfo *packet)
{
    int fc = 1; int fn = 1;
    data->segment.filed.fn[1] = (fc << 4) + fn++;
    data->segment.filed.len = 20;  //命名最长为20个字节
    data->segment.len = 1 + 2 + 2 + data->segment.filed.len ;
    //    qDebug() <<"设备名：" <<packet->mDevType.devName;
    data->segment.filed.data = (quint8 *)(packet->mDevType.devName);
    sent_msg_xor(data);

}

void environmentdata_filled(data_packet *data ,environmentdata *packet)
{
    int fc = 1;

    data->segment.filed.fn[1] = fc++ << 4;
    unit_filled(data,&(packet->tem),packet->len);

    data->segment.filed.fn[1] = fc++ << 4;
    unit_filled(data,&(packet->hum),packet->len);

    data->segment.filed.fn[1] = fc++ << 4;
    data->segment.filed.data = packet->door;  //门禁
    sent_msg_xor(data);

    data->segment.filed.fn[1] = fc++ << 4;
    data->segment.filed.data = packet->water;  //水
    sent_msg_xor(data);

    data->segment.filed.fn[1] = fc++ << 4;
    data->segment.filed.data = packet->smoke;  //烟雾
    sent_msg_xor(data);
}

/**
 * @brief 回路、输入相、输出位，确定数据域的低八位的高四位以及数据长度
 */
void electricData_filled(data_packet *data ,electricData *packet)
{
    //    qDebug()<<"electricData_filled";
    //    qDebug()<<"electricData_filled"<<packet->len;
    int fc = 1;

    data->segment.filed.fn[1] =fc++ << 4;
    unit_filled(data,&(packet->cur),packet->len);         //电流

    data->segment.filed.fn[1] = fc++ << 4;
    unit_filled(data,&(packet->vol),packet->len);         //电压

    data->segment.filed.fn[1] = fc++ << 4;
    int_to_char(data,packet->power,packet->len);
    sent_msg_xor(data);

    data->segment.filed.fn[1] = fc++ << 4;
    int_to_char(data,packet->energy,packet->len);
    sent_msg_xor(data);

    data->segment.filed.fn[1] = fc++ << 4;
    ushort_to_char(data,packet->powerFactor,packet->len);
    sent_msg_xor(data);
#if 1
    data->segment.filed.fn[1] = fc++ << 4;
    quint8_to_char(data,packet->switchStatus,packet->len);
    sent_msg_xor(data);
#endif
    data->segment.filed.fn[1] = fc++ << 4;
    ushort_to_char(data,packet->cDrined,packet->len);
    sent_msg_xor(data);

    data->segment.filed.fn[1] = fc++ << 4;
    ushort_to_char(data,packet->voltagefrequency,packet->len);
    sent_msg_xor(data);


}


/**
 * @brief 电流、电压，确定数据域的低八位的的低四位
 * @param data
 * @param cur
 */
void unit_filled(data_packet *data , unit *packet, int length)
{
    //    qDebug()<<"unit_filled" << data->segment.filed.fn[1];
    int fc = 1 ;
    uchar fn = data->segment.filed.fn[1]; //必须要用fn记录初始值，否则会叠加功能码


    data->segment.filed.fn[1] = fn + fc++;
    ushort_to_char(data,packet->currentValue,length);
    sent_msg_xor(data);


    data->segment.filed.fn[1] = fn + fc++;
    ushort_to_char(data,packet->minValue,length);
    sent_msg_xor(data);

    data->segment.filed.fn[1] = fn + fc++;
    ushort_to_char(data,packet->maxValue,length);
    sent_msg_xor(data);

    data->segment.filed.fn[1] = fn + fc++;
    //    data->segment.filed.data = packet->alarmValue;    //报警值
    quint8_to_char(data,packet->alarmValue,length);
    sent_msg_xor(data);

    data->segment.filed.fn[1] = fn + fc++;
    ushort_to_char(data,packet->cirMinValue,length);
    sent_msg_xor(data);

    data->segment.filed.fn[1] = fn + fc++;
    ushort_to_char(data,packet->cirMaxValue,length);
    sent_msg_xor(data);

    data->segment.filed.fn[1] = fn + fc++;
    //    data->segment.filed.data = packet->cirAlarmValue; //临界报警值
    quint8_to_char(data,packet->cirAlarmValue,length);
    sent_msg_xor(data);
#if 1
    if(data->segment.filed.fn[0] == 1 && (fn == (1 << 4)))  //只针对回路数目
    {
        //        qDebug()<< "success - 0 ";
        data->segment.filed.fn[1] = fn + fc++;
        //        qDebug()<< "success - 1 " <<  data->segment.filed.fn[1] ;
        *(data->segment.filed.data) = packet->num; //回路数目
        //        qDebug()<< "success - 2 " << data->segment.filed.data << "success - 2" << *(data->segment.filed.data) << "success - 2" << packet->num;
        data->segment.filed.len = 1;
        //        quint8_to_char(data,packet,length);
        //        qDebug()<< "success - 3 " << data->segment.filed.len ;
        data->segment.len = 1 + 2 +2 + 1;
        sent_msg_xor(data);
    }
#endif
}

/**
 * @brief 将数据转化成char型，同时计算数据长度
 * @param data
 * @param packet
 */
void ushort_to_char(data_packet *data ,quint16 *buffer ,int length)
{
    //    qDebug()<<"ushort_to_char";
    //    qDebug()<<"ushort_to_char"<< length ;

    int offset = 0 ;
    for(int i = 0 ; i < length ; i++)
    {
        *(data->segment.filed.data + offset++) = buffer[i] >> 8;
        *(data->segment.filed.data + offset++) = buffer[i] & 0xFF;
    }

    data->segment.filed.len = offset;
    data->segment.len = 1 + 2 + 2 + data->segment.filed.len;


    //    qDebug()<<*(data->segment.filed.data) <<*(data->segment.filed.data + 1);

}

void quint8_to_char(data_packet *data ,quint8 *buffer ,int length)
{
    //    qDebug()<<"ushort_to_char";
    //    qDebug()<<"ushort_to_char"<< length ;

    int offset = 0 ;
    for(int i = 0 ; i < length ; i++)
    {
        *(data->segment.filed.data + offset++) = buffer[i] ;
    }

    data->segment.filed.len = offset;
    data->segment.len = 1 + 2 + 2 + data->segment.filed.len;


    //    qDebug()<<*(data->segment.filed.data) <<*(data->segment.filed.data + 1);

}

/**
 * @brief 将int-》char
 * @param data
 * @param buffer
 * @param length
 */
void int_to_char(data_packet *data ,quint32 *buffer ,int length)
{
    //    qDebug()<<"int_to_char";

    int offset = 0 ;
    for(int i = 0 ; i < length ; i++)
    {
        *(data->segment.filed.data + offset++) = buffer[i] >> 24;
        *(data->segment.filed.data + offset++) = buffer[i] >> 16 & 0xFF;
        *(data->segment.filed.data + offset++) = buffer[i] >> 8 & 0xFF;
        *(data->segment.filed.data + offset++) = buffer[i] & 0xFF;
    }

    data->segment.filed.len = offset;
    data->segment.len = 1 + 2 + 2 + data->segment.filed.len;
}

/**
 * @brief 将已补充数据域的数据发送出来计算校验码
 * @param data
 */
void sent_msg_xor(data_packet *data)
{
#if 0
    //    qDebug()<<"sent_msg_xor" <<data->segment.filed.data[0] << data->segment.filed.data[1];
    data->segment.XOR = 0;
    data->segment.XOR += data->segment.filed.addr;
    data->segment.XOR += data->segment.filed.fn[0];
    data->segment.XOR += data->segment.filed.fn[1];
    data->segment.XOR += data->segment.filed.len;
    for(int i = 0 ; i < data->segment.filed.len ; i++)
    {
        data->segment.XOR += *(data->segment.filed.data+i);
        //        qDebug()<<"*"<<*(data->segment.filed.data+i);
    }
    sent_msg_end(data);
#endif

    devSetings *setings = get_devsetings();
    for(int i = 0 ; i < (setings->viceNumber + 1); i++)
    {
        set_vice_number(data,i);
    }

}

/**
 * @brief 确定副机个数
 * @param data
 */
void set_vice_number(data_packet *data ,int id)
{
    data->segment.filed.addr = id; //确定副机编号

    data->segment.XOR = 0;
    data->segment.XOR += data->segment.filed.addr;
    data->segment.XOR += data->segment.filed.fn[0];
    data->segment.XOR += data->segment.filed.fn[1];
    data->segment.XOR += data->segment.filed.len;
    for(int i = 0 ; i < data->segment.filed.len ; i++)
    {
        data->segment.XOR += *(data->segment.filed.data+i);
        //        qDebug()<<"*"<<*(data->segment.filed.data+i);
    }
    sent_msg_end(data);
}

/**
 * @brief 补充结束符，协议数据补充完整
 * @param data
 */
void sent_msg_end(data_packet *data)
{
    //    qDebug()<<"sent_msg_end";
    data->ED = 0x23;
    //    read_msg(data);
    get_bytearray(data);
}

/**
 * @brief 读取数据，判断是否符合格式
 */
void read_msg(data_packet *data)
{
    //   qDebug()<<""
    qDebug()<<"包头"<<data->hdr<<data->stx;
    qDebug()<<"地址" <<data->segment.filed.addr;
    qDebug()<<"功能码1" <<data->segment.filed.fn[0];
    qDebug()<<"功能码2高四位" <<(data->segment.filed.fn[1] >> 4);
    qDebug()<<"功能码2低四位" <<(data->segment.filed.fn[1] & 0x0F);
    qDebug()<<"数据长度" <<data->segment.filed.len;
    qDebug()<<"数据段长度："<<data->segment.len;
    qDebug()<<"数据" <<*(data->segment.filed.data + 1 );
    qDebug()<<"校验码" <<data->segment.XOR ;
}

void get_bytearray(data_packet *data)
{
    //    qDebug()<<"高四位："<<(data->segment.len >> 8)<<"低四位："<<(data->segment.len & 0xFF);
    QByteArray array ;
#if 1
    array.append(data->hdr);
    array.append(data->stx);

    int i = 0;
    array.append(data->code.devCode[i++]);
    array.append(data->code.devCode[i++]);
    array.append(data->code.devCode[i++]);
    array.append(data->code.devCode[i]);

    array.append(data->code.type);
    array.append(data->code.version);
    array.append(data->code.trans);

    array.append(data->code.reserve[0]);
    array.append(data->code.reserve[1]);

    array.append(data->segment.len >> 8);
    array.append(data->segment.len & 0xFF);
    array.append(data->segment.filed.addr);
    array.append(data->segment.filed.fn[0]);
    array.append(data->segment.filed.fn[1]);
    array.append(data->segment.filed.len >> 8);
    array.append(data->segment.filed.len & 0xFF);

    for( i = 0 ; i < data->segment.filed.len ; i++)
    {
        array.append(data->segment.filed.data[i]);
    }

    array.append(data->segment.XOR);

    array.append(data->ED);

    //    qDebug()<<"size:"<<array.size();

    for(int i = 0 ; i < array.size() ; i++)
    {

        //        qDebug()<<QString::number(array.at(i),16);
    }
#endif
    //    qDebug()<<"即将发送数据";
    HeartBeat *heartbeat = get_HeartBeat();
    //    qDebug()<<"data:"<<heartbeat->getData();

    //    if(heartbeat->getData() == "Clever-Manager PDU PC Server OK!")
    heartbeat->sendData(array);
}













