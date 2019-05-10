#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    DataPacket::bulid()->data;
    mIpsWid = new IpsWid(ui->ipWid);
    mUpgradeWid = new UpgradeWid(ui->upWid);

    mIpsTabWid = new IpsTabWid(ui->ipsTabWid);
    mOksTabWid = new OksTabWid(ui->oksTabWid);
    mErrsTabWid = new ErrsTabWid(ui->errsTabWid);
    mDevSelectWid = new DevSelectWid(ui->devWid);


    connect(this,SIGNAL(languageChangeSig()),mIpsTabWid,SLOT(languageChanged()));
    connect(this,SIGNAL(languageChangeSig()),mOksTabWid,SLOT(languageChanged()));
    connect(this,SIGNAL(languageChangeSig()),mErrsTabWid,SLOT(languageChanged()));
    //    mUdpTesting = new UdpTesting(this);

    //    timer = new QTimer(this);
    //    timer->start(200);
    //    connect(timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));

//    this->setWindowTitle(tr("PDU UpgradeTool"));
//    ui->ipWid->setTitle("IP Generation Zone");
//    ui->group->setTitle("IP List Show Zone");
//    ui->upWid->setTitle("Upgrade Operation Zone");
    qtrans = new QTranslator(this);

    connect(ui->EnradioBtn,SIGNAL(toggled(bool)),this,SLOT(languageChanged()));
    connect(ui->ChradioBtn,SIGNAL(toggled(bool)),this,SLOT(languageChanged()));
    connect(this,SIGNAL(languageChangeSig()),mIpsWid,SLOT(languageChanged()));
    connect(this,SIGNAL(languageChangeSig()),mUpgradeWid,SLOT(languageChanged()));
    connect(this,SIGNAL(languageChangeSig()),mDevSelectWid,SLOT(languageChanged()));
    ui->ChradioBtn->setChecked(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)  translateUi();
    else  QMainWindow::changeEvent(event);
}

void MainWindow::translateUi()
{
}

void MainWindow::languageChanged()
{
    if(ui->EnradioBtn->isChecked())
        qtrans->load("english.qm");
    else
        qtrans->load("chinese.qm");
    ui->retranslateUi(this);
    emit languageChangeSig();
}

void MainWindow::timeoutDone(void)
{
    //    QStringList ips;
    //    bool ret = mUdpTesting->check(ips);
    //    if(ret) {
    //        QString str = tr("有其它电脑已打开升级工具：");
    //        for(int i=0; i<ips.size(); ++i) {
    //            str += ips.at(i) + "; ";
    //        }
    //        ui->statusBar->showMessage(str);
    //    }
}

