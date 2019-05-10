#ifndef IPSTABWID_H
#define IPSTABWID_H

#include "comtablewid.h"
#include "datapacket.h"

class IpsTabWid : public ComTableWid
{
    Q_OBJECT
public:
    explicit IpsTabWid(QWidget *parent = nullptr);
    Ui::ComTableWid* get();

protected slots:
   virtual void timeoutDone();
   virtual void languageChanged();

protected:
    virtual void initWid();
    void updateData(QStringList &lst);

protected:
    sDataPacket *mData;
    QStringList mheader;
};

#endif // IPSTABWID_H
