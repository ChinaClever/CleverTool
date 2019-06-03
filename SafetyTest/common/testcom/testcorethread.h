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
    //void updateData(int i);
    void conditionExec(bool s);
    void updateProgress(bool status, QString &str);
    bool appendResult(sTestDataItem &item);

    void delayItem(sTestDataItem &item, int s);
    bool testReady();
    bool testResis(QString &recv);
    bool testInsulation(QString &recv);
    bool testWithstand(QString &recv);
    bool testEnd();

private:
    int mItemId;
    int mTestStep;
    int mStep;
    sTestConfigItem  *mItem;
    TestTransThread *mTrans;
    bool  getGndValue(QString &str);
    bool  getIRValue(QString &str);
    bool  getDCWValue(QString &str);
};

#endif // TESTCORETHREAD_H
