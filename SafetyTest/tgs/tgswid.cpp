#include "tgswid.h"
#include "ui_tgswid.h"
#include "dblogquery.h"

TgsWid::TgsWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TgsWid)
{
    ui->setupUi(this);

    groupBox_background_icon(this);
    QDate date = QDate::currentDate();
    ui->todayDateEdit->setDate(date);
    ui->startDateEdit->setDate(date);
    ui->endDateEdit->setDate(date);

    ui->errLcd->setStyleSheet("border: 1px solid black; color: black;");
    ui->numLcd->setStyleSheet("border: 1px solid black; color: black;");
    ui->okLcd->setStyleSheet("border: 1px solid black; color: black;");
}

TgsWid::~TgsWid()
{
    delete ui;
}

void TgsWid::updateData(int all, int ok)
{
    ui->numLcd->display(all);
    ui->okLcd->display(ok);
    ui->errLcd->display(all-ok);
    if(ok) ui->okLcd->setStyleSheet("border: 1px solid green; color: green;");
    else ui->okLcd->setStyleSheet("border: 1px solid black; color: black;");
    if(all-ok) ui->errLcd->setStyleSheet("border: 1px solid red; color: red;");
    else ui->errLcd->setStyleSheet("border: 1px solid black; color: black;");

    ui->numLcd->setStyleSheet("border: 1px solid black; color: black;");

    double value = 0;
    if(all) value = (ok*100.0) / all;
    ui->gaugeArc2->setValue(value);
}

int TgsWid::workDown()
{
    int all=0, ok=0; QString code;
    DbLogQuery *log = DbLogQuery::bulid();    
    int ms = QTime::currentTime().msecsSinceStartOfDay();

    if(ui->codeRadio->isChecked()) {
        code = ui->codeEdit->text();
    }

    if(ui->todayRadio->isChecked()) {
        all = log->todayAllCount(code);
        ok = log->todayOkCount(code);
    } else {
        QDate start = ui->startDateEdit->date();
        QDate end = ui->endDateEdit->date();
        all = log->byCodeAllCount(start, end, code);
        ok = log->byCodeOkCount(start, end, code);
    }

    updateData(all, ok);
    return (QTime::currentTime().msecsSinceStartOfDay() - ms);
}

bool TgsWid::checkInput()
{
    bool ret = true;
    if(ui->codeRadio->isChecked()) {
        QString code = ui->codeEdit->text();
        if(code.isEmpty()) {
            CriticalMsgBox box(this, tr("请输入工装条码!!!"));
            ret = false;
        }
    }

    if(ui->trRadio->isChecked()) {
        QDate start = ui->startDateEdit->date();
        QDate end = ui->endDateEdit->date();
        if(start > end) {
             CriticalMsgBox box(this, tr("起始时间大于对事时间，请重新输入!!!"));
            ret = false;
        }
    }

    return ret;
}

void TgsWid::on_queryBtn_clicked()
{
    bool ret = checkInput();
    if(ret) {
        int rtn = workDown();
        InfoMsgBox box(this, tr("\n 查询完成, 共耗时 %1ms     \n").arg(rtn));
    }
}

void TgsWid::on_trRadio_clicked(bool checked)
{
    ui->startDateEdit->setEnabled(checked);
    ui->endDateEdit->setEnabled(checked);
}

void TgsWid::on_todayRadio_clicked(bool checked)
{
    on_trRadio_clicked(!checked);
}

void TgsWid::on_codeRadio_clicked(bool checked)
{
    ui->codeEdit->setEnabled(checked);
}

void TgsWid::on_allRadio_clicked(bool checked)
{
    on_codeRadio_clicked(!checked);
}
