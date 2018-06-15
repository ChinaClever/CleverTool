#include "mytext.h"
#include "ui_mytext.h"
#include <QCoreApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QDateTime>

static data_json mJson;

MyText::MyText(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyText)
{
    ui->setupUi(this);

    QString dirName;
    QString iniFile = QCoreApplication::applicationDirPath() + "/config.ini";
    QSettings ini(iniFile, QSettings::IniFormat);
    ini.beginGroup("Value");
    nameValue = ini.value("NameValue", 1).toInt();
    dirName   = ini.value("DirName", "").toString();
    ini.endGroup();
    if(0 == nameValue){
        ini.beginGroup("Value");
        ini.setValue("NameValue", nameValue);
        ini.endGroup();
    }
    ui->pathLEdit->setText(dirName);

    initBackground();
    //initData();
    mFileName = QCoreApplication::applicationDirPath() + "/model.json";
    //const jsonRelated relatedData; // = get_relatedata();
    json *myJson = new json(mFileName);
    myJson->readJson(&mJson );
    initData();
    delete myJson;


    ui->lineEdit_serial_number->setFocus();
    connect(ui->lineEdit_serial_number, SIGNAL(returnPressed()), this, SLOT(on_pushButton_clicked()));
}

MyText::~MyText()
{
    delete ui;
}

/**
 * @brief 刷新mJson文件
 */
void MyText::updateJson()
{
    //工厂 + 加工线体
    strcpy(mJson.factory,ui->lineEdit_factory->text().toLatin1().data());
    strcpy(mJson.line,ui->lineEdit_line->text().toLatin1().data());

    //uut_info_mJson
    strcpy(mJson.uut_info.work_order,ui->lineEdit_work_order->text().toLatin1().data());
    strcpy(mJson.uut_info.uut_type,ui->lineEdit_uut_type->text().toLatin1().data());
    strcpy(mJson.uut_info.uut_name,ui->lineEdit_uut_name->text().toLatin1().data());
    strcpy(mJson.uut_info.uut_code,ui->lineEdit_uut_code->text().toLatin1().data());
    strcpy(mJson.uut_info.serial_number,ui->lineEdit_serial_number->text().toLatin1().data());
    strcpy(mJson.uut_info.supplier,ui->lineEdit_supplier->text().toLatin1().data());
    strcpy(mJson.uut_info.date_code,ui->lineEdit_date_code->text().toLatin1().data());
    strcpy(mJson.uut_info.lot_code,ui->lineEdit_lot_code->text().toLatin1().data());
    strcpy(mJson.uut_info.mould,ui->lineEdit_mould->text().toLatin1().data());
    strcpy(mJson.uut_info.cavity,ui->lineEdit_cavity->text().toLatin1().data());
    strcpy(mJson.uut_info.color,ui->lineEdit_color->text().toLatin1().data());

    //ate_info_mJson
    strcpy(mJson.ate_info.ate_name,ui->lineEdit_ate_name->text().toLatin1().data());
    strcpy(mJson.ate_info.computer_name,ui->lineEdit_computer_name->text().toLatin1().data());
    strcpy(mJson.ate_info.fixuer_id,ui->lineEdit_fixuer_id->text().toLatin1().data());

    //program_info_mJson
    strcpy(mJson.program_info.program_name,ui->lineEdit_program_name->text().toLatin1().data());
    strcpy(mJson.program_info.program_ver,ui->lineEdit_program_ver->text().toLatin1().data());

    //uut_result_mJson
    strcpy(mJson.uut_result.operatorx,ui->lineEdit_operator->text().toLatin1().data());
    strcpy(mJson.uut_result.operation_sequence,ui->lineEdit_operation_sequence->text().toLatin1().data());
    strcpy(mJson.uut_result.site_code,ui->lineEdit_site_code->text().toLatin1().data());
    //     strcpy(mJson.uut_result.operatorx,ui->lineEdit_operator->text());
    //     strcpy(mJson.uut_result.operatorx,ui->lineEdit_operator->text());
    //     strcpy(mJson.uut_result.operatorx,ui->lineEdit_operator->text());

}

