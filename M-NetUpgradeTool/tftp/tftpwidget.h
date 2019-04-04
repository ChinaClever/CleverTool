﻿#ifndef TFTPWIDGET_H
#define TFTPWIDGET_H

#include "export/exportdlg.h"
#include "tftpthread.h"

namespace Ui {
class UpgradeWid;
}

class UpgradeWid : public QWidget
{
    Q_OBJECT

public:
    explicit UpgradeWid(QWidget *parent = 0);
    ~UpgradeWid();

protected:
    bool checkFile();

private slots:
    void timeoutDone();
    void on_openBtn_clicked();
    void on_updateBtn_clicked();
    void on_exportBtn_clicked();

    void on_breakBtn_clicked();

private:
    Ui::UpgradeWid *ui;
    sDataPacket *mData;

    QTimer *timer;
    TftpThread *mTftpThread;
    ExportDlg *mExportDlg;
};

#endif // TFTPWIDGET_H
