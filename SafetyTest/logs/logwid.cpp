#include "logwid.h"
#include "dblogs.h"

LogWid::LogWid(QWidget *parent) : LogComWid(parent)
{

}


/**
 * @brief 获取表对象
 * @return
 */
BasicSql *LogWid::getDb()
{
    return  DbLogs::bulid();
}

/**
 * @brief 初始化表格
 */
void LogWid::initTable()
{
    QString table = getDb()->tableName();
    this->refreshTable(table);

    mTableTile = tr("测试纪录日志");
    setGroupBoxTitle(mTableTile);

    mHeadList.clear();
    mHeadList << tr("编号") << tr("日期") << tr("时间")
              << tr("结果") << tr("工状条码") << tr("批次")
              << tr("用途") << tr("电脑号") << tr("产品条码");

    model->setHeaders(mHeadList);
}
