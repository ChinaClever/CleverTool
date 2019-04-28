#ifndef TESTITEMS_H
#define TESTITEMS_H

#include "comtablewid.h"
#include "testserialnumdlg.h"

struct sTestItem
{
    sTestItem(){isExecute = true;}

    int id;
    bool isExecute; // 是否执行
    QString item; // 检查项目
    QString subItem; // 测试项目描述
    QString param; // 参数
};

class TestItems : public QObject
{
    Q_OBJECT
public:
    explicit TestItems(QObject *parent = nullptr);

    int bulidItems(QList<sTestItem> &items);

protected:
    int getReady(QList<sTestItem> &items);
    int getResis(QList<sTestItem> &items);
    int getInsulation(QList<sTestItem> &items);
    int getWithstand(QList<sTestItem> &items);
};

#endif // TESTITEMS_H
