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

    void saveData(const QString &fileName, QList<QStringList> &list);
    int getProgress();
protected:
    void run();
    bool init();
    void writeFile(QList<QStringList> &list);

private:
    QString mFileName;
    QList<QStringList> mList;
    int mPro;
};

#endif // EXCEL_SAVETHREAD_H
