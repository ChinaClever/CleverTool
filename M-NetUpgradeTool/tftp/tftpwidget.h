#ifndef TFTPWIDGET_H
#define TFTPWIDGET_H

#include "export/exportdlg.h"
#include "tftpthread.h"

namespace Ui {
class TftpWidget;
}

class TftpWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TftpWidget(QWidget *parent = 0);
    ~TftpWidget();

protected:
    bool checkFile();

private slots:
    void timeoutDone();
    void on_openBtn_clicked();
    void on_updateBtn_clicked();

    void on_exportBtn_clicked();

private:
    Ui::TftpWidget *ui;
    sDataPacket *mData;

    QTimer *timer;
    TftpThread *mTftpThread;
    ExportDlg *mExportDlg;
};

#endif // TFTPWIDGET_H
