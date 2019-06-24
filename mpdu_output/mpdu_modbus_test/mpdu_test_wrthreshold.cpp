#include "mpdu_test_wrthreshold.h"
#include "ui_mpdu_test_wrthreshold.h"
#include <QFileDialog>

Mpdu_Test_WRThreshold::Mpdu_Test_WRThreshold(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Mpdu_Test_WRThreshold)
{
    ui->setupUi(this);
    ui->stopBtn->setEnabled(false);
    mFile = new QFile;
    m_timer =  new QTimer(this);
    m_totalConut = 0;
    openFile();
    ui->TimeEdit->setText(QString("%1").arg(400));
    connect(m_timer, SIGNAL(timeout()), SLOT(timeoutDone()));
    m_cleartimer = new QTimer(this);
    connect(m_cleartimer, SIGNAL(timeout()), SLOT(timeoutClearDone()));
}

Mpdu_Test_WRThreshold::~Mpdu_Test_WRThreshold()
{
    delete ui;
}

void Mpdu_Test_WRThreshold::timeoutDone()
{
    if(ui->stopBtn->isEnabled()) {
        if(!mlogs.isEmpty())
        writeLog(mlogs);
        mlogs.clear();
    }
}

void Mpdu_Test_WRThreshold::writeLog(QString &str)
{
    mFile->write(str.toUtf8() + "\n");
    mFile->flush();
}

void Mpdu_Test_WRThreshold::openFile()
{
    QDir dataDir(QDir::currentPath());
    QString dirName = "logs";
    if(!dataDir.exists(dirName))dataDir.mkdir(dirName);
    dataDir.cd(dirName);

    if(mFile->isOpen()) mFile->close();
    QDateTime local(QDateTime::currentDateTime());
    QString localTime = local.toString("yyyy_MM_dd_hh_mm_ss_zzz");
    QString fn = dataDir.absoluteFilePath(localTime) + ".txt";
    mFile->setFileName(fn);
    bool ret = mFile->open(QIODevice::Append|QIODevice::Text);
    if(ret) {

    }
}

void Mpdu_Test_WRThreshold::initwid(SerialPort *serial)
{
    mRtu = new SI_RtuThread(this);
    mRtu->init(serial);
    connect(mRtu, SIGNAL(cmdSig(QString,int)), this, SLOT(updateTextSlot(QString,int)));
    connect(mRtu, SIGNAL(countSig(int)), this, SLOT(updateErrLabSlot(int)));
}

void Mpdu_Test_WRThreshold::updateErrLabSlot(int count)
{
    m_errConut = count;
    ui->ErrLab->setText(QString("%1次").arg(m_errConut));
}

void Mpdu_Test_WRThreshold::updateTextSlot(QString str,int color)
{
    if(color==1)
        ui->textEdit->setTextColor(QColor(255,0,0));
    else if(color ==2)
        ui->textEdit->setTextColor(QColor(0,0,255));
    else if(color ==0)
        ui->textEdit->setTextColor(QColor(0,0,0));
    ui->textEdit->append(str);
    mlogs+=str+"\n";
    if(color)
    m_totalConut++;
    ui->TotalLab->setText(QString("%1次").arg(m_totalConut));
}

void Mpdu_Test_WRThreshold::updateText(QStringList str)
{
    for(int i = 0 ; i < str.size();i++)
    ui->textEdit_2->append(str.at(i));
}

void Mpdu_Test_WRThreshold::on_pushButton_2_clicked()
{
    static QString fn("/");
    fn = QFileDialog::getOpenFileName(0,tr("文件选择"),fn,"",0);
    if (!fn.isNull()) {
        bool ret = readFile(fn);
        if(!ret)
        {
            CriticalMsgBox bos(this, tr("读取文件失败!!"));
        }
    }
}

bool Mpdu_Test_WRThreshold::readFile(QString &fn)
{
    int i=0;
    QFile file(fn);
    if(file.open(QIODevice::ReadOnly))
    {
        while (!file.atEnd())
        {
            i++;
            QString str = QString(file.readLine()).trimmed().remove("\r\n");
            if(str.isEmpty()) continue;
            if(mlist.contains(str)) continue;
            mlist  << str;
        }
        updateText(mlist);
    }
    return true;
}

void Mpdu_Test_WRThreshold::transformer(sSetCmd& cmd)
{
    for(int i = 0 ; i < mlist.size();++i)
    {
        //qDebug()<<mlist.at(i);
        QStringList str = mlist.at(i).split(" ");
        //for(int j = 0 ; j < str.size(); ++j )
            //qDebug()<<str.at(j);
        cmd.addr = 1;
        cmd.fn =  str.at(0).toInt();
        cmd.value =  str.at(1).toShort();
        cmd.reg =  str.at(2).toInt();
        mRtu->setCmd(cmd);
        //qDebug()<<"aaaaa"<<cmd.fn<<cmd.value<<cmd.reg ;
    }
}

void Mpdu_Test_WRThreshold::on_pushButton_clicked()
{

    sSetCmd cmd;
    transformer(cmd);

    mRtu->start();
    m_timer->start(2000);
    ui->textEdit->clear();
    m_totalConut = 0;
    m_errConut = 0;

    ui->pushButton->setEnabled(false);
    ui->stopBtn->setEnabled(true);
}

void Mpdu_Test_WRThreshold::on_stopBtn_clicked()
{
    mRtu->stopThread();
    if(mFile->isOpen()) {mFile->close();}
    if(m_timer->isActive()) m_timer->stop();
    ui->pushButton->setEnabled(true);
    ui->stopBtn->setEnabled(false);
    m_cleartimer->start(30*60*1000);
}

void Mpdu_Test_WRThreshold::on_EditBtn_clicked()
{
    if(!m_timer->isActive())
    mRtu->setTimer(ui->TimeEdit->text().toInt());
    if(!m_cleartimer->isActive()) m_cleartimer->stop();
}

void Mpdu_Test_WRThreshold::timeoutClearDone()
{
   ui->textEdit->clear();
}
