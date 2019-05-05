/*
 * log_datehash.cpp
 *
 *  Created on: 2017年10月11日
 *      Author: Lzy
 */
#include "log_datehash.h"

extern bool db_select_dateList(const QString &tableName, QStringList &dateList);
LOG_DateHash::LOG_DateHash()
{

}

void LOG_DateHash::initDate()
{
    mYearList.clear();
    mMonthList.clear();
    mDayList.clear();
}

void LOG_DateHash::updateTab(const QString &table)
{
    initDate();
    QStringList dateList;
    bool ret = db_select_dateList(table, dateList);
    if(ret)
        setDate(dateList);
}

/**
 * @brief 填充日期
 * @param dateList
 */
void LOG_DateHash::setDate(const QStringList &dateList)
{
    for(int i=0; i<dateList.size(); ++i)
        addDate(dateList.at(i));
}

/**
 * @brief 增加年月日
 */
void LOG_DateHash::addDate(const QString &date)
{
    QStringList list = date.split("-",QString::SkipEmptyParts);

    if(list.size() == 3) {
        QString str = list[0];
        if(!mYearList.contains(str))
            mYearList << str;

        str += "-"+list[1];
        if(!mMonthList.contains(str))
            mMonthList << str;

        if(!mDayList.contains(date))
            mDayList << date;
    }
}

void LOG_DateHash::getYearList(QStringList &list)
{
    list = mYearList;
}

void LOG_DateHash::getMonthList(const QString &year, QStringList &list)
{
    for(int i=0; i<mMonthList.size(); ++i)
    {
        QString str = mMonthList.at(i);
        if(str.contains(year))
            list << str;
    }
}

void LOG_DateHash::getDayList(const QString &month, QStringList &list)
{
    for(int i=0; i<mDayList.size(); ++i)
    {
        QString str = mDayList.at(i);
        if(str.contains(month))
            list << str;
    }
}
