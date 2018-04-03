#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "udp/datasent.h"
static devSetings setings;

int gCount=0;

/**
 * @brief 提供一个接口函数，供初始化数据选择设备配置
 * @return
 */
devSetings *get_devsetings()
{
    return &setings;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    souce = get_datasouce();
    qDebug()<<"MainWindow";
    getLocalIp();
    initThread();  //初始化tcp、udp线程
    initWidget();  //初始化表格

    initdevSeetings();   //初始化设备设置
    souce->changeDevSettings();


    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timeoutdone()));
    timer->start(4*1000); //时间必须比心跳包线程慢，心跳包线程会清零心跳包

    heartbeat = get_HeartBeat();
    connect(heartbeat,SIGNAL(receiveMessage(QString)),this,SLOT(initData(QString)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief 保存设备设置，检测输入是否符合格式要求
 */
void MainWindow::on_pushButton_clicked()
{
    if( !(isDigit(ui->lineEdit_2->text()) &&  isDigit(ui->lineEdit_3->text()) &&  isDigit(ui->lineEdit_4->text()) &&  isDigit(ui->lineEdit_5->text()) &&  isDigit(ui->lineEdit_6->text()) ))
        QMessageBox::warning(this,tr("waring"),tr("PDU配置有误，请检查！"),tr("确定"));
    else
    {

        initdevSeetings();
        souce->changeDevSettings();


    }
}

void MainWindow::timeoutdone()
{
    //    souceData_To_protcldata();

    //获取原始数据
    //    souce = new DataSouce;
    updateData();

    gCount--;
    if(gCount<=0 ) {
        ui->label_4->setText(tr("无连接！"));
        ui->label_2->clear();
    }
    //    initData();
}

void MainWindow::initThread()
{
    MyUdpThread *udpthread = new MyUdpThread;
    TcpThread *tcpthread = new TcpThread;
    GlobalReceiveThread *receivethread = new GlobalReceiveThread;

}

void MainWindow:: initData(QString str)
{
    //    HeartBeat *heartbeat = get_HeartBeat();

    //    qDebug()<<"ip:"<<heartbeat->getAddr();
    this->ui->label_2->setText(heartbeat->getAddr().toString().remove(0,7));

    if(str.contains("Clever-Manager PDU PC Server OK!")) {
        this->ui->label_4->setText(tr("连接正常！"));
        gCount = 3;
    }
    else
    {
        //        ui->label_4->setText(tr("无连接！"));
        //        ui->label_2->clear();
    }



}

void MainWindow::initWidget()
{
    this->ui->tabWidget->setCurrentIndex(0);

    initPhaseWidget();
    initLoopWidget();
    initOutWidget();
    initSensorWidget();
}

void MainWindow::initPhaseWidget()
{
    QGridLayout *mainlayout = new QGridLayout(this->ui->phasewidget);
    QSplitter *mainspliter = new QSplitter(Qt::Vertical,this);
    mainlayout->addWidget(mainspliter);

    /*上下分割*/
    QWidget *upwidget = new QWidget(this);
    QWidget *downwidget = new QWidget(this);
    mainspliter->addWidget(upwidget);
    mainspliter->addWidget(downwidget);

    /*上部分*/
    phasewidget_1 = new QTableWidget(this);
    QStringList verheader,horheader;
    horheader<<tr("输入相")<<tr("电压")<<tr("电流")<<tr("功率")<<tr("电能")<<tr("功率因素");
    phasewidget_1->setRowCount(0); //行
    phasewidget_1->setColumnCount(horheader.size()); //列
    phasewidget_1->setHorizontalHeaderLabels(horheader);
    phasewidget_1->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); //水平自动调整宽度
    //      phasewidget_1->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    phasewidget_1->horizontalHeader()->setStretchLastSection(true);
    phasewidget_1->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑


    QHBoxLayout *layout1 = new QHBoxLayout(upwidget);
    layout1->addWidget(phasewidget_1);

    /*下部分左右分割*/
    QSplitter *downspliter = new QSplitter(Qt::Horizontal,this);
    QGridLayout *downlayout = new QGridLayout(downwidget);
    downlayout->addWidget(downspliter);

    QWidget *leftwidget = new QWidget(this);
    QWidget *rightwidget = new QWidget(this);

    downspliter->addWidget(leftwidget);
    downspliter->addWidget(rightwidget);
#if 1
    /*左*/
    QLabel *label_1 = new QLabel(tr("相电压阈值设置"));
    lefttablewidget = new QTableWidget(this);
    QStringList verheader1,horheader1;
    horheader1<<tr("输入相")<<tr("最小值")<<tr("最大值");
    lefttablewidget->setRowCount(0); //行
    lefttablewidget->setColumnCount(horheader1.size()); //列
    lefttablewidget->setHorizontalHeaderLabels(horheader1);
    lefttablewidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); //水平自动调整宽度
    lefttablewidget->horizontalHeader()->setStretchLastSection(false);
    lefttablewidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑

    QGridLayout *leftlayout = new QGridLayout(leftwidget);

    leftlayout->addWidget(label_1);
    leftlayout->addWidget(lefttablewidget);

    connect(lefttablewidget,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(setThresholdAdd(QModelIndex)));

    /*右*/
    QLabel *label_2 = new QLabel(tr("相电流阈值设置"));
    righttablewidget = new QTableWidget(this);
    QStringList verheader2,horheader2;
    horheader2<<tr("输入相")<<tr("最小值")<<tr("下限界")<<tr("上限界")<<tr("最大值");
    righttablewidget->setRowCount(0); //行
    righttablewidget->setColumnCount(horheader2.size()); //列
    righttablewidget->setHorizontalHeaderLabels(horheader2);
    righttablewidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); //水平自动调整宽度
    righttablewidget->horizontalHeader()->setStretchLastSection(false);
    righttablewidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑

    QGridLayout *rightlayout = new QGridLayout(rightwidget);

    rightlayout->addWidget(label_2);
    rightlayout->addWidget(righttablewidget);

    connect(righttablewidget,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(setThreshold(QModelIndex)));
