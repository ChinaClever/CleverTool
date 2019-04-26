#ifndef LOG_DATEWIDGET_H
#define LOG_DATEWIDGET_H

#include <QWidget>
#include "log_datehash.h"
#include <QTreeWidgetItem>

namespace Ui {
class LOG_DateWidget;
}

class LOG_DateWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LOG_DateWidget(QWidget *parent = 0);
    ~LOG_DateWidget();

    void setTable(const QString &name);
    void updateWidget();
    void seekDate(const QString &date);
signals:
    void selectedSig(QString);

protected:
    void initWidget(void);
    void setHeaderLabels(QStringList &headers);

    void yearShow(void);
    int monthShow(QString &year, QTreeWidgetItem *item);
    int dayShow(QString &month, QTreeWidgetItem *item);
    bool isDate(const QString &date);

protected slots:
    bool selectedItemSlot(QTreeWidgetItem *item,int column);
    void clickedSLot(QModelIndex index);

public slots:
    void updateSlot();

private:
    Ui::LOG_DateWidget *ui;
    LOG_DateHash *mDateHash;
    QString mTableName;
};

#endif // LOG_DATEWIDGET_H
