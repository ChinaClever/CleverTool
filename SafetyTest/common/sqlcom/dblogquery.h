#ifndef DBLOGQUERY_H
#define DBLOGQUERY_H

#include "dblogs.h"

class DbLogQuery : public DbLogs
{
    Q_OBJECT
public:
    static DbLogQuery *bulid();

    int todayAllCount(const QString &code);
    int todayOkCount(const QString &code);

    int byCodeAllCount(const QDate &start, const QDate &end, const QString &code);
    int byCodeOkCount(const QDate &start, const QDate &end, const QString &code);

protected:
    int todayAllCount();
    int todayOkCount();
    int todayCodeAllCount(const QString &code);
    int todayCodeOkCount(const QString &code);

    int trAllCount(const QDate &start, const QDate &end);
    int trOkCount(const QDate &start, const QDate &end);
    int trCodeAllCount(const QString &code, const QDate &start, const QDate &end);
    int trCodeOkCount(const QString &code, const QDate &start, const QDate &end);

private:
    QString getTimeRange(const QDate &start, const QDate &end);
    QString getTimeCurrent();
    QString getPassField();
    QString getResultField();
    QString getBarcode(const QString &code);
};

#endif // DBLOGQUERY_H