#endif

}

void MainWindow::initLoopWidget()
{

    QGridLayout *mainlayout = new QGridLayout(this->ui->loopwidget);

    loopwidget = new QTableWidget(this);
    QStringList verheader,horheader;
    horheader<<tr("回路")<<tr("空开")<<tr("电流")<<tr("功率")<<tr("电能")<<tr("功率因素")<<tr("最小值")<<tr("下限界")<<tr("上限界")<<tr("最大值");
    loopwidget->setRowCount(0); //行
    loopwidget->setColumnCount(horheader.size()); //列
    loopwidget->setHorizontalHeaderLabels(horheader);
    loopwidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); //水平自动调整宽度
    loopwidget->horizontalHeader()->setStretchLastSection(true);
    loopwidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑

    mainlayout->addWidget(loopwidget);

    connect(loopwidget,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(setThreshold(QModelIndex)));
}

void MainWindow::initOutWidget()
{
    QGridLayout *mainlayout = new QGridLayout(this->ui->outwidget);

    outwidget = new QTableWidget(this);
    QStringList verheader,horheader;
    horheader<<tr("输出位")<<tr("开关状态")<<tr("电流")<<tr("功率")<<tr("电能")<<tr("功率因素")<<tr("最小值")<<tr("下限界")<<tr("上限界")<<tr("最大值");
    outwidget->setRowCount(0); //行
    outwidget->setColumnCount(horheader.size()); //列
    outwidget->setHorizontalHeaderLabels(horheader);
    outwidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); //水平自动调整宽度
    outwidget->horizontalHeader()->setStretchLastSection(true);
    outwidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑

    mainlayout->addWidget(outwidget);

    connect(outwidget,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(setThreshold(QModelIndex)));
}

