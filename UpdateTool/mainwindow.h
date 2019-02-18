#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QSettings>
#include <QTextStream>
#include "common/inofmanaging.h"
#include "seriorport/myseriorport.h"
#include "senddata.h"
#include "senddataall.h"

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
    void on_pushButton_12_clicked();
    void on_comboBox_port_currentIndexChanged(int index);
    void on_pushButton_port_clicked(); //打开串口
    void on_selectfileBtn_clicked();
    void on_startBtn_clicked();

    void onOSendOkSlot(int value); //传输结束
    void onProgressSlot(QString); //传输进度条

    void onOSendAllOkSlot(int value); //传输结束
    void onProgressAllSlot(QString); //传输进度条
    void onProgressAllSlot2(QString); //传输进度条

    void on_pushButton_clicked();

    void on_baudBox_currentIndexChanged(int index);

    void on_radioButton_clicked();

    void on_SetChannelBtn_clicked();

    void on_SetBaudBtn_clicked();

private:
    void initPortCombox(QStringList &portList);
    int getIndex(QStringList &list ,QString &str);
    void initButtonText();
    void sleep(unsigned int msec);
    bool sendUpdateCmd();
    int responseUpdate();
    bool responseSendFile(int num);
    void sendFile();
    int getPacketNum(int bytes);
    void SetChannelSendarray(QString str);
    bool SetChannelRecvarray(QString str);
    void SetChannelStatus(bool channelOrbaud, bool flag , int steps);
    bool isDirExist(QString fullPath ,QSettings *ini);

private:
    Ui::MainWindow *ui;
    MySeriorport *myPort;
    QString mUpdateFile;
    QString mCurrentPort;

    SendData *sendData;
    SendDataAll *sendDataAll;

    void closeEvent(QCloseEvent *);

    //后加波特率
    int mCbaud;
    bool mIsOpenSerial;
    QSettings* ini;
};

#endif // MAINWINDOW_H
