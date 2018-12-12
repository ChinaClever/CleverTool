#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qsshmanager.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    m_sshThread =new SshThread();
    connect(m_sshThread, SIGNAL(cmdSig(QString)), this, SLOT(updateTextSlot(QString)));
    connect(m_sshThread, SIGNAL(lableSig(int,int)), this, SLOT(updateLableSlot(int,int)));
    ui->setupUi(this);
    ui->lineEditTimeout->setText("1");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MainWindow::updateTextSlot(QString str)
{
    ui->textEdit->append(str);
}

void MainWindow::on_StopBtn_clicked()
{
    m_sshThread->stopThread();
}

void MainWindow::on_StartBtn_clicked()
{
    QString str = QString(tr("0 "));
    ui->OKlabel->setText(str);
    ui->ERRlabel->setText(str);
    ui->OKlabel->setStyleSheet("color:black");
    ui->ERRlabel->setStyleSheet("color:black");
    m_sshThread->init(ui->lineEditIp->text(),
                      ui->lineEditCount->text(),
                      ui->lineEditPassword->text(),
                      ui->lineEditTimeout->text().toInt(),
                      ui->ZPDURradioBtn->isChecked());
    m_sshThread->start();
}

void MainWindow::updateLableSlot(int Ok , int Err)
{
    QString str = QString(tr("%1 ")).arg(Ok);
    ui->OKlabel->setText(str);

    if (ui->ERRlabel->text().toInt() != Err)
        ui->ErrtextEdit->append(str);
    str = QString(tr("%1 ")).arg(Err);
    ui->ERRlabel->setText(str);
    if(Err!=0)
    {
        ui->OKlabel->setStyleSheet("color:red");
        ui->ERRlabel->setStyleSheet("color:red");
    }
}

