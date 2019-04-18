#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "snmpthread.h"

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
    void writeLog(const QString &str);

private slots:
    void timeoutDone();
    void requestSlot(const QString& str);
    void responseSlot(const QtSnmpDataList& values);
    void reqErrSlot();

    void on_startBtn_clicked();
    void on_overBtn_clicked();
    void on_clearBtn_clicked();

private:
    Ui::MainWindow *ui;
    SnmpThread *mSnmp;
    QTimer *m_timer;
    QFile *mFile;
    bool isSave;
};

#endif // MAINWINDOW_H
