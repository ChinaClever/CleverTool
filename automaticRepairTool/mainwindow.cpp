#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTableWidgetItem>

#define COUNT 8 //读取几次 最后一次统计
#define OFF_COUNT 2 //离线计数[取板用]
#define PASS_COUNT 2 //符合结果计数

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
    initIni();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    mSerialPortPlateWid = new SerialPortWid(ui->gBoxAWid);
    mSerialPortSourceWid = new SerialPortWid(ui->gBoxBWid);
    mSerialPortPlateWid->setBaudSlot(3); //默认波特率
    mSerialPortSourceWid->setBaudSlot(2); //默认波特率

    mRtuThread = new RtuThread(this);
    mRtuThread->mSerialPortPlate = mSerialPortPlateWid->getSerialPort();
    mRtuThread->mSerialPortSource = mSerialPortSourceWid->getSerialPort();
    mRtuThread->mCount = COUNT;
    mRtuThread->mOffCount = OFF_COUNT;
    connect(mRtuThread, SIGNAL(showDataSignal(int,ReturnTableData*)), this, SLOT(onGetShowData(int,ReturnTableData*)));
    connect(mRtuThread, SIGNAL(sendStation(QString)), this, SLOT(onGetStation(QString)));
    connect(mRtuThread, SIGNAL(sendEnd()), this, SLOT(onEnd()));
    connect(mRtuThread, SIGNAL(sendClear()), this, SLOT(onClear()));
    connect(mRtuThread, SIGNAL(sendTime(QString)), this, SLOT(onUpdataTime(QString)));
    ui->cBoxC1->setCurrentIndex(0); //默认执行板
    ui->cBoxC2->setCurrentIndex(1); //默认校准模式

    mSum = OKSum = NGSum = 0; //统计
    mCurOk = false; //当前次产品状态
    mPassSum = 0;

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);  //设置不可编辑
}

void MainWindow::initIni()
{
    double Amin;
    double Amax;
    double Wmin;
    double Wmax;
    int addrBox;
    int typeBox;

    QString iniFile = QCoreApplication::applicationDirPath() + "/config.ini";
    QSettings ini(iniFile, QSettings::IniFormat);
    ini.beginGroup("Value");
    Amin = ini.value("Amin", 5.5).toDouble();
    Amax = ini.value("Amax", 5.6).toDouble();
    Wmin = ini.value("Wmin", 130).toDouble();
    Wmax = ini.value("Wmax", 133).toDouble();
    addrBox = ini.value("Addr", 1).toInt();
    typeBox = ini.value("Type", 2).toInt();
    ini.endGroup();

    ui->minAEdit->setText(QString::number(Amin));
    ui->maxAEdit->setText(QString::number(Amax));
    ui->minWEdit->setText(QString::number(Wmin));
    ui->maxWEdit->setText(QString::number(Wmax));
    ui->cBoxC1->setCurrentIndex(addrBox -1);
    ui->cBoxC2->setCurrentIndex(typeBox -1);

    connect(ui->minAEdit, SIGNAL(textChanged(QString)), this, SLOT(onChangedIni()));
    connect(ui->maxAEdit, SIGNAL(textChanged(QString)), this, SLOT(onChangedIni()));
    connect(ui->minWEdit, SIGNAL(textChanged(QString)), this, SLOT(onChangedIni()));
    connect(ui->maxWEdit, SIGNAL(textChanged(QString)), this, SLOT(onChangedIni()));
    connect(ui->cBoxC1, SIGNAL(currentIndexChanged(QString)), this, SLOT(onChangedIni()));
    connect(ui->cBoxC2, SIGNAL(currentIndexChanged(QString)), this, SLOT(onChangedIni()));
}

void MainWindow::saveIni()
{
    double Amin = ui->minAEdit->text().toDouble();
    double Amax = ui->maxAEdit->text().toDouble();
    double Wmin = ui->minWEdit->text().toDouble();
    double Wmax = ui->maxWEdit->text().toDouble();
    int addrBox = ui->cBoxC1->currentIndex() + 1;
    int typeBox = ui->cBoxC2->currentIndex() + 1;

    QString iniFile = QCoreApplication::applicationDirPath() + "/config.ini";
    QSettings ini(iniFile, QSettings::IniFormat);
    ini.beginGroup("Value");
    ini.setValue("Amin", Amin);
    ini.setValue("Amax", Amax);
    ini.setValue("Wmin", Wmin);
    ini.setValue("Wmax", Wmax);
    ini.setValue("Addr", addrBox);
    ini.setValue("Type", typeBox);
    ini.endGroup();
}

void MainWindow::on_cBoxC1_currentIndexChanged(int index)
{
    mRtuThread->mAddr = index+1;
}

void MainWindow::on_cBoxC2_currentIndexChanged(int index)
{
    mRtuThread->mMethodType = index+1;
}

void MainWindow::on_startOrStop_clicked()
{
    if(mRtuThread->getRun()){ //STOP
        mRtuThread->stopThread();
        ui->startOrStop->setText("Start");
        ui->gBoxA->setEnabled(true);
        ui->gBoxB->setEnabled(true);
        ui->gBoxC->setEnabled(true);
    }else{ //start
        if(mRtuThread->mSerialPortPlate->isOpened()) {
                mRtuThread->startThread();
                ui->startOrStop->setText("Stop");
                ui->gBoxA->setEnabled(false);
                ui->gBoxB->setEnabled(false);
                ui->gBoxC->setEnabled(false);
            } else {
                 CriticalMsgBox box(this, tr("请打开串口"));
            }
    }
    ui->pushButton->setFocus(); //设置焦点
}

