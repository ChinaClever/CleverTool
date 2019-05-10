/*
 * excel_exportlogthread.cpp
 * excel导出线程
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "excel_dblogthread.h"

extern void DB_selectTableByTime(QString &name, QString &start, QString &end, QList<QStringList> &list);
Excel_DbLogThread::Excel_DbLogThread(QObject *parent): Excel_SaveThread(parent)
{
}

Excel_DbLogThread::~Excel_DbLogThread()
{
    wait();
}


void Excel_DbLogThread::set(sExcel_DbItem &ex)
{
    mEx = ex;
    mSize = 0;
    mAllSize = 1;
    start();
}

/**
 * @brief 读取数据库
 */
void Excel_DbLogThread::readSql(QList<QStringList> &list)
{
    DB_selectTableByTime(mEx.db, mEx.start, mEx.end, list);
}


void Excel_DbLogThread::run()
{
    mFileName = mEx.path + mEx.file;
    readSql(mList);
    workDown();
}
