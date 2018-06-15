#include "settingdialog.h"
#include "ui_settingdialog.h"

SettingDialog::SettingDialog(QWidget *parent, int currentindex, int datatype,int currentrow) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);
    index = currentindex;
    type = datatype;
    row = currentrow;

    initDialog();  //初始化界面
}

SettingDialog::~SettingDialog()
{
    delete ui;
}

void SettingDialog::initDialog()
{

    //    souce->setData(mUnit,row,buf);
    DataSouce *souce = get_datasouce();
    qDebug()<<"initDialog";
    parameterData *soucedata = souce->getSouceData();
    electricData *mData;
    environmentdata *envData;
    unit *mUnit;


    switch (index) {
    case 0:
        ui->label->setText(tr("输入相阈值设置"));
        ui->label_2->setText(tr("输入相名称:"));
        ui->label_5->setText(tr("L") + QString::number(row+1 ,10));
        mData = &(soucedata->inputPhrase);

        break;
    case 1:
        ui->label->setText(tr("回路阈值设置"));
        ui->label_2->setText(tr("回路名称:"));
        ui->label_5->setText(tr("C") + QString::number(row+1 ,10));
        mData = &(soucedata->loopData);
        break;
    case 2:
        ui->label->setText(tr("输出位阈值设置"));
        ui->label_2->setText(tr("输出位名称："));
        ui->label_5->setText(tr("output ") + QString::number(row+1 ,10));
        mData = &(soucedata->outData);
        break;
    case 3:
        ui->label->setText(tr("传感器阈值设置"));
        ui->label_2->setText(tr("传感器名称："));
        envData = &(soucedata->envData);
        //        ui->label_5->setText(tr("L") + QString::number(row+1 ,10));

        break;
    default:
        break;
    }

    switch (type) {
    case 0: //约定为电流
        initUnit(tr("A"));
        mUnit = &(mData->cur);
        break;
    case 1: //约定为电压
        initUnit(tr("V"));
        hideCir();
        mUnit = &(mData->vol);
        break;
    case 2: //约定为温度
        initUnit(tr("°C"));
        hideCir();
        ui->label_5->setText(tr("温度") + QString::number(row+1 ,10));
        mUnit = &(envData->tem);
        break;
    case 3: //约定为湿度
        initUnit(tr("%"));
        hideCir();
        ui->label_5->setText(tr("湿度") + QString::number(row+1 ,10));
        mUnit = &(envData->hum);
        break;
    default:
        break;
    }

    initData(mUnit);

}

void SettingDialog::initUnit(QString str)
{
    ui->label_6->setText(str);
    ui->label_7->setText(str);
    ui->label_9->setText(str);
    ui->label_11->setText(str);
}

/**
 * @brief 当为电压及温度湿度时，隐藏临界设置
 */
void SettingDialog::hideCir()
{
    ui->label_4->hide();
    ui->label_10->hide();

    ui->label_7->hide();
    ui->label_11->hide();

    ui->spinBox_2->hide();
    ui->spinBox_4->hide();
}

void SettingDialog::fillBuf(quint16 *buffer,quint8 *buf)
{
    int ret = 0;
    for(int i = 0 ; i < 4; i++)
    {
        *(buf + ret++) = *(buffer + i) >> 8 ;
        *(buf + ret++) = *(buffer + i) & 0xFF;
    }
}

/**
 * @brief 保存阈值设置信息
 */
void SettingDialog::on_pushButton_clicked()
{
    quint8 *buf = new quint8[12];
    quint16 *buffer = new quint16[12];

    DataSouce *souce = get_datasouce();
    qDebug()<<"on_pushButton_clicked";
    parameterData *soucedata = souce->getSouceData();
    electricData *mData;
    environmentdata *envData;
    unit *mUnit;

    switch (index) {
    case 0:

        mData = &(soucedata->inputPhrase);

        break;
    case 1:

        mData = &(soucedata->loopData);
        break;
    case 2:

        mData = &(soucedata->outData);
        break;
    case 3:

        envData = &(soucedata->envData);
        //        ui->label_5->setText(tr("L") + QString::number(row+1 ,10));

        break;
    default:
        break;
    }

    switch (type) {
    case 0: //约定为电流

        mUnit = &(mData->cur);
        break;
    case 1: //约定为电压

        mUnit = &(mData->vol);
        break;
    case 2: //约定为温度

        mUnit = &(envData->tem);
        break;
    case 3: //约定为湿度

        mUnit = &(envData->hum);
        break;
    default:
        break;
    }


    int offset = 0;
    *(buffer + offset++) = ui->spinBox->value();
    *(buffer + offset++) = ui->spinBox_3->value();
    *(buffer + offset++) = ui->spinBox_2->value();
    *(buffer + offset++) = ui->spinBox_4->value();

    fillBuf(buffer,buf);

    //操作mUnit即可，
    souce->setData(mUnit,row+1,buf);

}

/**
 * @brief 初始化数据
 * @param data
 */
void SettingDialog::initData(unit *data)
{
    ui->spinBox->setValue(*(data->minValue + row));
    ui->spinBox_3->setValue(*(data->maxValue + row));
    ui->spinBox_2->setValue(*(data->cirMinValue + row));
    ui->spinBox_4->setValue(*(data->cirMaxValue + row));
}

void SettingDialog::on_pushButton_2_clicked()
{
    this->close();
}
