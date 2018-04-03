#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>
//#include "udp/datadefine.h"
#include "udp/datasouce.h"


namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = 0 , int currentindex = 0, int datatype = 0, int currentrow = 0);
    ~SettingDialog();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::SettingDialog *ui;

    int index;
    int type;
    int row;


private:
    void initDialog();
    void initUnit(QString str);
    void initData(unit *data);
    void hideCir();
    void fillBuf(quint16 *buffer,quint8 *buf);
};



#endif // SETTINGDIALOG_H
