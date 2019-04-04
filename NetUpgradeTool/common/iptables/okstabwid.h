#ifndef OKSTABWID_H
#define OKSTABWID_H

#include "errstabwid.h"

class OksTabWid : public ErrsTabWid
{
    Q_OBJECT
public:
    explicit OksTabWid(QWidget *parent = nullptr);

protected slots:
   virtual void timeoutDone();

protected:
    virtual void initWid();
};

#endif // OKSTABWID_H
