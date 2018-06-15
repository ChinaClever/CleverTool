#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QPalette>
#include <QGridLayout>
#include <QBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QTableWidget>
#include <QCheckBox>
#include "udp/myudpthread.h"
#include "tcp/tcpthread.h"
#include "udp/globalreceivethread.h"
#include <ctype.h>
#include <QRadioButton>
#include <QButtonGroup>
#include <QHostInfo>
#include "thresholdsetting/settingdialog.h"
//#include "udp/datasent.h"



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
    void on_pushButton_clicked();
    void timeoutdone();

    void on_comboBox_currentIndexChanged(int index);
    void setThreshold(QModelIndex index);  //双击表格弹出阈值设置界面
    void setThresholdAdd(QModelIndex index);  //双击表格弹出阈值设置界面
    void setThresholdAddTem(QModelIndex index);  //双击表格弹出阈值设置界面
    void setThresholdAddHum(QModelIndex index);  //双击表格弹出阈值设置界面

    void radioButtonEntaanceOneClicked(int id);
    void radioButtonEntaanceTwoClicked(int id);
    void radioButtonSmokeClicked(int id);
    void radioButtonWaterClicked(int id);

    void initData(QString str);
private:
    Ui::MainWindow *ui;
    QTableWidget *phasewidget_1; //输入相
    QTableWidget *lefttablewidget;
    QTableWidget *righttablewidget;

    QTableWidget *loopwidget;

    QTableWidget *outwidget;
    QTableWidget *temwidget;
    QTableWidget *humwidget;

    DataSouce *souce;
    parameterData *souceData;
    HeartBeat *heartbeat;


private :
    void initThread();
//    void initData();

    void initWidget();
    void initPhaseWidget();
    void initLoopWidget();
    void initOutWidget();
    void initSensorWidget();

    void updateData();
    void updatePhaseData();  //更新输入相数据

    void updatePhaseOneData();
    void setPhaseName(int row, int count, QTableWidget *widget);
    void setPhaseVol(int row, int count);
    void setPhaseCur(int row ,int count);
    void setPhasePower(int row ,int count);
    void setPhaseEngry(int row ,int count);
    void setPhasePowerfactor(int row ,int count);

    void updatePhaseTwoData();
    void setPhaseVolMax(int row ,int count);
    void setPhaseVolMin(int row ,int count);

    void updatePhaseThreeData();
    void setPhaseCurMax(int row ,int count);
    void setPhaseCurMin(int row ,int count);
    void setPhaseCurCirMax(int row ,int count);
    void setPhaseCurCirMin(int row ,int count);

    void updateLoopData();//更新回路数据
    void setLoopName(int row ,int count);
    void setLoopSwitchStatus(int row ,int count);
    void setLoopCur(int row ,int count);
    void setLoopPower(int row ,int count);
    void setLoopEnergy(int row ,int count);
    void setLoopPowerFactor(int row ,int count);
    void setLoopMin(int row ,int count);
    void setLoopCirMin(int row ,int count);
    void setLoopCirMax(int row ,int count);
    void setLoopMax(int row ,int count);

    void updateOutData();
    void setOutName(int row ,int count);
    void setOutSwitchStatus(int row ,int count);
    void setOutCur(int row ,int count);
    void setOutPower(int row ,int count);
    void setOutEnergy(int row ,int count);
    void setOutPowerFactor(int row ,int count);
    void setOutMin(int row ,int count);
    void setOutCirMin(int row ,int count);
    void setOutCirMax(int row ,int count);
    void setOutMax(int row ,int count);

    void updateSensorData(); //刷新传感器界面数据
    void updateTemData();
    void setTemName(int row ,int count);
    void setTemStatus(int row ,int count);
    void setTemMin(int row ,int count);
    void setTemMax(int row ,int count);

    void updateHumData();
    void setHumName(int row ,int count);
    void setHumStatus(int row ,int count);
    void setHumMin(int row ,int count);
    void setHumMax(int row ,int count);


    void initTableWiget(QTableWidget *widget, int rowSize);

    void initdevSeetings();
    bool isDigit(QString str);
    void getLocalIp();
};

extern devSetings *get_devsetings();

#endif // MAINWINDOW_H
