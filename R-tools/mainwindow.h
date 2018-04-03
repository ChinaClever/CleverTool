#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QGroupBox>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QDebug>
#include <QString>
#include <QTimer>
#include "spcomm.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();



private:
    Ui::MainWindow *ui;
    SPComm *m_spcomm;

    QLabel *title;
    QLabel *state, *stateTag;
    QLabel *label_1, *label_2, *label_3, *label_4;
    QLabel *L1, *L2, *L3;
    QLabel *curL1, *curL2, *curL3;
    QLabel *volL1, *volL2, *volL3;
    QLabel *energyL1, *energyL2, *energyL3;
    QLabel *curA, *curB, *curC;
    QLabel *volA, *volB, *volC;
    QLabel *energyA, *energyB, *energyC;
    QLabel *mac, *type, *line1, *line2, *line3;
    QGroupBox *group_1, *group_2, *group_3, *group_4;
    QComboBox *combo_1, *combo_2, *combo_3;
    QPushButton *button_1, *button_2, *button_3, *button_4, *button_5, *button_6, *button_7;
    QLineEdit *editCurL1, *editCurL2, *editCurL3;
    QLineEdit *editVolL1, *editVolL2, *editVolL3;
    QLineEdit *editEngL1, *editEngL2, *editEngL3;
    QLineEdit *editMac, *editLine1, *editLine2, *editLine3;
    QTextEdit *textEdit;

protected:
    void paintEvent(QPaintEvent *);

private slots:
    void open_pushButton_clicked();
    void close_pushButton_clicked();
    void readDevice_pushButton_clicked();
    void writeDevice_pushButton_clicked();
    void readDate_pushButton_clicked();
    void cleanDate_pushButton_clicked();
    void adjustDate_pushButton_clicked();
    void timerDone();


private:
    QStringList str3;
};

#endif // MAINWINDOW_H
