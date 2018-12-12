#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "serialportwid.h"
#include "modubs_rtu/si_rtuthread.h"
#include "outputwid.h"

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
    void initwid();

protected slots:
   void updateTextSlot(QString str);

private slots:
    void on_checkBox_clicked(bool checked);
    void on_pushButton_clicked();
    void on_openRadio_clicked(bool checked);

    void on_closeRadio_clicked(bool checked);

private:
    Ui::MainWindow *ui;
    SerialPortWid *mSerialPortWid;
    SI_RtuThread *mRtu;

    OutputWid *mWid[24];
};

#endif // MAINWINDOW_H
