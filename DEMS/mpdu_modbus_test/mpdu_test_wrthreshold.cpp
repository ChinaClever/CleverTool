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
    //m_cleartimer = new QTimer(this);
    //connect(m_cleartimer, SIGNAL(timeout()), SLOT(timeoutClearDone()));

    ui->TimeEdit->hide();
    ui->ErrLab_2->hide();
    ui->EditBtn->hide();
    ui->ErrLab->hide();
    ui->TotalLab->hide();
    ui->comboBoxDev->hide();
    ui->label->hide();
    ui->label_3->hide();
    ui->label_2->hide();
    ui->textEdit_2->hide();
    ui->pushButton_2->hide();

    mSaveThread = new Excel_SaveThread(this);
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
    {
        ui->textEdit->setTextColor(QColor(255,0,0));
       // mSaveThread->saveData(mFileName,list);
    }
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
    int key = 0x00;
    for(int i = 0 ; i < 4; ++i)
    {
        switch(i)
        {
        case 0 : key = 0x00; break;
        case 1: key = 0x20; break;
        case 2: key = 0x40; break;
        case 3: key = 0x60; break;
        default: key = 0x00; break;
        }
        for(int j = 0 ; j < 7 ; ++j)
        {
            cmd.addr = key + j;
            cmd.fn =  0x03;
            cmd.value = 0xE6;
            cmd.reg =  0x00;
            mRtu->setCmd(cmd);
        }
    }
}

int Mpdu_Test_WRThreshold::analyseDevType(QString str)
{
    int len = 121;
    if(str == "V3")
        len = 121;
    else if(str == "V1")
        len = 103;
    else
        len = 87;
    return len;
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
    //this->saveTestData();
}

void Mpdu_Test_WRThreshold::on_stopBtn_clicked()
{
    mRtu->stopThread();
    if(mFile->isOpen()) {mFile->close();}
    if(m_timer->isActive()) m_timer->stop();
    ui->pushButton->setEnabled(true);
    ui->stopBtn->setEnabled(false);
    //m_cleartimer->start(30*60*1000);
}

void Mpdu_Test_WRThreshold::on_EditBtn_clicked()
{
    if(!m_timer->isActive())
    mRtu->setTimer(ui->TimeEdit->text().toInt());
}

void Mpdu_Test_WRThreshold::timeoutClearDone()
{
   ui->textEdit->clear();
}

void Mpdu_Test_WRThreshold::saveTestData()
{
    mHeaderList.clear();
    bulidHead();
    bulidTestData();
    mSaveThread->saveData(mFileName, mHeaderList);
}

QString Mpdu_Test_WRThreshold::bulidFileName()
{
    QString file = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss_zzz");
    return file;
}

QString Mpdu_Test_WRThreshold::bulidPath()
{
    QDir dir;
    QString path = "E:/" + tr("读取动环数据分析日志") + '/';
    dir.mkpath(path); //创建多级目录，如果已存在则会返回去true
    return path;
}

void Mpdu_Test_WRThreshold::bulidHead()
{
    mFileName = bulidPath() + bulidFileName();
}

void Mpdu_Test_WRThreshold::bulidTestData()
{
    QStringList header;
    header<<tr("地址")<< tr("温度1")<< tr("温度2")<< tr("温度3")
          << tr("断路器1")<< tr("断路器2")<< tr("断路器3")
          <<tr("电流A1")<<tr("电流B1")<< tr("电流C1")
         << tr("电流A2/零线电流")<< tr("电流B2")<< tr("电流C2")
         << tr("电流A3")<< tr("电流B3")<< tr("电流C3")
         <<tr("电压A1")<<tr("电压B1")<< tr("电压C1")
         << tr("电压A2")<< tr("电压B2")<< tr("电压C2")
         << tr("电压A3")<< tr("电压B3")<< tr("电压C3")
        <<tr("防雷开关状态");
    mHeaderList << header ;
}

