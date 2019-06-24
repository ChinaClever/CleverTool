#include "addsetcmddlg.h"
#include "ui_addsetcmddlg.h"

AddSetCmdDlg::AddSetCmdDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddSetCmdDlg)
{
    ui->setupUi(this);
    if(ui->AutoradioBtn->isChecked())
       this->on_AutoradioBtn_clicked();
    else
        this->on_SetradiaBtn_clicked();
    mTimer = 500;
    ui->TimelineEdit->setText(QString::number(mTimer));
    mBit = 24;
    ui->ValuelineEdit->setText(QString::number(mBit));
}

AddSetCmdDlg::~AddSetCmdDlg()
{
    delete ui;
}

void AddSetCmdDlg::on_okBtn_clicked()
{
    if(ui->AutoradioBtn->isChecked()){
        sSnmpSetCmd cmd ;
        cmd.type = SNMP_STRING_TYPE;
        QString str = "";
        mBit = ui->ValuelineEdit->text().toInt();
        QStringList list = ui->OIDlineEdit->text().split(".");
        for(int j = 1 ; j < list.size()-2; ++j)
            str+="."+list.at(j);

        if(ui->OFFcheckBox->isChecked())
        {
            for(int i = 0 ; i < mBit ; ++i)
            {
                cmd.oid = str+QString(".%1").arg(i+1)+QString(".0");
                qDebug()<<cmd.oid;
                cmd.value= QString("OFF").toLocal8Bit();
                this->mSetCmdList.append(cmd);
            }
        }
        if(ui->ONcheckBox->isChecked())
        {
            for(int i = 0 ; i < mBit ; ++i)
            {
                cmd.oid = str+QString(".%1").arg(i+1)+QString(".0");
                cmd.value= QString("ON").toLocal8Bit();
                this->mSetCmdList.append(cmd);
            }
        }
    }
    else{
    sSnmpSetCmd cmd ;
    cmd.type = SNMP_STRING_TYPE;
    cmd.oid = ui->OID1lineEdit->text();
    cmd.value= ui->Value1lineEdit->text().toLocal8Bit();
    this->mSetCmdList.append(cmd);
    cmd.oid = ui->OID2lineEdit->text();
    cmd.value= ui->Value2lineEdit->text().toLocal8Bit();
    this->mSetCmdList.append(cmd);
    cmd.oid = ui->OID3lineEdit->text();
    cmd.value= ui->Value3lineEdit->text().toLocal8Bit();
    this->mSetCmdList.append(cmd);
    cmd.oid = ui->OID4lineEdit->text();
    cmd.value= ui->Value4lineEdit->text().toLocal8Bit();
    this->mSetCmdList.append(cmd);
    cmd.oid = ui->OID5lineEdit->text();
    cmd.value= ui->Value5lineEdit->text().toLocal8Bit();
    this->mSetCmdList.append(cmd);
    cmd.oid = ui->OID6lineEdit->text();
    cmd.value= ui->Value6lineEdit->text().toLocal8Bit();
    this->mSetCmdList.append(cmd);
    }

    this->close();

    this->accept();

}

void AddSetCmdDlg::on_cancelBtn_clicked()
{
    this->close();
}

void AddSetCmdDlg::init(QList<sSnmpSetCmd> &cmd)
{
    mSetCmdList = cmd;
    ui->OIDlineEdit->setText(".1.3.6.1.4.1.30966.6.1.1.9.1.0");
    ui->OID1lineEdit->setText(".1.3.6.1.4.1.30966.6.1.1.9.1.0");
}

QList<sSnmpSetCmd>& AddSetCmdDlg::getSetCmdList()
{
    return mSetCmdList;
}

int AddSetCmdDlg::getSendTimer()
{
    return mTimer;
}

void AddSetCmdDlg::on_AutoradioBtn_clicked()
{
    ui->OID1lineEdit->hide();
    ui->Value1lineEdit->hide();
    ui->OID2lineEdit->hide();
    ui->Value2lineEdit->hide();
    ui->OID3lineEdit->hide();
    ui->Value3lineEdit->hide();
    ui->OID4lineEdit->hide();
    ui->Value4lineEdit->hide();
    ui->OID5lineEdit->hide();
    ui->Value5lineEdit->hide();
    ui->OID6lineEdit->hide();
    ui->Value6lineEdit->hide();
    ui->label->hide();
    ui->label_2->hide();
    ui->label_3->hide();
    ui->label_4->hide();
    ui->label_5->hide();
    ui->label_6->hide();
    ui->label_7->hide();
    ui->label_8->hide();
    ui->label_9->hide();
    ui->label_10->hide();
    ui->label_11->hide();
    ui->label_12->hide();

    ui->label_13->show();
    ui->OIDlineEdit->show();
    ui->label_14->show();
    ui->ValuelineEdit->show();
    ui->ONcheckBox->show();
    ui->OFFcheckBox->show();
}

void AddSetCmdDlg::on_SetradiaBtn_clicked()
{
    ui->OID1lineEdit->show();
    ui->Value1lineEdit->show();
    ui->OID2lineEdit->show();
    ui->Value2lineEdit->show();
    ui->OID3lineEdit->show();
    ui->Value3lineEdit->show();
    ui->OID4lineEdit->show();
    ui->Value4lineEdit->show();
    ui->OID5lineEdit->show();
    ui->Value5lineEdit->show();
    ui->OID6lineEdit->show();
    ui->Value6lineEdit->show();
    ui->label->show();
    ui->label_2->show();
    ui->label_3->show();
    ui->label_4->show();
    ui->label_5->show();
    ui->label_6->show();
    ui->label_7->show();
    ui->label_8->show();
    ui->label_9->show();
    ui->label_10->show();
    ui->label_11->show();
    ui->label_12->show();

    ui->label_13->hide();
    ui->OIDlineEdit->hide();
    ui->label_14->hide();
    ui->ValuelineEdit->hide();
    ui->ONcheckBox->hide();
    ui->OFFcheckBox->hide();
}
