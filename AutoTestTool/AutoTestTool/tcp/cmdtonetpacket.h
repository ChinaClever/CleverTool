#ifndef CMDTONETPACKET_H
#define CMDTONETPACKET_H

#include "datadefine.h"

extern void cmd_to_netpacket(bool isOut,bool isVol,data_packet *packet); //提供数据交互时的功能码确定接口
extern void filecmd_to_netpacket(bool isexport,data_packet *packet,quint16 len);  //提供导入导出命令接口
#endif // CMDTONETPACKET_H
