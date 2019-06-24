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
    getEnd(items);

    return items.size();
}

int TestItems::getReady(QList<sTestItem> &items)
{
    sTestItem item;

    item.item = tr("测试前准备");
    {
        item.subItem = tr("测试前接线");
        item.param = tr("将测试仪高压线（红色）接产品输入/输出L、N线，黑色线接产品输入PE线，另一线缆（红色）接产品机壳。");
        items.append(item);
    }

    {
        item.subItem = tr("延时5S");
        item.param = tr("5000ms");
        items.append(item);
    }

    return items.size();
}

int TestItems::getResis(QList<sTestItem> &items)
{
    sTestItem item;

    getInstep(items,tr("接地电阻测试准备"),1);

    item.item = tr("接地电阻参数设置");//FF+回车
    {
        item.subItem = tr("输出电流");//SY25+回车
        item.param = tr("25");
        items.append(item);
    }
    {
        item.subItem = tr("输出电压");//SZ8+回车
        item.param = tr("8");
        items.append(item);
    }
    {
        item.subItem = tr("测试时间");//S210+回车
        item.param = tr("10s");
        items.append(item);
    }

    {
        item.subItem = tr("电阻上限");//S0100+回车
        item.param = tr("100");
        items.append(item);
    }

    {
        item.subItem = tr("电阻下限");//S10+回车
        item.param = tr("0");
        items.append(item);
    }

    {
        item.subItem = tr("频率50Hz");//FP+回车
        item.param = tr("");
        items.append(item);
    }

    {
        item.subItem = tr("关于扫描器");//S30+回车
        item.param = tr("0");
        items.append(item);
    }

    {
        item.subItem = tr("测线电阻归零");//S424+回车
        item.param = tr("24");
        items.append(item);
    }

    {
        item.subItem = tr("关闭记忆步骤连接");//FR+回车
        item.param = tr("");
        items.append(item);
    }

    {
        item.subItem = tr("接地电阻测试");//FA+回车
        item.param = tr("");
        items.append(item);
    }

    {
        item.subItem = tr("delay");//10s
        item.param = tr("10000ms");
        items.append(item);
    }

    {
        item.subItem = tr("读取接地电阻测试结果");//?K+回车
        item.param = tr("0-100mΩ");
        items.append(item);
    }

    return items.size();
}

int TestItems::getInsulation(QList<sTestItem> &items)
{
    sTestItem item;

    getInstep(items,tr("绝缘电阻测试准备"),2);

    item.item = tr("输入对地绝缘测试参数设置");//FE+回车
    {
        item.subItem = tr("绝缘耐压");//SS500+回车
        item.param = tr("500");
        items.append(item);
    }

    {
        item.subItem = tr("绝缘电阻最低充电电流");//ST0+回车
        item.param = tr("0");
        items.append(item);
    }

    {
        item.subItem = tr("绝缘电阻上限");//SU0+回车
        item.param = tr("0");
        items.append(item);
    }

    {
        item.subItem = tr("绝缘电阻下限");//SV2+回车
        item.param = tr("2");
        items.append(item);
    }

    {
        item.subItem = tr("测试时间");//SW3+回车
        item.param = tr("3.0s");
        items.append(item);
    }

    {
        item.subItem = tr("关闭记忆步骤连接");//FR+回车
        item.param = tr("");
        items.append(item);
    }

    {
        item.subItem = tr("绝缘电阻测试");//FA+回车
        item.param = tr("");
        items.append(item);
    }

    {
        item.subItem = tr("delay");//3s
        item.param = tr("3000ms");
        items.append(item);
    }

    {
        item.subItem = tr("读取绝缘电阻测试结果");//?K+回车
        item.param = tr("2-9999MΩ");
        items.append(item);
    }
    return items.size();
}

int TestItems::getWithstand(QList<sTestItem> &items)
{
    sTestItem item;

    getInstep(items,tr("输入对地耐压测试准备"),3);

    item.item = tr("输入对地耐压参数设置");//FD+回车   DCW进入直流耐压
    {
        item.subItem = tr("直流电压");//SI2121+回车 SI
        item.param = tr("2121");
        items.append(item);
    }

    {
        item.subItem = tr("漏电流上限");//SJ5000+回车 SJ
        item.param = tr("5000");
        items.append(item);
    }

    {
        item.subItem = tr("漏电流下限");//SK0+回车
        item.param = tr("0");
        items.append(item);
    }

    {
        item.subItem = tr("缓升时间");//SL6+回车
        item.param = tr("6s");
        items.append(item);
    }

    {
        item.subItem = tr("持续时间");//SM3+回车
        item.param = tr("3s");
        items.append(item);
    }

    {
        item.subItem = tr("delay");//3s
        item.param = tr("3000ms");
        items.append(item);
    }

    {
        item.subItem = tr("直流最低充电电流");//SO零+回车    DCW
        item.param = tr("0");
        items.append(item);
    }

    {
        item.subItem = tr("开启直流缓动电流");//F8+回车      DCW
        item.param = tr("");
        items.append(item);
    }

    {
        item.subItem = tr("直流电弧敏感度");//SP3+回车      DCW
        item.param = tr("3");
        items.append(item);
    }

    {
        item.subItem = tr("关闭直流电弧判定模式");//FN+回车 DCW
        item.param = tr("");
        items.append(item);
    }

    {
        item.subItem = tr("直流测线漏电流归零");//SR零+回车   DCW
        item.param = tr("0");
        items.append(item);
    }

    {
        item.subItem = tr("关闭记忆步骤连接");//FR+回车
        item.param = tr("");
        items.append(item);
    }

    {
        item.subItem = tr("输入对地耐压测试");//FA+回车
        item.param = tr("");
        items.append(item);
    }

    {
        item.subItem = tr("delay");//9s
        item.param = tr("9000ms");
        items.append(item);
    }

    {
        item.subItem = tr("读取输入对地测试结果");//?K+回车
        item.param = tr("0-9.999mA");
        items.append(item);
    }

    return items.size();
}

int TestItems::getEnd(QList<sTestItem> &items)
{
    sTestItem item;
    item.item = tr("***测试结束***");

    {
    item.subItem = tr("复位");//FB+回车
    item.param = tr("");
    items.append(item);
    }

    {
    item.subItem = tr("结束");//
    item.param = tr("请去掉被测产品线缆");
    items.append(item);
    }

    return items.size();
}

void TestItems::getInstep(QList<sTestItem> &items,QString str,int step)
{
    sTestItem item;

    item.item = str;//*IDN?+回车 连接命令
    {
        {
            item.subItem = tr("延时1S");
            item.param = tr("1000ms");
            items.append(item);
        }

        {
            item.subItem = tr("复位");//FB+回车
            item.param = tr("");
            items.append(item);
        }

        {
            item.subItem = tr("延时1S");
            item.param = tr("1000ms");
            items.append(item);
        }

        {
            item.subItem = tr("进入记忆组1");//S51+回车
            item.param = tr("1");
            items.append(item);
        }

        {
            item.subItem = tr("延时500MS");
            item.param = tr("500ms");
            items.append(item);
        }

        {
            item.subItem = tr("进入测试步骤%1").arg(step);//S61+回车 S62+回车 S63+回车
            item.param = tr("%1").arg(step);
            items.append(item);
        }
    }
}
