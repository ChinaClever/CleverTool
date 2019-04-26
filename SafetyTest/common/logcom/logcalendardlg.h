#ifndef LOGCALENDARDLG_H
#define LOGCALENDARDLG_H

#include <QDialog>
#include <QDate>

namespace Ui {
class LogCalendarDlg;
}

class LogCalendarDlg : public QDialog
{
    Q_OBJECT

public:
    explicit LogCalendarDlg(QWidget *parent = 0);
    ~LogCalendarDlg();

    QDate getDate() {return mDate;}

protected slots:
    void selectionChangedSLot();

private slots:
    void on_pushButton_clicked();

private:
    Ui::LogCalendarDlg *ui;
    QDate mDate;
};

#endif // LOGCALENDARDLG_H
