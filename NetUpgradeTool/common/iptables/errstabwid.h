#ifndef ERRSTABWID_H
#define ERRSTABWID_H

#include "ipstabwid.h"

class ErrsTabWid : public IpsTabWid
{
    Q_OBJECT
public:
    explicit ErrsTabWid(QWidget *parent = nullptr);

protected slots:
   virtual void timeoutDone();

protected:
    virtual void initWid();
    virtual void languageChanged();
    QStringList mheader;
};

#endif // ERRSTABWID_H
