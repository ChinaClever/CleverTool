#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "logwid.h"
#include "testcorethread.h"
#include "tgswid.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void initWid();
    void startTest();
    void overTest();

private slots:
    void initFunSLot();
    void overSlot();
    void updateWidSlot(int id);
    void on_logBtn_clicked();
    void on_itemBtn_clicked();
    void on_dataBtn_clicked();
    void on_testBtn_clicked();

    void on_tgBtn_clicked();

private:
    Ui::MainWindow *ui;
    sTestConfigItem  *mItem;
    TestDataSave *mDataSave;

    TgsWid *mTgsWid;
    LogWid *mLogWid;
    TestResultWid *mResultWid;
    TestDataTableWid *mDataWid;
    TestItemTableWid *mItemWid;
    TestCoreThread *mCoreThread;
    TestSerialNumDlg *mSerialNumDlg;
};

#endif // MAINWINDOW_H
