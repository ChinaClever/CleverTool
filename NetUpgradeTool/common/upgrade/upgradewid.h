#ifndef TFTPWIDGET_H
#define TFTPWIDGET_H

#include "exports/exportdlg.h"
#include "tcpupgrade.h"

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

public slots:
    void languageChanged();

private:
    Ui::UpgradeWid *ui;
    sDataPacket *mData;

    QTimer *timer;
    ExportDlg *mExportDlg;
    TcpUpgrade *mTcpThread;
    TftpUpgrade *mTftpThread;
    UpgradeThread *mUpgradeThread;
};

#endif // TFTPWIDGET_H
