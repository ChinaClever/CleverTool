#ifndef ADDSETCMDDLG_H
#define ADDSETCMDDLG_H

#include <QDialog>
#include "snmpthread.h"
namespace Ui {
class AddSetCmdDlg;
}

class AddSetCmdDlg : public QDialog
{
    Q_OBJECT

public:
    explicit AddSetCmdDlg(QWidget *parent = 0);
    ~AddSetCmdDlg();
    void init(QList<sSnmpSetCmd> &cmd);
    QList<sSnmpSetCmd>& getSetCmdList();
    int getSendTimer();

private slots:
    void on_okBtn_clicked();

    void on_cancelBtn_clicked();

    void on_AutoradioBtn_clicked();

    void on_SetradiaBtn_clicked();

private:
    Ui::AddSetCmdDlg *ui;
    QList<sSnmpSetCmd> mSetCmdList;
    int mTimer,mBit;
};

#endif // ADDSETCMDDLG_H
