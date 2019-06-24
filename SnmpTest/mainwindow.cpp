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


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mFile = new QFile;
    mMutex = new QMutex();
    mSnmp = new SnmpThread(this);
    m_timer =  new QTimer(this);
    m_timer->start(2000);
    m_cleartimer = new QTimer(this);

    mAddSetCmdDlg = new AddSetCmdDlg(this);
    mAddSetCmdDlg->init(this->mSetCmdList);


    this->setWindowTitle(tr("SnmpTest"));
    connect(m_timer, SIGNAL(timeout()), SLOT(timeoutDone()));
    connect(m_cleartimer, SIGNAL(timeout()), SLOT(timeoutClearDone()));

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

void MainWindow::writeLog(QString &str)
{
    QMutexLocker locker(mMutex);
    if(isSave) {
        mFile->write(str.toUtf8() + "\n");
        mFile->flush();
    }
    str.clear();
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
        m_cleartimer->start(30*60*1000);
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
    QString str = tr("是否停止测试!!");
    QuMsgBox box(this,str);
    if(box.Exec()){
        bool ret = false;
        ui->groupBox->setDisabled(ret);
        ui->startBtn->setDisabled(ret);
        ui->overBtn->setEnabled(ret);
        mSnmp->stopRun();
        if(mFile->isOpen()) mFile->close();
        m_cleartimer->stop();
    }
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
    ui->outLcd->display(res->out);
    ui->errLcd->display(res->err);
    ui->longLcd->display(res->longmsec);
    ui->resLcd->display(res->risimsec);
    ui->currenttime->setDateTime(QDateTime::currentDateTime());
}

void MainWindow::timeoutClearDone()
{
   ui->textEdit->clear();
}

void MainWindow::timeoutDone()
{
    if(ui->overBtn->isEnabled()) {
        updateData();
        writeLog(logs);
    }
}

void MainWindow::requestSlot(const QString& str)
{
    QMutexLocker locker(mMutex);
    QString  timestr = QTime::currentTime().toString("hh:mm:ss.zzz");
    QString text = "\n----------------------------------------------";
    ui->textEdit->append(text);
    logs += text + "\n";

    text = tr("请求:") + timestr + "  " + str;
    ui->textEdit->append(text);
    logs += text + "\n";
}

void MainWindow::responseSlot(const QtSnmpDataList& values)
{
    QMutexLocker locker(mMutex);
    for( const auto& value : values ) {
        QString  str = tr("应答:") + QTime::currentTime().toString("hh:mm:ss.zzz");
        str += "             ";
        str += qPrintable( value.address() );
        str += "             ";
        str += qPrintable( value.data());
        ui->textEdit->append(str);
        logs += str + "\n";
    }
}


void MainWindow::reqErrSlot()
{
    //on_overBtn_clicked();
    // CriticalMsgBox box(this, str);
}

void MainWindow::on_addSetCmdBtn_clicked()
{
    int ret = mAddSetCmdDlg->exec();
    if( ret == QDialog::Accepted ) {
        this->mSetCmdList  = mAddSetCmdDlg->getSetCmdList();
        this->mSendTimer  = mAddSetCmdDlg->getSendTimer();
        mSnmp->setValue(this->mSetCmdList,this->mSendTimer);
    }
}
