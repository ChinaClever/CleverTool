#ifndef COMTABLEWID_H
#define COMTABLEWID_H

#include <QWidget>
#include "common.h"

namespace Ui {
class ComTableWid;
}

class ComTableWid : public QWidget
{
    Q_OBJECT

public:
    explicit ComTableWid(QWidget *parent = 0);
    ~ComTableWid();

protected:
    void initTableWid(QStringList &header, int line, const QString &title);
    void setTableItem(int id, int column, const QString &str);
    void checkTableRow(int line);

    void clearRow(int row);
    void clearTable();
    void setItemColor(int id, int column, int alarm);    

private:
    void initTableWidget(QStringList &header);
    void addItemContent(int row, int column, const QString &content);
    void addRowContent(QStringList &list);
    void addInitRow();

private:
    Ui::ComTableWid *ui;
};

#endif // COMTABLEWID_H
