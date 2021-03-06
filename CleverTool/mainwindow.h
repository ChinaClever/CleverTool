﻿#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QDebug>
#include<QTimer>
#include<QMessageBox>
#include"serialportoperate.h"
#include<stdio.h>
#include<QHBoxLayout>
#include<QMovie>
#include<QFile>
//#pragma execution_character_set("utf-8")

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
    quint8 sendData[32]; //采集信息时发送命令
    bool is_gather;  //用于判断采集命令或者是表格数据
    bool is_read;    //用于判断发送命令是否已有返回数据
    bool is_once;  //采集的循环轮流发送
    bool is_ele;   //采集电流电压功率还是电能
    QTimer *mTimer;  //表格刷新
    QTimer *timer;  //主程序读取数据及串口刷新
    QMovie* movie;
    int switchData; //开关状态
    int mCurrentButtonRow; //当前点击开关按钮所在行
    int mCurrentClearButtonRow; //当前点击清零按钮所在行
    int switchNum;
    bool switchFlag;

    quint8 onbuffer[8]; /*= {1,0,0,0,0,0,0,0}; */
    quint8 offbuffer[8]; /*= {0,1,1,1,1,1,1,1}; */

    quint8 init[8]; //初始化标志 -- My+
    int    pass;    //  -- My+
    int cSUM; //控制按钮轮询

    QTimer *timer2;
    
private slots:
    void on_pushButton_2_clicked();    
    void on_comboBox_currentTextChanged(const QString &arg1);    
    void timeoutDone();   
    void on_comboBox_2_currentIndexChanged(int index);    
    void on_pushButton_3_clicked();    //校准1
    void on_pushButton_4_clicked();
    void button_clicked();
    void button_clear_clicked();
    void on_pushButton_5_clicked();   //开始采集
    void delayTimeDone();
    void delaySetTrue();
    void collectLoopDone();   
    void on_pushButton_6_clicked();  //停止采集
    void on_pushButton_7_clicked();  //重新校准
    
    void on_pushButton_8_clicked(); //切换开关

    void on_comboBox_3_currentIndexChanged(int index);

    void on_pushButton_clicked();

    void onInitIni();


    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();
    void ontimeout();

    void on_pushButton12_clicked();

    void on_DelayZeroBtn_clicked();

protected:
    void initComboxDa(int flag);
    void updateStateAndButton();

    void sleep(unsigned int msec);

    
    
    void initText();
    
    void initGroupboxOne(int index);
    void updateGroupboxOne(quint8 *data, int flag, int length, int len);
    void initGroupboxTwo(int index);
    void updateGroupboxTwo(quint8 *data, int flag, int length, int len);
    void initGroupboxThree(int index);
    void updateGroupboxThree(quint8 *data, int len);
    void initData();
    void sendCmd(int flag); //flag用于判断是增益还是相位校准
    void readAnswer(); //只针对非表格数据更新
    void readTableAnswer(); //表格数据读取
    quint8 getXorNumber(quint8 *data, int len);
    //    quint8 getXorNumber(quint8 *data, int len, int flag);
    bool responseIsRight(quint8 *data, int flag, int length);
    bool cmdIsRight(int flag);
    void dataTopacket(quint8 *Data);
    void initTablewidget();
    void initTablewidgetOfButton();
    void initTablewidgetNoButton();
    //    void initCell();
    void sentDataToPacket(quint8 &addr, quint8 &flag, quint8 *onbit, quint8 *offbit, bool symbol);
    void sentDataToPacket(quint8 &addr);//采集指令打包
    quint8 bitControl(quint8 *buffer);
    void sendGatherCmd();
    void sendTableCmd();
    void sendGetEleCmd();
    void sendControlCmd(quint8 *onBuf, quint8 *offBuf);
    void returnToPacket(QByteArray &array);
    void updataTableData();

    void setAll(); //设置总和数据
    
    void setOnOffState(int row,int column);
    void setCurretnt(int row,int column);

    void setCurretVolate(int row,int column);
    void setCurretEle(int row, int column);
    void setCurretPower(int row,int column);

    void setFirstVolate();
    void setSecondVolate();
    void setFirstPower();
    void setSecondPower();
    
    void setSwitch(int row, int onOroff);
    void setClearEle(int row);
    
    //    void updateResponse(QString &str);
    
    QString quintToStr(quint8 *buf, int len);
    
    void clearTalbeText();
    
    
    
    
    
private:
    Ui::MainWindow *ui;
};



#endif // MAINWINDOW_H
