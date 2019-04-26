#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "logwid.h"

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
    void initWid();

private slots:
    void initFunSLot();
    void on_logBtn_clicked();

private:
    Ui::MainWindow *ui;

    LogWid *mLogWid;
};

#endif // MAINWINDOW_H