void MainWindow::initSensorWidget()
{
    QVBoxLayout *mainlayout =new QVBoxLayout(this->ui->sensorwidget);

    /*上部分布局*/
    QHBoxLayout *uplayout = new QHBoxLayout(this->ui->sensorwidget);

    temwidget = new QTableWidget(this);
    QStringList verheader,horheader;
    horheader<<tr("温度")<<tr("状态")<<tr("最小值")<<tr("最大值");
    temwidget->setRowCount(0); //行
    temwidget->setColumnCount(horheader.size()); //列
    temwidget->setHorizontalHeaderLabels(horheader);
    temwidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); //水平自动调整宽度
    temwidget->horizontalHeader()->setStretchLastSection(true);
    temwidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑

    humwidget = new QTableWidget(this);
    QStringList verheader1,horheader1;
    horheader1<<tr("湿度")<<tr("状态")<<tr("最小值")<<tr("最大值");
    humwidget->setRowCount(0); //行
    humwidget->setColumnCount(horheader.size()); //列
    humwidget->setHorizontalHeaderLabels(horheader1);
    humwidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); //水平自动调整宽度
    humwidget->horizontalHeader()->setStretchLastSection(true);
    humwidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑

    uplayout->addWidget(temwidget);
    uplayout->addWidget(humwidget);

    connect(temwidget,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(setThresholdAddTem(QModelIndex)));
    connect(humwidget,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(setThresholdAddHum(QModelIndex)));



    /*下部分布局*/
    QGridLayout *downlayout = new QGridLayout(this->ui->sensorwidget);

    /*左*/
    QGridLayout *layout1 = new QGridLayout(this->ui->sensorwidget);
    QLabel *label1 = new QLabel(tr("门禁1"));
    QRadioButton *openbox1 = new QRadioButton(tr("开"));
    QRadioButton *openbox2 = new QRadioButton(tr("关"));
    openbox2->setChecked(true);
    QButtonGroup *group1 = new QButtonGroup;
    group1->addButton(openbox1,1);
    group1->addButton(openbox2,0);

    QLabel *label2 = new QLabel(tr("门禁2"));
    QRadioButton *openbox3 = new QRadioButton(tr("开"));
    QRadioButton *openbox4 = new QRadioButton(tr("关"));
    openbox4->setChecked(true);
    QButtonGroup *group2 = new QButtonGroup;
    group2->addButton(openbox3,1);
    group2->addButton(openbox4,0);

    QLabel *spaceLabel = new QLabel(tr("                                                         "));  //
    spaceLabel->setFixedWidth(100);
    //    layout1->addWidget(spaceLabel,0,0);
    layout1->addWidget(label1,0,1);
    layout1->addWidget(openbox1,0,2);
    layout1->addWidget(openbox2,0,3);
    //    layout1->addWidget(spaceLabel,0,4);

    //    layout1->addWidget(spaceLabel,1,0);
    layout1->addWidget(label2,1,1);
    layout1->addWidget(openbox3,1,2);
    layout1->addWidget(openbox4,1,3);
    //    layout1->addWidget(spaceLabel,1,4);

    layout1->setSpacing(0);
    layout1->setContentsMargins(100,0,20,0);

    /*右*/
    QGridLayout *layout2 = new QGridLayout(this->ui->sensorwidget);
    QLabel *label_1 = new QLabel(tr("烟雾"));
    QRadioButton *openbox_1 = new QRadioButton(tr("开"));
    QRadioButton *openbox_2 = new QRadioButton(tr("关"));
    openbox_2->setChecked(true);
    QButtonGroup *group3 = new QButtonGroup;
    group3->addButton(openbox_1,1);
    group3->addButton(openbox_2,0);

    QLabel *label_2 = new QLabel(tr("水侵"));
    QRadioButton *openbox_3 = new QRadioButton(tr("开"));
    QRadioButton *openbox_4 = new QRadioButton(tr("关"));
    openbox_4->setChecked(true);
    QButtonGroup *group4 = new QButtonGroup;
    group4->addButton(openbox_3,1);
    group4->addButton(openbox_4,0);

    layout2->addWidget(label_1,0,0);
    layout2->addWidget(openbox_1,0,1);
    layout2->addWidget(openbox_2,0,2);

    layout2->addWidget(label_2,1,0);
    layout2->addWidget(openbox_3,1,1);
    layout2->addWidget(openbox_4,1,2);

    layout2->setContentsMargins(100,0,20,0);

    downlayout->addLayout(layout1,0,0);
    downlayout->addLayout(layout2,0,1);

    mainlayout->addLayout(uplayout);
    mainlayout->addLayout(downlayout);

    //单选框的槽函数

    connect(group1,SIGNAL(buttonClicked(int)),this,SLOT(radioButtonEntaanceOneClicked(int)));
    connect(group2,SIGNAL(buttonClicked(int)),this,SLOT(radioButtonEntaanceTwoClicked(int)));
    connect(group3,SIGNAL(buttonClicked(int)),this,SLOT(radioButtonSmokeClicked(int)));
    connect(group4,SIGNAL(buttonClicked(int)),this,SLOT(radioButtonWaterClicked(int)));
}

