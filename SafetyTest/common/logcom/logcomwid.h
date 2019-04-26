#ifndef LOGCOMWID_H
#define LOGCOMWID_H

#include <QWidget>
#include "logbtnbar.h"
#include "sqlcom/sqltablemodel.h"
#include "log_datewidget.h"

namespace Ui {
class LogComWid;
}

class LogComWid : public QWidget
{
    Q_OBJECT

public:
    explicit LogComWid(QWidget *parent = 0);
    ~LogComWid();

protected:
    void initBtnBar();
    virtual BasicSql *getDb()=0;
    virtual void initTable()=0;
    bool refreshTable(const QString &table);
    void setStretch();
    void setColumnHidden(int column);

protected slots:
    void initFunSLot();
    void doubleSlot(QModelIndex);
    void clearTableSlot();
    void refreshSlot();
    void exportSlot();
    void setGroupBoxTitle(const QString &title);

protected:
    Ui::LogComWid *ui;
    LOG_DateWidget *mDateWidget;

    QString mTableTile;
    QStringList mHeadList;
    LogBtnBar *mBtnBar;
    SqlTableModel *model;
};

#endif // LOGCOMWID_H
