#ifndef DATADEFINE_H
#define DATADEFINE_H
#include <qglobal.h>

typedef struct
{
    quint8 addr;  /*从机地址*/
    quint8 fn;  /*功能码*/
    quint16 registerAddr;/*寄存器地址*/
    quint16 len;/*数据长度*/
    quint16 CRC;  /*校验码*/
}data_packet;   //串口发送数据包格式

typedef struct
{
    quint8 addr;  /*从机地址*/
    quint8 fn;  /*功能码*/
    quint8 len;/*数据长度*/
    quint8 data[64];/*数据*/
    quint16 CRC;  /*校验码*/
}coming_packet;   //串口接收数据包格式

typedef struct
{
    quint8 addr;  /*从机地址*/
    quint8 phase;  /*相数，从而确定数据长度*/
    bool isUnitTest;/*是否为单元测试*/
    quint8 currentPhase;/*如果是单元测试，则该项有效，代表单元测试的当前项*/
}basicinfo;/*主界面获取信息用于发送命令*/

typedef struct
{
    bool isUnitTest;   /*是否为单元测试*/
    quint8 currentPhase;/*如果是单元测试，则该项有效，代表单元测试的当前项*/
    quint16 vol[32]; /*返回电压*/
    quint16 cur[32]; /*返回电流*/
    quint16 fre[32]; /*返回频率*/
    quint16 ery[32]; /*返回电能*/

}receiveData;  /*返回数据解析之后打包处理*/

typedef struct
{
    char phase; //主控相数，决定json文件中item个数
    char startTime[32]; //开始时间，一键测试成功作为开始时间
    char endTime[32];  //结束时间，点击保存按钮保存json文件时间作为结束时间
    bool result[32];    //测试结果，从第一相电压到第三相电流结束
    int predictVol;    //电压预测值，可以取一个范围作为上下限
    int predictCur;
    quint16 vol[32];   //和receivedata中的数据保持一致
    quint16 cur[32];

  //  int phase; //当前相数，用来确定item数
}jsonRelated; //json相关数据

typedef struct
{
    quint8 phaseNum;  /*相位数*/
    quint8 outNum;    /*输出位个数*/
    quint8 devType;   /*设备类型*/
}devInfo;

typedef struct
{
    quint16 cur[100];     /*电流*/
    quint16 vol[100];     /*电压*/
}unit;

typedef struct
{
    devInfo info;    /*设备信息*/
    unit phase;
    unit out;
}data_dev; //数据解析信息
/*********************************************************************************/
typedef struct
{
    char work_order[100];
    char uut_type[100];
    char uut_name[100];
    char uut_code[100];
    char serial_number[100];
    char supplier[100];
    char date_code[100];
    char lot_code[100];
    char mould[100];
    char cavity[100];
    char color[100];
}uut_info_json;  //被测对象基本信息

typedef struct
{
    char ate_name[100];
    char computer_name[100];
    char fixuer_id[100];
}ate_info_json; //测试设备信息

typedef struct
{
    char program_name[100];
    char program_ver[100];
}program_info_json; //测试程序信息

typedef struct
{
    char operatorx[100];
    char operation_sequence[100];
    char site_code[100];
    char start_time[100];
    char stop_time[100];
    char test_result[100];
}uut_result_json;  //物料测试结果

typedef struct
{
    char id[100];
    char item_name[100];
    char start_time[100];
    char stop_time[100];
    char test_result[100];
    char result_desc[4000];
    char value_flag[100];
    char low_limit[100];
    char up_limit[100];
    char test_value[100];
    char color[100];
}test_item_list_json;  //测试项

typedef struct
{
    char id[100];
    char item_name[100];
    char start_time[100];
    char stop_time[100];
    char test_result[100];
    char result_desc[4000];
    char value_flag[100];
    char low_limit[100];
    char up_limit[100];
    char test_value[100];
    char color[100];
}sub_test_item_list_json;  //测试子项

typedef struct
{
    quint8 itemum;  //测试主项个数
    quint8 subItemNum;  //测试子相数目
}basic_info_json;


typedef struct
{
    basic_info_json basic_info;
    char factory[100];
    char line[100];
    uut_info_json uut_info;
    ate_info_json ate_info;
    program_info_json program_info;
    uut_result_json uut_result;
    test_item_list_json test_item_list;
    sub_test_item_list_json sub_test_item_list;
}data_json;//json文件相关信息

//------------------[标准源]--------------------
typedef enum{ //命令类型
     ccTest
    ,ccReadConfig
    ,ccSetConfig
    ,ccRecordCount
    ,ccGetFirstRecord
    ,ccGetPriorRecord
    ,ccGetNextRecord
    ,ccGetLastRecord
    ,ccEraseRecord
    ,ccReadCalibration

    ,ccWriteCalibration
    ,ccErrorData
    ,ccReserved
    ,ccReadMeasure
    ,ccVoltageRange
    ,ccCurrentRange
    ,ccVoltage
    ,ccCurrent
    ,ccPhase
    ,ccFrequency

    ,ccConstant
    ,ccPulseConstant
    ,ccRing
    ,ccFilter
    ,ccRingFit
    ,ccManualTest
    ,ccStarting
    ,ccCreeping
    ,ccTime
    ,ccOverload

    ,ccResetOverload
    ,
} ComCmd;

//发送结构体
typedef struct{
    ComCmd Cmd;
    short  wParam;
    long   lParam;
    short  Res;       //reserved
    char   Unused;
} COM_COMMAND;

//返回数据
typedef struct{
    ComCmd cmd;
    short  size; //发送结构体长度 低位在前 高位在后
} COM_MSG_HEAD;

#endif // DATADEFINE_H
