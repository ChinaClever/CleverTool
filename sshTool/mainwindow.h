#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include "sshthread.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    SshThread* m_sshThread;

private slots:
    void updateTextSlot(QString str);
    void updateLableSlot(int Ok , int Err);
    void on_StopBtn_clicked();
    void on_StartBtn_clicked();

private:
    Ui::MainWindow *ui;
    void sleep(unsigned int msec);

};

#endif // MAINWINDOW_H
