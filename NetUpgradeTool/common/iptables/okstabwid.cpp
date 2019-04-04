#include "okstabwid.h"

OksTabWid::OksTabWid(QWidget *parent) : ErrsTabWid(parent)
{
    initWid();
}


void OksTabWid::initWid()
{
    QString title = tr("升级成功列表");
    QStringList header;
    header <<tr("升级成功列表");

    initTableWid(header, 0, title);
}


void OksTabWid::timeoutDone()
{
    updateData(mData->oks);
}
