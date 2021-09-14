#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QByteArray>
#include "serialport/serialportoperate.h"
//#include "showthread.h"
#include <QTimer>

class  returntableData1{
public:
   quint8 addr;            //板地址
   quint8 sign;            //功能码
   quint8 len[2];          //长度
   quint8 bufV[2];          //压
   quint8 bufA[2];          //流
   quint8 bufW[2];          //率

   quint8 bufEnergy[4];     //电能
   quint8 bufVMin[2];       //压
   quint8 bufVMax[2];       //压
   quint8 bufAMin[2];       //流
   quint8 bufAMax[2];       //流

   quint8 bufTem; //温度
   quint8 bufHum; //湿度
   quint8 bufBaud; //波特率
   quint8 bufUsefulW[2];       //有用功
   quint8 bufPowerFa;       //功率因素
};

class returntableData3{
public:
   quint8 addr;            //板地址
   quint8 sign;            //功能码
   quint8 len[2];          //长度
   quint8 bufV[3][2];      //压
   quint8 bufA[3][2];      //流
   quint8 bufW[3][2];      //率

   quint8 bufEnergy[3][4];     //电能
   quint8 bufVMin[3][2];       //压
   quint8 bufVMax[3][2];       //压
   quint8 bufAMin[3][2];       //流
   quint8 bufAMax[3][2];       //流

   quint8 bufTem; //温度
   quint8 bufHum; //湿度
   quint8 bufTemMax; //温度
   quint8 bufTemMin; //温度
   quint8 bufHumMax; //湿度
   quint8 bufHumMin; //湿度

   quint8 bufBaud; //波特率
   quint8 bufUsefulW[3][2];       //有用功
   quint8 bufPowerFa[3];       //功率因素
};


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

protected:

public:
    SerialportOperate *port;
    QString mCurrentPortName;
    int mAddr;

    void sleep(unsigned int msec);
protected:
    void initComboxDa(int flag);
    void updateStateAndButton();

    void initText();
    void initWid();

    void sendCmd(int flag1,int flag2);//获取数据
    void sendCmd(int flag); //flag用于判断是1还是3
    void getCmd(QByteArray &data); //获取数据

    int returnToPacket(QByteArray &array); //解析数据
    void upDataShow(int flag);
private slots:
    //-------------------------[1]-----------------------------
    void on_pushButton_2_clicked();
    void on_comboBox_currentIndexChanged(const QString &arg1);

    //-------------------------[2]-----------------------------
    void onReadAnswer(); //只针对非表格数据更新
    void on_pushButton_B1_clicked();
    void on_pushButton_B2_clicked();
    void on_pushButton_B3_clicked();
    void on_pushButton_BAll_clicked();
    void on_pushButton_clicked();
    void onTimer();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_comboBox_3_currentIndexChanged(int index);

    void onClosePort();
    void onClearSlot();


private:
    Ui::Widget *ui;
   // ShowThread *showThread;
    QTimer *timer;

    returntableData1 returnData1;
    returntableData3 returnData3;
};

#endif // WIDGET_H
