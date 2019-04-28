#ifndef TESTDATASAVE_H
#define TESTDATASAVE_H

#include "excel_savethread.h"
#include "testitemtablewid.h"

class TestDataSave : public QObject
{
    Q_OBJECT
public:
    explicit TestDataSave(QObject *parent = nullptr);

    void saveTestData();

protected:
    int bulidHead(sSnItem *item);
    int bulidProgressLog(int id, sTestProgress &arg);
    void bulidTestData(QList<QStringList> &list);
    void saveDb();

private:
    QString bulidFileName(sSnItem *item);
    QString bulidPath(sSnItem *item);

private:
    Excel_SaveThread *mSaveThread;
    sTestConfigItem *mItem;
    QString mFileName;
    QList<QStringList> mList;
};

#endif // TESTDATASAVE_H