/**
 * @brief 更新界面数据
 */
void MainWindow::updateData()
{
    souceData = souce->getSouceData();
    updatePhaseData();
    updateLoopData();
    updateOutData();
    updateSensorData();

}

/**
 * @brief 更新相位界面数据
 */
void MainWindow::updatePhaseData()
{
    updatePhaseOneData();
    updatePhaseTwoData();
    updatePhaseThreeData();

}

/**
 * @brief --------------------------------------更新相表格1数据---------------------------------------------------------
 */
void MainWindow::updatePhaseOneData()
{
    initTableWiget(phasewidget_1,souceData->inputPhrase.len);  //3位3相，根据实际相数传参

    for(int i = 0 ; i < (phasewidget_1->rowCount()); i++ )
    {
        int column = 0 ;
        setPhaseName(i,  column++ ,phasewidget_1);
        setPhaseVol( i,  column++);
        setPhaseCur( i , column++);
        setPhasePower( i , column++);
        setPhaseEngry( i,  column++);
        setPhasePowerfactor( i,  column);

    }

}

/**
 * @brief 设置相位名称
 */
void MainWindow::setPhaseName(int row ,int count ,QTableWidget *widget)
{
    QTableWidgetItem *item = widget->item(row,count);
    QString str = tr("L") + QString::number(row + 1,10);

    item->setText(str);
}

void MainWindow::setPhaseVol(int row ,int count)
{
    QTableWidgetItem *item = phasewidget_1->item(row,count);
    int value;
    QString str;
    value = *(souceData->inputPhrase.vol.currentValue + row)/10;
    str = QString::number(value,10);
    item->setText(str);

}

void MainWindow::setPhaseCur(int row ,int count)
{
    QTableWidgetItem *item = phasewidget_1->item(row,count);
    int value;
    QString str;
    value = *(souceData->inputPhrase.cur.currentValue + row)/10;
    str = QString::number(value,10);
    item->setText(str);
}

void MainWindow::setPhasePower(int row ,int count)
{
    QTableWidgetItem *item = phasewidget_1->item(row,count);
    int value;
    QString str;
    value = *(souceData->inputPhrase.power + row);
    str = QString::number(value,10);
    item->setText(str);
}

void MainWindow::setPhaseEngry(int row ,int count)
{
    QTableWidgetItem *item = phasewidget_1->item(row,count);
    int value;
    QString str;
    value = *(souceData->inputPhrase.energy + row);
    str = QString::number(value,10);
    item->setText(str);
}

void MainWindow::setPhasePowerfactor(int row ,int count)
{
    QTableWidgetItem *item = phasewidget_1->item(row,count);
    int value;
    QString str;
    value = *(souceData->inputPhrase.powerFactor + row);
    str = QString::number(value,10);
    item->setText(str);
}

/**
 * @brief ————————————————————————————————————————更新相表格2数据——————————————————————————————————————————————————————
 */
void MainWindow::updatePhaseTwoData()
{
    initTableWiget(lefttablewidget,souceData->inputPhrase.len);  //3位3相，根据实际相数传参
    for(int i = 0 ; i < (lefttablewidget->rowCount()) ; i++)
    {
        int column = 0 ;
        setPhaseName(i ,column++ ,lefttablewidget);
        setPhaseVolMax(i ,column++);
        setPhaseVolMin(i ,column );
    }

}

void MainWindow::setPhaseVolMax(int row ,int count)
{
    QTableWidgetItem *item = lefttablewidget->item(row,count);
    int value;
    QString str;
    value = *(souceData->inputPhrase.vol.maxValue + row)/10;
    str = QString::number(value,10);
    item->setText(str);
}

void MainWindow::setPhaseVolMin(int row ,int count)
{
    QTableWidgetItem *item = lefttablewidget->item(row,count);
    int value;
    QString str;
    value = *(souceData->inputPhrase.vol.minValue + row)/10;
    str = QString::number(value,10);
    item->setText(str);
}

