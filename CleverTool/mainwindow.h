#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QDebug>
#include<QTimer>
#include<QMessageBox>
#include"serialportoperate.h"
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

private slots:
    void on_pushButton_2_clicked();

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_pushButton_clicked();
    void timeoutDone();

private:
    void initComboxData(int flag);
    void updateStateAndButton();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
