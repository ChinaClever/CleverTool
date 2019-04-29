#include "dblogs.h"

DbLogs::DbLogs()
{
    createTable();
}


DbLogs *DbLogs::bulid()
{
    static DbLogs* sington = nullptr;
    if(sington == nullptr)
        sington = new DbLogs();
    return sington;
}


void DbLogs::createTable()
{
    QString cmd =
            "create table if not exists %1("
            "id             INTEGER primary key autoincrement not null,"
            "date           VCHAR,"
            "time           VCHAR,"
            "passed         VCHAR,"
            "barcode        VCHAR,"
            "batch          VCHAR,"
            "purpose        VCHAR,"
            "cn             VCHAR,"
            "sn             VCHAR);";
    QSqlQuery query;
    if(!query.exec(cmd.arg(tableName())))
    {
        throwError(query.lastError());
    }
}

bool DbLogs::insertItem(DbLogItem &item)
{
    bool ret = false;
//    item.id = maxId()+1;
    QString cmd = "insert into %1 (id, date, time, cn, barcode, batch, purpose, sn, passed) "
                  "values(:id,:date,:time,:cn,:barcode,:batch,:purpose,:sn,:passed)";
    ret = modifyItem(item,cmd.arg(tableName()));
    if(ret)
        emit itemChanged(item.id,Insert);

    return ret;
}


bool DbLogs::modifyItem(const DbLogItem &item, const QString &cmd)
{
    bool ret = false;
    QSqlQuery query;
    query.prepare(cmd);
//    query.bindValue(":id",item.id);
    query.bindValue(":date",item.date);
    query.bindValue(":time",item.time);
    query.bindValue(":cn",item.cn);
    query.bindValue(":barcode",item.barcode);
    query.bindValue(":batch",item.batch);
    query.bindValue(":purpose",item.purpose);
    query.bindValue(":sn",item.sn);
    query.bindValue(":passed",item.passed);
    ret = query.exec();
    if(!ret)
        throwError(query.lastError());
    return ret;
}

void DbLogs::selectItem(QSqlQuery &query, DbLogItem &item)
{
    item.id = query.value("id").toInt();
    item.date = query.value("date").toString();
    item.time = query.value("time").toString();
    item.cn = query.value("cn").toString();
    item.barcode = query.value("barcode").toString();
    item.batch = query.value("batch").toString();
    item.purpose = query.value("purpose").toString();
    item.sn = query.value("sn").toString();
    item.passed = query.value("passed").toString();
}
