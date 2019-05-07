#include "errstabwid.h"

ErrsTabWid::ErrsTabWid(QWidget *parent) : IpsTabWid(parent)
{
    initWid();
}

void ErrsTabWid::initWid()
{
    QString title = tr("升级失败列表");
    //QString title = tr("fail list");
    QStringList header;
    header <<title;
    //header <<tr("fail list");

    initTableWid(header, 0, title);
}


void ErrsTabWid::timeoutDone()
{
    updateData(mData->errs);
}

void ErrsTabWid::languageChanged()
{
    mheader.clear();
    static int count = 1;
    if(count%3)
    {
        if(count == 5)
            count = 1;
        mheader<<"升级失败列表";
    }
    else
    {
        mheader<<"fail list";
    }
    qDebug()<<count<<mheader;
    count++;
    changeHeader(mheader);
}
