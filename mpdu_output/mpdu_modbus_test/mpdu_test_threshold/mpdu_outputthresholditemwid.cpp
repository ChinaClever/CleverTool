#include "mpdu_outputthresholditemwid.h"
#include "ui_mpdu_outputthresholditemwid.h"
#include "common.h"

MPDU_OutputThresholdItemWid::MPDU_OutputThresholdItemWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MPDU_OutputThresholdItemWid)
{
    ui->setupUi(this);
    com_setBackColour("bb", this);
}

MPDU_OutputThresholdItemWid::~MPDU_OutputThresholdItemWid()
{
    delete ui;
}

void MPDU_OutputThresholdItemWid::init(int id, int mode)
{
    QString str = tr("小");
    if(mode) str = tr("大");

    ui->checkBox->setText(tr(" %1 输出位电流最%2值").arg(id+1).arg(str));
}


void MPDU_OutputThresholdItemWid::setSelect(bool checked)
{
    ui->checkBox->setChecked(checked);
}

bool MPDU_OutputThresholdItemWid::select()
{
    return ui->checkBox->isChecked();
}

void MPDU_OutputThresholdItemWid::setValue(double value)
{
    ui->spinBox->setValue(value);
}


int MPDU_OutputThresholdItemWid::status()
{
    int ret = -1;
    if(ui->checkBox->isChecked())
        ret = ui->spinBox->value()*10;

    return ret;
}

void MPDU_OutputThresholdItemWid::on_spinBox_valueChanged(int arg1)
{
    ui->checkBox->setChecked(true);
}
