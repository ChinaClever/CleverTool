#ifndef EXPORTDLG_H
#define EXPORTDLG_H

#include <QDialog>
#include "excel_savethread.h"

namespace Ui {
class ExportDlg;
}

class ExportDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ExportDlg(QWidget *parent = 0);
    ~ExportDlg();

protected:
    bool checkInput();
    void setExcel();
    void exportDone();

private slots:
    void timeoutDone();
    void on_pushButton_clicked();
    void on_exportBtn_clicked();

    void on_quitBtn_clicked();

private:
    Ui::ExportDlg *ui;
     QTimer *timer;
    Excel_SaveThread *mExportThread;
};

#endif // EXPORTDLG_H
