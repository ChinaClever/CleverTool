#include "upgradewid.h"
#include "ui_upgradewid.h"
#include "msgbox.h"

UpgradeWid::UpgradeWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UpgradeWid)
{
    ui->setupUi(this);

    mData = DataPacket::bulid()->data;
    mExportDlg = new ExportDlg(this);
    mTcpThread = new TcpUpgrade(this);
    mTftpThread = new TftpUpgrade(this);

    timer = new QTimer(this);
    timer->start(500);
    connect(timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
}

UpgradeWid::~UpgradeWid()
{
    delete ui;
}

bool UpgradeWid::checkFile()
{
    bool ret = false;
    QString fn = ui->lineEdit->text();
    if(!fn.isEmpty()) {
        QString str = fn.section(".", -1);
        if(str == "bin") {
            mData->file = fn;
            ret = true;
        } else {
            CriticalMsgBox box(this, tr("升级文件格式错误!, 请重新选择"));
        }
    } else {
        CriticalMsgBox box(this, tr("请选择升级文件"));
    }

    return ret;
}

void UpgradeWid::on_openBtn_clicked()
{
    static QString fn;
    fn = QFileDialog::getOpenFileName(0,tr("文件选择"),fn,"",0);
    if (!fn.isNull()) {
        ui->lineEdit->setText(fn);
        checkFile();
    }
}


void UpgradeWid::timeoutDone(void)
{
    QString str = mData->status;
    if(str.isEmpty()) str = tr("请开始");
    ui->stateLab->setText(str);

    if(mData->subPorgress>100) mData->subPorgress = 0;
    ui->progressBar_2->setValue(mData->subPorgress);

    bool en = mData->isRun;
    if(mData->ips.isEmpty()) en = true;
    ui->openBtn->setDisabled(en);
    ui->updateBtn->setDisabled(en);
    ui->exportBtn->setDisabled(en);
    ui->breakBtn->setEnabled(en);

    int x = 0;
    int count = mData->ips.size();
    if(count) x = ((mData->progress * 1.0) / count) *100;
    ui->progressBar->setValue(x);
}


void UpgradeWid::on_updateBtn_clicked()
{
    if(mData->devtype) mUpgradeThread = mTcpThread;
    else mUpgradeThread = mTftpThread;

    if(checkFile()) {
        mUpgradeThread->startSend();
    }
}

void UpgradeWid::on_exportBtn_clicked()
{
    mExportDlg->exec();
}

void UpgradeWid::on_breakBtn_clicked()
{
    if(mData->isRun) {
        QuMsgBox box(this, tr("是否要中断传输?"));
        if(box.Exec()) {
            mUpgradeThread->breakDown();
            if(!mData->devtype) {
                InfoMsgBox msg(this, tr("软件即将重启!!!"));
                QProcess *process = new QProcess(this);
                process->start("NetUpgradeTool.exe");
                exit(0);
            }
        }
    }
}
