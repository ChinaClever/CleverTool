void on_pushButton_clicked();
void on_pushButton_clicked();
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QByteArray>
#include <QTimer>
#include <QTime>
#include <json/json.h>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QLineEdit>
#include "mytext.h"
#include "seriorport/myseriorport.h"
#include <ctype.h>
#include <QDateTime>

#define COUNT 3  //修改接收时间，超过时间，将不再读取数据


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

    void on_pushButton_9_clicked();  //详细信息

    void on_pushButton_12_clicked();

    void on_comboBox_port_currentIndexChanged(int index);

    void on_pushButton_port_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_3_clicked(); //一键测试

    void on_doadmin_clicked();

    void on_doCorrect_clicked();

    void on_saverIni_clicked();
    void onInitIni();

    void on_comboBox_portB_currentIndexChanged(int index);
    void on_pushButton_portB_clicked();

    void on_pushButton_12B_clicked();
    void on_setSource_clicked(); //发送给标准源
    QByteArray onTidyData(ComCmd type, int data); //整理数据

    void on_comboBox_2_currentIndexChanged(int index);

    void on_pushButton_2_clicked();
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    MySeriorport *myPort;
    basicinfo mInfo; /*主界面获取信息用于发送命令*/
private:
    void initPortCombox(QStringList &portList);
    void initPortCombox2(QStringList &portList); //源用
    int getIndex(QStringList &list ,QString &str);
    void initButtonText();
    void initButtonText2();
    void initBasicInfo(int value);
    int changeStrToInt();
    bool buttonClicked( int value);
    void updateUnitInfo(receiveData *packet);
    void updateTextEdit(receiveData *packet);
    bool checkPredictValue();
    bool checkInputPhase();
    void sleep(unsigned int msec);
    bool checkDigit(QString str);
    void initRelatedAta(receiveData *data);
    void checkResult(bool *buf, receiveData *data);
    void strcpy_buf(quint16 *_Dest, const quint16 *_Source, int len);
    void initBackground();

    int mCaddr;

};

extern QByteArray packet_to_array(basicinfo info );
extern bool net_to_local(basicinfo info,QByteArray recvArray,receiveData *packet);


#endif // MAINWINDOW_H
