#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDesktopServices>
#include <QSettings>

static jsonRelated relatedata;

const jsonRelated &get_relatedata()
{
    return relatedata;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initBackground();

    myPort = new MySeriorport;
    QStringList portList = myPort->initSeriorPortInfos();
    initPortCombox(portList); //初始化串口信息
    initPortCombox2(portList); //初始化串口信息  -- 源

    onInitIni();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_9_clicked()
{
    MyText *text = new MyText(this);
    text->exec();

    ui->setSource->setFocus();

}

/**
 * @brief 刷新串口信息
 */
void MainWindow::initPortCombox(QStringList &portList)
{
    QString portName = ui->comboBox_port->currentText();

    ui->comboBox_port->clear();

    int ret = portList.size();

    if(ret)
    {
        //重新设置combox
        for(int i = 0 ; i < ret ; i++)
        {
            QString str = portList.at(i);
            //            qDebug() << "str:" <<str;
            ui->comboBox_port->insertItem(i,str);
        }

        //如果刷新后刷新前当前项仍存在，那么切换到该项，如果已不存在，那么切换到第一项
        if(portList.contains(portName))  //如果portlist含有之前的串口
        {
            int index = getIndex(portList,portName);
            qDebug() << "index:" << index;
            if(index != -1)
            {
                ui->comboBox_port->setCurrentIndex(index);
            }else
            {
                qDebug() << "串口不存在!!!";
            }
        }else
        {
            qDebug()<< "该串口已不存在";
            ui->comboBox_port->setCurrentText(0);
        }

    }else
    {
        qDebug() << "当前无串口连接";
        ui->pushButton_port->setText(tr(""));
    }

    //刷新串口信息，将会触发combox的index变化，将在对应槽函数中刷新buttontext；


}

void MainWindow::initPortCombox2(QStringList &portList)
{
    QString portName = ui->comboBox_portB->currentText();

    ui->comboBox_portB->clear();

    int ret = portList.size();

    if(ret)
    {
        //重新设置combox
        for(int i = 0 ; i < ret ; i++)
        {
            QString str = portList.at(i);
            //            qDebug() << "str:" <<str;
            ui->comboBox_portB->insertItem(i,str);
        }

        //如果刷新后刷新前当前项仍存在，那么切换到该项，如果已不存在，那么切换到第一项
        if(portList.contains(portName))  //如果portlist含有之前的串口
        {
            int index = getIndex(portList,portName);
            qDebug() << "index:" << index;
            if(index != -1)
            {
                ui->comboBox_portB->setCurrentIndex(index);
            }else
            {
                qDebug() << "串口不存在!!!";
            }
        }else
        {
            qDebug()<< "该串口已不存在";
            ui->comboBox_portB->setCurrentText(0);
        }

    }else
    {
        qDebug() << "当前无串口连接";
        ui->pushButton_portB->setText(tr(""));
    }

    //刷新串口信息，将会触发combox的index变化，将在对应槽函数中刷新buttontext；
}

/**
 * @brief 点击按钮刷新串口信息
 */
void MainWindow::on_pushButton_12_clicked()
{
    QStringList portList = myPort->updataSeriorPortinfos();
    initPortCombox(portList);
}

int MainWindow::getIndex(QStringList &list,QString &str)
{
    if(list.contains(str))
    {
        for(int i = 0 ; i < list.size() ;i++)
        {
            if(list.at(i) == str)
            {
                return i;
            }
        }
    }

    return -1;
}

/**
 * @brief 初始化按钮文字
 */
void MainWindow::initButtonText()
{
    QString str = ui->comboBox_port->currentText();

    if(!str.isEmpty())
    {
        if(myPort->portIsOpen(str))
        {
            ui->pushButton_port->setText(tr("关闭串口"));
        }else
            ui->pushButton_port->setText(tr("打开串口"));
    }else
        ui->pushButton_port->setText(tr(""));
}

void MainWindow::initButtonText2()
{
    QString str = ui->comboBox_portB->currentText();

    if(!str.isEmpty())
    {
        if(myPort->portIsOpen(str))
        {
            ui->pushButton_portB->setText(tr("关闭串口"));
        }else
            ui->pushButton_portB->setText(tr("打开串口"));
    }else
        ui->pushButton_portB->setText(tr(""));
}

void MainWindow::on_comboBox_port_currentIndexChanged(int index)
{

    QString str = ui->comboBox_port->currentText();
    if(!str.isEmpty())
    {
        initButtonText();
        myPort->viewAllPortStatus();
    }
}

/**
 * @brief 打开或者关闭串口
 */
void MainWindow::on_pushButton_port_clicked()
{
    QString str = ui->pushButton_port->text();
    QString portName = ui->comboBox_port->currentText();
    if(str == "打开串口")
    {
        myPort->openPort(portName);
    }else if(str == "关闭串口")
        myPort->closePort(portName);
    else
        qDebug() << "当前串口不存在！";

    initButtonText();
    initButtonText2();
}

/**
 * @brief 单元测试
 */
void MainWindow::on_pushButton_7_clicked()
{
    mInfo.isUnitTest = true;

   // if(checkInputPhase()) //为数字且不为空
        buttonClicked(ui->spinBox->value());
}

void MainWindow::sleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

/**
 * @brief 一键测试
 */
void MainWindow::on_pushButton_3_clicked()
{
    mInfo.isUnitTest = false;
    ui->textEdit->clear(); //清空

  //  if(checkPredictValue()) //不为空 且为数字
   // {
        mCaddr = ui->spinBox->value();
       // for(int i = ui->addrMin->text().toInt(); i <= ui->addrMax->text().toInt(); i++){
            buttonClicked(mCaddr);
      //  }

  //  }
    ui->pushButton_9->setFocus();


}

void MainWindow::initBasicInfo(int value)
{
    mInfo.addr =value;   //从机地址
    mInfo.phase = changeStrToInt(); //相数
   // relatedata.phase = mInfo.phase; //---------------------追加，确定item数用
    //    mInfo.isUnitTest = false;  //当是单元测试时需置一
    mInfo.currentPhase = ui->lineEdit_3->text().toInt();  //当前测试单元输入相
}

/**
 * @brief 单相返回数字1，三相返回数字3
 * @return
 */
int MainWindow::changeStrToInt()
{
    QString str = ui->comboBox_2->currentText();
    if(str == "单相")
        return 1;

    return 3;
}

bool MainWindow::buttonClicked(int value)
{
    QString portName = ui->comboBox_port->currentText(); //发送数据的端口[串口]

    //初始化info信息  -- 用于发送
    initBasicInfo(value);

    QByteArray array = packet_to_array(mInfo); //组合数据

    if(!mInfo.isUnitTest) //非单元测试
    ui->textEdit->append(QString("-----------[%1]--------------").arg(mCaddr));

    if(myPort->portIsOpen(portName)) //串口是打开状态
        myPort->sendData(array,portName); //通过串口 把数据发出
    else
    {
        if(!mInfo.isUnitTest){ //非单元测试
            ui->textEdit->setTextColor(Qt::red);
            ui->textEdit->append(QString("串口未打开"));
            ui->textEdit->setTextColor("#000000");
        }else{
            QMessageBox::information(this,tr("waring"),tr("串口未打开"),tr("确定"));
        }
        return false;
    }

    sleep(1000);

    qDebug() << array.toHex();
 /*   for(int i = 0 ; i < array.size() ; i++)
    {

        qDebug()<<QString::number(array.at(i),16); //打印发送数据每一项
    } */

    qDebug() << "---------------------------------------------";

    QByteArray marray;
    marray.resize(0);  //清空
    int count = 0;
    do{
        marray = myPort->readData(portName);  //接收数据
        count++;
        sleep(20);

    }while((marray.size() == 0) && (count < COUNT));

    qDebug() << marray.toHex();

 /*   for(int i = 0 ; i < marray.size() ; i++)
    {
        //        ui->textEdit->append(QString::number(marray.at(i),16));
        qDebug()<<QString::number(marray.at(i),16);  //输出收到的每一项
    } */

    if(marray.size() > 0)
    {
        receiveData packet; //返回的数据包
        bool ret = net_to_local(mInfo,marray,&packet);  //数据整理到packet
        if(ret)
        {
#if 0
            QString str = tr("isUnitTest:%1   currentPhase:%2").arg(packet.isUnitTest).arg(packet.currentPhase);
            ui->textEdit->append(str);
            QString str1 = tr("L1电压：%1    L2电压：%2    L3电压：%3").arg(packet.vol[0]).arg(packet.vol[1]).arg(packet.vol[2]);
            QString str2 = tr("L1电流：%1    L2电流：%2    L3电流：%3").arg(packet.cur[0]).arg(packet.cur[1]).arg(packet.cur[2]);
            ui->textEdit->append(str1);
            ui->textEdit->append(str2);
#endif
            if(mInfo.isUnitTest) //单元测试
                updateUnitInfo(&packet);
            else   //一键测试
            {
                updateTextEdit(&packet);  // 电压电流预测值与实际值对比  -- 显示在日志里
                initRelatedAta(&packet);  //relatedata赋值
            }
        }else
            return false;
    }else
    {
        myPort->clearPort(portName);  //如果超时，那么刷新缓存区，防止干扰下次数据接收

        if(!mInfo.isUnitTest){ //非单元测试
            ui->textEdit->setTextColor(Qt::red);
            ui->textEdit->append(QString("已超时，无数据返回，请检查串口是否选择正确或串口是否与设备保持连接！"));
            ui->textEdit->setTextColor("#000000");
        }else{
            QMessageBox::information(this,tr("warning"),tr("已超时，无数据返回，请检查串口是否选择正确或串口是否与设备保持连接！"));
        }

        return false;
    }

    return true;

}

void MainWindow::updateUnitInfo(receiveData *packet)
{

    int ret = packet->currentPhase; //单元测试的当前项
    ui->label_38->setText(QString::number(packet->vol[ret-1],10));
    ui->label_39->setText(QString::number(packet->cur[ret-1],10));


}

void MainWindow::updateTextEdit(receiveData *packet)
{
    int    phase = changeStrToInt(); //获取当前相数[1|3]
    double preidctVol    = ui->lineEdit_vol->text().toDouble();  //预测电压
    double preidctVolMax = ui->lineEdit_volMax->text().toDouble();  //预测电压

    double preidctCur    = ui->lineEdit_cur->text().toDouble();  //预测电流
    double preidctCurMax = ui->lineEdit_curMax->text().toDouble();  //预测电流

    double preidctEry    = ui->lineEdit_ery->text().toDouble();  //预测电流
    double preidctEryMax = ui->lineEdit_eryMax->text().toDouble();  //预测电流

    for(int i = 0 ; i < phase ; i++)
    {
        ui->textEdit->setTextColor("#000000");
        ui->textEdit->append("");
        QString str = QString("L%1 当前频率：%2").arg(i+1).arg(packet->fre[i]);
        ui->textEdit->append(str);

        if(packet->vol[i] < preidctVol || packet->vol[i] > preidctVolMax)
        {
            ui->textEdit->setTextColor(Qt::red);
            QString str = QString("L%1 相电压不符预期，当前电压：%2").arg(i+1).arg(packet->vol[i]);
            ui->textEdit->append(str);
            ui->textEdit->setTextColor("#000000");
        }else{
            QString str = QString("L%1 相电压OK，当前电压：%2").arg(i+1).arg(packet->vol[i]);
            ui->textEdit->append(str);
        }

        if(packet->cur[i] < preidctCur * 10 || packet->cur[i] > preidctCurMax * 10)
        {
            ui->textEdit->setTextColor(Qt::red);
            QString str = QString("L%1 相电流不符预期，当前电流：%2").arg(i+1).arg(packet->cur[i]);
            ui->textEdit->append(str);
            ui->textEdit->setTextColor("#000000");

        }else {
            QString str = QString("L%1 相电流OK，当前电流：%2").arg(i+1).arg(packet->cur[i]);
            ui->textEdit->append(str);
        }

        if(packet->ery[i] < preidctEry || packet->ery[i] > preidctEryMax)
        {
            ui->textEdit->setTextColor(Qt::red);
            QString str = QString("L%1 相电能不符预期，当前电能：%2").arg(i+1).arg(packet->ery[i]);
            ui->textEdit->append(str);
            ui->textEdit->setTextColor("#000000");

        }else {
            QString str = QString("L%1 相电能OK，当前电能：%2").arg(i+1).arg(packet->ery[i]);
            ui->textEdit->append(str);
        }
    }
    ui->textEdit->append("一键测试结束");
}

bool MainWindow::checkPredictValue()
{
    QString str1 = ui->lineEdit_vol->text();
    QString str2 = ui->lineEdit_cur->text();
    if(!str1.isEmpty()&& (!str2.isEmpty()))
    {
        if(checkDigit(str1) && checkDigit(str2))
        {
            return true;
        }else
            QMessageBox::information(this,tr("warning"),tr("预测值不为数字"),tr("确定"));

    }else
        QMessageBox::information(this,tr("warning"),tr("预测值为空"),tr("确定"));

    return false;
}

bool MainWindow::checkDigit(QString str)
{
    int size = str.size();
    QByteArray array = str.toLatin1();
    for(int i = 0 ; i < size ;i++)
    {
        if(!isdigit(array.at(i))) //是数字
            return false;
    }
    return true;
}

bool MainWindow::checkInputPhase()
{
    QString str = ui->lineEdit_3->text();

    if(!str.isEmpty())
    {
        if(checkDigit(str))
        {
            return true;
        }else
            QMessageBox::information(this,tr("warning"),tr("当前测试相不为数字"),tr("确定"));

    }else
        QMessageBox::information(this,tr("warning"),tr("当前测试相为空"),tr("确定"));

    return false;
}

/**
 * @brief 初始化json相关数据
 */
void MainWindow::initRelatedAta(receiveData *data)
{
    relatedata.phase = changeStrToInt();

    QDateTime currentTime = QDateTime::currentDateTime();
    //yyyy-MM-dd hh:mm:ss.zzz
    QString startTime  = currentTime.toString("yyyy-MM-dd hh:mm:ss");
    strcpy(relatedata.startTime ,startTime.toLatin1().data());

    QString endTime  = currentTime.addSecs(60).toString("yyyy-MM-dd hh:mm:ss");
    strcpy(relatedata.endTime ,endTime.toLatin1().data());

    checkResult(relatedata.result,data);  //补充校验结果

    int predictVol = ui->lineEdit_vol->text().toInt(); //预测值
    int predictCur = ui->lineEdit_cur->text().toInt();
    relatedata.predictVol = predictVol;
    relatedata.predictCur = predictCur;

    strcpy_buf(relatedata.vol,data->vol,6);
    strcpy_buf(relatedata.cur,data->cur,6);
}

/**
 * @brief 以预测值为标准，检测校验是否合格
 * @param buf
 */
void MainWindow::checkResult(bool *buf,receiveData *data)
{
    int predictVol = ui->lineEdit_vol->text().toInt();
    int predictCur = ui->lineEdit_cur->text().toInt();

    int ret  = 0 ;
    for(int i = 0 ; i < 6 ; i++)
    {
        if(i < 3)
        {
            if(data->vol[ret] != predictVol)
                buf[i] = false;
            else
                buf[i] = true;
        }else{   /////-----------------------------------------------------------------------------
           // ret = 0;
            if(data->cur[ret - 3] != predictCur)
                buf[i] = false;
            else
                buf[i] = true;
        }
        ret++;
    }
}

void MainWindow::strcpy_buf(quint16 *_Dest, const quint16 *_Source ,int len)
{
    if((_Dest != NULL) && (_Source != NULL))
        for(int i = 0 ; i < len ; i++)
            *(_Dest + i) = *(_Source + i);
}

/**
 * @brief 初始化背景
 */
void MainWindow::initBackground()
{
    this->setAutoFillBackground(true);
    QPalette palette;
    QPixmap map = QPixmap(":/new/prefix1/image/back.jpg").scaled(700,700);
    palette.setBrush(QPalette::Background,QBrush(map));  //设置背景图片
    this->setPalette(palette);

}

void MainWindow::on_doadmin_clicked()
{
    QString url = "http://"  + ui->ipLineEdit->text() +"/home.html";
    QDesktopServices::openUrl(QUrl(url));
}

void MainWindow::on_doCorrect_clicked()
{
    QString url = "http://"  + ui->ipLineEdit->text() +"/correct.html";
    QDesktopServices::openUrl(QUrl(url));
}

void MainWindow::on_saverIni_clicked()
{
    double preidctSouA   = ui->sourceA->text().toDouble(); //源电流
    double preidctSouV   = ui->sourceV->text().toDouble(); //源电压

    double preidctVol    = ui->lineEdit_vol->text().toDouble();  //预测电压
    double preidctVolMax = ui->lineEdit_volMax->text().toDouble();  //预测电压

    double preidctCur    = ui->lineEdit_cur->text().toDouble();  //预测电流
    double preidctCurMax = ui->lineEdit_curMax->text().toDouble();  //预测电流

    double preidctEry    = ui->lineEdit_ery->text().toDouble();  //预测电流
    double preidctEryMax = ui->lineEdit_eryMax->text().toDouble();  //预测电流

    QString iniFile = QCoreApplication::applicationDirPath() + "/config.ini";
    QSettings ini(iniFile, QSettings::IniFormat);
    ini.beginGroup("Value");
    ini.setValue("SourceA", preidctSouA);
    ini.setValue("SourceV", preidctSouV);
    ini.setValue("Amin", preidctCur);
    ini.setValue("Amax", preidctCurMax);
    ini.setValue("Vmin", preidctVol);
    ini.setValue("Vmax", preidctVolMax);
    ini.setValue("Wmin", preidctEry);
    ini.setValue("Wmax", preidctEryMax);
    ini.endGroup();
}

void MainWindow::onInitIni()
{
    double preidctSouA;
    double preidctSouV;
    double Amin;
    double Amax;
    double Vmin;
    double Vmax;
    double Wmin;
    double Wmax;
    int cObj;

    QString iniFile = QCoreApplication::applicationDirPath() + "/config.ini";
    QSettings ini(iniFile, QSettings::IniFormat);
    ini.beginGroup("Value");
    preidctSouA = ini.value("SourceA", 0).toDouble();
    preidctSouV = ini.value("SourceV", 0).toDouble();
    Amin = ini.value("Amin", 0).toDouble();
    Amax = ini.value("Amax", 0).toDouble();
    Vmin = ini.value("Vmin", 0).toDouble();
    Vmax = ini.value("Vmax", 0).toDouble();
    Wmin = ini.value("Wmin", 0).toDouble();
    Wmax = ini.value("Wmax", 0).toDouble();
    cObj      = ini.value("CObj", 0).toInt();
    ini.endGroup();

    ui->sourceA->setText(QString::number(preidctSouA));
    ui->sourceV->setText(QString::number(preidctSouV));
    ui->lineEdit_cur->setText(QString::number(Amin));
    ui->lineEdit_curMax->setText(QString::number(Amax));
    ui->lineEdit_vol->setText(QString::number(Vmin));
    ui->lineEdit_volMax->setText(QString::number(Vmax));
    ui->lineEdit_ery->setText(QString::number(Wmin));
    ui->lineEdit_eryMax->setText(QString::number(Wmax));


/*
    QString iniFile = QCoreApplication::applicationDirPath() + "/config.ini";
    QSettings ini(iniFile, QSettings::IniFormat);
    ini.beginGroup("Value");
    ini.endGroup();
    */
    ui->comboBox_2->setCurrentIndex(cObj);
}

void MainWindow::on_pushButton_2_clicked() //发送数据给标准源 0V 0A
{
    QString portName = ui->comboBox_portB->currentText(); //发送数据的端口[串口]

    ui->label_9->setText("---");

    for(int n = 0; n < 3; n++){
        QByteArray array;
        QString data;
        if(n == 0) { //量程
            data ="I0 220 6 15 1200";
        }else if(n == 1){ //V
            data = "V0";
        }else if(n == 2){  //A
            data = "A0";
        }

        int len = data.length();
        for(int i = 0; i < len; i++){
            array.append(data.at(i));
        }
        array.append(0x0D);
        qDebug() << array.toHex();
        if(myPort->portIsOpen(portName)) //串口是打开状态
            myPort->sendData(array,portName); //通过串口 把数据发出

        sleep(300);
    }

    ui->label_9->setText("完成");
}

void MainWindow::on_pushButton_clicked() //发送数据给标准源 220V 0A
{
    QString portName = ui->comboBox_portB->currentText(); //发送数据的端口[串口]

    ui->label_9->setText("---");

    for(int n = 0; n < 3; n++){
        QByteArray array;
        QString data;
        if(n == 0) { //量程
            data ="I0 220 6 15 1200";
        }else if(n == 1){ //V
            data = "V100";
        }else if(n == 2){  //A
            data = "A0";
        }

        int len = data.length();
        for(int i = 0; i < len; i++){
            array.append(data.at(i));
        }
        array.append(0x0D);
        qDebug() << array.toHex();
        if(myPort->portIsOpen(portName)) //串口是打开状态
            myPort->sendData(array,portName); //通过串口 把数据发出

        sleep(300);
    }

    ui->label_9->setText("完成");
}

void MainWindow::on_setSource_clicked() //发送数据给标准源
{
    QString portName = ui->comboBox_portB->currentText(); //发送数据的端口[串口]

    ui->label_9->setText("---");

    for(int n = 0; n < 3; n++){
        QByteArray array;
        QString data;
        if(n == 0) { //量程
            data ="I0 " + ui->sourceV->text() + " " + ui->sourceA->text() + " 15 1200";
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
        if(myPort->portIsOpen(portName)) //串口是打开状态
            myPort->sendData(array,portName); //通过串口 把数据发出

        sleep(300);
    }

    ui->label_9->setText("完成");

    ui->pushButton_3->setFocus(); //聚焦

 /*   QByteArray marray;
    marray.resize(0);  //清空
    int count = 0;
    do{
        marray = myPort->readData(portName);  //接收数据
        count++;
        sleep(200);

    }while((marray.size() == 0) && (count < COUNT));

        qDebug() << "data" << marray.toHex() << 1;

*/
}

QByteArray MainWindow::onTidyData(ComCmd type, int data)
{
    COM_COMMAND comd;
    comd.Cmd = (ComCmd)1;
    comd.wParam = 2;
    comd.lParam = 3;
    comd.Res = 4;
    comd.Unused = 5;

    qDebug() << sizeof(comd.lParam) << sizeof(comd.wParam);

    QByteArray res((char*)&comd, sizeof(comd));
  qDebug() << res.toHex();

  COM_COMMAND comd2;
  memcpy(&comd2, res.data(), res.size());

  return res;
}

void MainWindow::on_comboBox_portB_currentIndexChanged(int index)
{
    QString str = ui->comboBox_portB->currentText();
    if(!str.isEmpty())
    {
        initButtonText2();
        myPort->viewAllPortStatus();
    }
}

void MainWindow::on_pushButton_portB_clicked() //打开串口
{
    QString str = ui->pushButton_portB->text();
    QString portName = ui->comboBox_portB->currentText();
    if(str == "打开串口")
    {
        myPort->openPort(portName);
    }else if(str == "关闭串口")
        myPort->closePort(portName);
    else
        qDebug() << "当前串口不存在！";

    initButtonText();
    initButtonText2();
}

void MainWindow::on_pushButton_12B_clicked()
{
    QStringList portList = myPort->updataSeriorPortinfos();
    initPortCombox2(portList);
}

void MainWindow::on_comboBox_2_currentIndexChanged(int index)
{
    QString iniFile = QCoreApplication::applicationDirPath() + "/config.ini";
    QSettings ini(iniFile, QSettings::IniFormat);
    ini.beginGroup("Value");
    ini.setValue("CObj", index);
    ini.endGroup();

}




