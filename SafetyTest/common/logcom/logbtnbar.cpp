/*
 * logbtnbar.cpp
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "logbtnbar.h"
#include "ui_logbtnbar.h"
#include "logcalendardlg.h"
#include "common.h"
#include <QGridLayout>

LogBtnBar::LogBtnBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogBtnBar)
{
    ui->setupUi(this);
    ui->startDateEdit->setDate(QDate::currentDate());
    ui->endDateEdit->setDate(QDate::currentDate());
    connect(ui->refreshBtn, SIGNAL(clicked()),this,SIGNAL(refreshSig()));

    QGridLayout *gridLayout = new QGridLayout(parent);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->addWidget(this);
}

LogBtnBar::~LogBtnBar()
{
    delete ui;
}

void LogBtnBar::getDate(QDate &startDate, QDate &endDate)
{
    startDate = ui->startDateEdit->date();
    endDate = ui->endDateEdit->date();

}

void LogBtnBar::on_queryBtn_clicked()
{
    QDate startDate ,endDate;
    getDate(startDate ,endDate);
    if(startDate > endDate) {
        CriticalMsgBox box(this, tr("起始日期早于结束日期，请重新填写日期!"));
    } else {
        QString startDateStr = startDate.toString("yyyy-MM-dd");
        QString endDateStr = endDate.toString("yyyy-MM-dd");
        QString str = QString("date between \'%1\' and  \'%2\'").arg(startDateStr).arg(endDateStr);
        //        QString str = tr("date like '%%1%'").arg(date);
        emit querySig(str);
    }
}

void LogBtnBar::on_clearBtn_clicked()
{
    QuMsgBox box(this, tr("确认清空数据?"));
    if(box.Exec())
        emit clearSig();
}


void LogBtnBar::on_startDateBtn_clicked()
{
    LogCalendarDlg dlg(this);
    dlg.exec();
    ui->startDateEdit->setDate(dlg.getDate());
}

void LogBtnBar::on_endDateBtn_clicked()
{
    LogCalendarDlg dlg(this);
    dlg.exec();
    ui->endDateEdit->setDate(dlg.getDate());
}

void LogBtnBar::on_exportBtn_clicked()
{
    emit exportSig();
}
