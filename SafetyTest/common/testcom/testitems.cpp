#include "testitems.h"

TestItems::TestItems(QObject *parent) : QObject(parent)
{

}

int TestItems::bulidItems(QList<sTestItem> &items)
{
    getReady(items);
    getResis(items);
    getInsulation(items);
    getWithstand(items);

    return items.size();
}

int TestItems::getReady(QList<sTestItem> &items)
{
    sTestItem item;

    item.item = tr("测试前准备");
    {
        item.subItem = tr("测试前接线");
        item.param = tr("奖测试试议高压线");
        items.append(item);
    }
    {
        item.subItem = tr("延时5S");
        item.param = tr("5000ms");
        items.append(item);
    }
    {
        item.subItem = tr("串口检测");
        item.param = tr("串口号,波特率");
        items.append(item);
    }

    return items.size();
}

int TestItems::getResis(QList<sTestItem> &items)
{
    sTestItem item;

    item.item = tr("接地电阻参数设置");
    {
        item.subItem = tr("输出电流");
        item.param = tr("25");
        items.append(item);
    }
    {
        item.subItem = tr("输出电压");
        item.param = tr("8");
        items.append(item);
    }
    {
        item.subItem = tr("测试时间");
        item.param = tr("10s");
        items.append(item);
    }

    return items.size();
}

int TestItems::getInsulation(QList<sTestItem> &items)
{
    sTestItem item;

    item.item = tr("输入对地绝缘设置");
    {
        item.subItem = tr("绝缘耐心压");
        item.param = tr("2121");
        items.append(item);
    }

    return items.size();
}

int TestItems::getWithstand(QList<sTestItem> &items)
{
    sTestItem item;

    item.item = tr("输入对地耐压设置");
    {
        item.subItem = tr("绝缘耐心压");
        item.param = tr("500");
        items.append(item);
    }

    return items.size();
}
