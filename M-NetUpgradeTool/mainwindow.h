#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "bulidip/ipswid.h"
#include "tftpwidget.h"
#include "tables/okstabwid.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    IpsWid *mIpsWid;
    TftpWidget *mTftpWid;

    IpsTabWid *mIpsTabWid;
    OksTabWid *mOksTabWid;
    ErrsTabWid *mErrsTabWid;
};

#endif // MAINWINDOW_H
