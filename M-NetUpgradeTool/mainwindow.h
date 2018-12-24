#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "bulidip/ipswid.h"
#include "tftpwidget.h"
#include "tables/okstabwid.h"
#include "udp/udptesting.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void timeoutDone(void);

private:
    Ui::MainWindow *ui;
    IpsWid *mIpsWid;
    TftpWidget *mTftpWid;

    IpsTabWid *mIpsTabWid;
    OksTabWid *mOksTabWid;
    ErrsTabWid *mErrsTabWid;
    QTimer* timer;
    UdpTesting* mUdpTesting;
};

#endif // MAINWINDOW_H
