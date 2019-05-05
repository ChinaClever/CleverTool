#ifndef LOG_EXPORTDLG_H
#define LOG_EXPORTDLG_H

#include <QDialog>
#include "excel_dblogthread.h"

namespace Ui {
class LOG_ExportDlg;
}

class LOG_ExportDlg : public QDialog
{
    Q_OBJECT

public:
    explicit LOG_ExportDlg(QWidget *parent = 0);
    ~LOG_ExportDlg();

    void set(QString &, QStringList &, QString &);
    void setDate(QDate &start, QDate &end);

protected:
    bool checkInput();
    void setExcel();
    void exportDone();

    virtual int getProgress(){return mExportThread->getProgress();}
    virtual void setEx(sExcel_DbItem &ex) {mExportThread->set(ex);}

protected slots:
    void timeoutDone();
    void on_pushButton_clicked();

    void on_startBtn_clicked();

    void on_endBtn_clicked();

    void on_exportBtn_clicked();

protected:
    Ui::LOG_ExportDlg *ui;

    QString mTableName;
    QStringList mHeadList;
    QString mTableTile;
    QTimer *timer;

private:
    Excel_DbLogThread *mExportThread;
};

#endif // LOG_EXPORTDLG_H
