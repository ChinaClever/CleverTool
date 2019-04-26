#ifndef DATAPACKET_H
#define DATAPACKET_H
#include <QtCore>

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
    sTestConfigItem() {mode=0; devId=1;}

    int devId;
    int mode;
    bool isSnmp;
    sSnItem sn;
    sTestProgress progress;

    QList<sTestDataItem> dataItem;
    QList<QStringList> logList;
    QList<sTestDataItem> logItems;
};

class DataPacket
{
public:
    DataPacket();
};

#endif // DATAPACKET_H
