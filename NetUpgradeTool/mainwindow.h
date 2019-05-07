#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ipswid.h"
#include "upgradewid.h"
#include "okstabwid.h"
#include "udptesting.h"
#include "devselectwid.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void translateUi();
    QTranslator *qtrans;

public slots:
    void languageChanged();

protected:
    void changeEvent(QEvent *);

private slots:
    void timeoutDone(void);

signals:
    void languageChangeSig();

private:
    Ui::MainWindow *ui;
    QTimer* timer;
    IpsWid *mIpsWid;
    UpgradeWid *mUpgradeWid;
    IpsTabWid *mIpsTabWid;
    OksTabWid *mOksTabWid;
    ErrsTabWid *mErrsTabWid;
    UdpTesting* mUdpTesting;
    DevSelectWid *mDevSelectWid;
};

#endif // MAINWINDOW_H
