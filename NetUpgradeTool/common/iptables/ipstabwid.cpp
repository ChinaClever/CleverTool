#include "ipstabwid.h"

IpsTabWid::IpsTabWid(QWidget *parent) : ComTableWid(parent)
{
    mData = DataPacket::bulid()->data;
    initWid();
}

void IpsTabWid::initWid()
{
    QString title = tr("需升级IP列表");
    //QString title = tr("list of need to upgrade");
    QStringList header;
    header <<title;
    //header <<tr("list of need to upgrade");

    initTableWid(header, 0, title);
}

void IpsTabWid::updateData(QStringList &lst)
{
    if(lst.size()) {
        checkTableRow(lst.size());
        for(int i=0; i<lst.size(); ++i)
        {
            QStringList list;
            list << lst.at(i);
            setTableRow(i, list);
        }
    } else {
        clearTable();
    }

}

void IpsTabWid::timeoutDone()
{
    updateData(mData->ips);
}

void IpsTabWid::languageChanged()
{
    mheader.clear();
    static int count = 1;
    if(count%3)
    {
        if(count == 5)
            count = 1;
        mheader<<"需升级IP列表";
    }
    else
    {
        mheader<<"list of need to upgrade";
    }
    count++;
    changeHeader(mheader);
}