/**
 * @brief 更新相表格三数据
 */
void MainWindow::updatePhaseThreeData()
{
    initTableWiget(righttablewidget,souceData->inputPhrase.len);  //3位3相，根据实际相数传参
    for(int i = 0 ; i < (lefttablewidget->rowCount()) ; i++)
    {
        int column = 0 ;
        setPhaseName(i ,column++ ,righttablewidget);
        setPhaseCurMin(i ,column++);
        setPhaseCurCirMin(i ,column++);
        setPhaseCurCirMax(i ,column++);
        setPhaseCurMax(i ,column++);
    }
}

void MainWindow::setPhaseCurMin(int row, int count)
{
    QTableWidgetItem *item = righttablewidget->item(row,count);
    int value;
    QString str;
    value = *(souceData->inputPhrase.cur.minValue + row);
    str = QString::number(value,10);
    //    str.setNum(value);
    item->setText(str);
}

void MainWindow::setPhaseCurCirMin(int row, int count)
{
    QTableWidgetItem *item = righttablewidget->item(row,count);
    int value;
    QString str;
    value = *(souceData->inputPhrase.cur.cirMinValue + row);
    str = QString::number(value,10);
    item->setText(str);
}

void MainWindow::setPhaseCurCirMax(int row, int count)
{
    QTableWidgetItem *item = righttablewidget->item(row,count);
    int value;
    QString str;
    value = *(souceData->inputPhrase.cur.cirMaxValue + row);
    str = QString::number(value,10);
    item->setText(str);
}

void MainWindow::setPhaseCurMax(int row, int count)
{
    QTableWidgetItem *item = righttablewidget->item(row,count);
    int value;
    QString str;
    value = *(souceData->inputPhrase.cur.maxValue + row);
    str = QString::number(value,10);
    item->setText(str);
}

/**
 * @brief-----------------------------------------回路界面数据更新--------------------------------------------------------
 */
void MainWindow::updateLoopData()
{
    initTableWiget(loopwidget,souceData->loopData.len);  //3位3相，根据实际相数传参
    for(int i = 0 ; i < (loopwidget->rowCount()) ; i++)
    {
        int column = 0 ;
        setLoopName(i , column++);
        setLoopSwitchStatus( i , column++);
        setLoopCur( i , column++);
        setLoopPower( i , column++);
        setLoopEnergy( i , column++);
        setLoopPowerFactor( i , column++);
        setLoopMin( i , column++);
        setLoopCirMin( i , column++);
        setLoopCirMax( i , column++);
        setLoopMax(i , column++);
    }
}

void MainWindow::setLoopName(int row ,int count)
{
    QTableWidgetItem *item = loopwidget->item(row,count);
    QString str = tr("C") + QString::number(row + 1,10);

    item->setText(str);
}

void MainWindow::setLoopSwitchStatus(int row ,int count)
{
    QTableWidgetItem *item = loopwidget->item(row,count);
    int value;
    QString str;

    value = *(souceData->loopData.switchStatus + row);
    //    qDebug() <<"value:" <<*(souceData->loopData.switchStatus + row);

    str = QString::number(value,10);
    item->setText(str);
}

void MainWindow::setLoopCur(int row ,int count)
{
    QTableWidgetItem *item = loopwidget->item(row,count);
    int value;
    QString str;
    value = *(souceData->loopData.cur.currentValue + row)/10;
    str = QString::number(value,10);
    item->setText(str);
}

void MainWindow::setLoopPower(int row ,int count)
{
    QTableWidgetItem *item = loopwidget->item(row,count);
    int value;
    QString str;
    value = *(souceData->loopData.power + row);
    str = QString::number(value,10);
    item->setText(str);
}

void MainWindow::setLoopEnergy(int row ,int count)
{
    QTableWidgetItem *item = loopwidget->item(row,count);
    int value;
    QString str;
    value = *(souceData->loopData.energy + row);
    str = QString::number(value,10);
    item->setText(str);
}

void MainWindow::setLoopPowerFactor(int row ,int count)
{
    QTableWidgetItem *item = loopwidget->item(row,count);
    int value;
    QString str;
    value = *(souceData->loopData.powerFactor + row);
    str = QString::number(value,10);
    item->setText(str);
}

