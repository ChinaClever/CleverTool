/*
 * log_exportdlg.cpp
 * 导出窗口
 *
 *  Created on: 2017年10月11日
 *      Author: Lzy
 */
#include "log_exportdlg.h"
#include "ui_log_exportdlg.h"
#include <QFileDialog>
#include "logcalendardlg.h"
#include "common.h"

LOG_ExportDlg::LOG_ExportDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LOG_ExportDlg)
{

    ui->setupUi(this);
    groupBox_background_icon(this);
    this->setWindowTitle(tr("日志导出"));

    ui->startDateEdit->setDate(QDate::currentDate());
    ui->endDateEdit->setDate(QDate::currentDate());
    mExportThread = new Excel_DbLogThread(this);

    timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
}

LOG_ExportDlg::~LOG_ExportDlg()
{
    delete ui;
}

void LOG_ExportDlg::set(QString &name, QStringList &head, QString &title)
{
    mTableName = name;
    mHeadList = head;
    mTableTile = title;
    mTableTile+=tr("导出");
    ui->titleLab->setText(mTableTile);
    ui->fileEdit->setText(mTableTile);
}

/**
 * @brief 路径选择
 */
void LOG_ExportDlg::on_pushButton_clicked()
{
    QFileDialog dlg(this,tr("路径选择"));
    dlg.setFileMode(QFileDialog::DirectoryOnly);
    dlg.setDirectory("E:");
    if(dlg.exec() == QDialog::Accepted) {
        QString fn = dlg.selectedFiles().at(0);
        if(fn.right(0) != "/")  fn += "/";
        ui->pathEdit->setText(fn);
    }
}

void LOG_ExportDlg::on_startBtn_clicked()
{
    LogCalendarDlg calendar(this);
    int ret = calendar.exec();
    if(ret == QDialog::Accepted) {
        QDate date = calendar.getDate();
        ui->startDateEdit->setDate(date);
    }
}

void LOG_ExportDlg::on_endBtn_clicked()
{
    LogCalendarDlg calendar(this);
    int ret = calendar.exec();
    if(ret == QDialog::Accepted) {
        QDate date = calendar.getDate();
        ui->endDateEdit->setDate(date);
    }
}

/**
 * @brief 检查输入
 */
bool LOG_ExportDlg::checkInput()
{
    QString str = ui->pathEdit->text();
    if(str.isEmpty()) {
        CriticalMsgBox box(this, tr("导出路径不能为空！"));
        return false;
    }

    str = ui->fileEdit->text();
    if(str.isEmpty()) {
        CriticalMsgBox box(this, tr("导出文件名不能为空！"));
        return false;
    }

    str = ui->pathEdit->text() + ui->fileEdit->text() +".xlsx";
    QFile file(str);
    if (file.exists()){
        CriticalMsgBox box(this, str + tr("\n文件已存在！!"));
        return false;
    }

    QDate startDate = ui->startDateEdit->date();
    QDate endDate = ui->endDateEdit->date();
    if(startDate > endDate) {
        CriticalMsgBox box(this, tr("开始日期应早于结束日期，请重新输入日期！"));
        return false;
    }
    return true;
}

/**
 * @brief 导出完成
 */
void LOG_ExportDlg::exportDone()
{
    ui->exportBtn->setEnabled(true);
    ui->quitBtn->setEnabled(true);
    InfoMsgBox box(this, tr("\n导出完成!!\n"));
}

void LOG_ExportDlg::timeoutDone()
{
    int progress = getProgress();
    if(progress < 100)
        ui->progressBar->setValue(progress);
    else {
        ui->progressBar->setValue(progress);
        timer->stop();
        exportDone();
    }
}

void LOG_ExportDlg::setExcel()
{
    sExcel_DbItem ex;
    ex.path = ui->pathEdit->text();
    ex.file = ui->fileEdit->text();
    ex.start = ui->startDateEdit->date().toString("yyyy-MM-dd");
    ex.end = ui->endDateEdit->date().toString("yyyy-MM-dd");
    ex.db = mTableName;
    ex.title = mHeadList;
    setEx(ex);
}

void LOG_ExportDlg::on_exportBtn_clicked()
{

    bool ret = checkInput();
    if(ret) {
        timer->start(100);
        ui->exportBtn->setDisabled(true);
        ui->quitBtn->setDisabled(true);
        setExcel();
    }
}

void LOG_ExportDlg::setDate(QDate &start, QDate &end)
{
    ui->startDateEdit->setDate(start);
    ui->endDateEdit->setDate(end);

//    ui->startDateEdit->setEnabled(false);
//    ui->endDateEdit->setEnabled(false);
//    ui->startBtn->setEnabled(false);
//    ui->endBtn->setEnabled(false);
}
