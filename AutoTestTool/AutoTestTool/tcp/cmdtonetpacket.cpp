/**
  *1.将命令打包成网络数据发送
  *2.包括获取电流电压命令以及导入导出命令
  */

#include "cmdtonetpacket.h"

/**
 * @brief 确定低四位功能码
 * @param type --- 电流/电压
 * @param num --- 代表某一相
 */
void phaseout_to_netpacket(bool isVol ,data_packet *packet)
{
    quint8 fc = packet->fn;

    switch (isVol) {
    case 0: //电流
        packet->fn = fc + 1;

        break;
    case 1: //电压
        packet->fn = fc + 2;

        break;
    default:
        break;
    }
}

/**
 * @brief 初始化网络数据包的一些基本信息，例如包头包尾等
 * @param packet
 */
void initBasicPacket(data_packet *packet)
{
    packet->har = 0x7E;
    packet->stx = 0xAE;
    packet->trans = 0x01;
    //    packet->addr  len predict practicl 在主程序中进行初始化
    packet->reserve = 0;
    packet->result = 0;
    packet->ED =0x23;
}

/**
 * @brief 确定高四位功能码
 * @param type_1     --- 相位/输出位（0/1）
 * @param type_2     --- 电流/电压  （0/1）
 * @param num   --- 输出位、相位数
 */
void cmd_to_netpacket(bool isOut,bool isVol,data_packet *packet)
{
    initBasicPacket(packet);

    switch (isOut) {
    case 0:  //主控三相
        packet->fn = (2 << 4);

        break;
    case 1:  //输出位
        packet->fn = (3 << 4);

        break;
    default:
        break;
    }

    phaseout_to_netpacket(isVol,packet);
}

/**
 * @brief 导入命令，len为文件长度
 * @param packet
 * @param len
 */
void filecmd_to_netpacket(bool isexport,data_packet *packet,quint16 len)
{
    initBasicPacket(packet);

    switch (isexport) {
    case 0:
        packet->fn = (4<<4) + 1; //导入
        break;
    case 1:
        packet->fn = (4<<4) + 2; //导出
        break;
    default:
        break;
    }

    packet->len = 8;
    packet->addr = 0xFF;
    packet->predictValue =0;
    packet->practicalValue = len;

    packet->XOR = 0;
    packet->XOR += packet->addr;
    packet->XOR += packet->predictValue;
    packet->XOR += packet->practicalValue;
    packet->XOR += packet->reserve;
    packet->XOR += packet->result;
}