void MainWindow::onGetStation(QString str)
{
    ui->station->setText(str);
}

/** 线程过来的信号 */
void MainWindow::onEnd()
{
    //记录统计
    mSum++;
    if(mCurOk){
        OKSum++;
    }else{
        NGSum++;
    }
    ui->sumLabel->setText(tr("总数：%1  通过：%2  失败：%3").arg(mSum).arg(OKSum).arg(NGSum));
    mCurOk = false;
}

void MainWindow::onClear()
{
    //各种清空 | 同时记录统计
    ui->textEdit->clear();
    ui->tableWidget->clearContents();
    ui->PassLabel->clear();
}

void MainWindow::onUpdataTime(QString str)
{
    ui->timeLast->setText(ui->timeFirst->text());
    ui->timeFirst->setText(str);
}

void MainWindow::on_resBtu_clicked()
{
    mRtuThread->initState();
}

void MainWindow::onGetShowData(int value, ReturnTableData *data)
{
    ReturnTableData cData = *data;
    delete data;
    for(int i = 0 ; i < 8 ;i++ )
    {
       // int j = 0;
        setOnOffState(i, 0, cData); //开关
        setItem(i, 1, cData.current[i][0], cData.current[i][1], 10); //电流
        setItem(i, 2, cData.volate[i][0], cData.volate[i][1], 1); //电压
        setItem(i, 3, cData.power[i][0], cData.power[i][1], 1); //功率
    }

    if(setAll(cData)) mPassSum++;
    if(value == COUNT) {
        mCurOk = mPassSum >= PASS_COUNT ? true : false; //最后一次用于统计
        QPalette pa;
        if(mCurOk) {
            pa.setColor(QPalette::WindowText,Qt::green);
            ui->PassLabel->setPalette(pa);
            ui->PassLabel->setText("合格");

        }else{
            pa.setColor(QPalette::WindowText,Qt::red);
            ui->PassLabel->setPalette(pa);
            ui->PassLabel->setText("缺陷");
        }
        mPassSum = 0;
    }
}

/**
 * @brief 开关状态显示
 */
void MainWindow::setOnOffState(int row, int column, ReturnTableData &data)
{
    QTableWidgetItem *item = new QTableWidgetItem("Apple");
    bool state = (data.onoffState>>(7-row))&1; //判断该位，先将该位移到第一位，再与1相与，则第一位不变，其他位置0
    if(state){
        item->setText("打开");
    }else{
        item->setText("关闭");
    }
    ui->tableWidget->setItem(row, column,item);
}

/**
 * @brief 设置值
 * @param row
 * @param column
 */
bool MainWindow::setItem(int row, int column , quint8 valueH, quint8 valueD, int rate)
{
    int data = (valueH<<8 | valueD)/rate;
    QString str = QString("%1.%2").arg(data/10).arg(data%10);
    QTableWidgetItem *item = new QTableWidgetItem("Apple");
    item->setText(str);
    ui->tableWidget->setItem(row, column,item);
    return true;
}

bool MainWindow::setAll(ReturnTableData &data)
{
    bool ok = true;
    QTableWidgetItem *itemA = new QTableWidgetItem("Apple");
    QTableWidgetItem *itemW = new QTableWidgetItem("Apple");
    int dataA = 0;
    int dataW = 0;
    for(int i = 0; i < 8; i++){
        int data1 = (data.current[i][0]<<8 | data.current[i][1])/10;
        dataA += data1;
        int data2 = data.power[i][0]<<8 | data.power[i][1];
        dataW += data2;
    }

    QString str = QString("%1.%2").arg(dataA/10).arg(dataA%10);
    if(ui->minAEdit->text().toDouble() <= str.toDouble() &&
       ui->maxAEdit->text().toDouble() >= str.toDouble() ){
        itemA->setForeground(QBrush(QColor(0, 0, 0)));
    }else{
        ok = false;
        itemA->setForeground(QBrush(QColor(255, 0, 0)));
    }
    itemA->setText(str+ "A");

    /* 记录最后一次结果 */
    ui->textEdit->insertPlainText("[" + str+ "A  ");
    //-------------------

    str = QString("%1.%2").arg(dataW/10).arg(dataW%10);
    if(ui->minWEdit->text().toDouble() <= str.toDouble() &&
       ui->maxWEdit->text().toDouble() >= str.toDouble() ){
        itemW->setForeground(QBrush(QColor(0, 0, 0)));
    }else{
        ok = false;
        itemW->setForeground(QBrush(QColor(255, 0, 0)));
    }
    itemW->setText(str+ "KW ");

    /* 记录最后一次结果 */
    ui->textEdit->insertPlainText(str+ "KW]");

    //-------------------

    ui->tableWidget->setItem(8, 1,itemA);
    ui->tableWidget->setItem(8, 3,itemW);

    return ok;
}


void MainWindow::sleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MainWindow::on_pushButton_clicked()
{
    for(int n = 0; n < 3; n++){
        QByteArray array;
        QString data;
        if(n == 0) { //量程
            data ="I0 220 6 15 1200";
        }else if(n == 1){ //V
            data = "V100";
        }else if(n == 2){  //A
            data = "A100";
        }
        int len = data.length();
        for(int i = 0; i < len; i++){
            array.append(data.at(i));
        }
        array.append(0x0D);
        qDebug() << array.toHex();
        if(mSerialPortSourceWid->getSerialPort()->isOpened()) //串口是打开状态
            mSerialPortSourceWid->getSerialPort()->write(array);
        sleep(200);
        if(n == 1) sleep(5000);
    }
}

void MainWindow::onChangedIni()
{
    saveIni();
}
