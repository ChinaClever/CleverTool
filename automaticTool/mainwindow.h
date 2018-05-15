#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "rtuthread.h"

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

    void setOnOffState(int row, int column, ReturnTableData &data);
    bool setItem(int row, int column, quint8 valueH, quint8 valueD, int rate = 10);
    bool setAll(ReturnTableData &data);
    void sleep(unsigned int msec);

private:
    void init();
    void initIni(); //初始化配置文件
    void saveIni(); //保存配置

private slots:
    void on_cBoxC1_currentIndexChanged(int index); //addr切换
    void on_cBoxC2_currentIndexChanged(int index); //校准模式切换
    void on_startOrStop_clicked(); //开启线程
    void on_resBtu_clicked();
    void on_pushButton_clicked();
    void onChangedIni();

    void on_readBox_clicked(bool checked);

public slots:
    void onGetShowData(int value, ReturnTableData *data); //显示用参数
    void onGetStation(QString str);
    void onEnd();
    void onClear();
    void onUpdataTime(QString str);

private:
    SerialPortWid *mSerialPortPlateWid;
    SerialPortWid *mSerialPortSourceWid;
        RtuThread *mRtuThread;

        int mSum, OKSum, NGSum;
        bool mCurOk; //当前结果
        int mPassSum; //通过次数
};

#endif // MAINWINDOW_H
