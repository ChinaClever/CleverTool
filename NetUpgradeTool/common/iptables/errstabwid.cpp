#include "errstabwid.h"

ErrsTabWid::ErrsTabWid(QWidget *parent) : IpsTabWid(parent)
{
    initWid();
}

void ErrsTabWid::initWid()
{
    QString title = tr("升级失败列表");
    QStringList header;
    header <<tr("升级失败列表");

    initTableWid(header, 0, title);
}


void ErrsTabWid::timeoutDone()
{
    updateData(mData->errs);
}