void MainWindow::setLoopMin(int row ,int count)
{
    QTableWidgetItem *item = loopwidget->item(row,count);
    int value;
    QString str;
    value = *(souceData->loopData.cur.minValue + row);
    str = QString::number(value,10);
    item->setText(str);
}

void MainWindow::setLoopCirMin(int row ,int count)
{
    QTableWidgetItem *item = loopwidget->item(row,count);
    int value;
    QString str;
    value = *(souceData->loopData.cur.cirMinValue + row);
    str = QString::number(value,10);
    item->setText(str);
}

void MainWindow::setLoopCirMax(int row ,int count)
{
    QTableWidgetItem *item = loopwidget->item(row,count);
    int value;
    QString str;
    value = *(souceData->loopData.cur.cirMaxValue + row);
    str = QString::number(value,10);
    item->setText(str);
}

void MainWindow::setLoopMax(int row ,int count)
{
    QTableWidgetItem *item = loopwidget->item(row,count);
    int value;
    QString str;
    value = *(souceData->loopData.cur.maxValue + row);
    str = QString::number(value,10);
    item->setText(str);
}

/**
 * @brief -----------------------------------------更新输出位数据---------------------------------------------------------
 */
void MainWindow::updateOutData()
{
    initTableWiget(outwidget,souceData->outData.len);  //3位3相，根据实际相数传参
    for(int i = 0 ; i < (outwidget->rowCount()) ; i++)
    {
        int column = 0 ;
        setOutName(i , column++);
        setOutSwitchStatus( i , column++);
        setOutCur( i , column++);
        setOutPower( i , column++);
        setOutEnergy(i , column++);
        setOutPowerFactor( i , column++);
        setOutMin( i , column++);
        setOutCirMin( i , column++);
        setOutCirMax( i , column++);
        setOutMax(i , column);
    }
}

void MainWindow::setOutName(int row ,int count)
{
    QTableWidgetItem *item = outwidget->item(row,count);
    QString str;
    str =tr("output ") + QString::number(row+1 , 10) ;
    item->setText(str);

}

void MainWindow::setOutSwitchStatus(int row ,int count)
{
    QTableWidgetItem *item = outwidget->item(row,count);
    int value;
    QString str;
    value = *(souceData->outData.switchStatus + row);
    str = QString::number(value,10);
    item->setText(str);
}

void MainWindow::setOutCur(int row ,int count)
{
    QTableWidgetItem *item = outwidget->item(row,count);
    int value;
    QString str;
    value = *(souceData->outData.cur.currentValue + row)/10;
    str = QString::number(value,10);
    item->setText(str);
}

void MainWindow::setOutPower(int row ,int count)
{
    QTableWidgetItem *item = outwidget->item(row,count);
    int value;
    QString str;
    value = *(souceData->outData.power + row);
    str = QString::number(value,10);
    item->setText(str);
}

void MainWindow::setOutEnergy(int row ,int count)
{
    QTableWidgetItem *item = outwidget->item(row,count);
    int value;
    QString str;
    value = *(souceData->outData.energy + row);
    str = QString::number(value,10);
    item->setText(str);
}

void MainWindow::setOutPowerFactor(int row ,int count)
{
    QTableWidgetItem *item = outwidget->item(row,count);
    int value;
    QString str;
    value = *(souceData->outData.powerFactor + row);
    str = QString::number(value,10);
    item->setText(str);
}

void MainWindow::setOutMin(int row ,int count)
{
    QTableWidgetItem *item = outwidget->item(row,count);
    int value;
    QString str;
    value = *(souceData->outData.cur.minValue + row);
    str = QString::number(value,10);
    item->setText(str);
}

void MainWindow::setOutCirMin(int row ,int count)
{
    QTableWidgetItem *item = outwidget->item(row,count);
    int value;
    QString str;
    value = *(souceData->outData.cur.cirMinValue + row);
    str = QString::number(value,10);
    item->setText(str);
}

void MainWindow::setOutCirMax(int row ,int count)
{
    QTableWidgetItem *item = outwidget->item(row,count);
    int value;
    QString str;
    value = *(souceData->outData.cur.cirMaxValue + row);
    str = QString::number(value,10);
    item->setText(str);
}

