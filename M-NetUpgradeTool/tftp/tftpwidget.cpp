#include "tftpwidget.h"
#include "ui_tftpwidget.h"
#include "common\msgbox.h"

TftpWidget::TftpWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TftpWidget)
{
    ui->setupUi(this);

    mExportDlg = new ExportDlg(this);
    mTftpThread = new TftpThread(this);
    mData = DataPacket::bulid()->data;

    timer = new QTimer(this);
    timer->start(200);
    connect(timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
}

TftpWidget::~TftpWidget()
{
    delete ui;
}

bool TftpWidget::checkFile()
{
    bool ret = false;
    QString file = ui->lineEdit->text();
    if(!file.isEmpty()) {
        QString str = file.section(".", -1);
        if(str == "bin") {
            mData->file = str;
            ret = true;
        } else {
            CriticalMsgBox box(this, tr("升级文件格式错误!, 请重新选择"));
        }
    } else {
        CriticalMsgBox box(this, tr("请选择升级文件"));
    }

    return ret;
}

void TftpWidget::on_openBtn_clicked()
{
    QString fn = QFileDialog::getOpenFileName(0,tr("文件选择"),"/","",0);
    if (!fn.isNull()) {
        if(checkFile()) {
            ui->lineEdit->setText(fn);
        }
    }
}


void TftpWidget::timeoutDone(void)
{
    if(mData->isRun)
    {
        QString str = mData->status;
        if(str.isEmpty()) str = tr("请开始");
        ui->stateLab->setText(str);

        bool en = mData->isRun;
        if(mData->ips.isEmpty()) en = true;
        ui->openBtn->setDisabled(en);
        ui->updateBtn->setDisabled(en);

        int x = 0;
        int count = mData->ips.size();
        if(count) x = ((mData->progress * 1.0) / count) *100;
        ui->progressBar->setValue(x);
    }
}


void TftpWidget::on_updateBtn_clicked()
{
    if(checkFile())
    {
        mTftpThread->startSend();
    }
}

void TftpWidget::on_exportBtn_clicked()
{
    mExportDlg->exec();
}
