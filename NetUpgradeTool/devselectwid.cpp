#include "devselectwid.h"
#include "ui_devselectwid.h"

DevSelectWid::DevSelectWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DevSelectWid)
{
    ui->setupUi(this);
    mData = DataPacket::bulid()->data;
    timer = new QTimer(this);
    timer->start(200);
    connect(timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
}

DevSelectWid::~DevSelectWid()
{
    delete ui;
}


void DevSelectWid::timeoutDone(void)
{
    this->setDisabled(mData->isRun);
}

void DevSelectWid::on_ipRadio_clicked(bool checked)
{
    if(checked) mData->devtype = 0;
}

void DevSelectWid::on_rzRadio_clicked(bool checked)
{
    if(checked) mData->devtype = 1;
}
