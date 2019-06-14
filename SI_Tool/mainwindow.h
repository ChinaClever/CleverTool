#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "rtuthread.h"
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    Ui::MainWindow *ui;

    void sleep(unsigned int msec);


private:
    void init();
    bool setItem(int row, int column, uint value, int rate);
    bool setBreakItem(int row, int column , uchar value);



private slots:
    void onInit();
    void on_startOrStop_clicked(); //开启线程
    void on_readBox_clicked(bool checked);
    void onDebugEidt(QString str);
    void onEnd();

    void on_dcBox_currentIndexChanged(int index);

    void on_tip_clicked();

    void on_clearBtn_clicked(); //电能清零

    void on_ChangeLineBtn_clicked();

    void on_ChangeWifiBtn_clicked();

public slots:
    void onGetShowData(int value, Rtu_recv *data); //显示用参数


private:
    SerialPortWid *mSerialPortPlateWid;
    SerialPortWid *mSerialPortSourceWid;
        RtuThread *mRtuThread;
};

#endif // MAINWINDOW_H
