#ifndef EXCEL_EXPORTLOGTHREAD_H
#define EXCEL_EXPORTLOGTHREAD_H
#include "excel_savethread.h"

struct sExcel_DbItem
{
    QString path; // 导出路径
    QString file; // 导出文件名
    QString db; // 数据库表
    QString start; // 开始时间
    QString end; // 结束时间
    QStringList title; // 标头
};


class Excel_DbLogThread : public Excel_SaveThread
{
public:
    Excel_DbLogThread(QObject *parent = 0);
    ~Excel_DbLogThread();

    void set(sExcel_DbItem &ex);

protected:
    void run();
    virtual void readSql(QList<QStringList> &list);
    void exportExcel(QList<QStringList> &list);

protected:
    sExcel_DbItem mEx;
};

#endif // EXCEL_EXPORTLOGTHREAD_H
