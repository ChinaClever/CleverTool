#ifndef LOG_DATEHASH_H
#define LOG_DATEHASH_H
#include <QtCore>


class LOG_DateHash
{
public:
    LOG_DateHash();

    void updateTab(const QString &table);

    void getYearList(QStringList &list);
    void getMonthList(const QString &year, QStringList &list);
    void getDayList(const QString &month, QStringList &list);

protected:
    void initDate();
    void setDate(const QStringList &dateList);
    void addDate(const QString &date);

private:
    QStringList mYearList,mMonthList,mDayList;
};

#endif // LOG_DATEHASH_H
