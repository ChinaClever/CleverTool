#include "mpdu_outputthresholdwid.h"
#include "ui_mpdu_outputthresholdwid.h"

MPDU_OutputThresholdWid::MPDU_OutputThresholdWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MPDU_OutputThresholdWid)
{
    ui->setupUi(this);
    mReg = 157;
}

MPDU_OutputThresholdWid::~MPDU_OutputThresholdWid()
{
    delete ui;
}


void MPDU_OutputThresholdWid::initwid(int mode, SerialPort *serial)
{
    QWidget *wid[] = {ui->widget_1, ui->widget_2, ui->widget_3, ui->widget_4, ui->widget_5,ui->widget_6,
                      ui->widget_7, ui->widget_8, ui->widget_9, ui->widget_10, ui->widget_11, ui->widget_12,
                      ui->widget_13,ui->widget_14, ui->widget_15, ui->widget_16,ui->widget_17,ui->widget_18,
                      ui->widget_19,ui->widget_20,ui->widget_21,ui->widget_22,ui->widget_23,ui->widget_24};

    for(int i=0; i<24; ++i) {
        mWid[i] = new MPDU_OutputThresholdItemWid(wid[i]);
        mWid[i]->init(i, mode);
//        wid[i]->setStyleSheet("background-color: rgb(255, 255, 255);");
    }

    if(mode) mReg = 229;
    mRtu = new SI_RtuThread(this);
    mRtu->init(serial);
    connect(mRtu, SIGNAL(cmdSig(QString)), this, SLOT(updateTextSlot(QString)));
    mRtu->mreg = mReg;
}

void MPDU_OutputThresholdWid::updateTextSlot(QString str)
{
    ui->textEdit->append(str);
}

void MPDU_OutputThresholdWid::on_checkBox_clicked(bool checked)
{
    for(int i=0; i<24; ++i) {
        mWid[i]->setSelect(checked);
    }
    on_curDoubleSpinBox_valueChanged(ui->curDoubleSpinBox->value());
}

void MPDU_OutputThresholdWid::on_pushButton_clicked()
{
    for(int i=0; i<24; ++i) {
        if(mWid[i]->select()) {
            sSetCmd cmd;
            cmd.addr = ui->spinBox->value();
            cmd.reg = mReg + i;
            cmd.value = mWid[i]->status();
            mRtu->setCmd(cmd);
        }
    }

    mRtu->start();
    ui->textEdit->clear();
}


void MPDU_OutputThresholdWid::on_curDoubleSpinBox_valueChanged(double arg1)
{
    if(ui->checkBox->isChecked())
    {
        for(int i=0; i<24; ++i)
            mWid[i]->setValue(arg1);
    }
}
