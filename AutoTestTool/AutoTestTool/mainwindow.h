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
#include <QMessageBox>
#include <QLineEdit>
#include "datadefine.h"
#include "json/mysql.h"


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

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void readUdpData(QByteArray &array,QHostAddress *addr);

    void on_pushButton_continue_clicked();

    void on_pushButton_stop_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_7_clicked();

private:
    Ui::MainWindow *ui;

    bool isConnecting = false;
    QTimer *timer;

    quint8 addr ;


    int mNum = 0;
    int mOutNum ;
    int mPhaseNum;
    int mOutAddr,mPhaseAddr;
    int mConnectingNum;
    bool isStopButton;

    bool isOut,isVol;

    TcpClient *client_1;
    TcpClient *client_2;
    QHostAddress *mAddr;
    data_dev mDeviceData ;  //为当前连接的设备定义一个结构体，保存其设备状态、输出位、相位等基本信息

private:
    void initGatherCmd(data_packet &packet);
    void typeSet();
    void sleep(unsigned int msec);
    void packetToArray(data_packet &packet, QByteArray &array);
    void initJsonDataPacket();
    void initDeviceInfo(QHostAddress *addr);
    void clearDeviceInfo();
    bool checkIsDigit(QString &str);
    bool checkRadioInputIsRight(QLineEdit *lineEdit, int &upNum);
    void sentCmd();

};

extern UdpClient *getUdpClient();

#endif // MAINWINDOW_H
