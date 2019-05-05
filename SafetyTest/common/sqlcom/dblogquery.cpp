#include "dblogquery.h"


DbLogQuery *DbLogQuery::bulid()
{
    static DbLogQuery* sington = nullptr;
    if(sington == nullptr)
        sington = new DbLogQuery();
    return sington;
}


int DbLogQuery::todayAllCount()
{
    QString cmd = QString(" where %1").arg(getTimeCurrent());
    return count("id", cmd);
}

int DbLogQuery::todayOkCount()
{
    QString cmd = QString(" where %1 and %2")
            .arg(getTimeCurrent())
            .arg(getPassField());

    return count("id", cmd);
}

int DbLogQuery::todayCodeAllCount(const QString &code)
{
    QString cmd = QString(" where %1 and %2")
            .arg(getTimeCurrent())
            .arg(getBarcode(code));

    return count("id", cmd);
}


int DbLogQuery::todayCodeOkCount(const QString &code)
{
    QString cmd = QString(" where %1 and %2 and %3")
            .arg(getTimeCurrent())
            .arg(getBarcode(code))
            .arg(getPassField());

    return count("id", cmd);
}

int DbLogQuery::todayAllCount(const QString &code)
{
    int ret = 0;
    if(code.isEmpty()) {
        ret = todayAllCount();
    } else {
        ret = todayCodeAllCount(code);
    }

    return ret;
}

int DbLogQuery::todayOkCount(const QString &code)
{
    int ret = 0;
    if(code.isEmpty()) {
        ret = todayOkCount();
    } else {
        ret = todayCodeOkCount(code);
    }

    return ret;
}


int DbLogQuery::trAllCount(const QDate &start, const QDate &end)
{
    QString cmd = QString(" where %1").arg(getTimeRange(start, end));
    return count("id", cmd);
}

int DbLogQuery::trOkCount(const QDate &start, const QDate &end)
{
    QString cmd = QString(" where %1 and %2")
            .arg(getPassField())
            .arg(getTimeRange(start, end));
    return count("id", cmd);
}

int DbLogQuery::trCodeAllCount(const QString &code, const QDate &start, const QDate &end)
{
    QString cmd = QString(" where %1 and %2")
            .arg(getTimeRange(start, end))
            .arg(getBarcode(code));
    return count("id", cmd);
}

int DbLogQuery::trCodeOkCount(const QString &code, const QDate &start, const QDate &end)
{
    QString cmd = QString(" where %1 and %2 and %3")
            .arg(getBarcode(code))
            .arg(getPassField())
            .arg(getTimeRange(start, end));
    return count("id", cmd);
}


int DbLogQuery::byCodeAllCount(const QDate &start, const QDate &end, const QString &code)
{
    int ret = 0;
    if(code.isEmpty()) {
        ret = trAllCount(start, end);
    } else {
        ret = trCodeAllCount(code, start, end);
    }

    return ret;
}

int DbLogQuery::byCodeOkCount(const QDate &start, const QDate &end, const QString &code)
{
    int ret = 0;
    if(code.isEmpty()) {
        ret = trOkCount(start, end);
    } else {
        ret = trCodeOkCount(code, start, end);
    }

    return ret;
}



QString DbLogQuery::getTimeRange(const QDate &start, const QDate &end)
{
    QString cmd = QString(" date Between \'%1\' and  \'%2\'")
            .arg(start.toString("yyyy-MM-dd"))
            .arg(end.toString("yyyy-MM-dd"));

    return cmd;
}

QString DbLogQuery::getTimeCurrent()
{
    QString cmd = QString(" date = \'%1\'")
            .arg(QDate::currentDate().toString("yyyy-MM-dd"));

    return cmd;
}

QString DbLogQuery::getPassField()
{
    QString cmd = QString(" passed = \'%1\'")
            .arg(tr("通过"));

    return cmd;
}

QString DbLogQuery::getBarcode(const QString &code)
{
    QString cmd = QString(" barcode = \'%1\'")
            .arg(code);

    return cmd;
}
