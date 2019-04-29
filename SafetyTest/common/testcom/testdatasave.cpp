/*
 *
 *
 *  Created on: 2018年10月1日
 *      Author: Lzy
 */
#include "testdatasave.h"
#include "dblogs.h"

TestDataSave::TestDataSave(QObject *parent) : QObject(parent)
{
    mItem = TestConfig::bulid()->item;
    mSaveThread = new Excel_SaveThread(this);
}

void TestDataSave::saveDb()
{
    DbLogItem item;
    item.cn = mItem->sn.cn;
    item.barcode = mItem->sn.barCode; // 工状条码
    item.batch = mItem->sn.batch; // 批次
    item.purpose = mItem->sn.purpose; // 用途
    item.sn = mItem->sn.sn; // 产品条码

    QString str = tr("通过");
    if(mItem->progress.errNum) str = tr("未通过");
    item.passed = str;  // 结果
    DbLogs::bulid()->insertItem(item);
}

void TestDataSave::saveTestData()
{
    sTestConfigItem  *item = mItem;
    if(item->sn.isSave)
    {
        mList.clear();
        int id = bulidHead(&(item->sn));
        bulidProgressLog(id, item->progress);
        bulidTestData(item->logList);

        mSaveThread->saveData(mFileName, mList);
    }
    saveDb();
}

QString TestDataSave::bulidFileName(sSnItem *item)
{
    QString file = item->name + '_';
    file += item->purpose + '_';
    file += item->batch  + '_';
    file += item->sn  + '_';
    file += QDateTime::currentDateTime().toString("yyyyMMdd_hhmm");

    return file;
}

QString TestDataSave::bulidPath(sSnItem *item)
{
    QDir dir;
    QString path = "E:/" + tr("安规测试日志") + '/';
    path += item->purpose  + '/';
    path += item->name  + '/';
    path += "工装条码_"+item->barCode  + '/';
    path += "批次_"+item->batch  + '/';
    path += "日期_"+item->date.toString("yyyy_MM_dd")  + '/';
    dir.mkpath(path); //创建多级目录，如果已存在则会返回去true

    return path;
}

int TestDataSave::bulidHead(sSnItem *item)
{
    mFileName= bulidPath(item) + bulidFileName(item);

    int id = 1;
    QStringList list;
    list << ""; mList << list; list.clear();

    list <<"" << QString::number(id++) << tr("设备类型") << item->name;
    mList << list; list.clear();

    list <<"" << QString::number(id++) << tr("设备条码") << item->sn;
    mList << list; list.clear();

    list <<"" << QString::number(id++) << tr("工装条码") << item->barCode;
    mList << list; list.clear();    

    list <<"" << QString::number(id++) << tr("测试日期") << item->date.toString("yyyy-MM-dd");
    mList << list; list.clear();

    list <<"" << QString::number(id++) << tr("完成时间")  << QTime::currentTime().toString("hh:mm:ss");
    mList << list; list.clear();

    list <<"" << QString::number(id++) << tr("操作员") << item->op;
    mList << list; list.clear();

    list <<"" << QString::number(id++) << tr("电脑号") << item->cn;
    mList << list; list.clear();

    list <<"" << QString::number(id++) << tr("用途") << item->purpose;
    mList << list; list.clear();

    list <<"" << QString::number(id++) << tr("批次") << item->batch;
    mList << list; list.clear();

    list << ""; mList << list; list.clear();

    return id;
}

int TestDataSave::bulidProgressLog(int id, sTestProgress &arg)
{
    QStringList list;
    int ok = (arg.okNum * 100.0) / arg.allNum;
    int err = (arg.errNum * 100.0) / arg.allNum;

    QString str = tr("通过");
    if(arg.errNum) str = tr("未通过");
    list <<"" << QString::number(id++) << tr("测试结果") << str;
    mList << list; list.clear();

    list <<"" << QString::number(id++) << tr("测试项目数") << QString::number(arg.allNum);
    mList << list; list.clear();

    list <<"" << QString::number(id++) << tr("被测项目数") << QString::number(arg.finishNum);
    mList << list; list.clear();

    list <<"" << QString::number(id++) << tr("通过项目数") << QString::number(arg.okNum);
    mList << list; list.clear();

    list <<"" << QString::number(id++) << tr("失败项目数") << QString::number(arg.errNum);
    mList << list; list.clear();

    list <<"" << QString::number(id++) << tr("测试项目通过率") << QString::number(ok) + "%";
    mList << list; list.clear();

    list <<"" << QString::number(id++) << tr("测试项目失败率") << QString::number(err) + "%";
    mList << list; list.clear();

    list << ""; mList << list; list.clear();
    return id;
}

void TestDataSave::bulidTestData(QList<QStringList> &list)
{
    QStringList header;
    header<<tr("编号") <<tr("状态") << tr("检查项目") << tr("测试项目描述") << tr("期望值") << tr("实测值") << tr("结果");
    mList << header << list;
}
