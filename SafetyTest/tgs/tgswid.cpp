#include "tgswid.h"
#include "ui_tgswid.h"

TgsWid::TgsWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TgsWid)
{
    ui->setupUi(this);
}

TgsWid::~TgsWid()
{
    delete ui;
}
