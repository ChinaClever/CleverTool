#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<string.h>
#include<QDate>
#include<QMessageBox>
#include<QTime>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

      void is_register(); //判断是否注册


private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

   void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_lineEdit_2_textChanged(const QString &arg1);

    void on_pushButton_5_clicked();

private:
    Ui::MainWindow *ui;

    void sleep(unsigned int msec);
    void getdate();//获取日期
    void getenum();//获取数量
    bool is_number(QString src);//判断是否为纯数字
    //void setClipboard();
    bool is_right(QString str);
    void register_info();
    bool com_info();
    void logout();//注销


    QDate date;
    QString str;  //机器码
    QMessageBox msg;
    QTime t;
    bool is_late;
    bool is_num;
   bool is_free;  //判断到否为免费激活
    QPalette pal;
    int num;
    QString str5; //激活码


};

#endif // MAINWINDOW_H
