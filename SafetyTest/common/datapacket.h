#ifndef DATAPACKET_H
#define DATAPACKET_H
#include <QtCore>
#include "serialportdlg.h"

struct sSnItem
{
    QString name;
    QString op; // 操作员
    QString cn; // 电脑号
    QString sn; // 产品条码

    QString barCode; // 工状条码
    QString batch; // 批次
    QString purpose;
    QDate date;

    bool isGnd; // 接地测试
    QString gnd; // 接地测试结果

    bool isIr; //绝缘测试
    QString ir; // 绝缘测试结果

    bool isDcw; //直流耐压测试
    QString dcw; // 直流耐压测试结果

    bool isAcw; //交流耐压测试
    QString acw; // 交流耐压测试结果

    bool snClear;
    bool errStop;
    bool isSave;
};

struct sTestProgress
{
    sTestProgress() {okNum=errNum=finishNum=0; allNum=10;}

    int allNum;
    int okNum;
    int errNum;
    int finishNum;
    QString status;
};



/**
 * @brief 测试数据结构体
 */
struct sTestDataItem
{
    sTestDataItem(){id=0;}

    int id;
    bool status; // 状态

    QString item; // 检查项目
    QString subItem; // 测试项目描述

    QString expect; // 期望值
    QString measured; // 实测值
    QString result; // 结果
};

struct sTestConfigItem
{
    sTestConfigItem() {mode=0;}
    SerialPort *serial; // 串口对象

    int mode;
    sSnItem sn;
    sTestProgress progress;

    QList<sTestDataItem> dataItem;
    QList<QStringList> logList;
    QList<sTestDataItem> logItems;
};



#endif // DATAPACKET_H
