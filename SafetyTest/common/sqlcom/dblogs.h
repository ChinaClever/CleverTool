#ifndef DBLOGS_H
#define DBLOGS_H
#include "sqltablemodel.h"

#define DB_Logs "logs"

struct DbLogItem : public DbBasicItem {
    QString cn; // 电脑号
    QString barcode; // 工状条码
    QString batch; // 批次
    QString purpose; // 用途
    QString sn; // 产品条码
    QString status;  // 结果
};

class DbLogs: public SqlBasic<DbLogItem>
{
    DbLogs();
public:
    static DbLogs *bulid();
    bool insertItem(DbLogItem &item); // 插入
    QString tableName(){return QString("%1").arg(DB_Logs);}

protected:
    bool modifyItem(const DbLogItem& item,const QString& cmd);
    void selectItem(QSqlQuery &query,DbLogItem &item);
    void createTable();
};

#endif // DBLOGS_H