void MainWindow::setOutMax(int row ,int count)
{
    QTableWidgetItem *item = outwidget->item(row,count);
    int value;
    QString str;
    value = *(souceData->outData.cur.maxValue + row);
    str = QString::number(value,10);
    item->setText(str);
}

/**
 * @brief ---------------------------------------------更新传感器界面数据----------------------------------------------------
 */
void MainWindow::updateSensorData()
{
    updateTemData();
    updateHumData();
}

void MainWindow::updateTemData()
{
    initTableWiget(temwidget,souceData->envData.len);
    for(int i = 0 ; i < (temwidget->rowCount()) ; i++)
    {
        int column = 0 ;
        setTemName(i , column++);
        setTemStatus(i , column++);
        setTemMin(i , column++);
        setTemMax(i , column++);
    }
}

void MainWindow::setTemName(int row, int count)
{
    QTableWidgetItem *item = temwidget->item(row,count);
    QString str = tr("温度") + QString::number(row + 1,10);
    item->setText(str);
}

void MainWindow::setTemStatus(int row, int count)
{
    QTableWidgetItem *item = temwidget->item(row,count);
    int value;
    QString str;
    value = *(souceData->envData.tem.alarmValue + row);
    str = QString::number(value,10);
    item->setText(str);
}

void MainWindow::setTemMax(int row, int count)
{
    QTableWidgetItem *item = temwidget->item(row,count);
    int value;
    QString str;
    value = *(souceData->envData.tem.maxValue + row)/10;
    str = QString::number(value,10);
    item->setText(str);
}

void MainWindow::setTemMin(int row, int count)
{
    QTableWidgetItem *item = temwidget->item(row,count);
    int value;
    QString str;
    value = *(souceData->envData.tem.minValue + row)/10;
    str = QString::number(value,10);
    item->setText(str);
}

void MainWindow::updateHumData()
{
    initTableWiget(humwidget,souceData->envData.len);
    for(int i = 0 ; i < (humwidget->rowCount()) ; i++)
    {
        int column = 0 ;
        setHumName(i , column++);
        setHumStatus(i , column++);
        setHumMin(i , column++);
        setHumMax(i , column++);
    }
}

void MainWindow::setHumName(int row, int count)
{
    QTableWidgetItem *item = humwidget->item(row,count);
    QString str = tr("湿度") + QString::number(row + 1,10);
    item->setText(str);
}

void MainWindow::setHumStatus(int row, int count)
{
    QTableWidgetItem *item = humwidget->item(row,count);
    int value;
    QString str;
    value = *(souceData->envData.hum.alarmValue + row);
    str = QString::number(value,10);
    item->setText(str);
}

void MainWindow::setHumMax(int row, int count)
{
    QTableWidgetItem *item = humwidget->item(row,count);
    int value;
    QString str;
    value = *(souceData->envData.hum.maxValue + row)/10;
    str = QString::number(value,10);
    item->setText(str);
}

void MainWindow::setHumMin(int row, int count)
{
    QTableWidgetItem *item = humwidget->item(row,count);
    int value;
    QString str;
    value = *(souceData->envData.hum.minValue + row)/10;
    str = QString::number(value,10);
    item->setText(str);
}

/**
 * @brief 初始化表格
 * @param widget
 * @param rowSize
 */
void MainWindow::initTableWiget(QTableWidget *widget,int rowSize)
{

    int rowcount = widget->rowCount();
    for(int m = 0 ; m <rowcount ; m++)
    {
        widget->removeRow(0);
    }

    for(int i = 0 ; i < rowSize ; i++)
    {
        widget->insertRow(i);
        for(int j = 0 ; j < widget->columnCount() ; j++)
        {
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setText(tr("---"));
            item->setTextAlignment(Qt::AlignCenter);
            widget->setItem(i,j,item);
        }
    }
}

