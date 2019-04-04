#include "ipstabwid.h"

IpsTabWid::IpsTabWid(QWidget *parent) : ComTableWid(parent)
{
    mData = DataPacket::bulid()->data;
    initWid();
}

void IpsTabWid::initWid()
{
    QString title = tr("需升级IP列表");
    QStringList header;
    header <<tr("需升级IP列表");

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
