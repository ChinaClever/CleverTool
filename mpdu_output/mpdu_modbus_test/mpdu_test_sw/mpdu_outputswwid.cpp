#include "mpdu_outputswwid.h"
#include "ui_mpdu_outputswwid.h"

MPDU_OutputSwWid::MPDU_OutputSwWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MPDU_OutputSwWid)
{
    ui->setupUi(this);
    mSerialPortWid = new SerialPortWid(ui->serialWid);

    mRtu = SI_RtuThread::bulid(this);
    mRtu->init(mSerialPortWid->getSerialPort());
    connect(mRtu, SIGNAL(cmdSig(QString)), this, SLOT(updateTextSlot(QString)));

    initwid();
}

MPDU_OutputSwWid::~MPDU_OutputSwWid()
{
    delete ui;
}


void MPDU_OutputSwWid::initwid()
{
    QWidget *wid[] = {ui->widget_1, ui->widget_2, ui->widget_3, ui->widget_4, ui->widget_5,ui->widget_6,
                      ui->widget_7, ui->widget_8, ui->widget_9, ui->widget_10, ui->widget_11, ui->widget_12,
                      ui->widget_13,ui->widget_14, ui->widget_15, ui->widget_16,ui->widget_17,ui->widget_18,
                      ui->widget_19,ui->widget_20,ui->widget_21,ui->widget_22,ui->widget_23,ui->widget_24};

    for(int i=0; i<24; ++i) {
        mWid[i] = new MPDU_OutputSwItemWid(wid[i]);
        mWid[i]->init(i);

        wid[i]->setStyleSheet("background-color: rgb(255, 255, 255);");
    }
}

void MPDU_OutputSwWid::updateTextSlot(QString str)
{
    ui->textEdit->append(str);
}

void MPDU_OutputSwWid::on_checkBox_clicked(bool checked)
{
    for(int i=0; i<24; ++i) {
        mWid[i]->setSelect(checked);
    }
}

void MPDU_OutputSwWid::on_pushButton_clicked()
{
    for(int i=0; i<24; ++i) {
        if(mWid[i]->select()) {
            sSetCmd cmd;
            cmd.addr = ui->spinBox->value();
            cmd.reg = 339 + i;
            cmd.value = mWid[i]->status();
            mRtu->setCmd(cmd);
        }
    }

    mRtu->start();
    ui->textEdit->clear();
}

void MPDU_OutputSwWid::on_openRadio_clicked(bool checked)
{
    ui->closeRadio->setChecked(false);
    for(int i=0; i<24; ++i) {
        mWid[i]->setOpen(checked);
    }
}

void MPDU_OutputSwWid::on_closeRadio_clicked(bool checked)
{
    ui->openRadio->setChecked(false);
    for(int i=0; i<24; ++i) {
        mWid[i]->setClose(checked);
    }
}