void MainWindow::initdevSeetings()
{
    setings.devType = ui->comboBox->currentIndex()+1;
    setings.devName = ui->lineEdit->text();
    setings.viceNumber = ui->lineEdit_2->text().toInt();
    setings.devPhaseNum = ui->lineEdit_3->text().toInt();
    setings.devLoopNum = ui->lineEdit_4->text().toInt();
    setings.devOutNum = ui->lineEdit_5->text().toInt();
    setings.devSensorNum = ui->lineEdit_6->text().toInt();
    setings.volDownNum = ui->spinBox_3->value()*10;
    setings.volUpNum = ui->spinBox_4->value()*10;
    //    setings.entranceGuard1 =
}

/**
 * @brief 设备类型选择，例如IP-PDU时，设备无回路及输出位
 * @param index
 */
void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
        ui->lineEdit_4->setEnabled(false);
        ui->lineEdit_5->setEnabled(false);
        break;
    default:
        ui->lineEdit_4->setEnabled(true);
        ui->lineEdit_5->setEnabled(true);
        break;
    }
}

/**
 * @brief 检查字符串是否为纯数字，如果是则返回true，否则返回false
 * @param str
 * @return
 */
bool MainWindow::isDigit(QString str)
{
    char *ch;
    ch = str.toLatin1().data();
    for(int i = 0 ; i < str.toLatin1().size() ;i++)
    {
        if(!isdigit(*(ch + i)))
            return false;
    }

    return true;
}

/**
 * @brief 针对电流及温度湿度阈值
 * @param index
 */
void MainWindow::setThreshold(QModelIndex index)
{
    int ret = ui->tabWidget->currentIndex();
    switch (ret) {
    case 0:

        if((index.column() == 1) || (index.column() == 2)  || (index.column() == 3) || (index.column() == 4))
        {

            SettingDialog *dialog = new SettingDialog(this,ret,0,index.row());
            dialog->exec();
        }

        break;
    case 1:
        if((index.column() == 6) || (index.column() == 7)  || (index.column() == 8) || (index.column() == 9))
        {

            SettingDialog *dialog = new SettingDialog(this,ret,0,index.row());
            dialog->exec();
        }

        break;
    case 2:
        if((index.column() == 6) || (index.column() == 7)  || (index.column() == 8) || (index.column() == 9))
        {

            SettingDialog *dialog = new SettingDialog(this,ret,0,index.row());
            dialog->exec();
        }
        break;
    case 3:

        break;
    default:
        break;
    }
}

/**
 * @brief 针对电压
 * @param index
 */
void MainWindow::setThresholdAdd(QModelIndex index)
{
    int ret = ui->tabWidget->currentIndex();
    if(ret == 0 && ((index.column() == 1) || (index.column() == 2)))
    {
        SettingDialog *dialog = new SettingDialog(this,ret,1,index.row());
        dialog->exec();
    }

}

void MainWindow::setThresholdAddTem(QModelIndex index)
{
    int ret = ui->tabWidget->currentIndex();
    if(ret == 3 && ((index.column() == 2) || (index.column() == 3)))
    {
        SettingDialog *dialog = new SettingDialog(this,ret,2,index.row());
        dialog->exec();
    }

}

void MainWindow::setThresholdAddHum(QModelIndex index)
{
    int ret = ui->tabWidget->currentIndex();
    if(ret == 3 && ((index.column() == 2) || (index.column() == 3)))
    {
        SettingDialog *dialog = new SettingDialog(this,ret,3,index.row());
        dialog->exec();
    }

}


void MainWindow::radioButtonEntaanceOneClicked(int id)
{

    setings.entranceGuard1 = id;
    souce->changeDevSettings();
}

void MainWindow::radioButtonEntaanceTwoClicked(int id)
{

    setings.entranceGuard2 = id;
    souce->changeDevSettings();

}

void MainWindow::radioButtonSmokeClicked(int id)
{
    setings.smoke= id;
    souce->changeDevSettings();
}

void MainWindow::radioButtonWaterClicked(int id)
{
    setings.water = id;
    souce->changeDevSettings();
}

/**
 * @brief 获取本机IP
 */
void MainWindow::getLocalIp()
{
    QString localName = QHostInfo::localHostName();
    QHostInfo info = QHostInfo::fromName(localName);
    QList<QHostAddress> listAddress = info.addresses();
    foreach (QHostAddress address, listAddress) {
        if(address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress::Null && address != QHostAddress::LocalHost)
            ui->label_35->setText(address.toString());
    }
}




