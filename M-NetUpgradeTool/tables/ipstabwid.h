#ifndef IPSTABWID_H
#define IPSTABWID_H

#include "comtablewid.h"
#include "common/datapacket.h"

class IpsTabWid : public ComTableWid
{
    Q_OBJECT
public:
    explicit IpsTabWid(QWidget *parent = nullptr);

protected slots:
   virtual void timeoutDone();

protected:
    virtual void initWid();
    void updateData(QStringList &lst);

protected:
    sDataPacket *mData;
};

#endif // IPSTABWID_H
