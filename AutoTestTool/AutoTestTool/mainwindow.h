#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <udp/udpclient.h>
#include <tcp/tcpclient.h>
#include <QByteArray>
#include <tcp/cmdtonetpacket.h>
#include <QTimer>
#include <QTime>
#include <json/json.h>
#include <QHBoxLayout>
#include "datadefine.h"


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
    void on_pushButton_3_clicked();
    void timeoutDone();
    void timeoutDoneOfRefreshIp();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void readUdpData(QByteArray &array);

    void on_pushButton_continue_clicked();

    void on_pushButton_stop_clicked();

    void on_radioButton_3_clicked();

    void on_radioButton_2_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

private:
    Ui::MainWindow *ui;

//    TcpClient *client;

    bool isConnecting = false;
    QTimer *timer;

    quint8 addr ;


    int mNum = 0;
    int mOutNum ;
    int mPhaseNum;
    int mAddr;
    int mConnectingNum;
    bool isStopButton;

    bool isOut,isVol;
    TcpClient *client_1;
    TcpClient *client_2;
    data_dev mDeviceData ;  //为当前连接的设备定义一个结构体，保存其设备状态、输出位、相位等基本信息
    //    data_json mJsonPacket;

private:
    void initGatherCmd(data_packet &packet);
    void typeSet();
    void sleep(unsigned int msec);
    void packetToArray(data_packet &packet, QByteArray &array);
    void initJsonDataPacket();
    void initConnectingIp();
    void initDeviceInfo();

};

//data_json *getDataJsonPacket();
extern UdpClient *getUdpClient();

#endif // MAINWINDOW_H
