#include "okstabwid.h"

OksTabWid::OksTabWid(QWidget *parent) : ErrsTabWid(parent)
{
    initWid();
}


void OksTabWid::initWid()
{
    QString title = tr("升级成功列表");
    //QString title = tr("successful list");
    QStringList header;
    header <<title;
    //header <<tr("successful list");

    initTableWid(header, 0, title);
}


void OksTabWid::timeoutDone()
{
    updateData(mData->oks);
}

void OksTabWid::languageChanged()
{
    mheader.clear();
    static int count = 1;
    if(count%3)
    {
        if(count == 5)
            count = 1;
        mheader<<"升级成功列表";
    }
    else
    {
        mheader<<"successful list";
    }
    count++;
    changeHeader(mheader);
}
