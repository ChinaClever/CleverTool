#ifndef TESTCORETHREAD_H
#define TESTCORETHREAD_H

#include "testtransthread.h"


class TestCoreThread : public QThread
{
    Q_OBJECT
public:
    explicit TestCoreThread(QObject *parent = nullptr);
    ~TestCoreThread();

    void startThread();

signals:
    void overSig();

protected:
    void run();
    void stopThread();
    void updateData(int i);
    void conditionExec(bool s);
    void updateProgress(bool status, QString &str);
    bool appendResult(sTestDataItem &item);

    void delayItem(sTestDataItem &item, int s);
    bool testReady();

private:
    int mItemId;
    sTestConfigItem  *mItem;
    TestTransThread *mTrans;
};

#endif // TESTCORETHREAD_H
