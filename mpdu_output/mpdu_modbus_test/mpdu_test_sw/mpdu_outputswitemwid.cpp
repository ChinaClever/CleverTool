#include "mpdu_outputswitemwid.h"
#include "ui_mpdu_outputswitemwid.h"
#include "common.h"

MPDU_OutputSwItemWid::MPDU_OutputSwItemWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MPDU_OutputSwItemWid)
{
    ui->setupUi(this);
    ui->openRadio->setChecked(true);
    com_setBackColour("", this);
}

MPDU_OutputSwItemWid::~MPDU_OutputSwItemWid()
{
    delete ui;
}


void MPDU_OutputSwItemWid::init(int id)
{
    ui->checkBox->setText(tr(" %1 输出位").arg(id+1));
}

void MPDU_OutputSwItemWid::setSelect(bool checked)
{
    ui->checkBox->setChecked(checked);
}

bool MPDU_OutputSwItemWid::select()
{
    return ui->checkBox->isChecked();
}

void MPDU_OutputSwItemWid::setOpen(bool checked)
{
    ui->openRadio->setChecked(checked);
}

void MPDU_OutputSwItemWid::setClose(bool checked)
{
     ui->closeRadio->setChecked(checked);
}

int MPDU_OutputSwItemWid::status()
{
    int ret = 0;
    if(ui->openRadio->isChecked())
        ret = 1;
    return ret;
}
