#include "testitemtablewid.h"

TestItemTableWid::TestItemTableWid(QWidget *parent) : ComTableWid(parent)
{
    initWid();
    mTestItems = new TestItems(this);
    QTimer::singleShot(5,this,SLOT(startSlot()));
}


void TestItemTableWid::initWid()
{
    QString title = tr("测试项目列表");
    QStringList header;
    header <<tr("执行") << tr("检查项目") << tr("测试项目描述") << tr("参数");
    initTableWid(header, 0, title);
}


void TestItemTableWid::appendItem(const sTestItem &item)
{
    QStringList listStr;

    if(item.isExecute) listStr << "√"; else listStr << "×";
    listStr << item.item;
    listStr << item.subItem;
    listStr << item.param;
    appendTableRow(listStr);
}

int TestItemTableWid::updateWid(void)
{
    QList<sTestItem> items;
    mTestItems->bulidItems(items);
    for( const auto& value : items ) {
        appendItem(value);
    }
    return items.size();
}

void TestItemTableWid::startSlot()
{
    delTable();
    int ret = updateWid();
    TestConfig::bulid()->item->progress.allNum = ret;
}
