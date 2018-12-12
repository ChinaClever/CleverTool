#include "outputwid.h"
#include "ui_outputwid.h"
#include "common.h"

OutputWid::OutputWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OutputWid)
{
    ui->setupUi(this);
    ui->openRadio->setChecked(true);
    com_setBackColour("", this);
}

OutputWid::~OutputWid()
{
    delete ui;
}

void OutputWid::init(int id)
{
    ui->checkBox->setText(tr(" %1 输出位").arg(id+1));
}

void OutputWid::setSelect(bool checked)
{
    ui->checkBox->setChecked(checked);
}

bool OutputWid::select()
{
    return ui->checkBox->isChecked();
}

void OutputWid::setOpen(bool checked)
{
    ui->openRadio->setChecked(checked);
}

void OutputWid::setClose(bool checked)
{
     ui->closeRadio->setChecked(checked);
}

int OutputWid::status()
{
    int ret = 0;
    if(ui->openRadio->isChecked())
        ret = 1;
    return ret;
}