/**
 * @brief 点击保存按钮
 */
void MyText::on_pushButton_clicked()
{   
    QString dirName = ui->pathLEdit->text();
    if(dirName.isEmpty()) {
        QMessageBox::information(this,tr("waring"),tr("请设置保存路径"),tr("确定"));
        return;
    }

    updateJson();
    const jsonRelated relatedData = get_relatedata();
    QString cfileName;
    if(ui->checkBox->isChecked()){  //保存模板
        cfileName = mFileName;
    }else{
        QString name;
        if(0 == nameValue){ //物料编码
            name = ui->lineEdit_uut_code->text() + ".json";
        }else if(1 == nameValue){ //物料条码
            name = ui->lineEdit_serial_number->text() + ".json";
        }else{ //时间
            QDateTime currentTime = QDateTime::currentDateTime();
            //yyyy-MM-dd hh:mm:ss.zzz
            name  = currentTime.toString("hh:mm:ss") + ".json";
        }

        cfileName = dirName + "/" + name;
    }

    json *myJson = new json(cfileName);
    myJson->writeJson(&mJson , relatedData);
    delete myJson;

    this->close();
}

/**
 * @brief 初始化界面
 */
void MyText::initData()
{
    ui->lineEdit_factory->setText(QString(mJson.factory));
    ui->lineEdit_line->setText(QString(mJson.line));

    //uut_info_mJson
    ui->lineEdit_work_order->setText(QString(mJson.uut_info.work_order));
    ui->lineEdit_uut_type->setText(QString(mJson.uut_info.uut_type));
    ui->lineEdit_uut_name->setText(QString(mJson.uut_info.uut_name));
    ui->lineEdit_uut_code->setText(QString(mJson.uut_info.uut_code));
    ui->lineEdit_serial_number->setText(QString(mJson.uut_info.serial_number));
    ui->lineEdit_supplier->setText(QString(mJson.uut_info.supplier));
    ui->lineEdit_date_code->setText(QString(mJson.uut_info.date_code));
    ui->lineEdit_lot_code->setText(QString(mJson.uut_info.lot_code));
    ui->lineEdit_mould->setText(QString(mJson.uut_info.mould));
    ui->lineEdit_cavity->setText(QString(mJson.uut_info.cavity));
    ui->lineEdit_color->setText(QString(mJson.uut_info.color));

    //ate_info_mJson
    ui->lineEdit_ate_name->setText(QString(mJson.ate_info.ate_name));
    ui->lineEdit_computer_name->setText(QString(mJson.ate_info.computer_name));
    ui->lineEdit_fixuer_id->setText(QString(mJson.ate_info.fixuer_id));

    //program_info_mJson
    ui->lineEdit_program_name->setText(QString(mJson.program_info.program_name));
    ui->lineEdit_program_ver->setText(QString(mJson.program_info.program_ver));

    //uut_result_mJson
    ui->lineEdit_operator->setText(QString(mJson.uut_result.operatorx));
    ui->lineEdit_operation_sequence->setText(QString(mJson.uut_result.operation_sequence));
    ui->lineEdit_site_code->setText(QString(mJson.uut_result.site_code));
}

void MyText::initBackground()
{
    this->setAutoFillBackground(true);
    QPalette palette;
    QPixmap map = QPixmap(":/new/prefix1/image/back.jpg").scaled(700,700);
    palette.setBrush(QPalette::Background,QBrush(map));  //设置背景图片
    this->setPalette(palette);

}


void MyText::on_pushButton_2_clicked()
{
    QString dirName = QFileDialog::getExistingDirectory();
    if(dirName.isEmpty()) {
        QMessageBox::information(this,tr("waring"),tr("未选择路径"),tr("确定"));
        return;
    }
    ui->pathLEdit->setText(dirName);

    QString iniFile = QCoreApplication::applicationDirPath() + "/config.ini";
    QSettings ini(iniFile, QSettings::IniFormat);
    ini.beginGroup("Value");
    ini.setValue("DirName", dirName);
    ini.endGroup();
}
