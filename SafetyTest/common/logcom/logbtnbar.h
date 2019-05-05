#ifndef LOGBTNBAR_H
#define LOGBTNBAR_H

#include <QWidget>

namespace Ui {
class LogBtnBar;
}

class LogBtnBar : public QWidget
{
    Q_OBJECT
public:
    explicit LogBtnBar(QWidget *parent = 0);
    ~LogBtnBar();

    void getDate(QDate &start, QDate &endDate);

signals:
    void refreshSig();
    void querySig(QString);
    void clearSig();
    void exportSig();

private slots:
    void on_queryBtn_clicked();
    void on_clearBtn_clicked();
    void on_startDateBtn_clicked();
    void on_endDateBtn_clicked();

    void on_exportBtn_clicked();

private:
    Ui::LogBtnBar *ui;
};

#endif // LOGBTNBAR_H
