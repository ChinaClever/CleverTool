/*
 *
 *
 *  Created on: 2018年10月1日
 *      Author: Lzy
 */
#include "testdatatablewid.h"

TestDataTableWid::TestDataTableWid(QWidget *parent) : ComTableWid(parent)
{
    initWid();
    mItem = TestConfig::bulid()->item;

    timer = new QTimer(this);
    timer->start(4*100);
    connect(timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
}


void TestDataTableWid::initWid()
{
    QString title = tr("测试数据列表");
    QStringList header;
    header<<tr("状态") << tr("检查项目") << tr("测试项目描述") << tr("期望值") << tr("实测值") << tr("结果");
    initTableWid(header, 0, title);
}

void TestDataTableWid::startSLot()
{
    delTable();
    mItem->logList.clear();
}

void TestDataTableWid::appendItem(const sTestDataItem &item)
{
    QStringList listStr;

    if(item.status) listStr << "√"; else listStr << "×";
    listStr << item.item;
    listStr << item.subItem;
    listStr << item.expect;
    listStr << item.measured;
    listStr << item.result;
    appendTableRow(listStr, !item.status);

    listStr.insert(0, QString::number(item.id));
    mItem->logList.append(listStr);
    mItem->logItems.append(item);
}

void TestDataTableWid::timeoutDone()
{
    if(mItem->dataItem.size()) {
        appendItem(mItem->dataItem.first());
        mItem->dataItem.removeFirst();
    }
}
