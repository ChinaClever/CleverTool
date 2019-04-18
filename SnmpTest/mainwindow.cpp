#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDir>
#include "msgbox.h"

/**
 * @brief 是否为IP地址
 * @param ip
 * @return
 */
bool cm_isIPaddress(const QString& ip)
{
    QRegExp regExp("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
    if(regExp.exactMatch(ip))
        return true ;

    return false;
}

/***
  * 获取程序数据目录
  */
QString cm_pathOfData(const QString& name)
{
    QDir dataDir(QDir::home());
    QString dirName = ".CleverTestSystem";
    if(!dataDir.exists(dirName))
        dataDir.mkdir(dirName);
    dataDir.cd(dirName);
    return dataDir.absoluteFilePath(name);
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mFile = new QFile;
    mSnmp = new SnmpThread(this);
    m_timer =  new QTimer(this);
    m_timer->start(1000);
    connect(m_timer, SIGNAL(timeout()), SLOT(timeoutDone()));

    connect(mSnmp, SIGNAL(requestSig(QString)), this, SLOT(requestSlot(QString)));
    connect(mSnmp, SIGNAL(responseSig(QtSnmpDataList)), this, SLOT(responseSlot(QtSnmpDataList)));
    connect(mSnmp, SIGNAL(reqErrSig()), this, SLOT(reqErrSlot()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::openFile()
{
    QDir dataDir(QDir::currentPath());
    QString dirName = "logs";
    if(!dataDir.exists(dirName))dataDir.mkdir(dirName);
    dataDir.cd(dirName);

    if(mFile->isOpen()) mFile->close();
    QString fn = dataDir.absoluteFilePath(ui->ipEdit->text()) + ".txt";
    mFile->setFileName(fn);
    bool ret = mFile->open(QIODevice::Append|QIODevice::Text);
    if(ret) {

    }
}

void MainWindow::writeLog(const QString &str)
{
    if(isSave) {
        mFile->write(str.toUtf8() + "\n");
        mFile->flush();
    }
}

bool MainWindow::checkInput()
{
    bool ret = false;
    QString str = ui->ipEdit->text();
    if(str.isEmpty()) {
        str = tr("请填入IP地址!!");
    } else {
        ret = cm_isIPaddress(str);
        if(!ret) str = tr("IP地址错误，请重新填写!");
    }

    if(ret) {
        if(ui->oidRadio->isChecked()) {
            str = ui->oidEdit->text();
        } else {
            str = ui->suboidEdit->text();
        }
        if(str.size() < 14) {
            str = tr("OID输入过短，请重新输入!!");
            ret = false;
        } else {
            str.clear();
        }
    }

    if(str.size()) {
        CriticalMsgBox box(this, str);
    }

    return ret;
}

bool MainWindow::startFun()
{
    bool ret = checkInput();
    if(ret) {
        QString str = ui->ipEdit->text();
        int msec = ui->timeSpinBox->value() * 1000;
        mSnmp->startRead(str, msec);

        if(ui->oidRadio->isChecked()) {
            str = ui->oidEdit->text();
            mSnmp->requestValue(str);
        } else {
            str = ui->suboidEdit->text();
            mSnmp->requestSubValues(str);
        }

        isSave = ui->logCheckBox->isChecked();
        if(isSave) openFile();
        ui->starttime->setDateTime(QDateTime::currentDateTime());
    }

    return ret;
}

void MainWindow::on_startBtn_clicked()
{
    bool ret = startFun();
    if(ret) {
        ui->groupBox->setDisabled(ret);
        ui->startBtn->setDisabled(ret);
        ui->overBtn->setEnabled(ret);
        on_clearBtn_clicked();
    }
}

void MainWindow::on_overBtn_clicked()
{
    bool ret = false;
    ui->groupBox->setDisabled(ret);
    ui->startBtn->setDisabled(ret);
    ui->overBtn->setEnabled(ret);
    mSnmp->stopRun();
    if(mFile->isOpen()) mFile->close();
}

void MainWindow::on_clearBtn_clicked()
{    
    ui->textEdit->clear();
}

void MainWindow::updateData()
{
    sSnmpRes *res = mSnmp->getSnmpRes();
    ui->allLcd->display(res->all);
    ui->okLcd->display(res->ok);
    ui->errLcd->display(res->err);
    ui->longLcd->display(res->longmsec);
    ui->resLcd->display(res->risimsec);
    ui->currenttime->setDateTime(QDateTime::currentDateTime());
}

void MainWindow::timeoutDone()
{
    if(ui->overBtn->isEnabled()) {
        updateData();
    }
}

void MainWindow::requestSlot(const QString& str)
{
    QString  timestr = QTime::currentTime().toString("hh:mm:ss.zzz");
    QString text = "\n----------------------------------------------";
    ui->textEdit->append(text);
    writeLog(text);

    text = tr("请求:") + timestr + "  " + str;
    ui->textEdit->append(text);
    writeLog(text);
}

void MainWindow::responseSlot(const QtSnmpDataList& values)
{
    for( const auto& value : values ) {
        QString  str = tr("应答:") + QTime::currentTime().toString("hh:mm:ss.zzz") +  "  ";
        str += qPrintable( value.address() );
        str += "  ";
        str += qPrintable( value.data());
        ui->textEdit->append(str);
        writeLog(str);
    }
}

void MainWindow::reqErrSlot()
{
    on_overBtn_clicked();
    QString str = tr("SNMP请求错误，测试中断!!");
    CriticalMsgBox box(this, str);
}
