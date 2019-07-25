#ifndef EXCEL_SAVETHREAD_H
#define EXCEL_SAVETHREAD_H

#include "qexcel.h"
#include <QtCore>

class Excel_SaveThread : public QThread
{
    Q_OBJECT
public:
    explicit Excel_SaveThread(QObject *parent = nullptr);
    ~Excel_SaveThread();

    void saveData(const QString &fileName, QStringList list);
    int getProgress();

protected:
    virtual void run();
    void workDown();
    bool init();
    void writeFile(QList<QStringList> &list);

protected:
    QString mFileName;
    QList<QStringList> mList;
    int mSize, mAllSize;
};

#endif // EXCEL_SAVETHREAD_H
