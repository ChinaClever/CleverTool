#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "snmpthread.h"
#include "addsetcmddlg.h"
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
    bool startFun();
    bool checkInput();
    void updateData();
    void openFile();
    void writeLog(QString &str);

private slots:
    void timeoutDone();
    void timeoutClearDone();
    void requestSlot(const QString& str);
    void responseSlot(const QtSnmpDataList& values);
    void reqErrSlot();

    void on_startBtn_clicked();
    void on_overBtn_clicked();
    void on_clearBtn_clicked();

    void on_addSetCmdBtn_clicked();

private:
    Ui::MainWindow *ui;
    SnmpThread *mSnmp;
    QTimer *m_timer;
    QTimer *m_cleartimer;
    QFile *mFile;
    bool isSave;
    QString logs;
    QMutex *mMutex;
    AddSetCmdDlg *mAddSetCmdDlg;
    QList<sSnmpSetCmd> mSetCmdList;
    int mSendTimer;
};

#endif // MAINWINDOW_H
