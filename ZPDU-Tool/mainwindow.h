#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QDebug>
#include<QTimer>
#include<QMessageBox>
#include"serialportoperate.h"
#include<stdio.h>
#include <QSettings>
#define bitnumber 14
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
    SerialportOperate *port;
    QString mCurrentPortName;
    int mAddr;

    QString mGainStr; //增益校准返回
    int mflag;//判断增益及相位校准的标志位
    QMultiMap<int,QPushButton *> mAddressMap;  //一键多值
    QMap<int,QPushButton *> mAddrMap;  //一键一值
    quint8 sendData[80]; //采集信息时发送命令
    bool is_gather;  //用于判断采集命令或者是表格数据
    bool is_read;    //用于判断发送命令是否已有返回数据
    //bool is_once;  //采集的循环轮流发送
    //bool switchFlag;
    QTimer *mTimer;  //表格刷新
    QTimer *timer;  //主程序读取数据及串口刷新
    int switchData; //开关状态
    int mCurrentButtonRow; //当前点击按钮所在行
    int mCurrentClearButtonRow; //当前点击清零按钮所在行
    int switchNum;

    quint8 onbuffer[bitnumber]; /*= {1,0,0,0,0,0,0,0}; */
    quint8 offbuffer[bitnumber]; /*= {0,1,1,1,1,1,1,1}; */

    quint8 init[bitnumber]; //初始化标志 -- My+
    int    pass;    //  -- My+
    int cSUM; //控制按钮轮询

    QTimer *timer2;
    QSettings* ini;
protected:
    void initComboxDa(int flag);
    void updateStateAndButton();

    void sleep(unsigned int msec);

    //void initText();
    void initGroupboxOne(int index);
    void updateGroupboxOne(quint8 *data, int flag, int length, int len);


    //void initGroupboxThree(int index);
    void updateGroupboxThree(quint8 *data, int len);
    void initData();
    void sendCmd(int flag); //flag用于判断是增益还是相位校准
    void readAnswer(); //只针对非表格数据更新
    void readTableAnswer(); //表格数据读取
    quint8 getXorNumber(quint8 *data, int len);

    bool responseIsRight(quint8 *data, int flag, int length);
    bool cmdIsRight(int flag);
    void dataTopacket(quint8 *Data);
    void initTablewidget();
    void initTablewidgetOfButton();
    void initTablewidgetNoButton();

    void sentDataToPacket(quint8 &addr);
    //void sentDataToPacket(quint8 &addr);//采集指令打包
    //quint8 bitControl(quint8 *buffer);
    void sendGatherCmd();
    void sendTableCmd();
    void sendControlCmd(quint8 *onBuf, quint8 *offBuf);
    void returnToPacket(QByteArray &array);
    void updataTableData();

    void setAll(); //设置总和数据

    void setOnOffState(int row,int column);
    void setCurretnt(int row,int column);

    void setCurretVolate(int row,int column);
    void setCurretPower(int row,int column);
    void setCurretEle(int row, int column);
    void setPF(int row, int column);

    void setSwitch(int row, int onOroff , bool flag);
    void setClearEle(int row);

    QString quintToStr(quint8 *buf, int len);

    void clearTalbeText();
    void onInitIni();
    bool isDirExist(QString fullPath,QSettings * ini);
private slots:
    void button_clicked();
    void button_clear_clicked();
    void on_comBox_State_currentIndexChanged(int index);
    void on_comBox_Addr_currentIndexChanged(int index);
    void on_RecorrectBtn_clicked();
    void on_comBox_currentTextChanged(const QString &);
    void on_workBtn_clicked();
    void on_GaincorrectBtn_clicked();
    void on_StartBtn_clicked();
    void on_StopBtn_clicked();
    //void on_ChangeSWBtn_clicked();
    void on_SaveBtn_clicked();
    void on_AllOpenBtn_clicked();
    void on_AutoChangeBtn_clicked();
    void on_UpBtn_clicked();
    void on_DownBtn_clicked();

    void timeoutDone();

    void delaySetTrue();
    void ontimeout();
    void collectLoopDone();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
