#ifndef DATADEFINE_H
#define DATADEFINE_H
#include <qglobal.h>
#include <ctype.h>

typedef struct
{
    quint8 har;  /*同步头：0x7E*/
    quint8 stx;  /*识别码：0xAE*/
    quint8 trans;/*方向识别码*/
    quint8 fn;/*功能码*/
    quint8 len;  /*长度*/
    quint8 addr; /*地址*/
    quint16 predictValue;  /*预测值*/
    quint16 practicalValue;/*实际值*/
    quint16 reserve;/*保留*/
    quint8 result;  /*结果：1-succe 2-fail*/
    quint8 XOR;     /*校验码*/
    quint8 ED;      /*结束符*/
}data_packet;   //网络数据包

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
    quint8 subOutItemNum;  //测试电流子相数目
    quint8 subPhaseItemNum;  //测试电压子相数目
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

#endif // DATADEFINE_H
