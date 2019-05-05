#ifndef TESTDATATABLEWID_H
#define TESTDATATABLEWID_H
#include "testdatasave.h"

class TestDataTableWid : public ComTableWid
{
    Q_OBJECT
public:
    explicit TestDataTableWid(QWidget *parent = nullptr);

public slots:
    void startSLot();
    void timeoutDone();

protected:
    void initWid();
    void appendItem(const sTestDataItem &item);

private:
     QTimer *timer;
     sTestConfigItem  *mItem;
};

#endif // TESTDATATABLEWID_H
