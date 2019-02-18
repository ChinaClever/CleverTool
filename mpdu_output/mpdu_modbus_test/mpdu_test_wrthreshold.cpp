#include "mpdu_test_wrthreshold.h"
#include "ui_mpdu_test_wrthreshold.h"
#include <QFileDialog>

Mpdu_Test_WRThreshold::Mpdu_Test_WRThreshold(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Mpdu_Test_WRThreshold)
{
    ui->setupUi(this);
}

Mpdu_Test_WRThreshold::~Mpdu_Test_WRThreshold()
{
    delete ui;
}

void Mpdu_Test_WRThreshold::initwid(SerialPort *serial)
{
    mRtu = new SI_RtuThread(this);
    mRtu->init(serial);
    connect(mRtu, SIGNAL(cmdSig(QString)), this, SLOT(updateTextSlot(QString)));
    connect(mRtu, SIGNAL(countSig(int)), this, SLOT(updateErrLabSlot(int)));
}

void Mpdu_Test_WRThreshold::updateErrLabSlot(int count)
{
    ui->ErrLab->setText(QString("%1次").arg(count));
}

void Mpdu_Test_WRThreshold::updateTextSlot(QString str)
{
    ui->textEdit->append(str);
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
    ui->textEdit->clear();
}

void Mpdu_Test_WRThreshold::on_stopBtn_clicked()
{
    mRtu->stopThread();
}
